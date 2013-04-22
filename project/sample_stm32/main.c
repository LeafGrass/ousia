#include <ousia/ousia.h>
#include <ousia/ousia_type.h>

#include <ulib/string.h>
#include <sys/time.h>
#include <sys/mm.h>
#include <sys/sched.h> /* FIXME This bull shit expose the _pcb_t... */
#include <sys/print.h>
#include <console/console.h>

/* FIXME for temporary debug only */
#include <stm32/libmaple/include/libmaple/libmaple.h>
#include <stm32/libmaple/include/libmaple/gpio.h>
#include <stm32/libmaple/include/libmaple/i2c.h>
#include <stm32/stm32utils/stm32utils.h>

#include "eeprom.h"

#define PS_CHILD_STACK_SIZE	256
#define PS_BUTTON_STACK_SIZE	256
#define PS_CONSOLE_STACK_SIZE	2048
#define PS_DEBUG_STACK_SIZE	2048

static uint8 ps_child_stack[PS_CHILD_STACK_SIZE] = {0};
static uint8 ps_button_stack[PS_BUTTON_STACK_SIZE] = {0};
static uint8 ps_console_stack[PS_CONSOLE_STACK_SIZE] = {0};
static uint8 ps_debug_stack[PS_DEBUG_STACK_SIZE] = {0};
static struct _pcb_t ps_child_pcb;
static struct _pcb_t ps_button_pcb;
static struct _pcb_t ps_console_pcb;
static struct _pcb_t ps_debug_pcb;

static int signal;

static void ps_debug(void *args)
{
	for (;;) {
		if (!signal) {
			os_process_sleep(50);
			continue;
		} else
			signal = 0;
		i2c_test_read();
		i2c_test_write();
		i2c_test_read();
	}
}

static void ps_button(void *args)
{
	signal = 0;
	for (;;) {
		if (gpio_read_bit(USR_BUT_PORT, USR_BUT_PIN)) {
			os_log(LOG_INFO, "%s - pressed.\n", __func__);
			signal = 1;
		}
		os_process_sleep(100);
	}
}

static void ps_child(void *args)
{
	for (;;) {
		gpio_write_bit(ERROR_LED_PORT, ERROR_LED_PIN, 1);
		os_process_sleep(25);
		gpio_write_bit(ERROR_LED_PORT, ERROR_LED_PIN, 0);
		os_process_sleep(4974);
	}
}

void ps_main(void *args)
{
	eeprom_init();
	os_process_create(&ps_child_pcb, ps_child, NULL,
			  ps_child_stack, PS_CHILD_STACK_SIZE);
	os_process_create(&ps_button_pcb, ps_button, NULL,
			  ps_button_stack, PS_BUTTON_STACK_SIZE);
	os_process_create(&ps_console_pcb, ps_console, NULL,
			  ps_console_stack, PS_CONSOLE_STACK_SIZE);
	os_process_create(&ps_debug_pcb, ps_debug, NULL,
			  ps_debug_stack, PS_DEBUG_STACK_SIZE);
	for (;;) {
		os_process_sleep(10);
	}
}
