/* *****************************************************************************
 * @file    core/sys_debug.c
 *
 * @brief   debuging services
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
#include <sys/sys_sched.h>
#include <sys/sys_print.h>
#include <sys/sys_utils.h>
#include <sys/sys_debug.h>

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

