/* *****************************************************************************
 * @file    platform/x86/x86utils/x86utils.c
 *
 * @brief   x86 utilities
 *
 * @log     2011-08-19 Initial revision
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

#include <stdio.h>
#include <x86/x86utils/x86utils.h>

/*
 * @brief   x86 specific init
 * @param   none
 * @return  none
 * @note    none
 */
void x86utils_system_init(void)
{
    return;
}

/**
 * @brief   x86 io putchar routine
 * @param   p -i- device pointer
 *          ch -i- data to transmit
 * @return  none
 * @note    none
 */
void x86utils_io_putc(void *p, char ch)
{
    putchar(ch);
}

