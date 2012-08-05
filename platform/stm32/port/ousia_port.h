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
 * @file    platform/stm32/port/ousia_port.h
 * @brief   stm32 porting code types and macros
 * @log     2011.8 initial revision
 */

#ifndef __OUSIA_PORT_H__
#define __OUSIA_PORT_H__


#include <port/ousia_cfg.h>

#define OS_THROB_RATE   1000

#define OS_SET_INTERRUPT_MASK() \
	__asm volatile \
	( \
	  "   mov r0, %0         \n" \
	  "   msr basepri, r0    \n" \
	  ::"i"(200):"r0" \
	)

#define OS_CLEAR_INTERRUPT_MASK() \
        __asm volatile \
	( \
	  "   mov r0, #0         \n" \
	  "   msr basepri, r0    \n" \
	  :::"r0" \
	)

#define OS_DISABLE_INTERRUPTS() OS_SET_INTERRUPT_MASK()
#define OS_ENABLE_INTERRUPTS()  OS_CLEAR_INTERRUPT_MASK()

#define os_enter_critical() _os_enter_critical()
#define os_exit_critical()  _os_exit_critical()

extern uint32 var_dbg;

void _os_enter_critical(void);
void _os_exit_critical(void);
void _os_port_init(void);
void _port_assert_fail(const char *file, int line, const char *exp);
void _port_printf_init(void **stdout_putp, void (**stdout_putf)(void *dev, char ch));
void _port_systick_init(void (*callback)(void));
void _port_context_switch(uint32 curr_pcb, uint32 target_pcb) __attribute__ ((naked));
void _port_first_switch(uint32 target_pcb) __attribute__ ((naked));
uint32 *_port_context_init(void *pentry, void *args, void *stack_base);


#endif /* OUSIA_PORT_H */
