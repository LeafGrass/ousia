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
 * @brief   initialze memory heap
 * @param   none
 * @return  status code
 */
int32 _mm_heap_init(void)
{
	uint32 heap_size = 0;
	if (__heap_start == NULL || __heap_end == NULL ||
			__heap_end < __heap_start) {
		os_printk(LOG_ERROR, "%s - failed.\n", __func__);
		return OS_EFAIL;
	}
	heap_size = (uint32)(__heap_end - __heap_start);
	os_printk(LOG_INFO, "%s - 0x%08p ~ 0x%08p (%dKB)\n",
			__func__, __heap_start, __heap_end, heap_size/1024);
	memset(__heap_start, 0, heap_size);
	return 0;
}
