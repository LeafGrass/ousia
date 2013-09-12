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

/*
 * TODO
 * We may need mutiple regions management further...
 * And put these configs to system config.
 */
#define CONFIG_MM_REGIONS	1
#define CONFIG_DEBUG_MM

typedef uint32 uintptr_t;
typedef uint32 mmsize_t;

#ifdef CONFIG_MM_SMALL
#define MM_MIN_SIZE_SHIFT	2	/* 4 bytes */
#define MM_MAX_SIZE_SHIFT	15	/* 32 Kb */
#else
#define MM_MIN_SIZE_SHIFT	2	/* 4 bytes, orig: 16 bytes */
#define MM_MAX_SIZE_SHIFT	22	/* 4 Mb */
#endif

#define MM_MIN_SIZE		(1<<MM_MIN_SIZE_SHIFT)
#define MM_MAX_SIZE		(1<<MM_MAX_SIZE_SHIFT)

#define VALIDATE_SIZE(_sz)	((_sz > 0)&&(_sz < MM_MAX_SIZE))

int32 _mm_heap_init(void);
void _mm_dump(void *addr, int32 nb, int32 type);

void *mm_malloc(mmsize_t size);
void *mm_memalign(mmsize_t alignment, mmsize_t size);
void mm_free(void *mem);
void *mm_zalloc(mmsize_t size);
inline int32 mm_mallinfo(struct mallinfo *info);

/* FIXME should be put in stdlib.h */
#define malloc(_size)		mm_malloc(_size)
#define memalign(align, size)	mm_memalign(align, size)
#define free(_m)		mm_free(_m)
#define zalloc(_size)		mm_zalloc(_size)
#define mallinfo(_info)		mm_mallinfo(_info)

/*
 * TODO
 * Core memory management, similar with kmm,
 * could be distinguished from user memory.
 * Leave a different interface name here.
 */
#define cmalloc(_size)		mm_malloc(_size)
#define cmemalign(align, size)	mm_memalign(align, size)
#define cfree(_m)		mm_free(_m)
#define czalloc(_size)		mm_zalloc(_size)

#endif /* __SYS_MM_H__ */
