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

static const char __logo1[] =
	"                     _\n"
	"     _              / /\n"
	"    | |  ___  __ _ _| |_ __ _ _  __  __ _   _   _\n"
	"    | | / _ \\/ _` |_   _/ _` | \\/ _)/ _` | / / / /\n"
	"    | |_  __( (_| | | |  (_| | | | ( (_| | \\ \\ \\ \\\n"
	"    |_ _\\___|\\__,_| | | \\__, / | |  \\__,_| /_/ /_/\n"
	"                    /_/ \\_ _/\n\n";
static const char __logo2[] =
	"\t\tby LeafGrass - leafgrass.g@gmail.com\n\n";

#define PS_INIT_STACK_SIZE	1024
#define PS_IDLE_STACK_SIZE	1024
#define PS_MAIN_STACK_SIZE	2048

/* FIXME memory of init process should be recycled */
static uint8 __ps_init_stack[PS_INIT_STACK_SIZE] = {0};
static uint8 __ps_idle_stack[PS_IDLE_STACK_SIZE] = {0};
static uint8 __ps_main_stack[PS_MAIN_STACK_SIZE] = {0};
static struct _pcb_t ps_init_pcb;
static struct _pcb_t ps_idle_pcb;
static struct _pcb_t ps_main_pcb;

static uint32 n_sched = 0;
static const struct _pqcb_t *pqcb_hook = NULL;
static const struct _pcb_t *pcb_curr_hook = NULL;

static void __sched_hook(const void *args)
{
	pcb_curr_hook = (const struct _pcb_t *)args;
	n_sched++;
}

/*
 * @brief   process - idle
 * @param   args -i/o- reserved
 * @return  void
 */
static void __ps_idle(void *args)
{
	static uint32 last = 0, curr = 0;

	_sched_register_hook(__sched_hook);

	while (1) {
		/* TODO collect statistics */
		curr = os_systime_get();
		if (curr - last > 20000) {
			os_printk(LOG_INFO, "%s - n_sched: %d, "
					"pcb_curr_hook: 0x%08p\n",
					__func__, n_sched, pcb_curr_hook);
			last = os_systime_get();
			_sched_dump_pq(pqcb_hook);
		}
		os_process_yield();
	}
}

/*
 * @brief   process - init
 * @param   args -i/o- reserved
 * @return  void
 */
static void __ps_init(void *args)
{
	os_printk(LOG_INFO, "process %s is here!\n", __func__);
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
static int32 __process_init(void)
{
	int32 ret = OS_OK;

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
int32 os_init(void)
{
	int32 ret = OS_OK;

	_os_port_init();
	_init_printf();
	BOOT_LOGO(__logo1, __logo2);
	pqcb_hook = _sched_init();
	os_assert(pqcb_hook != NULL);
	_sys_time_systick_init();
	ret = __process_init();
	os_assert(ret == 0);

	return ret;
}

/*
 * @brief   now it's time to run
 * @param   none
 * @return  none
 * @note    will never return, everything will be handled by ousia then
 */
inline void os_kick_off(void)
{
	_sched_startup();
}
