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

static int signal;

/* eeprom test */
static struct eeprom_priv_s ee24c08 = {
	.bus_id = 1,
	.dev_addr = 0x50,
	.regaddr_size = 1,
	.page_size = 16,
	.pages = 64,
};

static struct eeprom_priv_s *gee = &ee24c08;

/* FIXME malloc them! */
#define BUFFER_SIZE	64
static uint8 buffer_r[BUFFER_SIZE];
static uint8 buffer_w[BUFFER_SIZE + 1];

static void ps_debug(void *args)
{
	int ret;

	for (;;) {
		if (!signal) {
			os_process_sleep(50);
			continue;
		} else
			signal = 0;

		eeprom_erase(gee, 0, 16);
		eeprom_read(gee, buffer_r, 0, 16);
		eeprom_write(gee, buffer_w, 0, 16);
		ret = eeprom_verify(gee, buffer_w + gee->regaddr_size, 0, 16);
		if (ret != 0)
			os_log(LOG_ERROR, "Verify failed.\n");
		else
			os_log(LOG_INFO, "Verify passed.\n");
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

static void eeprom_test_setup(void)
{
	int i;

	i2c_master_enable(I2C1, I2C_BUS_RESET);
	memset(buffer_r, 0, sizeof(buffer_r));

	buffer_w[0] = 0x0;
	for (i = 1; i < BUFFER_SIZE + 1; i++)
		buffer_w[i] = i - 1;
#if 0
	_mm_dump(buffer_r, sizeof(buffer_r), 0);
	_mm_dump(buffer_w, sizeof(buffer_w), 0);
	os_log(LOG_INFO, "buffers are ready: r: 0x%x, w: 0x%x\n", buffer_r, buffer_w);
#endif
}

void ps_main(void *args)
{
	eeprom_test_setup();
	os_process_create(ps_child, NULL, PS_CHILD_STACK_SIZE);
	os_process_create(ps_button, NULL, PS_BUTTON_STACK_SIZE);
	os_process_create(ps_console, NULL, PS_CONSOLE_STACK_SIZE);
	os_process_create(ps_debug, NULL, PS_DEBUG_STACK_SIZE);
	for (;;)
		os_process_sleep(10);
}
