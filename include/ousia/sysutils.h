/* *****************************************************************************
 * @file    core/sysutils.h
 *
 * @brief   Header of sysutils.c
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

#ifndef __SYSUTILS_H__
#define __SYSUTILS_H__

#if (1)
#define init_os_printf  init_printf
#define os_printf   tfp_printf
#define os_sprintf  tfp_sprintf
#else
int os_printf(const char *fmt, ...);
#endif

#endif /* __SYSUTILS_H__ */

