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
#include <ulib/string.h>
#include <ulib/stdlib.h>
#include <sys/mm.h>
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
static const char __logo2[] = "\t\tby LeafGrass - leafgrass.g@gmail.com\n\n";

#define CPS_INIT_STACK_SIZE	512
#define CPS_IDLE_STACK_SIZE	1024
#define PS_MAIN_STACK_SIZE	1024

struct idle_statistics {
	uint32 n_sched_per_stat;
	const struct _pqcb_t *pqcb_hook;
	const struct _pcb_t *pcb_curr_hook;
};

static struct idle_statistics stat = { 0, NULL, NULL };

#define STATISTICS_TIME		(10*1000)

static void __sched_hook(const void *args)
{
	static uint32 curr = 0, last = 0;
	static uint32 n_sched = 0;
	stat.pcb_curr_hook = (const struct _pcb_t *)args;
	curr = os_systime_get();
	n_sched++;
	if (curr - last > STATISTICS_TIME - 1) {
		stat.n_sched_per_stat = n_sched;
		n_sched = 0;
		last = curr;
	}
}

/*
 * @brief   core process - idle
 * @param   args -i/o- reserved
 * @return  void
 * @note    TODO Collect statistics while idle.
 */
static void __cps_idle(void *args)
{
	static uint32 last = 0, curr = 0;
	static uint32 last_ticks = 0;
	struct _pcb_t *idle = _sched_get_curr_pcb();
	struct _pcb_t *pcb;

	uint32 ticks_delta = 0;

	_sched_attach_hook(__sched_hook);

	while (1) {
		curr = os_systime_get();

		if (curr - last > STATISTICS_TIME - 1) {
			/*os_putchar(0x0c); */
			last = os_systime_get();
			ticks_delta = idle->tcb.ticks_running - last_ticks;
			last_ticks = idle->tcb.ticks_running;
			/* TODO Collect the statistics and **store** them */
			os_printk(LOG_DEBUG, "%d sched in last %ds, "
				  "cpu usage: %%%d\n",
				  stat.n_sched_per_stat,
				  STATISTICS_TIME / 1000,
				  100 - 100 * ticks_delta / STATISTICS_TIME);
		}

		/*
		 * Check if there is any process ready,
		 * or leave in the idle loop of itself.
		 * TODO Preemption.
		 */
		list_for_each_entry(pcb, &stat.pqcb_hook->pq, list) {
			if (pcb == idle)
				continue;
			if (pcb->stat == PSTAT_READY)
				os_process_yield();
		}
	}
}

/*
 * @brief   core process - init
 * @param   args -i/o- reserved
 * @return  void
 * @note    Now we can start the user main entry: ps_main.
 *          The commonly known main().
 */
static void __cps_init(void *args)
{
	int32 ret;

	os_putchar(0x0c);
//	BOOT_LOGO(__logo1, __logo2);
	ret = os_process_create(__cps_idle, NULL, CPS_IDLE_STACK_SIZE);
	os_assert(ret > 0);
	ret = os_process_create(ps_main, NULL, PS_MAIN_STACK_SIZE);
	os_assert(ret > 0);
	os_process_suspend();
}

/*
 * @brief   initialize core process before user application starts
 * @param   none
 * @return  pid if create success
 * @note    none
 */
static inline int32 __process_init(void)
{
	return os_process_create(__cps_init, NULL, CPS_INIT_STACK_SIZE);
}

/*
 * @brief   the very first init of ousia
 * @param   none
 * @return  none
 * @note    this function should be called before all other syscalls
 *          interrupts should better no be enabled before os init finished
 */
void os_init(void)
{
	int32 ret = OS_OK;
	lldbg("starting os...\n");
	port_init();

	lldbg("init stdio...\n");
	_init_printf();

	lldbg("init memory heap...\n");
	ret = _mm_heap_init();
	os_assert(ret == 0);

	lldbg("init scheduler...\n");
	stat.pqcb_hook = _sched_init();
	os_assert(stat.pqcb_hook != NULL);

	lldbg("init system timer...\n");
	_sys_time_systick_init();

	lldbg("prepare system process...\n");
	ret = __process_init();
	os_assert(ret > 0);
}

/*
 * @brief   now it's time to run
 * @param   none
 * @return  none
 * @note    will never return, everything will be handled by ousia then
 */
inline void os_kick_off(void)
{
	lldbg("kick off os...\n");
	_sched_startup();
}
