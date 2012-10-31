#include <ousia/ousia.h>
#include <ousia/ousia_type.h>

#include <sys/time.h>
#include <sys/sched.h> /* FIXME This bull shit expose the _pcb_t... */
#include <sys/print.h>
#include <console/console.h>

/* FIXME for temporary debug only */
#include <stm32/libmaple/include/libmaple/libmaple.h>
#include <stm32/libmaple/include/libmaple/gpio.h>
#include <stm32/libmaple/include/libmaple/i2c.h>
#include <stm32/stm32utils/stm32utils.h>

#define PS_CHILD_STACK_SIZE	512
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

static i2c_msg write_msg;
static uint8 write_msg_data[3];

void i2c_setup(void)
{
	write_msg.addr = 0x16;
	write_msg.flags = 0;
	write_msg.length = sizeof(write_msg_data);
	write_msg.xferred = 0;
	write_msg.data = write_msg_data;
	i2c_master_enable(I2C1, I2C_BUS_RESET);
}

static void ps_child(void *args)
{
	int ret;

	i2c_setup();

	for (;;) {
		gpio_write_bit(ERROR_LED_PORT, ERROR_LED_PIN, 1);

		ret = i2c_master_xfer(I2C1, &write_msg, 1, 0);
		if (ret != 0) {
			/* error handling */
		}

		os_process_sleep(10);
		gpio_write_bit(ERROR_LED_PORT, ERROR_LED_PIN, 0);
		os_process_sleep(10);
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
	for (;;)
		os_process_sleep(10);
}
