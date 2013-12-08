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
 * @file    core/ulib/string.c
 * @brief   libc string related implementation
 * @log     2012.9 initial revision
 */

#include <ulib/stddef.h>
#include <ulib/string.h>

void *memset(void *dst, int c, size_t n)
{
	char *q = dst;
	while (n--)
		*q++ = c;
	return dst;
}

int memcmp(const void *s1, const void *s2, size_t n)
{
	unsigned char *p1 = (unsigned char *)s1;
	unsigned char *p2 = (unsigned char *)s2;

	while (n-- > 0) {
		if (*p1 < *p2)
			return -1;
		else if (*p1 > *p2)
			return 1;
		p1++;
		p2++;
	}
	return 0;
}

void *memcpy(void *dest, const void *src, size_t n)
{
	unsigned char *pout = (unsigned char*)dest;
	unsigned char *pin  = (unsigned char*)src;
	while (n-- > 0)
		*pout++ = *pin++;
	return dest;
}

int strcmp(const char *s1, const char *s2)
{
	const unsigned char *c1 = (const unsigned char *)s1;
	const unsigned char *c2 = (const unsigned char *)s2;
	unsigned char ch;
	int d = 0;

	while (1) {
		d = (int)(ch = *c1++) - (int)*c2++;
		if (d || !ch)
			break;
	}

	return d;
}
