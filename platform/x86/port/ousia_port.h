/* *****************************************************************************
 * @file    platform/x86/port/ousia_port.c
 *
 * @brief   porting code types and macros
 *
 * @log     2011-08-14 Initial revision
 *
 * *****************************************************************************
 * COPYRIGHT (C) LEAFGRASS - LeafGrass (g.leafgrass@gmail.com)
 *
 * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 * ARM SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR
 * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 * ****************************************************************************/

#ifndef __OUSIA_PORT_H__
#define __OUSIA_PORT_H__

#include <port/ousia_cfg.h>

typedef signed char int8;
typedef unsigned char uint8;
typedef signed short int16;
typedef unsigned short uint16;
typedef signed int int32;
typedef unsigned int uint32;
typedef signed long long int64;
typedef unsigned long long uint64;

#define OS_SET_INTERRUPT_MASK()
#define OS_CLEAR_INTERRUPT_MASK()

#define OS_DISABLE_INTERRUPTS() OS_SET_INTERRUPT_MASK()
#define OS_ENABLE_INTERRUPTS()  OS_CLEAR_INTERRUPT_MASK()

#define os_enter_critical() _os_enter_critical()
#define os_exit_critical()  _os_exit_critical()

void _os_enter_critical(void);
void _os_exit_critical(void);
void _os_port_init(void);
void _os_port_assert_fail(const char* file, int line, const char *exp);
void _port_init_printf(void **stdout_putp, void (**stdout_putf)(void *dev, char ch));
void _systick_register_callback(void (*callback)(void));

#endif /* OUSIA_PORT_H */

