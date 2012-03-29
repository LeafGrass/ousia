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
 * @file    platform/x86/port/ousia_port.c
 * @brief   x86 virtual port
 * @log     2011.8 initial revision
 */

#include <stdlib.h>
#include <x86/x86utils/x86utils.h>
#include <port/ousia_port.h>

static unsigned int critical_nest;

/*
 * @brief   porting related init
 * @param   none
 * @return  none
 * @note    none
 */
void _os_port_init(void)
{
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
        if (critical_nest == 0)
        {
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
        return;
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
        *stdout_putf = x86utils_io_putc;
}

/*
 * @brief   register callback function of system tick handler and init
 * @param   pointer to callback function
 * @return  none
 * @note    WARNING if libmaple for stm32 is used, this function must be called
 */
void _port_systick_init(void (*callback)(void))
{
        x86utils_attach_systick_callback(callback);
	/* here call fake systick enable */
        return;
}

/*
 * @brief   context switch routine
 * @param   curr_pcb (r0) -i- old process control block
 *          target_pcb (r1) -i- new process control block
 * @return  none
 * @note    FIXME needs verification and complete
 */
void _port_context_switch(void *curr_pcb, void *target_pcb)
{
}

/*
 * @brief   first switch for os start
 * @param   target_pcb (r0) -i- new process control block
 * @return  none
 * @note    none
 */
void _port_first_switch(void *target_pcb)
{
}

/*
 * @brief   process private context initialize
 * @param   pentry -i- process main function entry
 *          args -i- process main function args
 *          stack_base -i- start address of stack
 * @return  pointer to initialized stack
 * @note    This is a fake stack init function
 */
void *_port_context_init(void *pentry, void *args, void *stack_base)
{
	void *stack;
	stack = stack_base;
	return stack;
}
