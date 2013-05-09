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
#include <stm32/libmaple/include/libmaple/exti.h>
#include <stm32/stm32utils/stm32utils.h>

#define PS_CHILD_STACK_SIZE	512
#define PS_BUTTON_STACK_SIZE	256
#define PS_CONSOLE_STACK_SIZE	2048
#define PS_IMU_STACK_SIZE	512

static void ps_button(void *args)
{
	for (;;) {
		if (gpio_read_bit(USR_BUT_PORT, USR_BUT_PIN))
			os_log(LOG_INFO, "%s - pressed.\n", __func__);
		os_process_sleep(10);
	}
}

/* MPU 6050 Registers */
#define MPUREG_WHOAMI		0x75
#define MPUREG_SMPLRT_DIV	0x19
#define MPUREG_CONFIG		0x1A
#define MPUREG_GYRO_CONFIG	0x1B
#define MPUREG_ACCEL_CONFIG	0x1C
#define MPUREG_FIFO_EN		0x23
#define MPUREG_INT_PIN_CFG	0x37
#define MPUREG_INT_ENABLE	0x38
#define MPUREG_INT_STATUS	0x3A
#define MPUREG_ACCEL_XOUT_H	0x3B
#define MPUREG_ACCEL_XOUT_L	0x3C
#define MPUREG_ACCEL_YOUT_H	0x3D
#define MPUREG_ACCEL_YOUT_L	0x3E
#define MPUREG_ACCEL_ZOUT_H	0x3F
#define MPUREG_ACCEL_ZOUT_L	0x40
#define MPUREG_TEMP_OUT_H	0x41
#define MPUREG_TEMP_OUT_L	0x42
#define MPUREG_GYRO_XOUT_H	0x43
#define MPUREG_GYRO_XOUT_L	0x44
#define MPUREG_GYRO_YOUT_H	0x45
#define MPUREG_GYRO_YOUT_L	0x46
#define MPUREG_GYRO_ZOUT_H	0x47
#define MPUREG_GYRO_ZOUT_L	0x48
#define MPUREG_USER_CTRL	0x6A
#define MPUREG_PWR_MGMT_1	0x6B
#define MPUREG_PWR_MGMT_2	0x6C
#define MPUREG_FIFO_COUNTH	0x72
#define MPUREG_FIFO_COUNTL	0x73
#define MPUREG_FIFO_R_W		0x74

/* configuration bits MPU 3000 and MPU 6000 (not revised)? */
#define BIT_SLEEP		0x40
#define BIT_H_RESET		0x80
#define BITS_CLKSEL		0x07
#define MPU_CLK_SEL_PLLGYROX	0x01
#define MPU_CLK_SEL_PLLGYROZ	0x03
#define MPU_EXT_SYNC_GYROX	0x02
#define BITS_FS_250DPS		0x00
#define BITS_FS_500DPS		0x08
#define BITS_FS_1000DPS		0x10
#define BITS_FS_2000DPS		0x18
#define BITS_FS_MASK		0x18
#define BITS_DLPF_CFG_256HZ_NOLPF2 0x00
#define BITS_DLPF_CFG_188HZ	0x01
#define BITS_DLPF_CFG_98HZ	0x02
#define BITS_DLPF_CFG_42HZ	0x03
#define BITS_DLPF_CFG_20HZ	0x04
#define BITS_DLPF_CFG_10HZ	0x05
#define BITS_DLPF_CFG_5HZ	0x06
#define BITS_DLPF_CFG_2100HZ_NOLPF 0x07
#define BITS_DLPF_CFG_MASK	0x07
#define BIT_INT_ANYRD_2CLEAR	0x10
#define BIT_RAW_RDY_EN		0x01
#define BIT_I2C_IF_DIS		0x10
#define BIT_INT_STATUS_DATA	0x01


#define I2C_ADDR_MPU	0x68
#define MPU_INT_PORT	GPIOC
#define MPU_INT_PIN	5

static i2c_msg write_msg;
static uint8 write_msg_data[3];
static bool mpu_int = 0;
static int16 an[6];
static uint8 buffer[14];
static int16 temp_raw;

static void i2c_setup(void)
{
	write_msg.addr = I2C_ADDR_MPU;
	write_msg.flags = 0;
	write_msg.length = sizeof(write_msg_data);
	write_msg.xferred = 0;
	write_msg.data = write_msg_data;
	i2c_master_enable(I2C1, I2C_BUS_RESET | I2C_FAST_MODE);
}

static void imu_irq(void)
{
	mpu_int = 1;
}

static void imu_write(uint8 reg, uint8 val)
{
	uint8 buf[2];
	i2c_msg msgs[1];
	int ret;

	buf[0] = reg;
	buf[1] = val;

	msgs[0].addr = I2C_ADDR_MPU;
	msgs[0].flags = 0;
	msgs[0].length = 2;
	msgs[0].data = buf;
	ret = i2c_master_xfer(I2C1, msgs, 1, 0);
	if (ret != 0)
		os_log(LOG_ERROR, "%s - ret = %d.\n", __func__, ret);
}

void mpu_raw_read(void)
{
	int ret;
	i2c_msg msgsl[2];

	msgsl[0].addr = I2C_ADDR_MPU;
	msgsl[0].flags = 0;
	msgsl[0].length = 1;
	msgsl[0].data = buffer;
	/* address to read */
	buffer[0] = 0x3B;
	/* repeated start condition, then read 6 byte back */
	msgsl[1].addr = I2C_ADDR_MPU;
	msgsl[1].flags = I2C_MSG_READ;
	msgsl[1].length = 14;
	msgsl[1].data = buffer;
	ret = i2c_master_xfer(I2C1, msgsl, 2, 0);
	if (ret != 0)
		os_log(LOG_ERROR, "%s - ret = %d.\n", __func__, ret);

	/* ax */
	an[3] = ((buffer[0] << 8) | buffer[1]);
	/* ay */
	an[4] = ((buffer[2] << 8) | buffer[3]);
	/* az */
	an[5] = ((buffer[4] << 8) | buffer[5]);
	temp_raw = ((buffer[6] << 8) | buffer[7]);
	/* gx */
	an[0] = ((buffer[8] << 8) | buffer[9]);
	/* gy */
	an[1] = ((buffer[10] << 8) | buffer[11]);
	/* gz */
	an[2] = ((buffer[12] << 8) | buffer[13]);
}


static void imu_init(void)
{
	i2c_setup();
	os_process_sleep(100);

	/* INT of MPU */
	gpio_set_mode(MPU_INT_PORT, MPU_INT_PIN, GPIO_INPUT_FLOATING);
	exti_attach_interrupt(MPU_INT_PIN, gpio_exti_port(MPU_INT_PORT),
			      imu_irq, EXTI_RISING);

	imu_write(MPUREG_PWR_MGMT_1, BIT_H_RESET);
	os_process_sleep(100);

	/* wake up device and select GyroZ clock (better performance) */
	imu_write(MPUREG_PWR_MGMT_1, MPU_CLK_SEL_PLLGYROZ);
	os_process_sleep(100);

	/*
	 * sample rate = 200Hz
	 * Fsample= 1Khz/(4+1) = 200Hz
	 * 0x13 = 50hz
	 * 5 hz= c7
	 */
	imu_write(MPUREG_SMPLRT_DIV, 0x04);
	os_process_sleep(100);

	/* DLPF = 20Hz (low pass filter) BITS_DLPF_CFG_188HZ */
	imu_write(MPUREG_CONFIG, BITS_DLPF_CFG_188HZ);
	os_process_sleep(100);

	/* gyro scale 2000º/s */
	imu_write(MPUREG_GYRO_CONFIG, BITS_FS_2000DPS);
	os_process_sleep(100);

	/* accel scale 4g (4096LSB/g) */
	imu_write(MPUREG_ACCEL_CONFIG, 0x08);
	os_process_sleep(100);

	/* INT CFG => Interrupt on Data Ready */
	imu_write(MPUREG_INT_ENABLE, BIT_RAW_RDY_EN);
	os_process_sleep(100);

	/* INT: clear on any read */
	imu_write(MPUREG_INT_PIN_CFG, BIT_INT_ANYRD_2CLEAR);
	os_process_sleep(100);
}

static void ps_child(void *args)
{
	int i;
	for (;;) {
		gpio_write_bit(ERROR_LED_PORT, ERROR_LED_PIN, 1);
		for (i = 0; i < 100; i++)
			os_putchar(' ');
		os_printf("\r");
		os_log(LOG_INFO,
			"mpu_int, axyz: %d %d %d, gxyz %d %d %d\r",
			an[3], an[4], an[5], an[0], an[1], an[2]);
		os_process_sleep(59);
		gpio_write_bit(ERROR_LED_PORT, ERROR_LED_PIN, 0);
		os_process_sleep(149);
	}
}

static void ps_imu(void *args)
{
	int ret;
	int i;

	imu_init();

	/* warm up adc */
	for (i = 0; i < 100; i++)
		mpu_raw_read();

	for (;;) {
		if (mpu_int) {
			mpu_int = 0;
			mpu_raw_read();
		}
		ret = i2c_master_xfer(I2C1, &write_msg, 1, 0);
		if (ret != 0) {
			/* error handling */
		}
		os_process_sleep(10);
	}
}

void ps_main(void *args)
{
	os_process_create(ps_child, NULL, PS_CHILD_STACK_SIZE);
	os_process_create(ps_button, NULL, PS_BUTTON_STACK_SIZE);
	os_process_create(ps_console, NULL, PS_CONSOLE_STACK_SIZE);
	os_process_create(ps_imu, NULL, PS_IMU_STACK_SIZE);
	for (;;)
		os_process_sleep(10);
}
