#include <stdio.h>

#include <ousia/scheduler.h>

#include <libmaple.h>
#include <libmaple_types.h>
#include <flash.h>
#include <rcc.h>
#include <nvic.h>
#include <systick.h>
#include <gpio.h>
#include <adc.h>
#include <timer.h>
#include <usb.h>
#include <usart.h>

int main(void)
{
    int result = 0;

    sample_function(10, 20, &result);
    //printf(>result = %d\n>, result);

    //printf(>Build success.\n>);
    for (;;)
    {
        ;
    }

    return 0;
}
