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
 * @file    core/sys_utils.c
 * @brief   ousia system utilities
 * @log     2011.8 initial revision
 */

#include <ousia/ousia.h>
#include <ousia/ousia_type.h>
#include <port/ousia_port.h>
#include <sys/time.h>
#include <sys/sched.h>
#include <sys/print.h>
#include <sys/utils.h>
#include <sys/debug.h>


#define PS_INIT_STACK_SIZE	128
#define PS_IDLE_STACK_SIZE	128
#define PS_MAIN_STACK_SIZE	512

/* FIXME memory of init process should be recycled */
static uint8 __ps_init_stack[PS_INIT_STACK_SIZE] = {0};
static uint8 __ps_idle_stack[PS_IDLE_STACK_SIZE] = {0};
static uint8 __ps_main_stack[PS_MAIN_STACK_SIZE] = {0};
static struct _pcb_t ps_init_pcb;
static struct _pcb_t ps_idle_pcb;
static struct _pcb_t ps_main_pcb;

/*
 * @brief   process - idle
 * @param   args -i/o- reserved
 * @return  void
 */
static void __ps_idle(void *args)
{
	os_logk(LOG_INFO, "process %s is here!\n", __func__);
	while (1) {
		/* collect statistics */
	}
}

/*
 * @brief   process - init
 * @param   args -i/o- reserved
 * @return  void
 */
static void __ps_init(void *args)
{
	os_process_create(&ps_idle_pcb, __ps_idle, NULL,
			  __ps_idle_stack, PS_IDLE_STACK_SIZE);
	os_process_create(&ps_main_pcb, ps_main, NULL,
			  __ps_main_stack, PS_MAIN_STACK_SIZE);
	os_process_suspend(ps_init_pcb.pid);
}

/*
 * @brief   initialize process before user application starts
 * @param   none
 * @return  pid if create success
 * @note    none
 */
static os_status __sys_process_init(void)
{
	os_status ret = OS_OK;

	/* TODO create two processes at init */
	os_process_create(&ps_init_pcb, __ps_init, NULL,
			  __ps_init_stack, PS_INIT_STACK_SIZE);

	return ret;
}

/*
 * @brief   the very first init of ousia
 * @param   none
 * @return  os status code
 * @note    this function should be called before all other syscalls
 *          interrupts should better no be enabled before os init finished
 */
os_status os_init(void)
{
	os_status ret = OS_OK;

	_os_port_init();
	_init_printf();
	BOOT_LOGO();
	_sys_timetick_init();
	ret = _sys_sched_init();
	ret = __sys_process_init();

	return ret;
}

/*
 * @brief   now it's time to run
 * @param   none
 * @return  none
 * @note    will never return, everything will be handled by ousia then
 */
void os_kick_off(void)
{
	_sys_sched_startup();
}
