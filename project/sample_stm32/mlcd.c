#include <ousia/ousia.h>
#include <ousia/ousia_type.h>

#include <ulib/string.h>
#include <ulib/stdlib.h>
#include <sys/time.h>
#include <sys/mm.h>
#include <sys/sched.h>		/* FIXME This bull shit expose the _pcb_t... */
#include <sys/print.h>

#include "console/console.h"

#include "stm32/libmaple/include/libmaple/libmaple.h"
#include "stm32/libmaple/include/libmaple/libmaple_types.h"
#include "stm32/libmaple/include/libmaple/gpio.h"
#include "stm32/libmaple/include/libmaple/spi.h"

#include "stm32/utils/utils.h"
#include "mlcd_img.h"

#define LS013B7DH03_CMD_UPDATE     (0x01)
#define LS013B7DH03_CMD_ALL_CLEAR  (0x04)
#define LS013B7DH03_CONTROL_BYTES  (0)
#define MLCD_XRES	128
#define MLCD_YRES	128

static char frameBuffer[MLCD_XRES / 8 * MLCD_YRES] = { 0 };

void spi_send(uint8 *data, uint32 length)
{
	uint32 txed = 0;
	while (txed < length)
		txed += spi_tx(SPI1, data + txed, length - txed);
	while (spi_is_busy(SPI1)) ;
}

/******************************************************************************
 * @brief Move and show the contents of a pixel matrix buffer onto the display.
 *
 * @param[in] device       Display device pointer.
 * @param[in] pixelMatrix  Pointer to the pixel matrix buffer to draw.
 * @param[in] startColumn  Start column on the display where to start
 *                         drawing the pixel matrix. Must be 0 if addressing
 *                         mode is DISPLAY_ADDRESSING_BY_ROWS_ONLY.
 * @param[in] width        Width in pixel columns of the pixel matrix to draw
 *                         Must be max width of display if the addresssing
 *                         mode is DISPLAY_ADDRESSING_BY_ROWS_ONLY.
 * @param[in] startRow     Start row on the display where to start drawing
 *                         the pizel matrix.
 * @param[in] height       Height in pixel rows/lines of the pixel matrix.
 *
 * @return  EMSTATUS code of the operation.
 *****************************************************************************/
void PixelMatrixDraw(void *pixelMatrix,
		     unsigned int startColumn,
		     unsigned int width,
		     unsigned int startRow, unsigned int height)
{
	unsigned int i;
	uint16 *p = (uint16 *) pixelMatrix;
	uint16 cmd;
	(void)width;		/* Suppress compiler warning: unused parameter. */
	(void)startColumn;	/* Suppress compiler warning: unused parameter. */

	/* Need to adjust start row by one because LS013B7DH03 starts counting lines
	   from 1, while the DISPLAY interface starts from 0. */
	startRow++;

	/* Assert SCS */
	gpio_write_bit(GPIOA, GPIO_MEMLCD_CS, 1);

	/* SCS setup time: min 6us */
//      delay_us(6);

	/* Send update command and first line address */
	cmd = LS013B7DH03_CMD_UPDATE | (startRow << 8);
	spi_send((uint8 *) & cmd, 2);

	/* Get start address to draw from */
	for (i = 0; i < height; i++) {

		/* Send pixels for this line */
		spi_send((uint8 *) p,
			 MLCD_YRES / 8 + LS013B7DH03_CONTROL_BYTES);
		p += (MLCD_YRES / 8 +
		      LS013B7DH03_CONTROL_BYTES) / sizeof(uint16);

		if (i == height - 1) {
			cmd = 0xffff;
		} else {
			cmd = 0xff | ((startRow + i + 1) << 8);
		}
		spi_send((uint8 *) & cmd, 2);
	}

	/* SCS hold time: min 2us */
//      delay_us(2);

	/* De-assert SCS */
	gpio_write_bit(GPIOA, GPIO_MEMLCD_CS, 0);
}

void memlcd_disp(uint32 on)
{
	if (on)
		gpio_write_bit(GPIOA, GPIO_MEMLCD_DISP, 1);
	else
		gpio_write_bit(GPIOA, GPIO_MEMLCD_DISP, 0);
}

void memlcd_clear(void)
{
	uint16 cmd = LS013B7DH03_CMD_ALL_CLEAR;
	gpio_write_bit(GPIOA, GPIO_MEMLCD_CS, 1);
	delay_us(2);
	spi_send((uint8 *) & cmd, 2);
	delay_us(6);
	gpio_write_bit(GPIOA, GPIO_MEMLCD_CS, 0);
}

void memlcd_scroll_down(int nrows)
{
	int x, y, xn;
	os_assert(nrows < MLCD_YRES);
	for (x = 0; x < MLCD_YRES; x++) {
		for (y = 0; y < MLCD_XRES / 8; y++) {
			if (nrows + x > MLCD_YRES - 1)
				xn = nrows + x - MLCD_YRES;
			else
				xn = nrows + x;
			frameBuffer[xn * MLCD_XRES / 8 + y] =
			    img[x * MLCD_XRES / 8 + y];
		}
	}
}

void memlcd_draw(int index)
{
	memlcd_scroll_down(index);
//      memcpy( frameBuffer, img, (MLCD_XRES*MLCD_YRES/8) );
	PixelMatrixDraw(frameBuffer, 0, MLCD_XRES, 0, MLCD_YRES);
}

void memlcd_init(void)
{
	gpio_write_bit(GPIOA, GPIO_MEMLCD_DISP, 0);
	gpio_write_bit(GPIOA, GPIO_MEMLCD_EXTCOMIN, 0);
	gpio_write_bit(GPIOA, GPIO_MEMLCD_CS, 0);
	gpio_set_mode(GPIOA, GPIO_MEMLCD_DISP, GPIO_OUTPUT_PP);
	gpio_set_mode(GPIOA, GPIO_MEMLCD_EXTCOMIN, GPIO_OUTPUT_PP);
	gpio_set_mode(GPIOA, GPIO_MEMLCD_CS, GPIO_OUTPUT_PP);
}
