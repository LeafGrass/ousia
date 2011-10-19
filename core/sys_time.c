/* *****************************************************************************
 * @file    core/sys_time.c
 *
 * @brief   timer related routines
 *
 * @log     2011.8 initial revision
 *
 * *****************************************************************************
 * COPYRIGHT (C) LEAFGRASS - LeafGrass (leafgrass.g@gmail.com)
 *
 * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 * ARM SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR
 * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 * ****************************************************************************/

#include <port/ousia_port.h>
#include <ousia/ousia.h>
#include <ousia/ousia_type.h>
#include <sys/print.h>
#include <sys/time.h>

static unsigned long long _systime;

static void __systick_interrupt(void);

/*
 * @brief   os timer init
 * @param   none
 * @return  none
 * @note    none
 */
void _os_timer_init(void)
{
    _systime = 0UL;
    _systick_register_callback(&__systick_interrupt);
}

/*
 * @brief   os systick interrupt handler
 * @param   none
 * @return  none
 * @note    none
 */
static void __systick_interrupt(void)
{
    os_enter_critical();
    _systime++;
    os_exit_critical();

    return;
}

