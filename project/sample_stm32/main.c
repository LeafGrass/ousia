#include <ousia/ousia.h>
#include <ousia/ousia_type.h>

/* FIXME for temporary debug only */
#include <stm32/libmaple/include/libmaple/libmaple.h>
#include <stm32/libmaple/include/libmaple/gpio.h>
#include <stm32/stm32utils/stm32utils.h>

#include <sys/time.h>
#include <sys/sched.h>
#include <sys/print.h>

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

void ps_main(void *args)
{
	char ch = 0;

	os_dump_stack();
	for (;;) {
#if 1
		/* usb */
		if (stm32utils_usb_getc(NULL, &ch) == 0) {
			switch(ch) {
			case '\r':
				os_printf("\n");
				os_printk(LOG_INFO, "");
				gpio_toggle_bit(ERROR_LED_PORT, ERROR_LED_PIN);
				break;
			case '\b':
				os_printf("\b \b");
				break;
			default:
				os_printf("%c", ch);
				break;
			}
		}
		delay(20);
		os_process_sleep(10);
#else
		/* uart */
		if (USART_CONSOLE_BANK->flag_trigger) {
			for (i = 0; i < USART_CONSOLE_BANK->cnt_trigger; i++) {
				stm32utils_io_getc(USART_CONSOLE_BANK, &ch);
				switch (ch) {
				case 0:
					break;
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
		}
#endif
	}
}
