/* *****************************************************************************
 * @file    sample_x86/main.c
 *
 * @brief   sample code
 *
 * @log     2011-08-19 Initial revision
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

#include <unistd.h>

#include <x86/x86utils/x86utils.h>
#include <ousia/ousia.h>
#include <ousia/ousia_type.h>
#include <ousia/tprintf.h>
#include <ousia/sysutils.h>

int main(void)
{
    os_status stat = OS_OK;

    stat = os_init();
    stat = stat;
    x86utils_system_init(); 

    BOOT_LOGO();

    while(1)
    {
        usleep(10000);
    }

    return 0;
}

