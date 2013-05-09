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
 * @file    include/sys/sched.h
 * @brief   header of implementation of ousia scheduler
 * @log     2011.7 initial revision
 */

#ifndef __SYS_SCHED_H__
#define __SYS_SCHED_H__

#include <sys/ds.h>

/*
 * process state
 * FIXME need an exact requirement for each state
 */
enum _pstate {
	PSTAT_KILLED    = -2,
	PSTAT_BLOCKED   = -1,
	PSTAT_RUNNING   = 0,
	PSTAT_SLEEPING  = 1,
	PSTAT_READY     = 2,
};

/*
 * process control block
 */
struct _pcb_t {
	void *stack_ptr;
	void (*pentry)(void *args);
	char *name;
	uint32 stack_sz;
	uint32 pid;
	int32 prio;
	enum _pstate stat;
	struct _ptcb_t tcb;
	struct list_head list;
};

/*
 * process queue control block
 */
struct _pqcb_t {
	uint32 n_pcb;
	struct list_head pq;
	struct _pcb_t *p_head;
	struct _pcb_t *p_tail;
};

/*
 * scheduler class
 */
struct _sched_class_t {
	void (*sched_hook)(const void *args);
	struct _pcb_t* (*do_schedule)(struct _pqcb_t *pq);
};

void _sched_dump_pcb(const struct _pcb_t *p_pcb);
void _sched_dump_pq(void);
const struct _pqcb_t* _sched_init(void);
void _sched_schedule(void);
void _sched_startup(void);
void _sched_attach_hook(void (*fn)(const void *args));
void _sched_systick_call(void);
void os_dump_stack(void);
int32 __os_process_create(void *pentry, char *name,
			  void *args, uint32 stack_sz);
#define os_process_create(_pentry, _args, _stack_size) \
	do { \
		__os_process_create(_pentry, __stringify(_pentry), \
				    _args, _stack_size); \
	} while (0)
int32 os_process_delete(uint32 pid);
int32 os_process_sleep(uint32 tms);
int32 os_process_suspend(void);
int32 os_process_resume(uint32 pid);
int32 os_process_yield(void);


#endif /* __SYS_SCHED_H__ */
