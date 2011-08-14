/* *****************************************************************************
 * @file    core/scheduler.c
 *
 * @brief   implementation of ousia scheduler
 *
 * @log     2011-07-28 Initial revision
 *
 * *****************************************************************************
 * COPYRIGHT (C) LEAFGRASS - Librae (librae8226@gmail.com)
 *
 * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 * ARM SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR
 * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 * ****************************************************************************/

#include <ousia/ousia.h>
#include <ousia/scheduler.h>

static os_status _scheduler_start(void);

/*
 * @brief   start ousia scheduler to work
 * @param   none
 * @return  os_status
 * @note    none
 */
void os_start(void)
{
    os_status stat = OS_ERR;
    stat = _scheduler_start();
}

/*
 * @brief   start ousia scheduler to work
 * @param   none
 * @return  os_status
 * @note    none
 */
static os_status _scheduler_start(void)
{
    os_status stat = OS_OK;
    return stat;
}

