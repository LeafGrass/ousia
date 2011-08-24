/* *****************************************************************************
 * @file    core/sysutils.c
 *
 * @brief   ousia system utilities
 *
 * @log     2011-08-12 Initial revision
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

#include <port/ousia_port.h>
#include <ousia/ousia.h>
#include <ousia/systimer.h>
#include <ousia/scheduler.h>
#include <ousia/tprintf.h>
#include <ousia/sysutils.h>

/*
 * @brief   the very first init of ousia
 * @param   none
 * @return  none
 * @note    this function should be called before all other syscalls
 */
os_status os_init(void)
{
    os_status stat = OS_OK;

    _os_port_init();
    _init_printf();
    _os_timer_init();
    stat = _scheduler_init();

    return stat;
}


