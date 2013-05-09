/*
 * Copyright (c) 2011-2013 LeafGrass <leafgrass.g@gmail.com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 * ARM SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR
 * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 *
 * Original License
 * ----------------
 *
 * mm/mm_malloc.c
 *
 *   Copyright (C) 2007, 2009, 2013  Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * @file    core/mm_std.c
 * @brief   memory management in a rough standard way
 * @log     2013.5 initial revision
 */

#include <ousia/ousia.h>
#include <ousia/ousia_type.h>
#include <port/ousia_port.h>
#include <ulib/string.h>
#include <sys/print.h>
#include <sys/time.h>
#include <sys/debug.h>
#include <sys/mm.h>

#include "mm_internal.h"
#include "mm_std.h"

struct mm_heap __mm_heap_priv;

/*
 * @brief   Convert the size to a nodelist index.
 * @param   size -i- size of the heap region
 * @return  nodelist index
 */
int __mm_size2ndx(mmsize_t size)
{
	int ndx = 0;

	if (size >= MM_MAX_CHUNK)
		return MM_NNODES - 1;

	size >>= MM_MIN_SHIFT;
	while (size > 1) {
		ndx++;
		size >>= 1;
	}

	return ndx;
}

/*
 * @brief   Add a free chunk to the node next.
 *          It is assumed that the caller holds the mm semaphore.
 * @param   heap -i/o- the selected heap
 *          start -i/o- start of the heap region
 *          size -i- size of the heap region
 * @return  status code
 */
static int32 __mm_addfreechunk(struct mm_heap *heap, struct mm_freenode_s *node)
{
	struct mm_freenode_s *next;
	struct mm_freenode_s *prev;

	/* Convert the size to a nodelist index */
	int ndx = __mm_size2ndx(node->size);

	/* Now put the new node int the next */
	for (prev = &heap->mm_nodelist[ndx], next = heap->mm_nodelist[ndx].flink;
	     next && next->size && next->size < node->size;
	     prev = next, next = next->flink);

	/* Does it go in mid next or at the end? */
	prev->flink = node;
	node->blink = prev;
	node->flink = next;

	/* The new node goes between prev and next */
	if (next)
		next->blink = node;
	return 0;
}

/*
 * @brief   This function adds a region of contiguous
 *          memory to the selected heap.
 * @param   heap -i/o- the selected heap
 *          start -i/o- start of the heap region
 *          size -i- size of the heap region
 * @return  status code
 */
static int32 __mm_addregion(struct mm_heap *heap, void *heapstart, mmsize_t heapsize)
{
	struct mm_freenode_s *node;
	uintptr_t heapbase;
	uintptr_t heapend;
#if CONFIG_MM_REGIONS > 1
	int IDX = heap->mm_nregions;
#else
#  define IDX 0
#endif

	/*
	 * If the MCU handles wide addresses but the memory manager is configured
	 * for a small heap, then verify that the caller is  not doing something
	 * crazy.
	 */
#if defined(CONFIG_MM_SMALL) && !defined(CONFIG_SMALL_MEMORY)
	os_assert(heapsize <= MMSIZE_MAX+1);
#endif

	/*
	 * Adjust the provide heap start and size so that they are both aligned
	 * with the MM_MIN_CHUNK size.
	 */
	heapbase = MM_ALIGN_UP((uintptr_t)heapstart);
	heapend  = MM_ALIGN_DOWN((uintptr_t)heapstart + (uintptr_t)heapsize);
	heapsize = heapend - heapbase;

	os_printk(LOG_INFO, "%s - region %d: base: %p size: %u\n",
		  __func__, IDX + 1, heapstart, heapsize);

	/* Add the size of this region to the total size of the heap */

	heap->mm_heapsize += heapsize;

	/*
	 * Create two "allocated" guard nodes at the beginning and end of
	 * the heap.  These only serve to keep us from allocating outside
	 * of the heap.
	 *
	 * And create one free node between the guard nodes that contains
	 * all available memory.
	 */

	heap->mm_heapstart[IDX] = (struct mm_allocnode_s *)heapbase;
	heap->mm_heapstart[IDX]->size = SIZEOF_MM_ALLOCNODE;
	heap->mm_heapstart[IDX]->preceding = MM_ALLOC_BIT;

	node = (struct mm_freenode_s *)(heapbase + SIZEOF_MM_ALLOCNODE);
	node->size = heapsize - 2*SIZEOF_MM_ALLOCNODE;
	node->preceding = SIZEOF_MM_ALLOCNODE;

	heap->mm_heapend[IDX] = (struct mm_allocnode_s *)(heapend - SIZEOF_MM_ALLOCNODE);
	heap->mm_heapend[IDX]->size = SIZEOF_MM_ALLOCNODE;
	heap->mm_heapend[IDX]->preceding = node->size | MM_ALLOC_BIT;

#undef IDX

#if CONFIG_MM_REGIONS > 1
	heap->mm_nregions++;
#endif

	/* Add the single, large free node to the nodelist */
	return __mm_addfreechunk(heap, node);
}

/****************************************************************************
 * Name: mm_shrinkchunk
 *
 * Description:
 *   Reduce the size of the chunk specified by the node structure to the
 *   specified size.  this internal logic is used both from memalign to
 *   dispose of any trailing memory in the aligned allocation and also by
 *   realloc when there is a request to reduce the size of an allocation.
 *
 *   NOTES:
 *     (1) size is the whole chunk size (payload and header)
 *     (2) the caller must hold the MM semaphore.
 *
 ****************************************************************************/
void __mm_shrinkchunk(struct mm_allocnode_s *node, mmsize_t size)
{
	struct mm_heap *heap = &__mm_heap_priv;
	struct mm_freenode_s *next;

	/* Get a reference to the next node */

	next = (struct mm_freenode_s*)((char*)node + node->size);

	/* Check if it is free */

	if ((next->preceding & MM_ALLOC_BIT) == 0)
	{
		struct mm_allocnode_s *andbeyond;
		struct mm_freenode_s *newnode;

		/* Get the chunk next the next node (which could be the tail chunk) */

		andbeyond = (struct mm_allocnode_s*)((char*)next + next->size);

		/* Remove the next node.  There must be a predecessor, but there may
		 * not be a successor node.
		 */

		os_assert(next->blink);
		next->blink->flink = next->flink;
		if (next->flink)
		{
			next->flink->blink = next->blink;
		}

		/* Create a new chunk that will hold both the next chunk and the
		 * tailing memory from the aligned chunk.
		 */

		newnode = (struct mm_freenode_s*)((char*)node + size);

		/* Set up the size of the new node */

		newnode->size        = next->size + node->size - size;
		newnode->preceding   = size;
		node->size           = size;
		andbeyond->preceding = newnode->size | (andbeyond->preceding & MM_ALLOC_BIT);

		/* Add the new node to the freenodelist */

		__mm_addfreechunk(heap, newnode);
	}

	/* The next chunk is allocated.  Try to free the end portion at the end
	 * chunk to be shrunk.
	 */

	else if (node->size >= size + SIZEOF_MM_FREENODE)
	{
		struct mm_freenode_s *newnode;

		/* Create a new chunk that will hold both the next chunk and the
		 * tailing memory from the aligned chunk.
		 */

		newnode = (struct mm_freenode_s*)((char*)node + size);

		/* Set up the size of the new node */

		newnode->size        = node->size - size;
		newnode->preceding   = size;
		node->size           = size;
		next->preceding      = newnode->size | MM_ALLOC_BIT;

		/* Add the new node to the freenodelist */

		__mm_addfreechunk(heap, newnode);
	}
}

/*
 * @brief   initializing of memory heap
 * @param   heap_start -i/o- start of the heap region
 *          size -i- size of the heap region
 * @return  status code
 */
int32 _mm_init(void *heap_start, mmsize_t size)
{
	int32 i;
	struct mm_heap *heap = &__mm_heap_priv;

	heap->mm_heapsize = 0;

#if CONFIG_MM_REGIONS > 1
	heap->mm_nregions = 0;
#endif

	memset(heap->mm_nodelist, 0, sizeof(struct mm_freenode_s) * MM_NNODES);
	for (i = 1; i < MM_NNODES; i++)
	{
		heap->mm_nodelist[i-1].flink = &heap->mm_nodelist[i];
		heap->mm_nodelist[i].blink   = &heap->mm_nodelist[i-1];
	}

	mm_sem_init(heap);

	/* Add the initial region of memory to the heap */
	return __mm_addregion(heap, heap_start, size);
}

/*
 * @brief   Find the smallest chunk that satisfies the request.
 *          Take the memory from that chunk, save the remaining,
 *          smaller chunk (if any).
 *
 *          8-byte alignment of the allocated data is assured.
 *
 * @param   size -i- size of the heap region
 * @return  status code
 */
void *_mm_malloc(mmsize_t size)
{
	struct mm_heap *heap = &__mm_heap_priv;
	struct mm_freenode_s *node;
	void *ret = NULL;
	int ndx;

	if (size <= 0)
		return NULL;

	/*
	 * Adjust the size to account for (1) the size of the allocated node and
	 * (2) to make sure that it is an even multiple of our granule size.
	 */
	size = MM_ALIGN_UP(size + SIZEOF_MM_ALLOCNODE);

	/* We need to hold the MM semaphore while we muck with the nodelist. */
	mm_sem_take(heap);

	/*
	 * Get the location in the node list to start the search.
	 * Special case, really big allocations.
	 * Or, normally, convert the request size into a nodelist index.
	 */
	if (size >= MM_MAX_CHUNK)
		ndx = MM_NNODES - 1;
	else
		ndx = __mm_size2ndx(size);

	/*
	 * Search for a large enough chunk in the list of nodes. This list is
	 * ordered by size, but will have occasional zero sized nodes as we visit
	 * other mm_nodelist[] entries.
	 */
	for (node = heap->mm_nodelist[ndx].flink;
			node && node->size < size;
			node = node->flink);

	/*
	 * If we found a node with non-zero size, then this is one to use. Since
	 * the list is ordered, we know that is must be best fitting chunk
	 * available.
	 */
	if (node) {
		struct mm_freenode_s *remainder;
		struct mm_freenode_s *next;
		mmsize_t remaining;

		/*
		 * Remove the node.  There must be a predecessor, but there may not be
		 * a successor node.
		 */
		os_assert(node->blink);
		node->blink->flink = node->flink;
		if (node->flink)
			node->flink->blink = node->blink;

		/*
		 * Check if we have to split the free node into one of the allocated
		 * size and another smaller freenode.  In some cases, the remaining
		 * bytes can be smaller (they may be SIZEOF_MM_ALLOCNODE).  In that
		 * case, we will just carry the few wasted bytes at the end of the
		 * allocation.
		 */
		remaining = node->size - size;
		if (remaining >= SIZEOF_MM_FREENODE) {
			/* Get a pointer to the next node in physical memory */
			next = (struct mm_freenode_s*)(((char*)node) + node->size);

			/* Create the remainder node */
			remainder = (struct mm_freenode_s*)(((char*)node) + size);
			remainder->size = remaining;
			remainder->preceding = size;

			/* Adjust the size of the node under consideration */
			node->size = size;

			/*
			 * Adjust the 'preceding' size of the (old) next node, preserving
			 * the allocated flag.
			 */
			next->preceding = remaining | (next->preceding & MM_ALLOC_BIT);

			/* Add the remainder back into the nodelist */
			__mm_addfreechunk(heap, remainder);
		}

		/* Handle the case of an exact size match */
		node->preceding |= MM_ALLOC_BIT;
		ret = (void*)((char*)node + SIZEOF_MM_ALLOCNODE);
	}

	mm_sem_give(heap);

	/*
	 * If CONFIG_DEBUG_MM is defined, then output the result of the allocation
	 * to the SYSLOG.
	 */

#ifdef CONFIG_DEBUG_MM
	if (!ret)
		os_printk(LOG_DEBUG, "Allocation failed, size %d\n", size);
	else
		os_printk(LOG_DEBUG, "Allocated %p, size %d\n", ret, size);
#endif

	return ret;
}

/*
 * @brief   memalign requests more than enough space from malloc,
 *          finds a region within that chunk that meets the alignment
 *          request and then frees any leading or trailing space.
 *
 *          The alignment argument must be a power of two (not checked).
 *          8-byte alignment is guaranteed by normal malloc calls.
 *
 * @param   alignment -i- size for alignment
 *          size -i- size of the heap region
 * @return  status code
 */
void *_mm_memalign(mmsize_t alignment, mmsize_t size)
{
	struct mm_heap *heap = &__mm_heap_priv;
	struct mm_allocnode_s *node;
	mmsize_t rawchunk;
	mmsize_t alignedchunk;
	mmsize_t mask = (mmsize_t)(alignment - 1);
	mmsize_t allocsize;

	/*
	 * If this requested alignement less than or equal to the natural alignment
	 * of malloc, then just let malloc do the work.
	 */
	if (alignment <= MM_MIN_CHUNK)
		return _mm_malloc(size);

	/*
	 * Adjust the size to account for (1) the size of the allocated node, (2)
	 * to make sure that it is an even multiple of our granule size, and to
	 * include the alignment amount.
	 *
	 * Notice that we increase the allocation size by twice the requested
	 * alignment.  We do this so that there will be at least two valid
	 * alignment points within the allocated memory.
	 *
	 * NOTE:  These are sizes given to malloc and not chunk sizes. They do
	 * not include SIZEOF_MM_ALLOCNODE.
	 */
	size      = MM_ALIGN_UP(size);   /* Make multiples of our granule size */
	allocsize = size + 2*alignment;  /* Add double full alignment size */

	/* Then malloc that size */
	rawchunk = (mmsize_t)_mm_malloc(allocsize);
	if (rawchunk == 0)
		return NULL;

	/*
	 * We need to hold the MM semaphore while we muck with the chunks and
	 * nodelist.
	 */
	mm_sem_take(heap);

	/*
	 * Get the node associated with the allocation and the next node after
	 * the allocation.
	 */
	node = (struct mm_allocnode_s*)(rawchunk - SIZEOF_MM_ALLOCNODE);

	/* Find the aligned subregion */
	alignedchunk = (rawchunk + mask) & ~mask;

	/* Check if there is free space at the beginning of the aligned chunk */
	if (alignedchunk != rawchunk)
	{
		struct mm_allocnode_s *newnode;
		struct mm_allocnode_s *next;
		mmsize_t precedingsize;

		/* Get the node the next node after the allocation. */
		next = (struct mm_allocnode_s*)((char*)node + node->size);

		/*
		 * Make sure that there is space to convert the preceding mm_allocnode_s
		 * into an mm_freenode_s.  I think that this should always be true
		 */
		os_assert(alignedchunk >= rawchunk + 8);

		newnode = (struct mm_allocnode_s*)(alignedchunk - SIZEOF_MM_ALLOCNODE);

		/*
		 * Preceding size is full size of the new 'node,' including
		 * SIZEOF_MM_ALLOCNODE
		 */
		precedingsize = (mmsize_t)newnode - (mmsize_t)node;

		/*
		 * If we were unlucky, then the alignedchunk can lie in such a position
		 * that precedingsize < SIZEOF_NODE_FREENODE.  We can't let that happen
		 * because we are going to cast 'node' to struct mm_freenode_s below.
		 * This is why we allocated memory large enough to support two
		 * alignment points.  In this case, we will simply use the second
		 * alignment point.
		 */
		if (precedingsize < SIZEOF_MM_FREENODE)
		{
			alignedchunk += alignment;
			newnode       = (struct mm_allocnode_s*)(alignedchunk - SIZEOF_MM_ALLOCNODE);
			precedingsize = (mmsize_t)newnode - (mmsize_t)node;
		}

		/* Set up the size of the new node */
		newnode->size = (mmsize_t)next - (mmsize_t)newnode;
		newnode->preceding = precedingsize | MM_ALLOC_BIT;

		/* Reduce the size of the original chunk and mark it not allocated, */
		node->size = precedingsize;
		node->preceding &= ~MM_ALLOC_BIT;

		/* Fix the preceding size of the next node */
		next->preceding = newnode->size | (next->preceding & MM_ALLOC_BIT);

		/*
		 * Convert the newnode chunk size back into malloc-compatible size by
		 * subtracting the header size SIZEOF_MM_ALLOCNODE.
		 */
		allocsize = newnode->size - SIZEOF_MM_ALLOCNODE;

		/* Add the original, newly freed node to the free nodelist */
		__mm_addfreechunk(heap, (struct mm_freenode_s *)node);

		/*
		 * Replace the original node with the newlay realloaced,
		 * aligned node
		 */
		node = newnode;
	}

	/*
	 * Check if there is free space at the end of the aligned chunk.
	 * If so, shrink the chunk by that much -- remember, mm_shrinkchunk wants
	 * internal chunk sizes that include SIZEOF_MM_ALLOCNODE, and not the
	 * malloc-compatible sizes that we have.
	 */
	if (allocsize > size)
		__mm_shrinkchunk(node, size + SIZEOF_MM_ALLOCNODE);

	mm_sem_give(heap);
	return (void *)alignedchunk;
}

/*
 * @brief   Returns a chunk of memory to the list of free nodes,
 *          merging with adjacent free chunks if possible.
 * @param   mem -i- address of memory to be freed
 * @return  status code
 */
void _mm_free(void *mem)
{
	struct mm_heap *heap = &__mm_heap_priv;
	struct mm_freenode_s *node;
	struct mm_freenode_s *prev;
	struct mm_freenode_s *next;

	os_printk(LOG_DEBUG, "Freeing %p\n", mem);

	/* Protect against attempts to free a NULL reference */
	if (!mem)
		return;

	/*
	 * We need to hold the MM semaphore while we muck with the
	 * nodelist.
	 */
	mm_sem_take(heap);

	/* Map the memory chunk into a free node */
	node = (struct mm_freenode_s *)((char*)mem - SIZEOF_MM_ALLOCNODE);
	node->preceding &= ~MM_ALLOC_BIT;

	/* Check if the following node is free and, if so, merge it */
	next = (struct mm_freenode_s *)((char*)node + node->size);
	if ((next->preceding & MM_ALLOC_BIT) == 0) {
		struct mm_allocnode_s *andbeyond;

		/*
		 * Get the node following the next node (which will
		 * become the new next node). We know that we can never
		 * index past the tail chunk because it is always allocated.
		 */
		andbeyond = (struct mm_allocnode_s*)((char*)next + next->size);

		/*
		 * Remove the next node.  There must be a predecessor,
		 * but there may not be a successor node.
		 */
		os_assert(next->blink);
		next->blink->flink = next->flink;
		if (next->flink)
			next->flink->blink = next->blink;

		/* Then merge the two chunks */
		node->size          += next->size;
		andbeyond->preceding =  node->size | (andbeyond->preceding & MM_ALLOC_BIT);
		next                 = (struct mm_freenode_s *)andbeyond;
	}

	/*
	 * Check if the preceding node is also free and, if so, merge
	 * it with this node
	 */
	prev = (struct mm_freenode_s *)((char*)node - node->preceding);
	if ((prev->preceding & MM_ALLOC_BIT) == 0) {
		/*
		 * Remove the node.  There must be a predecessor, but there may
		 * not be a successor node.
		 */
		os_assert(prev->blink);
		prev->blink->flink = prev->flink;
		if (prev->flink)
			prev->flink->blink = prev->blink;

		/* Then merge the two chunks */
		prev->size     += node->size;
		next->preceding = prev->size | (next->preceding & MM_ALLOC_BIT);
		node            = prev;
	}

	/* Add the merged node to the nodelist */
	__mm_addfreechunk(heap, node);
	mm_sem_give(heap);
}

/*
 * @brief   mm_zalloc calls mm_malloc, then zeroes out the allocated chunk.
 * @param   size -i- size to be allocated
 * @return  Return the pointer to that chunk of memory if success.
 */
void *_mm_zalloc(mmsize_t size)
{
	void *alloc = _mm_malloc(size);
	if (alloc)
		memset(alloc, 0, size);
	return alloc;
}
