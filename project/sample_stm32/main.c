#include <ousia/ousia.h>
#include <ousia/ousia_type.h>

#include <sys/time.h>
#include <sys/sched.h> /* FIXME This bull shit expose the _pcb_t... */
#include <sys/print.h>
#include <console/console.h>

/* FIXME for temporary debug only */
#include <stm32/libmaple/include/libmaple/libmaple.h>
#include <stm32/libmaple/include/libmaple/gpio.h>
#include <stm32/stm32utils/stm32utils.h>

#define PS_CHILD_STACK_SIZE	256
#define PS_BUTTON_STACK_SIZE	256
#define PS_CONSOLE_STACK_SIZE	2048

static uint8 ps_child_stack[PS_CHILD_STACK_SIZE] = {0};
static uint8 ps_button_stack[PS_BUTTON_STACK_SIZE] = {0};
static uint8 ps_console_stack[PS_CONSOLE_STACK_SIZE] = {0};
static struct _pcb_t ps_child_pcb;
static struct _pcb_t ps_button_pcb;
static struct _pcb_t ps_console_pcb;

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
	os_process_create(&ps_console_pcb, ps_console, NULL,
			  ps_console_stack, PS_CONSOLE_STACK_SIZE);
	for (;;) {
		os_process_sleep(10);
	}
}
