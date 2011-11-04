/*
 * Copyright (c) 2011-2012 LeafGrass <leafgrass.g@gmail.com>
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
 */

/*
 * @file    framework/mem/mempool.c
 * @brief   memory pool simple implementation
 * @log     2011.11 initial revision
 * @note    TODO not completed yet, and only test on x86
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mempool.h"

/*
 * @brief    alloc a continuous memory block for list storage
 * @param    pp_mem_base -i/o- pointer to the address of memory base if success
 *           max_node_num -i- number of node entities user wants to alloc
 * @return   return a pointer to the memory head if success, else a NULL
 */
struct mp_mem_head_t *mp_alloc(mp_i32 **pp_mem_base, mp_u32 max_node_num)
{
	struct mp_mem_head_t *p_mem_head = NULL;
	mp_u32 size = 0;

	size = max_node_num * sizeof(struct mp_node_t) +
		sizeof(struct mp_mem_head_t);
	/* FIXME check this size? */
	*pp_mem_base = (mp_i32 *)mp_malloc(size);
	if (*pp_mem_base == NULL)
		return NULL;

	memset(*pp_mem_base, 0, size);
	p_mem_head = (struct mp_mem_head_t *)*pp_mem_base;
	p_mem_head->p_node_first = NULL;
	p_mem_head->size = size;
	p_mem_head->used_node_num = 0;
	p_mem_head->max_node_num = max_node_num;

#ifdef DEBUG_PRINT_ON
	printf("static list info\n");
	printf("------------------------\n");
	printf("p_mem_base: %p\np_node_first: %p\np_node_tail: %p\n:"
			"total size: %d\nsize of head: %u\nsize of node: %u\n"
			"max node number: %u\nused node number: %u\n",
			*pp_mem_base,
			p_mem_head->p_node_first,
			p_mem_head->p_node_tail,
			p_mem_head->size,
			(mp_u32)sizeof(struct mp_mem_head_t),
			(mp_u32)sizeof(struct mp_node_t),
			p_mem_head->max_node_num,
			p_mem_head->used_node_num);
	printf("------------------------\n");
#endif
	return p_mem_head;
}

/*
 * @brief    free the continuous memory block that has been allocated
 * @param    p_mem_base -i- address of memory base
 * @return   status code
 * @note     this function will not check if this memory is allocated by system
 */
status_t mp_clean(mp_i32 *p_mem_base)
{
	mp_free(p_mem_base);
	return 0;
}

/*
 * @brief    create a new node
 * @param    p_mem_base -i- address of memory base
 * @return   pointer to new node if success, else return a NULL
 */
struct mp_node_t *mp_new_node(mp_i32 *p_mem_base)
{
	struct mp_mem_head_t *p_mem_head = NULL;
	struct mp_node_t *p_mem_it = NULL;
	struct mp_node_t *p_node_new = NULL;
	mp_u32 i = 0;

	p_mem_head = (struct mp_mem_head_t *)p_mem_base;
	if (p_mem_head->size < sizeof(struct mp_mem_head_t))
		return NULL;

	if (p_mem_head->p_node_first == NULL) {
		p_mem_head->p_node_first = (struct mp_node_t *)
			((struct mp_mem_head_t *)p_mem_head + 1);
		p_mem_head->p_node_tail = p_mem_head->p_node_first;
	}
	p_mem_it = p_mem_head->p_node_first;
	while ((p_mem_it->used == 1) && (i < p_mem_head->max_node_num)) {
		p_mem_it++;
		i++;
	}
	if (i == p_mem_head->max_node_num) {
		/* fail, memory pool nodes are all in use */
		return NULL;
	} else {
		/* success, add new node following the tail of memory pool list */
		p_node_new = p_mem_it;
		p_node_new->used = 1;
		p_node_new->p_next = NULL;
		if (p_mem_head->used_node_num == 0) {
			p_node_new->p_prev = NULL;
			p_mem_head->p_node_tail->p_next = NULL;
		} else {
			p_node_new->p_prev = p_mem_head->p_node_tail;
			p_mem_head->p_node_tail->p_next = p_node_new;
		}
		p_mem_head->p_node_tail = p_node_new;
		p_mem_head->used_node_num++;
		return p_node_new;
	}
#ifdef DEBUG_PRINT_ON
	printf("\n");
#endif
	return p_node_new;
}

/*
 * @brief    delete a node
 * @param    p_node -i- pointer to specific node
 * @return   status code
 */
status_t mp_del_node(struct mp_node_t *p_node)
{
	return 0;
}
