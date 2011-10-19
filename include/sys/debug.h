/* *****************************************************************************
 * @file    include/sys/debug.h
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

#ifndef __SYS_DEBUG_H__
#define __SYS_DEBUG_H__

void _os_assert_fail(const char *p_file, int line, const char *p_exp);

#define DEBUG_NONE      0
#define DEBUG_FAULT     1
#define DEBUG_ALL       2

#define DEBUG_LEVEL DEBUG_ALL

#if (DEBUG_LEVEL >= DEBUG_ALL)
#define _OS_ASSERT(exp) \
    if (exp) { \
    } else { \
        _os_assert_fail(__FILE__, __LINE__, #exp); \
    }
#else
#define ASSERT(exp) (void)((0))
#endif

#define os_assert(exp)   _OS_ASSERT(exp)

#endif /* __SYS_DEBUG_H__ */

