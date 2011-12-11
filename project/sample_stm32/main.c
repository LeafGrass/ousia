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
#include <sys/time.h>
#include <sys/debug.h>
#include <char/i2c/i2c_soft.h>

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
	ch = ch;

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

#if 0
	for (;;) {
#if 0
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
		/* usb */
		if (stm32utils_usb_getc(NULL, &ch) == 0) {
			switch(ch) {
			case '\r':
				os_printf("\r\n");
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
	}
#endif
	for (;;) {
		i2c_soft_init();
		delay(500);
	}

	return 0;
}
