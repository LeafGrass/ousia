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
 * @file    include/ousia/ousia.h
 * @brief   ousia header
 * @log     2011.8 initial revision
 *          2012.3 add version code
 */

#ifndef __OUSIA_H__
#define __OUSIA_H__

#include <ulib/stdarg.h>
#include <ulib/stddef.h>

#define VERCODE		(VERSION*100 + SUBLEVEL*10 + DEVNUM)
#define VERSTR(str)	#str

#define BOOT_LOGO(str1, str2) \
{ \
	os_putchar('\n'); \
	os_printf(str1, NULL); \
	os_printf("\tOusia v%d.%d.%d %s\n", VERSION, SUBLEVEL, DEVNUM, VERNAME); \
	os_printf(str2, NULL); \
}

#define container_of(ptr, type, member) \
({ \
	const typeof(((type *)0)->member) *__mptr = (ptr); \
	(type *)((char *)__mptr - offsetof(type, member)); \
})

#define __stringify_1(x...)	#x
#define __stringify(x...)	__stringify_1(x)

#endif /* __OUSIA_H__ */
