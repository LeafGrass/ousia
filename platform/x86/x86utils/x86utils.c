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
 * @file    platform/x86/x86utils/x86utils.c
 * @brief   x86 utilities
 * @log     2011.8 initial revision
 */

#include <ousia/ousia_type.h>
#include <stdio.h>
#include </usr/include/time.h>
#include </usr/include/sys/time.h>
#include <stdlib.h>
#include <signal.h>
#include <x86/x86utils/x86utils.h>

#define OS_THROB_RATE		1000
#define X86_MILLISEC_PER_SEC	1000000
#define TIME_TO_START		1

static void (*systick_user_callback)(void);
static unsigned long long _uptime;

static void __timer_init(void);
static void __pseudo_systick(int sig);

/*
 * @brief   x86 specific init
 * @param   none
 * @return  none
 * @note    none
 */
void x86utils_system_init(void)
{
	signal(SIGALRM, __pseudo_systick);
	__timer_init();
}

/**
 * @brief   x86 io putchar routine
 * @param   p -i- device pointer
 *          ch -i- data to transmit
 * @return  none
 * @note    none
 */
void x86utils_io_putc(void *p, char ch)
{
	putchar(ch);
}

/**
 * @brief   attach usr systick callback function
 * @param   callback_fn -i- address of user callback function
 * @return  none
 * @note    none
 */
void x86utils_attach_systick_callback(void (*callback_fn)(void))
{
	systick_user_callback = callback_fn;
}

/**
 * @brief   x86 timer init
 * @param   none
 * @return  none
 * @note    temporarily only support linux
 */
static void __timer_init(void)
{
	struct itimerval itv, oldtv;

	_uptime = 0;

	itv.it_interval.tv_sec = 0;
	itv.it_interval.tv_usec = X86_MILLISEC_PER_SEC/OS_THROB_RATE;
	itv.it_value.tv_sec = TIME_TO_START;
	itv.it_value.tv_usec = 0;

	setitimer(ITIMER_REAL, &itv, &oldtv);
}

/**
 * @brief   use linux timer to implement a systick
 * @param   sig -i- signal (no use here)
 * @return  none
 * @note    none
 */
static void __pseudo_systick(int sig)
{
	_uptime++;
	systick_user_callback();
}
