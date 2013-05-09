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
 * @file    core/mm_std.h
 * @brief   standard memory management internal header
 * @log     2013.5 initial revision
 */

#ifndef __MM_STD_H__
#define __MM_STD_H__

/*
 * Chunk Header Definitions
 *
 * These definitions define the characteristics of allocator
 *
 * MM_MIN_SHIFT is used to define MM_MIN_CHUNK.
 * MM_MIN_CHUNK - is the smallest physical chunk that can
 *   be allocated.  It must be at least a large as
 *   sizeof(struct mm_freenode_s).  Larger values may
 *   improve performance slightly, but will waste memory
 *   due to quantization losses.
 *
 * MM_MAX_SHIFT is used to define MM_MAX_CHUNK
 * MM_MAX_CHUNK is the largest, contiguous chunk of memory
 *   that can be allocated.  It can range from 16-bytes to
 *   4Gb.  Larger values of MM_MAX_SHIFT can cause larger
 *   data structure sizes and, perhaps, minor performance
 *   losses.
 */

#define MM_MIN_SHIFT	MM_MIN_SIZE_SHIFT
#define MM_MAX_SHIFT	MM_MAX_SIZE_SHIFT

/* All other definitions derive from these two */

#define MM_MIN_CHUNK		(1 << MM_MIN_SHIFT)
#define MM_MAX_CHUNK		(1 << MM_MAX_SHIFT)
#define MM_NNODES		(MM_MAX_SHIFT - MM_MIN_SHIFT + 1)

#define MM_GRAN_MASK		(MM_MIN_CHUNK-1)
#define MM_ALIGN_UP(a)		(((a) + MM_GRAN_MASK) & ~MM_GRAN_MASK)
#define MM_ALIGN_DOWN(a)	((a) & ~MM_GRAN_MASK)

/* An allocated chunk is distinguished from a free chunk by bit 31 (or 15)
 * of the 'preceding' chunk size.  If set, then this is an allocated chunk.
 */

#ifdef CONFIG_MM_SMALL
#  define MM_ALLOC_BIT	0x8000
#else
#  define MM_ALLOC_BIT	0x80000000
#endif

/* Determines the size of the chunk size/offset type */
#ifdef CONFIG_MM_SMALL
#  define MMSIZE_MAX	0xffff
#else
#  define MMSIZE_MAX	0xffffffff
#endif

/*
 * This describes an allocated chunk.  An allocated chunk is
 * distinguished from a free chunk by bit 15/31 of the 'preceding' chunk
 * size.  If set, then this is an allocated chunk.
 */
struct mm_allocnode_s
{
	/* Size of this chunk */
	mmsize_t size;

	/* Size of the preceding chunk */
	mmsize_t preceding;
};
#ifdef CONFIG_MM_SMALL
#  define SIZEOF_MM_ALLOCNODE	4
#else
#  define SIZEOF_MM_ALLOCNODE	8
#endif

/*
 * This describes a free chunk
 */
struct mm_freenode_s
{
	/* Size of this chunk */
	mmsize_t size;

	/* Size of the preceding chunk */
	mmsize_t preceding;

	/* Supports a doubly linked list */
	struct mm_freenode_s *flink;
	struct mm_freenode_s *blink;
};
#ifdef CONFIG_MM_SMALL
#  ifdef CONFIG_SMALL_MEMORY
#    define SIZEOF_MM_FREENODE	8
#  else
#    define SIZEOF_MM_FREENODE	12
#  endif
#else
#  define SIZEOF_MM_FREENODE	16
#endif

struct mm_heap {
	/* This is the size of the heap provided to mm */
	mmsize_t mm_heapsize;

	/* This is the first and last nodes of the heap */
	struct mm_allocnode_s *mm_heapstart[CONFIG_MM_REGIONS];
	struct mm_allocnode_s *mm_heapend[CONFIG_MM_REGIONS];

#if CONFIG_MM_REGIONS > 1
	int mm_nregions;
#endif

	/*
	 * All free nodes are maintained in a doubly linked list.  This
	 * array provides some hooks into the list at various points to
	 * speed searches for free nodes.
	 */
	struct mm_freenode_s mm_nodelist[MM_NNODES];
};

#define mm_sem_init(_s)
#define mm_sem_take(_s)
#define mm_sem_give(_s)

#endif /* __MM_STD_H__ */
