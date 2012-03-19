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
 * @file    core/sys_sched.c
 * @brief   implementation of ousia scheduler
 * @log     2011.7 initial revision
 */

#include <ousia/ousia_type.h>

#include <port/ousia_cfg.h>
#include <port/ousia_port.h>

#include <sys/print.h>
#include <sys/time.h>
#include <sys/sched.h>
#include <sys/debug.h>


#define PS_INIT_STACK_SIZE	128
#define PS_IDLE_STACK_SIZE	128

#ifdef OUSIA_SCHED_STRATEGY_EDFS
static os_status __do_strategy_edfs(struct _pqcb_t *pq);
#endif
#ifdef OUSIA_SCHED_STRATEGY_EDFS_OPT
static os_status __do_strategy_edfs_optimized(struct _pqcb_t *pq);
#endif
#ifdef OUSIA_SCHED_STRATEGY_CFS
static os_status __do_strategy_cfs(struct _pqcb_t *pq);
#endif
#ifdef OUSIA_SCHED_STRATEGY_HPFS
static os_status __do_strategy_hpfs(struct _pqcb_t *pq);
#endif
#ifdef OUSIA_SCHED_STRATEGY_RGHS
static os_status __do_strategy_rghs(struct _pqcb_t *pq);
#endif
static void __ps_init(void *args);
static void __ps_idle(void *args);


static struct _pcb_t curr_pcb = {
	.stack_ptr = NULL,
	.pentry = NULL,
	.stack_size = 0,
	.pid = 0,
	.prio = 0,
	.stat = PSTAT_SLEEP,
	.timer = NULL,
	.p_prev = NULL,
	.p_next = NULL
};

static struct _pqcb_t pqcb = {
	.pnum = 0,
	.p_head = NULL,
	.p_tail = NULL
};

static struct _sched_class_t sched_class = {
#if defined(OUSIA_SCHED_STRATEGY_EDFS)
	.do_schedule = __do_strategy_edfs
#elif defined(OUSIA_SCHED_STRATEGY_CFS)
	.do_schedule = __do_strategy_cfs
#elif defined(OUSIA_SCHED_STRATEGY_HPFS)
	.do_schedule = __do_strategy_hpfs
#elif defined(OUSIA_SCHED_STRATEGY_RGHS)
	.do_schedule = __do_strategy_rghs
#else
	.do_schedule = __do_strategy_edfs_optimized
#endif
};

static uint8 __ps_init_stack[PS_INIT_STACK_SIZE] = {0};
static uint8 __ps_idle_stack[PS_IDLE_STACK_SIZE] = {0};
static struct _pcb_t ps_init_pcb;
static struct _pcb_t ps_idle_pcb;


/*
 * @brief   create a process
 * @param   pcb -i- pointer of process control block
 *          pentry -i- process main function entry
 *          args -i- process main function args
 *          stack_base -i- start address of stack
 *          stack_size -i- process private stack size
 * @return  pid if create success
 * @note    TODO we'd better use dynamic memory in the future
 *          to allocate a pcb and a stack if for a new process
 *          FIXME if pcb is a pointer holder, it should be **p_pcb
 */
int32 os_process_create(void *pcb, void *pentry, void *args,
			void *stack_base, uint32 stack_size)
{
	struct _pcb_t *new_pcb = (struct _pcb_t *)pcb;

	/* TODO here to allocate resources to a process */

	if (pcb == NULL || pentry == NULL || stack_base == NULL)
		return -OS_ERR;

	new_pcb->stack_ptr = _port_context_init(pentry, args, stack_base);
	new_pcb->pentry = pentry;
	new_pcb->stack_size = stack_size;

	/* TODO enqueue pcb */
	pqcb.p_head = new_pcb;

	return new_pcb->pid;
}

/*
 * @brief   os process sleep routine
 * @param   tms -i- sleep time in ms
 * @return  os_status
 */
os_status os_process_sleep(uint32 tms)
{
	os_status ret = OS_OK;

	/* TODO here to calculate time */

	/*
	 * call scheduler
	 * FIXME need to make sure everything is ready for process
	 * scheduling and context switch before start a schedule
	 */
	_sys_sched_schedule();

	return ret;
}

/*
 * @brief   suspend a process
 * @param   pid -i- pid of target process
 * @return  os_status
 * @note    none
 */
os_status os_process_suspend(uint32 pid)
{
	os_status ret = OS_OK;
	return ret;
}

/*
 * @brief   resume a process
 * @param   pid -i- pid of target process
 * @return  os_status
 * @note    none
 */
os_status os_process_resume(uint32 pid)
{
	os_status ret = OS_OK;
	return ret;
}

/*
 * @brief   start ousia scheduler to work
 * @param   none
 * @return  os_status
 * @note    none
 */
os_status _sys_sched_init(void)
{
	os_status ret = OS_OK;
	return ret;
}

/*
 * @brief   initialize process before user application starts
 * @param   none
 * @return  pid if create success
 * @note    none
 */
os_status _sys_sched_process_init(void)
{
	os_status ret = OS_OK;
	void *ps_init_stack_base;
	void *ps_idle_stack_base;

#if (OUSIA_PORT_STACK_TYPE == OUSIA_PORT_STACK_DEC)
	ps_init_stack_base = (void *)&__ps_init_stack[PS_INIT_STACK_SIZE - 1];
	ps_idle_stack_base = (void *)&__ps_idle_stack[PS_IDLE_STACK_SIZE - 1];
#else
	ps_init_stack_base = __ps_init_stack;
	ps_idle_stack_base = __ps_idle_stack;
#endif

	/* TODO create two processes at init */
	os_process_create(&ps_init_pcb, __ps_init, NULL,
			ps_init_stack_base, PS_INIT_STACK_SIZE);
	os_process_create(&ps_idle_pcb, __ps_idle, NULL,
			ps_idle_stack_base, PS_IDLE_STACK_SIZE);

	return ret;
}

/*
 * @brief   start a schedule
 * @param   none
 * @return  os_status
 * @note    none
 */
os_status _sys_sched_schedule(void)
{
	os_status ret = OS_OK;

	ret = sched_class.do_schedule(&pqcb);

	/* TODO here to trigger os context switch */
	_port_context_switch(&curr_pcb, &pqcb.p_head);

	return ret;
}

/*
 * @brief   start our scheduler, os begin to run
 * @param   none
 * @return  none
 * @note    we should be getting into the first pendsv isr
 *          after first switch and never back again
 */
void _sys_sched_startup(void)
{
	uint32 i;
	if (&pqcb.p_head == NULL) {
		os_logk(LOG_ERROR, "first process is not ready!\n");
	} else {
		os_logk(LOG_ERROR, "first process is ready, pcb = 0x%X\n",
				&pqcb.p_head);
		_port_first_switch(&pqcb.p_head);
	}
	while (1) {
		os_logk (LOG_ERROR, "should not get here!\n");
		for(i = 0; i < 1000000UL; i++);
	}
}

#ifdef OUSIA_SCHED_STRATEGY_EDFS
/*
 * @brief   earliest deadline first scheduling
 * @param   pq -i/o- process queue control block
 * @return  os_status
 * @note
 */
static os_status __do_strategy_edfs(struct _pqcb_t *pq)
{
	os_status ret = OS_OK;
	return ret;
}
#endif

#ifdef OUSIA_SCHED_STRATEGY_EDFS_OPT
/*
 * @brief   earliest deadline first scheduling, optimized for overall
 * @param   pq -i/o- process queue control block
 * @return  os_status
 * @note
 */
static os_status __do_strategy_edfs_optimized(struct _pqcb_t *pq)
{
	os_status ret = OS_OK;
	return ret;
}
#endif

#ifdef OUSIA_SCHED_STRATEGY_CFS
/*
 * @brief   completely fair scheduling
 * @param   pq -i/o- process queue control block
 * @return  os_status
 * @note
 */
static os_status __do_strategy_cfs(struct _pqcb_t *pq)
{
	os_status ret = OS_OK;
	return ret;
}
#endif

#ifdef OUSIA_SCHED_STRATEGY_HPFS
/*
 * @brief   highest priority first scheduling
 * @param   pq -i/o- process queue control block
 * @return  os_status
 * @note
 */
static os_status __do_strategy_hpfs(struct _pqcb_t *pq)
{
	os_status ret = OS_OK;
	return ret;
}
#endif

#ifdef OUSIA_SCHED_STRATEGY_RGHS
/*
 * @brief   rough scheduling
 * @param   pq -i/o- process queue control block
 * @return  os_status
 * @note    :P
 */
static os_status __do_strategy_rghs(struct _pqcb_t *pq)
{
	os_status ret = OS_OK;
	return ret;
}
#endif

/*
 * @brief   process - init
 * @param   args -i/o- reserved
 * @return  void
 */
static void __ps_init(void *args)
{
	os_logk(LOG_INFO, "process %s is here!\n", __FUNCTION__);
	os_process_suspend(curr_pcb.pid);
	while (1) {
	}
}

/*
 * @brief   process - idle
 * @param   args -i/o- reserved
 * @return  void
 */
static void __ps_idle(void *args)
{
	os_logk(LOG_INFO, "process %s is here!\n", __FUNCTION__);
	while (1) {
		os_process_sleep(1000);
	}
}
