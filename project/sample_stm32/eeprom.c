#include <ousia/ousia.h>
#include <ousia/ousia_type.h>

#include <ulib/string.h>
#include <sys/time.h>
#include <sys/mm.h>
#include <sys/sched.h> /* FIXME This bull shit expose the _pcb_t... */
#include <sys/print.h>

/* FIXME for temporary debug only */
#include <stm32/libmaple/include/libmaple/libmaple.h>
#include <stm32/libmaple/include/libmaple/gpio.h>
#include <stm32/libmaple/include/libmaple/i2c.h>
#include <stm32/stm32utils/stm32utils.h>

#define I2C_ADDR_AT24	0x50
#define BUFFER_SIZE	16

static uint8 buffer_w[BUFFER_SIZE+1] = {0};
static uint8 buffer_r[BUFFER_SIZE] = {0};

static void i2c_send_byte(uint8 slave, uint8 data)
{
	i2c_msg msg;
	int ret;

	msg.addr = slave;
	msg.flags = I2C_MSG_READ;
	msg.length = 1;
	msg.data = &data;
	ret = i2c_master_xfer(I2C1, &msg, 1, 0);
	if (ret != 0)
		os_log(LOG_ERROR, "%s - ret = %d.\n", __func__, ret);
}

static void i2c_recv_byte(uint8 slave, uint8 *pdata)
{
	i2c_msg msg;
	int ret;

	msg.addr = slave;
	msg.flags = 0;
	msg.length = 1;
	msg.data = pdata;
	ret = i2c_master_xfer(I2C1, &msg, 1, 0);
	if (ret != 0)
		os_log(LOG_ERROR, "%s - ret = %d.\n", __func__, ret);
}

void i2c_test_read(void)
{
	int ret;
	i2c_msg msgs[2];
	uint8 tmp = 0;

	os_log(LOG_INFO, "%s start...\n", __func__);

	msgs[0].addr = I2C_ADDR_AT24;
	msgs[0].flags = 0;
	msgs[0].length = 1;
	msgs[0].data = &tmp;

	msgs[1].addr = I2C_ADDR_AT24;
	msgs[1].flags = I2C_MSG_READ;
	msgs[1].length = BUFFER_SIZE;
	msgs[1].data = buffer_r;
	ret = i2c_master_xfer(I2C1, msgs, 2, 0);
	if (ret != 0)
		os_log(LOG_ERROR, "%s - ret = %d.\n", __func__, ret);
	else
		_mm_dump(buffer_r, sizeof(buffer_r), 0);
	os_log(LOG_INFO, "%s done...\n", __func__);
}

void i2c_test_write(void)
{
	int ret;
	i2c_msg msgs[2];
	uint8 tmp = 0;

	os_log(LOG_INFO, "%s start...\n", __func__);
	_mm_dump(buffer_w+1, sizeof(buffer_w)-1, 0);

	msgs[0].addr = I2C_ADDR_AT24;
	msgs[0].flags = 0;
	msgs[0].length = 1;
	msgs[0].data = &tmp;

	msgs[1].addr = I2C_ADDR_AT24;
	msgs[1].flags = 0;
	msgs[1].length = BUFFER_SIZE + 1;
	msgs[1].data = buffer_w;
	ret = i2c_master_xfer(I2C1, &msgs[1], 1, 0);
	if (ret != 0)
		os_log(LOG_ERROR, "%s - ret = %d.\n", __func__, ret);
	os_log(LOG_INFO, "%s done...\n", __func__);
}

void eeprom_init(void)
{
	int i;
	i2c_master_enable(I2C1, I2C_BUS_RESET);

	memset(buffer_r, 0, sizeof(buffer_r));

	buffer_w[0] = 0x0;
	for (i = 1; i < BUFFER_SIZE+1; i++)
		buffer_w[i] = i-1;

	_mm_dump(buffer_r, sizeof(buffer_r), 0);
	_mm_dump(buffer_w, sizeof(buffer_w), 0);
	os_log(LOG_INFO, "buffers are ready: r: 0x%x, w: 0x%x\n", buffer_r, buffer_w);
}
