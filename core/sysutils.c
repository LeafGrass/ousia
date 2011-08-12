/* *****************************************************************************
 * @file    core/sysutils.c
 *
 * @brief   Ousia system utilities
 *
 * @log     2011-08-12 Initial revision
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

#include <ousia/sysutils.h>

#if (1)
#else
#include <stdarg.h>
/*
 * @brief   ousia print mechanism
 * @param   fmt -i- a formatted string
 * @return  an integer
 * @note    none
 */
int os_printf(const char *fmt, ...)
{
    va_list args;
    int r = 0;

    va_start(args, fmt);
    va_end(args);

    return r;
}
#endif

