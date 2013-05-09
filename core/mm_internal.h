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
 */

/*
 * @file    core/mm_std.h
 * @brief   standard memory management internal header
 * @log     2013.5 initial revision
 */

#ifndef __MM_INTERNAL_H__
#define __MM_INTERNAL_H__

/*
 * TODO
 * We may need mutiple regions management further...
 * And put these configs to system config.
 */
#define CONFIG_MM_REGIONS	1
#define CONFIG_DEBUG_MM
typedef uint32	uintptr_t;

int32 _mm_init(void *heap_start, mmsize_t size);
void *_mm_malloc(mmsize_t size);
void *_mm_memalign(mmsize_t alignment, mmsize_t size);
void _mm_free(void *mem);
void *_mm_zalloc(mmsize_t size);

#define mm_sem_init(_s)
#define mm_sem_take(_s)
#define mm_sem_give(_s)

#endif /* __MM_INTERNAL_H__ */
