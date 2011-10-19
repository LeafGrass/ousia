/* *****************************************************************************
 * @file    sample_stm32/main.c
 *
 * @brief   sample code
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

#include <stm32/libmaple/libmaple.h>
#include <stm32/libmaple/libmaple_types.h>
#include <stm32/libmaple/gpio.h>
#include <stm32/libmaple/usart.h>
#include <stm32/stm32utils/stm32utils.h>

#include <ousia/ousia.h>
#include <ousia/ousia_type.h>
#include <sys/sched.h>
#include <sys/utils.h>
#include <sys/print.h>
#include <sys/debug.h>

void delayMicroseconds(uint32 us)
{
    delay_us(us);
}

void delay(uint32 ms)
{
    uint32 i;
    for (i = 0; i < ms; i++)
        delayMicroseconds(1000);
}

int main(void)
{
    int i = 0;
    char ch = 0;
    os_status stat = OS_OK;

    stat = os_init();
    os_assert(stat == OS_OK);
    stm32utils_board_init();

    /*
     * led flashes, sign of system starts to run
     * FIXME at least 1000+ms to wait for usb device, or the foregoing
     * characters may be lost, that is too long to wait ...
     */
    for(i = 0; i < 24; i++) {
        gpio_toggle_bit(ERROR_LED_PORT, ERROR_LED_PIN);
        delay(50);
    }

    BOOT_LOGO();

    for (;;) {
        if (stm32utils_usb_getc(NULL, &ch) == 0) {
            switch(ch) {
            case '\r':
                os_printf( "\r\n" );
                gpio_toggle_bit(ERROR_LED_PORT, ERROR_LED_PIN);
                break;
            case '\b':
                os_printf( "\b \b" );
                break;
            default:
                os_printf( "%c", ch );
                break;
            }
        }
        delay(20);
    }

    return 0;
}

