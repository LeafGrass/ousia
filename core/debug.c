/* *****************************************************************************
 * @file    core/debug.c
 *
 * @brief   debuging services
 *
 * @log     2011-08-25 Initial revision
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
#include <ousia/scheduler.h>
#include <ousia/tprintf.h>
#include <ousia/sysutils.h>
#include <ousia/debug.h>

/*
 * @brief   os assert failing routine
 * @param   file -i- __FILE__
 *          line -i- __LINE__
 *          exp -i- assert expression
 * @return  none
 * @note    may call porting assert service or implemented solely
 */
void _os_assert_fail(const char *p_file, int line, const char *p_exp)
{
    _os_port_assert_fail(p_file, line, p_exp);
}

