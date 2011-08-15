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
#include <ousia/sysutils.h>

/*
 * @brief   init ousia print mechanism
 * @param   putp -i- NULL
 *          putf -i- callback function of hardware ioputc driver
 * @return  an integer
 * @note    none
 */

