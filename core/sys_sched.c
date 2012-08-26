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


static struct _pcb_t *curr_pcb;

/*
 * FIXME Use memory pool here?
 */
static struct _pqcb_t pqcb = {
	.pnum = 0,
	.p_head = NULL,
	.p_tail = NULL
};

#ifdef OUSIA_SCHED_STRATEGY_EDFS
/*
 * @brief   earliest deadline first scheduling
 * @param   pq -i/o- process queue control block
 * @return  int32
 * @note
 */
static int32 __do_strategy_edfs(struct _pqcb_t *pq)
{
	int32 ret = OS_OK;
	return ret;
}
#endif

#ifdef OUSIA_SCHED_STRATEGY_EDFS_OPT
/*
 * @brief   earliest deadline first scheduling, optimized for overall
 * @param   pq -i/o- process queue control block
 * @return  int32
 * @note
 */
static int32 __do_strategy_edfs_optimized(struct _pqcb_t *pq)
{
	int32 ret = OS_OK;
	return ret;
}
#endif

#ifdef OUSIA_SCHED_STRATEGY_CFS
/*
 * @brief   completely fair scheduling
 * @param   pq -i/o- process queue control block
 * @return  int32
 * @note
 */
static int32 __do_strategy_cfs(struct _pqcb_t *pq)
{
	int32 ret = OS_OK;
	return ret;
}
#endif

#ifdef OUSIA_SCHED_STRATEGY_HPFS
/*
 * @brief   highest priority first scheduling
 * @param   pq -i/o- process queue control block
 * @return  int32
 * @note
 */
static int32 __do_strategy_hpfs(struct _pqcb_t *pq)
{
	int32 ret = OS_OK;
	return ret;
}
#endif

#ifdef OUSIA_SCHED_STRATEGY_RGHS
/*
 * @brief   rough scheduling
 * @param   pq -i/o- process queue control block
 * @return  int32
 * @note    :P
 */
static int32 __do_strategy_rghs(struct _pqcb_t *pq)
{
	int32 ret = OS_OK;
	if (pq == NULL) {
		os_assert(pq);
		/* should be -EPARAM */
		return -OS_EFAIL;
	}

	/*
	 * TODO
	 * schedule algorithm implementation here:
	 * pq->pnum =
	 * pq->p_head =
	 * pq->p_tail =
	 */

	os_logk(LOG_DEBUG, "%s, schedule done.\n", __func__);
	return ret;
}
#endif

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

/*
 * @brief   dump process control block
 * @param   p_pcb -i- pointer of pcb
 * @return  nothing
 * @note    FIXME this function should has more clear info
 */
static void __dump_pcb(const struct _pcb_t *p_pcb)
{
	if (p_pcb == NULL) {
		os_logk(LOG_ERROR, "%s - pcb: 0x%X\n", __func__, p_pcb);
		return;
	}
	os_logk(LOG_INFO, "--- dump pcb: 0x%08X ---\n", p_pcb);
	os_logk(LOG_INFO, "stack_ptr: 0x%08X\n", p_pcb->stack_ptr);
	os_logk(LOG_INFO, "pentry:    0x%08X\n", p_pcb->pentry);
	os_logk(LOG_INFO, "stack_sz:  %d\n", p_pcb->stack_sz);
	os_logk(LOG_INFO, "pid:       %d\n", p_pcb->pid);
	os_logk(LOG_INFO, "prio:      %d\n", p_pcb->prio);
	os_logk(LOG_INFO, "stat:      %d\n", p_pcb->stat);
	os_logk(LOG_INFO, "timer:     0x%08X\n", p_pcb->timer);
	os_logk(LOG_INFO, "prev:      0x%08X\n", p_pcb->p_prev);
	os_logk(LOG_INFO, "next:      0x%08X\n", p_pcb->p_next);
	os_logk(LOG_INFO, "----------------------------\n");
	_port_dump_stack((pt_regs_t *)p_pcb->stack_ptr);
}

/*
 * @brief   start ousia scheduler to work
 * @param   none
 * @return  int32
 * @note    none
 */
int32 _sys_sched_init(void)
{
	int32 ret = OS_OK;
	return ret;
}

/*
 * @brief   start a schedule
 * @param   none
 * @return  int32
 * @note    none
 */
void _sys_sched_schedule(void)
{
	int32 ret = OS_OK;
	struct _pcb_t *tmp = curr_pcb;

	ret = sched_class.do_schedule(&pqcb);
	os_assert(ret == 0);

	os_logk(LOG_INFO, "%s, curr_pcb: 0x%08X, p_head: 0x%08X\n",
			__func__, (uint32)curr_pcb, (uint32)pqcb.p_head);
	/* TODO here to trigger os context switch */
	curr_pcb = pqcb.p_head;
	_port_context_switch((uint32)tmp, (uint32)pqcb.p_head);
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
	if (pqcb.p_head == NULL) {
		os_logk(LOG_ERROR, "first process is not ready!\n");
	} else {
		os_logk(LOG_ERROR, "first process is ready, pcb = 0x%X\n", pqcb.p_head);
		curr_pcb = pqcb.p_head;
		_port_first_switch((uint32)pqcb.p_head);
	}
	os_logk(LOG_ERROR, "%s, shoud never be here!\n");
	while (1);
}

/*
 * @brief   create a process
 * @param   pcb -i- pointer of process control block
 *          pentry -i- process main function entry
 *          args -i- process main function args
 *          stack_base -i- start address of stack
 *          stack_sz -i- process private stack size
 * @return  pid if create success
 * @note    TODO we'd better use dynamic memory in the future
 *          to allocate a pcb and a stack if for a new process
 *          so, pcb structure should not be opened for user
 *          FIXME if pcb is a pointer holder, it should be **p_pcb
 */
int32 os_process_create(void *pcb, void *pentry, void *args,
			void *stack_base, uint32 stack_sz)
{
	struct _pcb_t *new_pcb = (struct _pcb_t *)pcb;
	uint8 *stk = (uint8 *)stack_base;

	os_logk(LOG_DEBUG, "new pcb: 0x%08X, stack_base: 0x%08X\n", new_pcb, stack_base);

	/* TODO here to allocate resources to a process */

	if (pcb == NULL || pentry == NULL || stack_base == NULL)
		return -OS_EFAIL;

#if (OUSIA_PORT_STACK_TYPE == OUSIA_PORT_STACK_DEC)
	/* FIXME ARCH_BIT can only be 32, hard code here :( */
	stk = stk + stack_sz - 4;
#endif

	new_pcb->stack_ptr = _port_context_init(pentry, args, (void *)stk);
	new_pcb->pentry = pentry;
	new_pcb->stack_sz = stack_sz;

	/* TODO enqueue pcb */
	pqcb.p_head = new_pcb;
#if 1
	__dump_pcb(new_pcb);
#endif

	return new_pcb->pid;
}

/*
 * @brief   delete a process
 * @param   pid -i- pid of target process
 * @return  int32
 * @note    none
 */
int32 os_process_delete(uint32 pid)
{
	int32 ret = OS_OK;

	/*
	 * TODO
	 * 1. remove the process from pqcb
	 * 2. reschedule
	 */
	_sys_sched_schedule();

	return ret;
}

/*
 * @brief   os process sleep routine
 * @param   tms -i- sleep time in ms
 * @return  int32
 */
int32 os_process_sleep(uint32 tms)
{
	int32 ret = OS_OK;

	os_logk(LOG_DEBUG, "%s, tms: %d\n", __func__, tms);

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
 * @return  int32
 * @note    none
 */
int32 os_process_suspend(uint32 pid)
{
	int32 ret = OS_OK;

	os_logk(LOG_INFO, "%s, pid: %d\n", __func__, pid);
	_sys_sched_schedule();

	return ret;
}

/*
 * @brief   resume a process
 * @param   pid -i- pid of target process
 * @return  int32
 * @note    none
 */
int32 os_process_resume(uint32 pid)
{
	int32 ret = OS_OK;
	return ret;
}
