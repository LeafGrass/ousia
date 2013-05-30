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
 * @file    platform/armv5/port/ousia_port.c
 * @brief   armv5 port
 * @log     2013.5 initial revision
 */

#include <ousia/ousia.h>
#include <ousia/ousia_type.h>
#include <sys/debug.h>
#include <sys/print.h>
#include <sys/time.h>
#include <common/utils.h>
#include <port/ousia_port.h>


/*
 * CPU Mode
 */
#define USERMODE	0x10
#define FIQMODE		0x11
#define IRQMODE		0x12
#define SVCMODE		0x13
#define ABORTMODE	0x17
#define UNDEFMODE	0x1b
#define MODEMASK	0x1f
#define NOINT		0xc0

extern uint32 _lm_heap_start;
extern uint32 _lm_heap_end;

uint8 *__heap_start;
uint8 *__heap_end;

static uint32 critical_nest = 0;
static volatile uint32 old_pcb = 0;
static volatile uint32 new_pcb = 0;

static void (*__hard_fault_call)(void *args);

void __exc_pendsv(void) __attribute__ ((naked));
void __exc_svc(void) __attribute__ ((naked));

static void __busy_wait(uint32 ms)
{
}

/*
 * @brief   porting related init
 * @param   none
 * @return  none
 * @note    FIXME Do it later than or before bsp_init?
 */
void port_init(void)
{
	old_pcb = 0;
	new_pcb = 0;
	critical_nest = 0;
	__heap_start = (uint8 *)&_lm_heap_start;
	__heap_end = __heap_start + OUSIA_MM_HEAP_SIZE;
	__heap_end = (__heap_end > (uint8 *)&_lm_heap_end) ?
		(uint8 *)&_lm_heap_end : __heap_end;
}

/*
 * @brief   bsp related init
 * @param   none
 * @return  none
 * @note    none
 */
void port_bsp_init(void)
{
	bsp_init();
}

/*
 * @brief   attach to hard fault handler
 * @param   none
 * @return  none
 * @note    none
 */
void port_hard_fault_attach(void (*fn)(void *args))
{
	__hard_fault_call = fn;
}

/*
 * @brief   band printf callback to low-level io control
 * @param   stdout_putp -i/o- generally none
 *          stdout_putf -i/o- low-level printf specific io implementation
 * @return  none
 * @note    none
 */
void port_printf_init(void (**stdout_putf)(void *dev, char ch),
		      char (**stdin_getf)(void *dev))
{
	*stdout_putf = __io_putc;
	*stdin_getf = __io_getc;
}

/*
 * @brief   register callback function of system tick handler and init
 * @param   callback -i- pointer to callback function
 * @return  none
 */
void port_systick_init(void (*callback)(void))
{
}

/*
 * @brief   process private context initialize
 * @param   pentry -i- process main function entry
 *          args -i- process main function args
 *          stack_base -i- start address of stack
 * @return  pointer to initialized stack
 * @note    TODO we may not need to initialize each reg
 *               init value of lr needs to be confirmed
 */
uint32 *port_context_init(void *pentry, void *args, void *stack_base)
{
	uint32 *stack = NULL;
	stack = (uint32 *)stack_base;

	*stack     = (uint32)pentry;	/* pc */
	*(--stack) = (uint32)pentry;	/* lr */
	*(--stack) = 0;			/* r12 */
	*(--stack) = 0;			/* r11 */
	*(--stack) = 0;			/* r10 */
	*(--stack) = 0;			/* r9 */
	*(--stack) = 0;			/* r8 */
	*(--stack) = 0;			/* r7 */
	*(--stack) = 0;			/* r6 */
	*(--stack) = 0;			/* r5 */
	*(--stack) = 0;			/* r4 */
	*(--stack) = 0;			/* r3 */
	*(--stack) = 0;			/* r2 */
	*(--stack) = 0;			/* r1 */
	*(--stack) = (uint32)args;	/* r0 */
	*(--stack) = SVCMODE;		/* cpsr */
	*(--stack) = SVCMODE;		/* spsr */

	/* return task's current stack address */
	return stack;
}

/*
 * @brief   assert fail alarm
 * @param   none
 * @return  none
 * @note    may not needed
 */
void port_assert_fail(void)
{
	while (1) {
		__busy_wait(250);
	}
}

/*
 * @brief   dump the stack of specific pcb
 * @param   p_pcb -i- pointer of pcb
 * @return  nothing
 * @note    FIXME need to resolve big/little endian
 */
void port_dump_stack(const pt_regs_t *pt)
{
	if (pt == NULL) {
		os_printk(LOG_ERROR, "%s - pt_regs is NULL, ignored\n", __func__);
		return;
	}
	os_printk(LOG_CRITICAL, "pc:   0x%08X\n", pt->pc);
	os_printk(LOG_CRITICAL, "lr:   0x%08X\n", pt->lr);
	os_printk(LOG_CRITICAL, "r12:  0x%08X\n", pt->r12);
	os_printk(LOG_CRITICAL, "r11:  0x%08X\n", pt->r11);
	os_printk(LOG_CRITICAL, "r10:  0x%08X\n", pt->r10);
	os_printk(LOG_CRITICAL, "r9:   0x%08X\n", pt->r9);
	os_printk(LOG_CRITICAL, "r8:   0x%08X\n", pt->r8);
	os_printk(LOG_CRITICAL, "r7:   0x%08X\n", pt->r7);
	os_printk(LOG_CRITICAL, "r6:   0x%08X\n", pt->r6);
	os_printk(LOG_CRITICAL, "r5:   0x%08X\n", pt->r5);
	os_printk(LOG_CRITICAL, "r4:   0x%08X\n", pt->r4);
	os_printk(LOG_CRITICAL, "r3:   0x%08X\n", pt->r3);
	os_printk(LOG_CRITICAL, "r2:   0x%08X\n", pt->r2);
	os_printk(LOG_CRITICAL, "r1:   0x%08X\n", pt->r1);
	os_printk(LOG_CRITICAL, "r0:   0x%08X\n", pt->r0);
	os_printk(LOG_CRITICAL, "spsr: 0x%08X\n", pt->cpsr);
	os_printk(LOG_CRITICAL, "cpsr: 0x%08X\n", pt->cpsr);
}

/*
 * @brief   context switch routine
 * @param   curr_pcb (r0) -i- addr of old process control block
 *          target_pcb (r1) -i- addr of new process control block
 * @return  none
 * @note    none
 */
void port_context_switch(uint32 curr_pcb, uint32 target_pcb)
{
	__asm volatile
	(
	 "	bx	lr				\n"
	);
}

/*
 * @brief   first switch for os start
 * @param   target_pcb (r0) -i- addr of new process control block
 * @return  none
 * @note    none
 */
void port_first_switch(uint32 target_pcb)
{
	__asm volatile
	(
	 "	bx	lr				\n"
	);
}

/*
 * @brief   pendsv exception handler
 * @param   none
 * @return  none
 * @note    none
 */
void __exc_pendsv(void)
{
	__asm volatile
	(
	 "	bx	lr				\n"
	);
}

/*
 * @brief   svc exception handler
 * @param   none
 * @return  none
 * @note    none
 */
void __exc_svc(void)
{
}
