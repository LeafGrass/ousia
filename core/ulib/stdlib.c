/*
 * Copyright (c) 2011-2014 LeafGrass <leafgrass.g@gmail.com>
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
 * @file    core/ulib/stdlib.c
 * @brief   micro implementation of stdlib.c of libc
 * @log     2014.9 initial revision
 */

#include <ulib/stddef.h>
#include <ulib/stdlib.h>

static int char2hex(char c)
{
	if (c >= '0' && c <= '9')
		return c - '0';
	else if (c >= 'a' && c <= 'f')
		return c - 'a' + 10;
	else if (c >= 'A' && c <= 'F')
		return c - 'A' + 10;

	return 0;
}

inline int isdigit(int c)
{
	return ((c >= '0') && (c <= '9'));
}

inline int isxdigit(int c)
{
	return isdigit(c) ||
		((c >= 'a') && (c <= 'f')) ||
		((c >= 'A') && (c <= 'F'));
}

long atol(const char *str)
{
	long value = 0;
	int neg = 0;

	if (str[0] == '0' && str[1] == 'x') {
		str += 2;
		while (*str && isxdigit(*str))
			value = value * 16 + char2hex(*str++);
	} else {
		if (str[0] == '-') {
			neg = 1;
			str++;
		}
		while (*str && isdigit(*str))
			value = value * 10 + *str++  - '0';
	}

	if (neg)
		value = -value;

	return value;
}

unsigned long atoul(const char *str)
{
	unsigned long value = 0;
	if (str[0] == '0' && str[1] == 'x') {
		str += 2;
		while (*str && isxdigit(*str))
			value = value * 16 + char2hex(*str++);
	} else {
		while (*str && isdigit(*str))
			value = value * 10 + *str++  - '0';
	}

	return value;
}

unsigned long long atoull(const char *str)
{
	unsigned long long value = 0;
	if (str[0] == '0' && str[1] == 'x') {
		str += 2;
		while (*str && isxdigit(*str))
			value = value * 16 + char2hex(*str++);
	} else {
		while (*str && isdigit(*str))
			value = value * 10 + *str++  - '0';
	}

	return value;
}
