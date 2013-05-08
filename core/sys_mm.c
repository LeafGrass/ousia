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
 * @file    core/sys_mm.c
 * @brief   core memory management
 * @log     2012.9 initial revision
 */

#include <ousia/ousia.h>
#include <ousia/ousia_type.h>
#include <port/ousia_port.h>
#include <ulib/string.h>
#include <sys/print.h>
#include <sys/time.h>
#include <sys/mm.h>

extern uint8 *__heap_start;
extern uint8 *__heap_end;

/*
 * @brief   memory heap initialization entry
 * @param   none
 * @return  status code
 */
int32 _mm_heap_init(void)
{
	mmsize_t heap_size = 0;
	if (__heap_start == NULL || __heap_end == NULL ||
			__heap_end < __heap_start) {
		os_printk(LOG_ERROR, "%s - failed.\n", __func__);
		return OS_EFAIL;
	}
	heap_size = (mmsize_t)(__heap_end - __heap_start);
	os_printk(LOG_INFO, "%s - 0x%08p ~ 0x%08p (%dKB)\n",
			__func__, __heap_start, __heap_end, heap_size/1024);
	memset(__heap_start, 0, heap_size);
	return _mm_init(__heap_start, heap_size);
}

/*
 * @brief   dump memory
 * @param   addr -i- memory address
 *          nb -i- number of bytes to dump
 *          type -i- byte, half-word or word
 * @return  none
 * @note    TODO Only supports nb that can be divide by 8.
 *          TODO Need to complete type implementation.
 */
void _mm_dump(void *addr, int32 nb, int32 type)
{
	uint8 *m = (uint8 *)addr;
	int32 i, row;
	row = nb/8;
	for (i = 0; i < row; i++) {
		m = (uint8 *)addr + i*8;
		os_printk(LOG_INFO,
			  "%08x: %02x %02x %02x %02x %02x %02x %02x %02x\n",
			  m, *m, *(m+1), *(m+2), *(m+3),
			  *(m+4), *(m+5), *(m+6), *(m+7));
	}
}

/*
 * @brief   Allocate "size" of memory.
 * @param   size -i- size to be allocated
 * @return  Return the pointer to that chunk of memory if success.
 */
inline void *mm_malloc(mmsize_t size)
{
	return _mm_malloc(size);
}

/*
 * @brief   Allocate "size" of aligned memory.
 * @param   size -i- size to be allocated
 * @return  Return the pointer to that chunk of memory if success.
 */
inline void *mm_memalign(mmsize_t alignment, mmsize_t size)
{
	return _mm_memalign(alignment, size);
}

/*
 * @brief   Free allocated memory.
 * @param   size -i- size to be allocated
 * @return  none
 */
inline void mm_free(void *mem)
{
	_mm_free(mem);
}
