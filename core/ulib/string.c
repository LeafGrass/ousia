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
	unsigned char *pout = (unsigned char *)dest;
	unsigned char *pin = (unsigned char *)src;
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

char *strcpy(char *dest, const char *src)
{
	char *tmp = dest;
	while ((*dest++ = *src++) != '\0') ;
	return tmp;
}

char *strchr(const char *s, int c)
{
	if (s) {
		for (;; s++) {
			if (*s == c)
				return (char *)s;

			if (!*s)
				break;
		}
	}

	return NULL;
}

size_t strlen(const char *s)
{
	const char *sc;
	for (sc = s; *sc != '\0'; ++sc);
	return sc - s;
}

static char *g_saveptr = NULL;

char *strtok_r(char *str, const char *delim, char **saveptr)
{
	char *pbegin;
	char *pend = NULL;

	if (str)
		pbegin = str;
	else if (saveptr && *saveptr)
		pbegin = *saveptr;
	else
		return NULL;

	for (; *pbegin && strchr(delim, *pbegin) != NULL; pbegin++) ;

	if (!*pbegin)
		return NULL;

	for (pend = pbegin + 1; *pend && strchr(delim, *pend) == NULL; pend++) ;

	if (*pend)
		*pend++ = '\0';

	if (saveptr)
		*saveptr = pend;
	return pbegin;
}

char *strtok(char *str, const char *delim)
{
	return strtok_r(str, delim, &g_saveptr);
}
