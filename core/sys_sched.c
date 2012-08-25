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

uint32 var_dbg = 0;

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
	if (pq == NULL) {
		os_assert(pq);
		/* should be -EPARAM */
		return -OS_ERR;
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
}

/*
 * @brief   dump stack of process
 * @param   p_pcb -i- pointer of pcb
 * @return  nothing
 * @note    FIXME need to resolve big/little endian
 *          FIXME this function should has no business with specific chip
 */
static void __dump_stack(const struct _pcb_t *p_pcb)
{
	uint32 *stk = NULL;
	if (p_pcb == NULL || p_pcb->stack_ptr == NULL) {
		os_logk(LOG_ERROR, "%s - pcb: 0x%X, stk: 0x%X\n",
				__func__, p_pcb, p_pcb->stack_ptr);
		return;
	}
	stk = p_pcb->stack_ptr;
	os_logk(LOG_INFO, "pcb:  0x%X, stk: 0x%X\n", p_pcb, stk);
	os_logk(LOG_INFO, "xpsr: 0x%08X\t| 0x%08X\n", *(stk + 15), stk + 15);
	os_logk(LOG_INFO, "pc:   0x%08X\t| 0x%08X\n", *(stk + 14), stk + 14);
	os_logk(LOG_INFO, "lr:   0x%08X\t| 0x%08X\n", *(stk + 13), stk + 13);
	os_logk(LOG_INFO, "r12:  0x%08X\t| 0x%08X\n", *(stk + 12), stk + 12);
	os_logk(LOG_INFO, "r3:   0x%08X\t| 0x%08X\n", *(stk + 11), stk + 11);
	os_logk(LOG_INFO, "r2:   0x%08X\t| 0x%08X\n", *(stk + 10), stk + 10);
	os_logk(LOG_INFO, "r1:   0x%08X\t| 0x%08X\n", *(stk + 9), stk + 9);
	os_logk(LOG_INFO, "r0:   0x%08X\t| 0x%08X\n", *(stk + 8), stk + 8);
	os_logk(LOG_INFO, "r11:  0x%08X\t| 0x%08X\n", *(stk + 7), stk + 7);
	os_logk(LOG_INFO, "r10:  0x%08X\t| 0x%08X\n", *(stk + 6), stk + 6);
	os_logk(LOG_INFO, "r9:   0x%08X\t| 0x%08X\n", *(stk + 5), stk + 5);
	os_logk(LOG_INFO, "r8:   0x%08X\t| 0x%08X\n", *(stk + 4), stk + 4);
	os_logk(LOG_INFO, "r7:   0x%08X\t| 0x%08X\n", *(stk + 3), stk + 3);
	os_logk(LOG_INFO, "r6:   0x%08X\t| 0x%08X\n", *(stk + 2), stk + 2);
	os_logk(LOG_INFO, "r5:   0x%08X\t| 0x%08X\n", *(stk + 1), stk + 1);
	os_logk(LOG_INFO, "r4:   0x%08X\t| 0x%08X\n", *stk, stk);
	__dump_pcb(p_pcb);
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
 * @brief   start a schedule
 * @param   none
 * @return  os_status
 * @note    none
 */
os_status _sys_sched_schedule(void)
{
	os_status ret = OS_OK;

	ret = sched_class.do_schedule(&pqcb);
	os_assert(ret == 0);

	os_logk(LOG_INFO, "%s, curr_pcb: 0x%08X, p_head: 0x%08X\n",
			__func__, (uint32)curr_pcb, (uint32)pqcb.p_head);

	__dump_stack(curr_pcb);

	/* TODO here to trigger os context switch */
	_port_context_switch((uint32)curr_pcb, (uint32)pqcb.p_head);

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
		return -OS_ERR;

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
	__dump_stack(new_pcb);
#endif

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
 * @return  os_status
 * @note    none
 */
os_status os_process_suspend(uint32 pid)
{
	os_status ret = OS_OK;
	os_logk(LOG_INFO, "%s, pid: %d\n", __func__, pid);
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
