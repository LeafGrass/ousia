/* *****************************************************************************
 * @file    core/port/ousia_port.c
 *
 * @brief   ousia system utilities
 *
 * @log     2011-08-14 Initial revision
 *
 * *****************************************************************************
 * COPYRIGHT (C) LEAFGRASS - Librae (g.leafgrass@gmail.com)
 *
 * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 * ARM SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR
 * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 * ****************************************************************************/

#include <stm32/libmaple/systick.h>
#include <stm32/stm32utils/stm32utils.h>
#include <ousia/tprintf.h>
#include <port/ousia_port.h>

static uint32 critical_nest;

static void __systick_register_callback(void (*callback)(void));
static void __port_systick_handler(void);
static void __port_init_printf(void *putp,void (*putf) (void*, char));

/*
 * @brief   porting related init
 * @param   none
 * @return  none
 * @note    none
 */
void _os_port_init(void)
{
    __systick_register_callback(&__port_systick_handler);
    __port_init_printf(NULL, stm32utils_io_putc);
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
 * @brief   register callback function of system tick handler
 * @param   pointer to callback function
 * @return  none
 * @note    WARNING if libmaple for stm32 is used, this function must be called
 */
static void __systick_register_callback(void (*callback)(void))
{
    systick_attach_callback(callback);
}

/*
 * @brief   systick timer interrupt user handler
 * @param   none
 * @return  none
 * @note    none
 */
static void __port_systick_handler(void)
{
    return;
}

/*
 * @brief   band printf callback to low-level io control
 * @param   putp -i- generally none
 *          putf -i- low-level printf specific io implementation
 * @return  none
 * @note    none
 */
static void __port_init_printf(void *putp,void (*putf)(void *, char))
{
    init_printf(putp, putf);
}

