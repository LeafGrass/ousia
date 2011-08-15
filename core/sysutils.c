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
#include <ulib/stdarg.h>
#include <ousia/sysutils.h>

#ifdef OUSIA_USE_TPRINTF
#include <ulib/tprintf.h>

void init_os_printf(void *putp, void (*putf)(void *, char))
{
    init_printf(putp, putf);
}
int os_printf(const char *p_fmt, ...)
{
    va_list args;
    int r = 0;

    va_start(args, p_fmt);
    tfp_printf(p_fmt, args);
    va_end(args);

    return r;
}
void os_sprintf(char *p_buf, const char *p_fmt, ...)
{
    va_list args;

    va_start(args, p_fmt);
    tfp_sprintf(p_buf, p_fmt, args);
    va_end(args);
}

#else  /* OUSIA_USE_TPRINTF */
/*
 * @brief   init ousia print mechanism
 * @param   putp -i- NULL
 *          putf -i- callback function of hardware ioputc driver
 * @return  an integer
 * @note    none
 */
void init_os_printf(void *putp, void (*putf)(void *, char))
{
    return;
}

/*
 * @brief   ousia print mechanism
 * @param   fmt -i- a formatted string
 * @return  an integer
 * @note    none
 */
int os_printf(const char *p_fmt, ...)
{
    va_list args;
    int r = 0;

    va_start(args, p_fmt);
    va_end(args);

    return r;
}

/*
 * @brief   write to buffer as formatted string
 * @param   p_buf -o- a buffer to store output data
 *          p_fmt -i- a formatted string
 * @return  none
 * @note    none
 */
void os_sprintf(char *p_buf, const char *p_fmt, ...)
{
    return;
}
#endif /* OUSIA_USE_TPRINTF */

