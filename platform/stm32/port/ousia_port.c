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

#include <ousia/ousia.h>
#include <ousia/ousia_type.h>
#include <sys/debug.h>
#include <sys/print.h>
#include <sys/time.h>
#include <libmaple/systick.h>
#include <libmaple/util.h>
#include <libmaple/delay.h>
#include <libmaple/gpio.h>
#include <stm32utils/stm32utils.h>
#include <port/ousia_port.h>


#define PSR_INIT_VALUE	0x01000000L

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
	while (ms--)
		delay_us(1000);
}

/*
 * @brief   default exception hook, called from asm code
 * @param   p_pcb -i- pointer of pcb
 * @return  nothing
 * @note    For hard fault handler callback.
 *          When hard fault occurs, psp locates to r0,
 *          so (psp - 8 * 4) bytes is the entry of pt_regs (r4).
 *          Even though psp might be not that useful to upper level,
 *          still pass it back anyway.
 */
static void __hard_fault_handler(uint32 psp, uint32 exc_num)
{
	pt_regs_t *p;

	/*
	 * FIXME It seems only thread mode fault can we get a
	 *       **correct** pt_regs table?
	 *       However, in fact, it may not like this, the case in handler
	 *       mode leads to **incorrect** might because the stack is mussed
	 *       by us!
	 */
	p = (pt_regs_t *)(psp - 32);
	os_printk(LOG_CRITICAL, "%s - psp: 0x%08X, exception number: %d\n",
			__func__, psp, exc_num);
	port_dump_stack(p);

	__hard_fault_call((void *)psp);
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
	attach_exc_hook(__hard_fault_handler);
}

/*
 * @brief   bsp related init
 * @param   none
 * @return  none
 * @note    none
 */
void port_bsp_init(void)
{
	int i;

	stm32utils_board_init();

	/*
	 * led flashes, sign of system starts to run
	 * FIXME at least 1000+ms to wait for usb device, or the foregoing
	 * characters may be lost, that is too long to wait ...
	 */
	for (i = 0; i < 24; i++) {
		gpio_toggle_bit(ERROR_LED_PORT, ERROR_LED_PIN);
		__busy_wait(50);
	}
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
void port_printf_init(void **stdout_putp, void (**stdout_putf)(void *dev, char ch))
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
void port_systick_init(void (*callback)(void))
{
	/* api of libmaple */
	systick_attach_callback(callback);
	systick_init(SYSTICK_RELOAD_VAL);
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
 * @brief   assert fail alarm
 * @param   none
 * @return  none
 * @note    may not needed
 */
void port_assert_fail(void)
{
	while (1) {
		gpio_toggle_bit(ERROR_LED_PORT, ERROR_LED_PIN);
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
	os_printk(LOG_CRITICAL, "xpsr: 0x%08X\n", pt->xpsr);
	os_printk(LOG_CRITICAL, "pc:   0x%08X\n", pt->pc);
	os_printk(LOG_CRITICAL, "lr:   0x%08X\n", pt->lr);
	os_printk(LOG_CRITICAL, "r12:  0x%08X\n", pt->r12);
	os_printk(LOG_CRITICAL, "r3:   0x%08X\n", pt->r3);
	os_printk(LOG_CRITICAL, "r2:   0x%08X\n", pt->r2);
	os_printk(LOG_CRITICAL, "r1:   0x%08X\n", pt->r1);
	os_printk(LOG_CRITICAL, "r0:   0x%08X\n", pt->r0);
	os_printk(LOG_CRITICAL, "r11:  0x%08X\n", pt->r11);
	os_printk(LOG_CRITICAL, "r10:  0x%08X\n", pt->r10);
	os_printk(LOG_CRITICAL, "r9:   0x%08X\n", pt->r9);
	os_printk(LOG_CRITICAL, "r8:   0x%08X\n", pt->r8);
	os_printk(LOG_CRITICAL, "r7:   0x%08X\n", pt->r7);
	os_printk(LOG_CRITICAL, "r6:   0x%08X\n", pt->r6);
	os_printk(LOG_CRITICAL, "r5:   0x%08X\n", pt->r5);
	os_printk(LOG_CRITICAL, "r4:   0x%08X\n", pt->r4);
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
	 /* store necessary regs */
	 "	push	{r4, r5}			\n"
	 /* store pcb instances to local */
	 "	ldr	r4, =old_pcb			\n"
	 "	str	r0, [r4]			\n"
	 "	ldr	r5, =new_pcb			\n"
	 "	str	r1, [r5]			\n"
	 /* trigger a pendsv exception */
	 "	ldr	r4, =0xE000ED04			\n"
	 "	ldr	r5, =0x10000000			\n"
	 "	str	r5, [r4]			\n"
	 /* restore pushed regs and go back to wait pendsv */
	 "	pop	{r4, r5}			\n"
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
	 /* store necessary regs */
	 "	push	{r4, r5}			\n"
	 /* reset psp */
	 "	mov	r4, #0				\n"
	 "	msr	psp, r4				\n"
	 /* load addr of new_pcb in ram to local*/
	 "	ldr	r5, =new_pcb			\n"
	 /* update new_pcb by target_pcb */
	 "	str	r0, [r5]			\n"
	 /* set pendsv interrupt priority as lowest */
	 "	ldr	r4, =0xE000ED20			\n"
	 "	ldr	r5, =0x00FF0000			\n"
	 "	str	r5, [r4]			\n"
	 /* trigger a pendsv exception */
	 "	ldr	r4, =0xE000ED04			\n"
	 "	ldr	r5, =0x10000000			\n"
	 "	str	r5, [r4]			\n"
	 /* restore pushed regs and go back to wait pendsv */
	 "	pop	{r4, r5}			\n"
	 /* enable interrupts at processer level */
	 "	cpsie	i				\n"
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
	 /* backup interrupts' mask status */
	 "	mrs	r2, primask			\n"
	 /* disable interrups in processor level */
	 "	cpsid	i				\n"
	 /* get current psp */
	 "	mrs	r0, psp				\n"
	 /* if first switch, skip regs save and old_pcb load */
	 "	cbz	r0, __pendsv_skip		\n"
	 /* store r4-r11*/
	 "	stmfd	r0!, {r4-r11}			\n"
	 /* load ram addr of old_pcb into r1 */
	 "	ldr	r1, =old_pcb			\n"
	 /* load content of old_pcb (addr of sp) into r1 */
	 "	ldr	r1, [r1]			\n"
	 /* save psp to this sp */
	 "	str	r0, [r1]			\n"
	 "__pendsv_skip:				\n"
	 /* load new pcb to into r0 */
	 "	ldr	r0, =new_pcb			\n"
	 /* load ram addr of new pcb into r0 */
	 "	ldr	r0, [r0]			\n"
	 /* load sp of new pcb into r0 */
	 "	ldr	r0, [r0]			\n"
	 /* restore r4-r11 */
	 "	ldmfd	r0!, {r4-r11}			\n"
	 /* save sp of new pcb to psp*/
	 "	msr	psp, r0				\n"
	 /* restore interrupts' mask status */
	 "	msr	primask, r2			\n"
	 /* ensure exception returns uses psp */
	 "	orr	lr, lr, #0x04			\n"
	 /* let's move! */
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
