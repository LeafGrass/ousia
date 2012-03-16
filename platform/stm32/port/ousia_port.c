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
 * @file    platform/stm32/port/ousia_port.c
 * @brief   stm32 port
 * @log     2011.8 initial revision
 */

#include <ousia/ousia_type.h>
#include <stm32/libmaple/systick.h>
#include <stm32/libmaple/util.h>
#include <stm32/stm32utils/stm32utils.h>
#include <port/ousia_port.h>


#define PSR_INIT_VALUE	0x01000000L

static uint32 critical_nest;
static volatile void *old_pcb;
static volatile void *new_pcb;

void __exc_pendsv(void) __attribute__ ((naked));
void __exc_svc(void) __attribute__ ((naked));

#if 0
static void __port_systick_handler(void);
#endif

/*
 * @brief   porting related init
 * @param   none
 * @return  none
 * @note    none
 */
void _os_port_init(void)
{
	old_pcb = NULL;
	new_pcb = NULL;
	return;
}

/*
 * @brief   enter critical
 * @param   none
 * @return  none
 * @note    none
 */
void _os_enter_critical(void)
{
	OS_DISABLE_INTERRUPTS();
	critical_nest++;
}

/*
 * @brief   exit critical
 * @param   none
 * @return  none
 * @note    none
 */
void _os_exit_critical(void)
{
	critical_nest--;
	if (critical_nest == 0)	{
		OS_ENABLE_INTERRUPTS();
	}
}

/*
 * @brief   a simple wrap of lower level assert
 * @param   file -i- __FILE__
 *          line -i- __LINE__
 *          exp -i- assert expression
 * @return  none
 * @note    may not needed
 */
void _port_assert_fail(const char* file, int line, const char *exp)
{
	_fail(file, line, exp);
}

/*
 * @brief   band printf callback to low-level io control
 * @param   stdout_putp -i/o- generally none
 *          stdout_putf -i/o- low-level printf specific io implementation
 * @return  none
 * @note    none
 */
void _port_printf_init(void **stdout_putp, void (**stdout_putf)(void *dev, char ch))
{
	*stdout_putp = NULL;
#if (OUSIA_PRINT_TYPE == OUSIA_PRINT_TYPE_USB)
	*stdout_putf = stm32utils_usb_putc;
#else
	*stdout_putf = stm32utils_io_putc;
#endif
}

/*
 * @brief   register callback function of system tick handler and init
 * @param   callback -i- pointer to callback function
 * @return  none
 * @note    WARNING if libmaple for stm32 is used, this function must be called
 */
void _port_systick_init(void (*callback)(void))
{
	systick_attach_callback(callback);
	systick_init(SYSTICK_RELOAD_VAL);
}

#if 0
/*
 * @brief   systick timer interrupt user handler
 * @param   none
 * @return  none
 * @note    see _systick_interrupt for real implementation
 */
static void __port_systick_handler(void)
{
	return;
}
#endif

/*
 * @brief   context switch routine
 * @param   curr_pcb (r0) -i- old process control block
 *          target_pcb (r1) -i- new process control block
 * @return  none
 * @note    none
 */
void _port_context_switch(void *curr_pcb, void *target_pcb)
{
	__asm volatile
	(
	 /* store necessary regs */
	 "	push	{r4, r5}			\n"
	 /* store pcb instances to local */
	 "	ldr	r4, old_pcb_local		\n"
	 "	ldr	r5, new_pcb_local		\n"
	 "	str	r0, [r4]			\n"
	 "	str	r1, [r5]			\n"
	 /* trigger a pendsv exception */
	 "	ldr	r4, =0xE000ED04			\n"
	 "	ldr	r5, =0x10000000			\n"
	 "	str	r5, [r4]			\n"
	 /* restore pushed regs and go back to wait pendsv */
	 "	pop	{r4, r5}			\n"
	 "	bx	lr				\n"
	 "old_pcb_local: .word old_pcb			\n"
	 "new_pcb_local: .word new_pcb			\n"
	);
}

/*
 * @brief   first switch for os start
 * @param   target_pcb (r0) -i- new process control block
 * @return  none
 * @note    none
 */
void _port_first_switch(void *target_pcb)
{
	__asm volatile
	(
	 /* store necessary regs */
	 "	push	{r4, r5}			\n"
	 /* store pcb instances to local */
	 "	ldr	r4, old_pcb_first		\n"
	 "	ldr	r5, new_pcb_first		\n"
	 /* set old_pcb as null */
	 "	mov	r4, #0				\n"
	 "	str	r1, [r5]			\n"
	 /* set pendsv interrupt priority as lowest */
	 "	ldr	r4, =0xE000ED20			\n"
	 "	ldr	r5, =0x00FF0000			\n"
	 "	str	r5, [r4]			\n"
	 /* reset psp */
	 "	mov	r4, #0				\n"
	 "	msr	psp, r4				\n"
	 /* trigger a pendsv exception */
	 "	ldr	r4, =0xE000ED04			\n"
	 "	ldr	r5, =0x10000000			\n"
	 "	str	r5, [r4]			\n"
	 /* restore pushed regs and go back to wait pendsv */
	 "	pop	{r4, r5}			\n"
	 "	cpsie	i				\n"
	 "old_pcb_first: .word old_pcb			\n"
	 "new_pcb_first: .word new_pcb			\n"
	);
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
uint32 *_port_context_init(void *pentry, void *args, void *stack_base)
{
	uint32 *stack;
	stack = (uint32 *)stack_base;

	*stack     = PSR_INIT_VALUE;	/* xpsr */
	*(--stack) = (uint32)pentry;	/* pc */
	*(--stack) = (uint32)pentry;	/* lr */
	*(--stack) = 0;			/* r12 */
	*(--stack) = 0;			/* r3 */
	*(--stack) = 0;			/* r2 */
	*(--stack) = 0;			/* r1 */
	*(--stack) = (uint32)args;	/* r0 */
	*(--stack) = 0;			/* r11 */
	*(--stack) = 0;			/* r10 */
	*(--stack) = 0;			/* r9 */
	*(--stack) = 0;			/* r8 */
	*(--stack) = 0;			/* r7 */
	*(--stack) = 0;			/* r6 */
	*(--stack) = 0;			/* r5 */
	*(--stack) = 0;			/* r4 */

	return stack;
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
	 /* backup interrupts' mask status */
	 "	mrs	r3, primask			\n"
	 /* disable interrups */
	 "	cpsid	i				\n"
	 /* get current psp */
	 "	mrs	r0, psp				\n"
	 /* store r4-r11*/
	 "	stmfd	r0!, {r4-r11}			\n"
	 /* update sp of current pcb with psp */
	 "	ldr	r1, old_pcb_const		\n"
	 "	ldr	r1, [r1]			\n"
	 "	str	r0, [r1]			\n"
	 /* load new pcb to into r4 */
	 "	ldr	r0, new_pcb_const		\n"
	 /* load sp of new pcb into r4 */
	 "	ldr	r0, [r0]			\n"
	 "	ldr	r0, [r0]			\n"
	 /* restore r4-r11 */
	 "	ldmfd	r0!, {r4-r11}			\n"
	 /* save sp of new pcb to psp*/
	 "	msr	psp, r0				\n"
	 /* restore interrupts' mask status */
	 "	msr	primask, r3			\n"
	 /* ensure exception returns uses psp */
	 "	orr	lr, lr, #0x04			\n"
	 /* let's move! */
	 "	bx	lr				\n"
	 "old_pcb_const: .word old_pcb			\n"
	 "new_pcb_const: .word new_pcb			\n"
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
