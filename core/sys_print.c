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
 * @file    core/sys_print.c
 * @brief   implementation of ousia print system
 * @log     2011.8 initial revision
 *          2011.10 modified by Librae
 *          2012.8 modified by Librae
 */

/*
File: tprintf.c

Copyright (C) 2004  Kustaa Nyholm

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

*/

#include <ousia/ousia.h>
#include <ousia/ousia_type.h>
#include <port/ousia_port.h>
#include <sys/print.h>

typedef void (*putcf)(void *dev, char ch);
static putcf stdout_putf;
static void *stdout_putp;

static void tfp_format(void *putp, void (*putf)(void *, char), const char *fmt, va_list va);

static void ui2a(unsigned int num, unsigned int base, int uc, char *bf)
{
	int n = 0;
	unsigned int d = 1;
	while (num/d >= base)
		d *= base;
	while (d != 0) {
		int dgt = num / d;
		num %= d;
		d /= base;
		if (n || dgt > 0 || d == 0) {
			*bf++ = dgt + (dgt < 10 ? '0' : (uc ? 'A' : 'a') - 10);
			++n;
		}
	}
	*bf = 0;
}

static void i2a(int num, char *bf)
{
	if (num < 0) {
		num =- num;
		*bf++ = '-';
	}
	ui2a(num, 10, 0, bf);
}

static int a2d(char ch)
{
	if (ch >= '0' && ch <= '9')
		return ch - '0';
	else if (ch >= 'a' && ch <= 'f')
		return ch - 'a' + 10;
	else if (ch >= 'A' && ch <= 'F')
		return ch - 'A' + 10;
	else return -1;
}

static char a2i(char ch, const char **src, int base, int *nump)
{
	const char *p = *src;
	int num = 0;
	int digit;
	while ((digit = a2d(ch)) >= 0) {
		if (digit > base) break;
		num = num*base + digit;
		ch = *p++;
	}
	*src = p;
	*nump = num;
	return ch;
}

static void putchw(void *putp, putcf putf, int n, char z, char *bf)
{
	char fc = z ? '0' : ' ';
	char ch;
	char *p = bf;
	while (*p++ && n > 0)
		n--;
	while (n-- > 0)
		putf(putp, fc);
	while ((ch = *bf++))
		putf(putp, ch);
}

static void tfp_format(void *putp, putcf putf, const char *fmt, va_list va)
{
	char bf[12];
	char ch;

	while ((ch = *(fmt++))) {
		if (ch != '%') {
#ifndef __PLATFORM_X86__
			if (ch == '\n')
				putf(putp, '\r');
#endif
			putf(putp, ch);
		}
		else {
			char lz = 0;
			int w = 0;
			ch = *(fmt++);
			if (ch == '0') {
				ch = *(fmt++);
				lz = 1;
			}
			if (ch >= '0' && ch <= '9') {
				ch = a2i(ch, &fmt, 10, &w);
			}

			switch (ch) {
			case 0:
				goto abort;
			case 'u':
				ui2a(va_arg(va, unsigned int), 10, 0, bf);
				putchw(putp, putf, w, lz, bf);
				break;
			case 'd':
				i2a(va_arg(va, int), bf);
				putchw(putp, putf, w, lz, bf);
				break;
			case 'p':
			case 'x':
			case 'X':
				ui2a(va_arg(va, unsigned int), 16, (ch == 'X'), bf);
				putchw(putp, putf, w, lz, bf);
				break;
			case 'c':
				putf(putp, (char)(va_arg(va, int)));
				break;
			case 's':
				putchw(putp, putf, w, 0, va_arg(va, char *));
				break;
			case '%':
				putf(putp, ch);
			default:
				break;
			}
		}
	}
abort:
	;
}

void _init_printf(void)
{
	_port_printf_init(&stdout_putp, &stdout_putf);
}

void tfp_putchar(char ch)
{
	stdout_putf(NULL, ch);
}

void tfp_printf(const char *fmt, ...)
{
	va_list va;
	va_start(va, fmt);
	tfp_format(stdout_putp, stdout_putf, fmt, va);
	va_end(va);
}

static void putcp(void *p, char c)
{
	*(*((char**)p))++ = c;
}

void tfp_sprintf(char *s, const char *fmt, ...)
{
	va_list va;
	va_start(va, fmt);
	tfp_format(&s, putcp, fmt, va);
	putcp(&s, 0);
	va_end(va);
}
