#include <ousia/ousia.h>
#include <ousia/ousia_type.h>

#include <ulib/string.h>
#include <ulib/stdlib.h>
#include <sys/time.h>
#include <sys/mm.h>
#include <sys/sched.h>		/* FIXME This bull shit expose the _pcb_t... */
#include <sys/print.h>

/* FIXME for temporary debug only */
#include "stm32/libmaple/include/libmaple/libmaple.h"
#include "stm32/libmaple/include/libmaple/i2c.h"
#include "stm32/utils/utils.h"

#include "eeprom.h"

static inline i2c_dev *i2c_bus_to_dev(int id)
{
	switch (id) {
	case 1:
		return I2C1;
	case 2:
		return I2C2;
	default:
		os_log(LOG_INFO, "Undefined bus id, use default I2C1.\n");
		return I2C1;
	}
}

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

static void i2c_recv_byte(uint8 slave, uint8 * pdata)
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

int eeprom_byte_write(struct eeprom_priv_s *ee, uint8 data, uint32 offset,
		      uint32 nb)
{
	return 0;
}

/* page read */
int eeprom_read(struct eeprom_priv_s *ee, uint8 * buf, uint32 offset, uint32 nb)
{
	int ret;
	i2c_msg msgs[2];

	os_log(LOG_INFO, "%s start...\n", __func__);

	/* XXX 16 bit addr? */
	msgs[0].addr = ee->dev_addr;
	msgs[0].flags = 0;
	msgs[0].length = 1;
	offset &= 0xFF;
	msgs[0].data = (uint8 *) & offset;

	msgs[1].addr = ee->dev_addr;
	msgs[1].flags = I2C_MSG_READ;
	msgs[1].length = nb;
	msgs[1].data = buf;

	ret = i2c_master_xfer(I2C1, msgs, 2, 0);
	if (ret != 0)
		os_log(LOG_ERROR, "%s - ret = %d.\n", __func__, ret);
	else {
		_mm_dump(buf, nb, 0);
		os_log(LOG_INFO, "%s done...\n", __func__);
	}

	return ret;
}

/* page write */
int eeprom_write(struct eeprom_priv_s *ee, uint8 * buf, uint32 offset,
		 uint32 nb)
{
	int ret;
	i2c_msg msg;

	if (nb > ee->page_size) {
		os_log(LOG_ERROR, "%s too large.\n", __func__);
		return -1;
	}

	os_log(LOG_INFO, "%s start...\n", __func__);
	_mm_dump(buf + ee->regaddr_size, nb, 0);

	msg.addr = ee->dev_addr;
	msg.flags = 0;
	msg.length = nb + ee->regaddr_size;
	msg.data = buf;
	ret = i2c_master_xfer(I2C1, &msg, 1, 0);
	if (ret != 0) {
		os_log(LOG_ERROR, "%s - ret = %d.\n", __func__, ret);
		return ret;
	}

	os_log(LOG_INFO, "%s done...\n", __func__);
	return 0;
}

/* page erase */
int eeprom_erase(struct eeprom_priv_s *ee, uint32 offset, uint32 nb)
{
	uint8 *buf = NULL;
	int32 ret;

	buf = (uint8 *) malloc(17);
	os_assert(buf != NULL);
	memset(buf, 0xFF, 17);
	buf[0] = offset;
	ret = eeprom_write(ee, buf, offset, nb);
	free(buf);
	return ret;
}

/* FIXME malloc them! */
static uint8 buf_r[64];

int eeprom_verify(struct eeprom_priv_s *ee, uint8 * buf, uint32 offset,
		  uint32 nb)
{
	memset(buf_r, 0, sizeof(buf_r));
	eeprom_read(ee, buf_r, offset, nb);
	return memcmp(buf_r, buf, nb);
}
