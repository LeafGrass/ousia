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

#include <ousia/ousia.h>
#include <ousia/ousia_type.h>
#include <port/ousia_cfg.h>
#include <port/ousia_port.h>
#include <sys/print.h>
#include <sys/time.h>
#include <sys/debug.h>
#include <sys/sched.h>


static struct _pcb_t *curr_pcb;

/*
 * FIXME Use memory pool here?
 */
static struct _pqcb_t pqcb = {
	.n_pcb = 0,
	.p_head = NULL,
	.p_tail = NULL
};

/*
 * @brief   process enqueue
 * @param   p -i- pointer of pcb
 * @return  status code
 * @note
 */
static int32 __pcb_enqueue(struct _pcb_t *p)
{
	list_add_tail(&p->list, &pqcb.pq);
	pqcb.n_pcb++;
	return 0;
}

/*
 * @brief   process dequeue
 * @param   p -i- pointer of pcb
 * @return  status code
 * @note
 */
static int32 __pcb_dequeue(struct _pcb_t *p)
{
	list_del(&p->list);
	pqcb.n_pcb--;
	return 0;
}

/*
 * @brief   __pcb_get_prev
 * @param   p -i- pointer of pcb
 * @return  the prev pcb
 * @note
 */
static inline struct _pcb_t *__pcb_get_prev(struct _pcb_t *p)
{
	return list_entry(p->list.prev, struct _pcb_t, list);
}

/*
 * @brief   __pcb_get_next
 * @param   p -i- pointer of pcb
 * @return  the next pcb
 * @note
 */
static inline struct _pcb_t *__pcb_get_next(struct _pcb_t *p)
{
	return list_entry(p->list.next, struct _pcb_t, list);
}

/*
 * @brief   __pq_get_head
 * @param   p_pqcb -i- pointer of pqcb
 * @return  pcb at the queue head
 * @note
 */
static inline struct _pcb_t *__pq_get_head(struct _pqcb_t *p_pqcb)
{
	return list_entry(p_pqcb->pq.next, struct _pcb_t, list);
}

/*
 * @brief   __pq_get_tail
 * @param   p_pqcb -i- pointer of pqcb
 * @return  pcb at the queue tail
 * @note
 */
static inline struct _pcb_t *__pq_get_tail(struct _pqcb_t *p_pqcb)
{
	return list_entry(p_pqcb->pq.prev, struct _pcb_t, list);
}

#ifdef OUSIA_SCHED_STRATEGY_EDFS
/*
 * @brief   earliest deadline first scheduling
 * @param   pqcb -i/o- process queue control block
 * @return  int32
 * @note
 */
static int32 __do_strategy_edfs(struct _pqcb_t *pqcb)
{
	int32 ret = OS_OK;
	return ret;
}
#endif

#ifdef OUSIA_SCHED_STRATEGY_EDFS_OPT
/*
 * @brief   earliest deadline first scheduling, optimized for overall
 * @param   pqcb -i/o- process queue control block
 * @return  int32
 * @note
 */
static int32 __do_strategy_edfs_optimized(struct _pqcb_t *pqcb)
{
	int32 ret = OS_OK;
	return ret;
}
#endif

#ifdef OUSIA_SCHED_STRATEGY_CFS
/*
 * @brief   completely fair scheduling
 * @param   pqcb -i/o- process queue control block
 * @return  int32
 * @note
 */
static int32 __do_strategy_cfs(struct _pqcb_t *pqcb)
{
	int32 ret = OS_OK;
	return ret;
}
#endif

#ifdef OUSIA_SCHED_STRATEGY_HPFS
/*
 * @brief   highest priority first scheduling
 * @param   pqcb -i/o- process queue control block
 * @return  int32
 * @note
 */
static int32 __do_strategy_hpfs(struct _pqcb_t *pqcb)
{
	int32 ret = OS_OK;
	return ret;
}
#endif

#ifdef OUSIA_SCHED_STRATEGY_RGHS
/*
 * @brief   rough scheduling
 * @param   pqcb -i/o- process queue control block
 * @return  int32
 * @note    :P
 */
static int32 __do_strategy_rghs(struct _pqcb_t *pqcb)
{
	int32 ret = OS_OK;
	if (pqcb == NULL) {
		os_assert(pqcb);
		/* should be -EPARAM */
		return -OS_EFAIL;
	}

	/*
	 * TODO
	 * schedule algorithm implementation here:
	 * pqcb->n_pcb =
	 * pqcb->p_head =
	 * pqcb->p_tail =
	 */
#if 0
	os_printk(LOG_DEBUG, "%s, schedule done.\n", __func__);
#endif
	return ret;
}
#endif

static struct _sched_class_t sched_class = {
	.sched_hook = NULL,
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
static void __dump_pcb(struct _pcb_t *p_pcb)
{
	if (p_pcb == NULL) {
		os_printk(LOG_ERROR, "%s - pcb is NULL\n", __func__);
		return;
	}
	os_printk(LOG_INFO, "--- dump pcb: 0x%08X ---\n", p_pcb);
	os_printk(LOG_INFO, "stack_ptr: 0x%08X\n", p_pcb->stack_ptr);
	os_printk(LOG_INFO, "pentry:    0x%08X\n", p_pcb->pentry);
	os_printk(LOG_INFO, "stack_sz:  %d\n", p_pcb->stack_sz);
	os_printk(LOG_INFO, "pid:       %d\n", p_pcb->pid);
	os_printk(LOG_INFO, "prio:      %d\n", p_pcb->prio);
	os_printk(LOG_INFO, "stat:      %d\n", p_pcb->stat);
	os_printk(LOG_INFO, "timer:     0x%08X\n", p_pcb->timer);
	os_printk(LOG_INFO, "prev:      0x%08X\n", __pcb_get_prev(p_pcb));
	os_printk(LOG_INFO, "next:      0x%08X\n", __pcb_get_next(p_pcb));
	os_printk(LOG_INFO, "----------------------------\n");
	_port_dump_stack((pt_regs_t *)p_pcb->stack_ptr);
}

/*
 * @brief   dump process queue
 * @param   p_pqcb -i- pointer of pqcb
 * @return  nothing
 * @note    none
 */
static void __dump_pq(struct _pqcb_t *p_pqcb)
{
	struct _pcb_t *pcb;
	if (p_pqcb == NULL) {
		os_printk(LOG_ERROR, "%s - pqcb is NULL\n", __func__);
		return;
	}
	os_printk(LOG_INFO, "%d processes in queue:\n", p_pqcb->n_pcb);
	list_for_each_entry(pcb, &p_pqcb->pq, list) {
		if (pcb == __pq_get_head(p_pqcb))
			os_printk(LOG_INFO, "\t0x%08p <- head\n", pcb);
		else if (pcb == __pq_get_tail(p_pqcb))
			os_printk(LOG_INFO, "\t0x%08p <- tail\n", pcb);
		else
			os_printk(LOG_INFO, "\t0x%08p\n", pcb);
	}
}

/*
 * @brief   start ousia scheduler to work
 * @param   pq -i/o- pointer to process queue list head
 * @return  int32
 * @note    basically, pq is the list_head of init process
 */
int32 _sys_sched_init(void)
{
	int32 ret = OS_OK;

	INIT_LIST_HEAD(&pqcb.pq);

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

	if (sched_class.sched_hook)
		sched_class.sched_hook(NULL);

#if 0
	os_printk(LOG_INFO, "%s, curr_pcb: 0x%08X, head: 0x%08X\n",
			__func__, (uint32)curr_pcb, __pq_get_head(&pqcb));
	__dump_pcb(__pq_get_head(&pqcb));
	__dump_pq(&pqcb);
#endif
	/* TODO here to trigger os context switch */
	curr_pcb = __pq_get_head(&pqcb);
	_port_context_switch((uint32)tmp, (uint32)__pq_get_head(&pqcb));
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
	if (__pq_get_head(&pqcb) == NULL) {
		os_printk(LOG_ERROR, "first process is not ready!\n");
	} else {
		os_printk(LOG_ERROR, "first process is ready, pcb = 0x%X\n",
				__pq_get_head(&pqcb));
		curr_pcb = __pq_get_head(&pqcb);
		_port_first_switch((uint32)__pq_get_head(&pqcb));
	}
	os_printk(LOG_ERROR, "%s, shoud never be here!\n");
	while (1);
}

/*
 * @brief   register a scheduler hook, called in every scheduling
 * @param   none
 * @return  none
 * @note    WARNING This hook should not take a long time!
 */
void _sys_sched_register_hook(void (*fn)(void *args))
{
	sched_class.sched_hook = fn;
}

/*
 * @brief   dump stack api for user
 * @param   none
 * @return  none
 * @note    FIXME temporary used for debug
 */
void os_dump_stack(void)
{
	__dump_pcb(__pq_get_tail(&pqcb));
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

	os_printk(LOG_DEBUG, "===> new process: 0x%08p\n"
			     "\t\t+-------------------\n"
			     "\t\t| stack_base: 0x%08p\n"
			     "\t\t| stack_sz: %d\n"
			     "\t\t| entry: 0x%08p\n"
			     "\t\t| prio: %d\n"
			     "\t\t+-------------------\n",
			     new_pcb, stack_base, stack_sz,
			     pentry, new_pcb->prio);

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

	__pcb_enqueue(new_pcb);

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
#if 0
	os_printk(LOG_DEBUG, "%s, tms: %d\n", __func__, tms);
#endif
	/* TODO here to calculate time */

	/*
	 * call scheduler
	 * FIXME need to make sure everything is ready for process
	 * scheduling and context switch before start a schedule
	 */
	__pcb_dequeue(curr_pcb);
	__pcb_enqueue(curr_pcb);
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

	/* FIXME hack here, dequeue curr_pcb directly */
	__pcb_dequeue(curr_pcb);

	os_printk(LOG_INFO, "%s, pid: %d\n", __func__, pid);
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
