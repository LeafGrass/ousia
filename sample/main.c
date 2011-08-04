#include <stdio.h>

#include <ousia/scheduler.h>
#include <stm32/libmaple/libmaple.h>
#include <stm32/libmaple/libmaple_types.h>
#include <stm32/libmaple/gpio.h>
#include <stm32/stm32utils/stm32utils.h>

#include <tprintf/tprintf.h>

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
    int result = 0;
    int i = 0;

    sample_function(10, 20, &result);

    init_printf(NULL, stm32utils_io_putc);
    stm32utils_system_init(); 

    /* led flashes -> sign of system reset ok */
    for(i = 0; i < 6; i++) {
        gpio_toggle_bit(GPIOA, 1);
        delay(50);
    }   

    //printf("result = %d\n", result);
    printf("Build success.\n");

    for (;;) {
        asm volatile("nop");
    }

    return 0;
}
