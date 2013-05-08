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
 * @file    include/sys/mm.h
 * @brief   header of ousia memory management
 * @log     2012.9 initial revision
 */

#ifndef __SYS_MM_H__
#define __SYS_MM_H__

typedef uint32	mmsize_t;

int32 _mm_heap_init(void);
void _mm_dump(void *addr, int32 nb, int32 type);

void *mm_malloc(mmsize_t size);
void *mm_memalign(mmsize_t alignment, mmsize_t size);
void mm_free(void *mem);

int32 _mm_init(void *heap_start, mmsize_t size);
void *_mm_malloc(mmsize_t size);
void *_mm_memalign(mmsize_t alignment, mmsize_t size);
void _mm_free(void *mem);

#define malloc(_size)		_mm_malloc(_size)
#define memalign(align, size)	_mm_memalign(align, size)
#define free(_m)		_mm_free(_m)

#endif /* __SYS_MM_H__ */
