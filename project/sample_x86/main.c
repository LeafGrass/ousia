/* *****************************************************************************
 * @file    sample_x86/main.c
 *
 * @brief   sample code
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

#include <unistd.h>

#include <x86/x86utils/x86utils.h>
#include <ousia/ousia.h>
#include <ousia/tprintf.h>
#include <ousia/sysutils.h>

#define VERSION     "v0.0.3"
#define VER_NAME    "Sweet Hibiscus"

int main(void)
{
    os_status stat = OS_OK;

    stat = os_init();
    stat = stat;
    x86utils_system_init(); 

    os_putchar(0x0C);   /* clear screen */

    os_printf("\r\nBooting...\r\n\r\n");
    os_printf("                       _\r\n");
    os_printf("     _                / /\r\n");
    os_printf("    | |    ___  __ _ _| |_ __ _ _  __  __ _   _   _\r\n");
    os_printf("    | |   / _ \\/ _` |_   _/ _` | \\/ _)/ _` | / / / /\r\n");
    os_printf("    | |_ _  __( (_| | | |  (_| | | | ( (_| | \\ \\ \\ \\\r\n");
    os_printf("    |_ _ _\\___|\\__,_| | | \\__, / | |  \\__,_| /_/ /_/\r\n");
    os_printf("                      /_/ \\_ _/\r\n" );
    os_printf("\r\n");
    os_printf("Ousia "); os_printf(VERSION); os_printf(" "); os_printf(VER_NAME);
    os_printf("\r\n\tby Librae - g.leafgrass@gmail.com");
    os_printf("\r\n\r\n");
    os_printf("Hello, Ousia ~\r\n");

    while(1)
    {
        usleep(10000);
    }

    return 0;
}

