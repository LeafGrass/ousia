#include <ousia/ousia.h>
#include <ousia/ousia_type.h>

#include <stm32/libmaple/libmaple.h>
#include <stm32/libmaple/libmaple_types.h>
#include <stm32/libmaple/gpio.h>
#include <stm32/libmaple/usart.h>
#include <stm32/stm32utils/stm32utils.h>

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
	os_status ret = OS_OK;
	ch = ch;
//	uint8 wdata = 0xF0;
//	uint8 rdata = 0x00;

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

	ret = os_init();
	os_assert(ret == OS_OK);
	/*os_kick_off();*/

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
				os_printf("\n");
				os_logk(LOG_INFO, "");
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
#if 0
	for (;;) {
		i2c_soft_init();
		ret = i2c_soft_eeprom_write(0x00, &wdata, 1);
		os_log(LOG_CRITICAL, "ret write: 0x%X\n", ret);
		gpio_toggle_bit(ERROR_LED_PORT, ERROR_LED_PIN);
		delay(500);
		ret = i2c_soft_eeprom_read(0x00, &rdata, 1);
		gpio_toggle_bit(ERROR_LED_PORT, ERROR_LED_PIN);
		os_log(LOG_CRITICAL, "ret read: 0x%X\n", ret);
		os_log(LOG_CRITICAL, "data read: 0x%02X\n", rdata);
		delay(500);
	}
#endif

	return 0;
}
