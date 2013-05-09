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
#include <ulib/stdlib.h>
#include <sys/print.h>
#include <sys/time.h>
#include <sys/debug.h>
#include <sys/mm.h>
#include <sys/sched.h>


/*
 * FIXME Use memory pool here?
 */
static struct _pqcb_t pqcb;
static struct _pcb_t *curr_pcb;
static struct _sched_class_t sched_class;

/*
 * @brief   process enqueue
 * @param   p -i/o- pointer of pcb
 * @return  status code
 * @note
 */
static inline void __pcb_enqueue(struct _pcb_t *p)
{
	list_add_tail(&p->list, &pqcb.pq);
	pqcb.n_pcb++;
}

/*
 * @brief   process dequeue
 * @param   p -i/o- pointer of pcb
 * @return  status code
 * @note
 */
static inline void __pcb_dequeue(struct _pcb_t *p)
{
	list_del(&p->list);
	pqcb.n_pcb--;
}

/*
 * @brief   __pcb_get_prev
 * @param   p -i- pointer of pcb
 * @return  the prev pcb
 * @note
 */
static inline struct _pcb_t *__pcb_get_prev(const struct _pcb_t *p)
{
	return list_entry(p->list.prev, struct _pcb_t, list);
}

/*
 * @brief   __pcb_get_next
 * @param   p -i- pointer of pcb
 * @return  the next pcb
 * @note
 */
static inline struct _pcb_t *__pcb_get_next(const struct _pcb_t *p)
{
	return list_entry(p->list.next, struct _pcb_t, list);
}

/*
 * @brief   iterate the pcb list to find the pid match one
 * @param   pid -i- pid of target pcb
 * @return  pointer of the target pcb if successfully find
 *          return NULL if no one match
 *          FIXME normal list would not be efficient
 *                but for a small system, isn't it enough?
 */
static struct _pcb_t *__pcb_from_pid(uint32 pid)
{
	struct _pcb_t *pcb;
	list_for_each_entry(pcb, &pqcb.pq, list) {
		if (pcb->pid == pid)
			return pcb;
	}
	return NULL;
}

/*
 * @brief   __pq_get_head
 * @param   p_pqcb -i- pointer of pqcb
 * @return  pcb at the queue head
 * @note
 */
static inline struct _pcb_t *__pq_get_head(const struct _pqcb_t *p_pqcb)
{
	return list_entry(p_pqcb->pq.next, struct _pcb_t, list);
}

/*
 * @brief   __pq_get_tail
 * @param   p_pqcb -i- pointer of pqcb
 * @return  pcb at the queue tail
 * @note
 */
static inline struct _pcb_t *__pq_get_tail(const struct _pqcb_t *p_pqcb)
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
static struct _pcb_t* __do_strategy_edfs(struct _pqcb_t *pqcb)
{
	struct _pcb_t *pcb = curr_pcb;
	return pcb;
}
#endif

#ifdef OUSIA_SCHED_STRATEGY_EDFS_OPT
/*
 * @brief   earliest deadline first scheduling, optimized for overall
 * @param   pqcb -i/o- process queue control block
 * @return  int32
 * @note
 */
static struct _pcb_t* __do_strategy_edfs_optimized(struct _pqcb_t *pqcb)
{
	struct _pcb_t *pcb = curr_pcb;
	return pcb;
}
#endif

#ifdef OUSIA_SCHED_STRATEGY_CFS
/*
 * @brief   completely fair scheduling
 * @param   pqcb -i/o- process queue control block
 * @return  int32
 * @note
 */
static struct _pcb_t* __do_strategy_cfs(struct _pqcb_t *pqcb)
{
	struct _pcb_t *pcb = curr_pcb;
	return pcb;
}
#endif

#ifdef OUSIA_SCHED_STRATEGY_HPFS
/*
 * @brief   highest priority first scheduling
 * @param   pqcb -i/o- process queue control block
 * @return  int32
 * @note
 */
static struct _pcb_t* __do_strategy_hpfs(struct _pqcb_t *pqcb)
{
	struct _pcb_t *pcb = curr_pcb;
	return pcb;
}
#endif

#ifdef OUSIA_SCHED_STRATEGY_RGHS
/*
 * @brief   rough scheduling
 * @param   pqcb -i/o- process queue control block
 * @return  pcb of the ready process scheduled out
 * @note    A rough scheduling indicates that the processes are being
 *          scheduled to run one by one in an apple-pie order. :P
 */
static struct _pcb_t* __do_strategy_rghs(struct _pqcb_t *pqcb)
{
	struct _pcb_t *pcb = curr_pcb;

	os_assert(pqcb != NULL);

	do {
		__pcb_dequeue(pcb);
		if (pcb->stat != PSTAT_KILLED)
			__pcb_enqueue(pcb);
		pcb = __pq_get_head(pqcb);
	} while (pcb->stat != PSTAT_READY);

	return pcb;
}
#endif

/*
 * @brief   hook to porting layer then asm code
 * @param   none
 * @return  none
 * @note    none
 */
void __sched_hard_fault_hook(void *args)
{
	_sched_dump_pcb(curr_pcb);
	_sched_dump_pq();
}

/*
 * @brief   called in systick for scheduler
 * @param   none
 * @return  none
 * @note    none
 */
void _sched_systick_call(void)
{
	struct _pcb_t *pcb;
	list_for_each_entry(pcb, &pqcb.pq, list) {
		switch (pcb->stat) {
		case PSTAT_BLOCKED:
			pcb->tcb.ticks_running = 0;
			break;
		case PSTAT_RUNNING:
			pcb->tcb.ticks_running++;
			break;
		case PSTAT_SLEEPING:
			pcb->tcb.ticks_sleeping--;
			if (pcb->tcb.ticks_sleeping == 0)
				pcb->stat = PSTAT_READY;
			break;
		case PSTAT_READY:
			/*
			 * FIXME How about this stat?
			 *       ticks_sleeping--, too?
			 */
			break;
		default:
			break;
		}
	}
}

/*
 * @brief   dump process control block
 * @param   p_pcb -i- pointer of pcb
 * @return  nothing
 * @note    FIXME this function should has more clear info
 */
void _sched_dump_pcb(const struct _pcb_t *p_pcb)
{
	if (p_pcb == NULL) {
		os_printk(LOG_ERROR, "%s - pcb is NULL\n", __func__);
		return;
	}
	os_printk(LOG_INFO, ">>> dump pcb: 0x%08X <<<\n", p_pcb);
	os_printk(LOG_INFO, "stack_ptr: 0x%08X\n", p_pcb->stack_ptr);
	os_printk(LOG_INFO, "pentry:    0x%08X\n", p_pcb->pentry);
	os_printk(LOG_INFO, "name:      %s\n", p_pcb->name);
	os_printk(LOG_INFO, "stack_sz:  %d\n", p_pcb->stack_sz);
	os_printk(LOG_INFO, "pid:       %d\n", p_pcb->pid);
	os_printk(LOG_INFO, "prio:      %d\n", p_pcb->prio);
	os_printk(LOG_INFO, "stat:      %d\n", p_pcb->stat);
	os_printk(LOG_INFO, "tcb:       0x%08X\n", &p_pcb->tcb);
	os_printk(LOG_INFO, "prev:      0x%08X\n", __pcb_get_prev(p_pcb));
	os_printk(LOG_INFO, "next:      0x%08X\n", __pcb_get_next(p_pcb));
	os_printk(LOG_INFO, "----------------------------\n");
}

/*
 * @brief   dump process queue
 * @param   p_pqcb -i- pointer of pqcb
 * @return  nothing
 * @note    currently, only one process queue, so dump it!
 */
void _sched_dump_pq(void)
{
	struct _pcb_t *pcb;
	const struct _pqcb_t *p_pqcb = &pqcb;

	os_printk(LOG_INFO, "%d processes in queue:\n", p_pqcb->n_pcb);
	os_printk(LOG_INFO, "   pid   pcb      state  prio      "
			    "run      sleep   name\n");
	list_for_each_entry(pcb, &p_pqcb->pq, list) {
		os_printk(LOG_INFO, " %4d  0x%8p  %2d   %4d "
				"%7d.%03d %4d.%03d   "
				"%s\n",
				pcb->pid, pcb, pcb->stat, pcb->prio,
				pcb->tcb.ticks_running/1000,
				pcb->tcb.ticks_running%1000,
				pcb->tcb.ticks_sleeping/1000,
				pcb->tcb.ticks_sleeping%1000,
				pcb->name);
	}
}

/*
 * @brief   start ousia scheduler to work
 * @param   pq -i/o- pointer to process queue list head
 * @return  process control block, basically for collecting statistics
 * @note    none
 */
const struct _pqcb_t* _sched_init(void)
{
	INIT_LIST_HEAD(&pqcb.pq);
	sched_class.sched_hook = NULL;
	sched_class.do_schedule = __do_strategy_rghs;
	port_hard_fault_attach(__sched_hard_fault_hook);
	return &pqcb;
}

/*
 * @brief   start a schedule
 * @param   none
 * @return  int32
 * @note    none
 */
void _sched_schedule(void)
{
	struct _pcb_t *tmp = curr_pcb;
	struct _pcb_t *ready = NULL;

	/* TODO This args should be able for collecting statistics */
	if (sched_class.sched_hook)
		sched_class.sched_hook(curr_pcb);

	ready = sched_class.do_schedule(&pqcb);
	os_assert(ready != NULL);

	if (ready != curr_pcb) {
		curr_pcb = ready;
		curr_pcb->stat = PSTAT_RUNNING;
		port_context_switch((uint32)tmp, (uint32)curr_pcb);
	}
}

/*
 * @brief   start our scheduler, os begin to run
 * @param   none
 * @return  none
 * @note    we should be getting into the first pendsv isr
 *          after first switch and never back again
 */
void _sched_startup(void)
{
	curr_pcb = __pq_get_head(&pqcb);
	curr_pcb->stat = PSTAT_RUNNING;
	port_first_switch((uint32)__pq_get_head(&pqcb));
	os_printk(LOG_ERROR, "%s, shoud never be here!\n");
	os_assert(0);
}

/*
 * @brief   get current process control block
 * @param   none
 * @return  current pcb
 * @note    none
 */
inline struct _pcb_t *_sched_get_curr_pcb(void)
{
	return curr_pcb;
}

/*
 * @brief   register a scheduler hook, called in every scheduling
 * @param   args is often a const value which cannot be modified externally
 * @return  none
 * @note    WARNING This hook should not take a long time!
 */
void _sched_attach_hook(void (*fn)(const void *args))
{
	sched_class.sched_hook = fn;
}

/*
 * @brief   dump stack api for user
 * @param   none
 * @return  none
 */
void os_dump_stack(void)
{
	_sched_dump_pcb(curr_pcb);
	port_dump_stack((pt_regs_t *)curr_pcb->stack_ptr);
}

/*
 * @brief   create a process
 * @param   pcb -i- pointer of process control block
 *          pentry -i- process main function entry
 *          name -i- process name
 *          args -i- process main function args
 *          stack_base -i- start address of stack
 *          stack_sz -i- process private stack size
 * @return  pid if create success
 */
int32 __os_process_create(void *pentry, char *name, void *args, uint32 stack_sz)
{
	struct _pcb_t *new_pcb = NULL;
	uint8 *stk = NULL;
	static unsigned int cnt = 0;

	if (pentry == NULL || !VALIDATE_SIZE(stack_sz))
		return -OS_EFAIL;

	new_pcb = (struct _pcb_t *)czalloc(sizeof(struct _pcb_t));
	if (new_pcb == NULL)
		goto err_pcb;

	stk = (uint8 *)czalloc(stack_sz);
	if (stk == NULL)
		goto err_stk;

#if (OUSIA_PORT_STACK_TYPE == OUSIA_PORT_STACK_DEC)
	/* FIXME ARCH_BIT can only be 32, hard code here :( */
	stk = stk + stack_sz - 4;
#endif

	new_pcb->stack_ptr = port_context_init(pentry, args, (void *)stk);
	new_pcb->pentry = pentry;
	new_pcb->name = name;
	new_pcb->pid = ++cnt;
	new_pcb->stack_sz = stack_sz;
	new_pcb->tcb.deadline = 0;
	new_pcb->tcb.ticks_sleeping = 0;
	new_pcb->tcb.ticks_running = 0;
	new_pcb->stat = PSTAT_READY;

	__pcb_enqueue(new_pcb);
	os_printk(LOG_DEBUG, "==> new: %s (%u)\n", name, new_pcb->pid);
	return new_pcb->pid;

err_stk:
	cfree(new_pcb);
err_pcb:
	return -OS_EFAIL;
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

	curr_pcb->stat = PSTAT_KILLED;
	_sched_schedule();

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

	/*
	 * FIXME Ticks should be calculated from tms * frequency_factor.
	 *       Here, for stm32, just 1 ms for each tick.
	 */
	curr_pcb->tcb.ticks_sleeping = tms;
	curr_pcb->stat = PSTAT_SLEEPING;

	/*
	 * call scheduler
	 * FIXME need to make sure everything is ready for process
	 * scheduling and context switch before start a schedule
	 */
	_sched_schedule();

	return ret;
}
int32 (*non_busy_wait)(uint32) = os_process_sleep;

/*
 * @brief   suspend a process
 * @param   pid -i- pid of target process
 * @return  int32
 * @note    FIXME only support intiative suspend for now
 */
int32 os_process_suspend(void)
{
	curr_pcb->stat = PSTAT_BLOCKED;
	os_printk(LOG_DEBUG, "%s, pid: %d %s\n",
			     __func__, curr_pcb->pid, curr_pcb->name);
	_sched_schedule();
	return OS_OK;
}

/*
 * @brief   resume a process
 * @param   pid -i- pid of target process
 * @return  int32
 * @note    FIXME should only resume those who went to PSTAT_BLOCKED
 *                by calling os_process_suspend
 */
int32 os_process_resume(uint32 pid)
{
	struct _pcb_t *pcb;
	pcb = __pcb_from_pid(pid);
	if (pcb == NULL) {
		os_printk(LOG_ERROR, "%s failed.\n", __func__);
		return -OS_EFAIL;
	}
	if (pcb->stat == PSTAT_BLOCKED)
		pcb->stat = PSTAT_READY;
	os_printk(LOG_INFO, "%s, pid: %d %s\n", __func__, pcb->pid, pcb->name);
	return OS_OK;
}

/*
 * @brief   yield the cpu resource
 * @param   none
 * @return  int32
 * @note    none
 */
int32 os_process_yield(void)
{
	int32 ret = OS_OK;

	curr_pcb->stat = PSTAT_READY;

	/*
	 * call scheduler
	 * FIXME need to make sure everything is ready for process
	 * scheduling and context switch before start a schedule
	 */
	_sched_schedule();

	return ret;
}
