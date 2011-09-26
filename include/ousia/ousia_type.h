/* *****************************************************************************
 * @file    include/ousia/ousia_type.h
 *
 * @brief   micro implementation of stdarg.h of libc
 *
 * @log     2011-09-26 Initial revision
 *
 * *****************************************************************************
 * COPYRIGHT (C) LEAFGRASS - LeafGrass (g.leafgrass@gmail.com)
 *
 * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 * ARM SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR
 * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 * ****************************************************************************/

#ifndef __OUSIA_TYPE_H__
#define __OUSIA_TYPE_H__

#include <port/ousia_cfg.h>

#ifdef OUSIA_USE_LIBMAPLE
#include <stm32/libmaple/libmaple_types.h>
#else
typedef signed char int8;
typedef unsigned char uint8;
typedef signed short int16;
typedef unsigned short uint16;
typedef signed int int32;
typedef unsigned int uint32;
typedef signed long long int64;
typedef unsigned long long uint64;
#endif /* OUSIA_USE_LIBMAPLE */
typedef enum {
    FALSE = 0, TRUE  = !FALSE
} bool;

#endif /* __OUSIA_TYPE_H__ */

