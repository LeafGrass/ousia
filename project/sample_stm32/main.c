#include <ousia/ousia.h>
#include <ousia/ousia_type.h>

/* FIXME for temporary debug only */
#include <stm32/libmaple/include/libmaple/libmaple.h>
#include <stm32/libmaple/include/libmaple/gpio.h>
#include <stm32/stm32utils/stm32utils.h>

#include <sys/time.h>
#include <sys/sched.h> /* FIXME This bull shit expose the _pcb_t... */
#include <sys/print.h>

#define USB_SERIAL

#define PS_CHILD_STACK_SIZE	256
#define PS_BUTTON_STACK_SIZE	256

static uint8 ps_child_stack[PS_CHILD_STACK_SIZE] = {0};
static uint8 ps_button_stack[PS_BUTTON_STACK_SIZE] = {0};
static struct _pcb_t ps_child_pcb;
static struct _pcb_t ps_button_pcb;

static void app_console_echo(void)
{
	char ch = 0;

#ifdef USB_SERIAL
	if (stm32utils_usb_getc(NULL, &ch) == 0) {
		switch(ch) {
		case '\r':
			os_printf("\n");
			os_printk(LOG_INFO, "");
			break;
		case '\b':
			os_printf("\b \b");
			break;
		default:
			os_printf("%c", ch);
			break;
		}
	}
#else
	if (USART_CONSOLE_BANK->flag_trigger) {
		for (i = 0; i < USART_CONSOLE_BANK->cnt_trigger; i++) {
			stm32utils_io_getc(USART_CONSOLE_BANK, &ch);
			switch (ch) {
			case 0:
				break;
			case '\r':
				os_printf( "\r\n" );
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

static void ps_button(void *args)
{
	for (;;) {
		if (gpio_read_bit(USR_BUT_PORT, USR_BUT_PIN))
			os_log(LOG_INFO, "%s - pressed.\n", __func__);
		os_process_sleep(10);
	}
}

static void ps_child(void *args)
{
	os_dump_stack();
	for (;;) {
		gpio_write_bit(ERROR_LED_PORT, ERROR_LED_PIN, 1);
		os_process_sleep(99);
		gpio_write_bit(ERROR_LED_PORT, ERROR_LED_PIN, 0);
		os_process_sleep(4900);
	}
}

void ps_main(void *args)
{
	os_process_create(&ps_child_pcb, ps_child, NULL,
			  ps_child_stack, PS_CHILD_STACK_SIZE);
	os_process_create(&ps_button_pcb, ps_button, NULL,
			  ps_button_stack, PS_BUTTON_STACK_SIZE);
	for (;;) {
		app_console_echo();
		os_process_sleep(10);
	}
}
