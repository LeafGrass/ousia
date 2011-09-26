/* *****************************************************************************
 * @file    include/ousia/ousia.h
 *
 * @brief   micro implementation of stdarg.h of libc
 *
 * @log     2011-08-14 Initial revision
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

#ifndef __OUSIA_H__
#define __OUSIA_H__

#define VERSION     "v0.0.4"
#define VER_NAME    "Delavay Fir"

#define BOOT_LOGO() \
{ \
    os_putchar(0x0C); \
    os_printf("                       _\r\n"); \
    os_printf("     _                / /\r\n"); \
    os_printf("    | |    ___  __ _ _| |_ __ _ _  __  __ _   _   _\r\n"); \
    os_printf("    | |   / _ \\/ _` |_   _/ _` | \\/ _)/ _` | / / / /\r\n"); \
    os_printf("    | |_ _  __( (_| | | |  (_| | | | ( (_| | \\ \\ \\ \\\r\n"); \
    os_printf("    |_ _ _\\___|\\__,_| | | \\__, / | |  \\__,_| /_/ /_/\r\n"); \
    os_printf("                      /_/ \\_ _/\r\n" ); \
    os_printf("\r\n"); \
    os_printf("Ousia "); os_printf(VERSION); os_printf(" "); os_printf(VER_NAME); \
    os_printf("\r\n\tby LeafGrass - g.leafgrass@gmail.com"); \
    os_printf("\r\n\r\n"); \
    os_printf("Hello, Ousia ~\r\n"); \
}

typedef enum _os_status
{
    OS_ERR = -1,
    OS_OK = 0
} os_status;

#endif /* __OUSIA_H__ */

