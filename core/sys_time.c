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
 * @file    core/sys_time.c
 * @brief   timer related routines
 * @log     2011.8 initial revision
 */

#include <port/ousia_port.h>
#include <ousia/ousia_type.h>
#include <sys/sched.h>
#include <sys/print.h>
#include <sys/time.h>

static uint32 __systime = 0;

static void __systick_interrupt(void);

/*
 * @brief   get system time
 * @param   none
 * @return  32 bit integer value for time
 * @note    none
 */
uint32 os_systime_get(void)
{
	return __systime;
}

/*
 * @brief   set system time
 * @param   time -i- 32 bit integer value for time
 * @return  none
 * @note    FIXME is this necessary?
 */
void os_systime_set(uint32 time)
{
}

/*
 * @brief   system timetick init
 * @param   none
 * @return  none
 * @note    none
 */
void _sys_timetick_init(void)
{
	__systime = 0UL;
	_port_systick_register(&__systick_interrupt);
}

/*
 * @brief   os systick interrupt handler
 * @param   none
 * @return  none
 * @note    none
 */
static void __systick_interrupt(void)
{
	os_enter_critical();
	__systime++;
	os_exit_critical();
	return;
}
