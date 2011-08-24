/* *****************************************************************************
 * @file    platform/x86/x86utils/utils.h
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

#ifndef __X86UTILS_H__
#define __X86UTILS_H__

void x86utils_system_init(void);
void x86utils_io_putc(void *p, char ch);
void x86utils_attach_systick_callback(void (*callback_fn)(void));


#endif /* __X86UTILS_H__ */

