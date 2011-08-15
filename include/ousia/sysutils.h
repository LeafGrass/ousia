/* *****************************************************************************
 * @file    core/sysutils.h
 *
 * @brief   header of sysutils.c
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

#ifndef __SYSUTILS_H__
#define __SYSUTILS_H__

void init_os_printf(void *putp, void (*putf)(void *, char));
int os_printf(const char *p_fmt, ...);
void os_sprintf(char *p_buf, const char *p_fmt, ...);

#endif /* __SYSUTILS_H__ */

