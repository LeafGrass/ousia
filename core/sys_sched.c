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

#include <port/ousia_cfg.h>
#include <port/ousia_port.h>
#include <ousia/ousia_type.h>
#include <sys/print.h>
#include <sys/time.h>
#include <sys/sched.h>


typedef os_status (*do_sched_func_t)(struct _pqcb_t *pq);

static void __schedule_assign(do_sched_func_t func_strategy);
static os_status __do_strategy_edfs(struct _pqcb_t *pq);
static os_status __do_strategy_edfs_optimized(struct _pqcb_t *pq);
static os_status __do_strategy_cfs(struct _pqcb_t *pq);
static os_status __do_strategy_hpfs(struct _pqcb_t *pq);
static void __ps_init(void *args);
static void __ps_idle(void *args);

static do_sched_func_t __do_schedule = NULL;

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

/*
 * @brief   create a process
 * @param   pentry -i- process entry
 * @return  pid if create success
 * @note    none
 */
int32 os_process_create(void *pentry)
{
	struct _pcb_t *pcb = NULL;

	/* TODO here to allocate resources to a process */

	if (pcb == NULL)
		return -1;
	return pcb->pid;
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
 * @param   strategy -i- scheduling strategy
 * @return  os_status
 * @note    none
 */
os_status _sys_sched_init(uint32 strategy)
{
	os_status ret = OS_OK;
	switch (strategy) {
	case OUSIA_SCHED_STRATEGY_EDFS:
		__schedule_assign(&__do_strategy_edfs);
		break;
	case OUSIA_SCHED_STRATEGY_EDFS_OPT:
		__schedule_assign(&__do_strategy_edfs_optimized);
		break;
	case OUSIA_SCHED_STRATEGY_HPFS:
		__schedule_assign(&__do_strategy_hpfs);
		break;
	case OUSIA_SCHED_STRATEGY_CFS:
		__schedule_assign(&__do_strategy_cfs);
		break;
	default:
		__schedule_assign(&__do_strategy_edfs_optimized);
		break;
	}
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

	/* TODO create two processes at init, then initialize curr_pcb & pqcb */
	os_process_create(__ps_init);
	os_process_create(__ps_idle);

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

	ret = __do_schedule(&pqcb);

	/* TODO here to trigger os context switch */

	return ret;
}

/*
 * @brief   assign a real schedule strategy to _sched_schedule
 * @param   none
 * @return  none
 * @note    FIXME is this function a waste of ram?
 */
static void __schedule_assign(do_sched_func_t func_strategy)
{
	__do_schedule = func_strategy;
}

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

/*
 * @brief   process - init
 * @param   args -i/o- reserved
 * @return  void
 */
static void __ps_init(void *args)
{
	os_process_suspend(curr_pcb.pid);
	return;
}

/*
 * @brief   process - idle
 * @param   args -i/o- reserved
 * @return  void
 */
static void __ps_idle(void *args)
{
	os_process_sleep(1000);
	return;
}
