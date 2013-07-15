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
 * @file    platform/sim/port/ousia_port.c
 * @brief   sim porting code types and macros
 * @log     2011.8 initial revision
 */

#ifndef __OUSIA_PORT_H__
#define __OUSIA_PORT_H__


#include <port/ousia_cfg.h>

#define OS_SET_INTERRUPT_MASK()
#define OS_CLEAR_INTERRUPT_MASK()

#define OS_DISABLE_INTERRUPTS() OS_SET_INTERRUPT_MASK()
#define OS_ENABLE_INTERRUPTS()  OS_CLEAR_INTERRUPT_MASK()

struct pt_regs {
	uint32 reserved;
};
typedef struct pt_regs pt_regs_t;

void port_init(void);
void port_bsp_init(void);
void port_hard_fault_attach(void (*fn)(void *args));
void port_printf_init(void (**stdout_putf)(void *dev, char ch),
		      char (**stdin_getf)(void *dev));
void port_systick_init(void (*callback)(void));
void *port_context_init(void *pentry, void *args, void *stack_base);
void port_assert_fail(void);
void port_dump_stack(const pt_regs_t *pt);
void port_context_switch(uint32 curr_pcb, uint32 target_pcb);
void port_first_switch(uint32 target_pcb);


#endif /* OUSIA_PORT_H */
