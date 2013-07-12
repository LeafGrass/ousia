#include <ousia/ousia.h>
#include <ousia/ousia_type.h>
#include <sys/debug.h>
#include <sys/print.h>
#include <sys/time.h>

#include "port/ousia_port.h"
#include "common/apb_uart_defs.h"
#include "utils/utils.h"

#define UART_LSR_THRE   0x20	/* Transmit holding bit */
#define UART_LSR_RBS    0x01	/* Receive buffer status */

static void cpu_init(void)
{
	uint32 cp15_r1;

	/* Change to low exception vectos */
	asm volatile (
			"mrc p15, 0, %0, c1, c0, 0\n"
			/* disable thumb state */
			/*"orr %0, #0x8000\n"*/
			/* exception vectors=0x00000000 */
			"bic %0, #0x2000\n"
			/* alignment fault check enabled */
			"orr %0, #0x0002\n"
			"mcr p15, 0, %0, c1, c0, 0\n"
			: "=r" (cp15_r1)
			:
			:"memory", "cc"
		     );
}

static int dtcm_clear(void)
{
	uint32 *pdtcm, dtcm_size, i;

	pdtcm = (uint32 *)DTCM_BASE;
	dtcm_size = DTCM_SIZE;

	pdtcm = (uint32 *)(DTCM_BASE + 0x3800);
	for (i = 0x3800; i < dtcm_size; i += 4)
		*pdtcm ++ = 0;

	return 0;
}

static void enable_irq(void)
{
	unsigned long cpsr;

	asm volatile (
			"mrs %0, cpsr\n"
			"bic %0, #128\n"
			"msr cpsr, %0\n"
			: "=r" (cpsr)
			:
			: "memory","cc"
		     );
}

static void disable_irq(void)
{
	unsigned long cpsr;

	asm volatile (
			"mrs %0, cpsr\n"
			"orr %0, #128\n"
			"msr cpsr, %0\n"
			: "=r" (cpsr)
			:
			: "memory","cc"
		     );
}

static void disable_fiq(void)
{
	unsigned long cpsr;

	asm volatile (
			"mrs %0, cpsr\n"
			"orr %0, #64\n"
			"msr cpsr, %0\n"
			: "=r" (cpsr)
			:
			: "memory","cc"
		     );
}

static void uart_putc(char c)
{
	uint32 tmp;
	do {
		tmp = __raw_read(UART_LSR(0));
	} while (!(tmp & UART_LSR_THRE));
	__raw_write(UART_THR(0), c);
}

static char uart_getc(void)
{
	uint32 tmp;
	tmp = __raw_read(UART_RBR(0));
	return (tmp & 0xff);
}

inline void __io_putc(void *dev, char ch)
{
	uart_putc(ch);
}

inline char __io_getc(void *dev)
{
	return uart_getc();
}

static void apb_init(void)
{
}

static void disable_soc(void)
{
	uint32 tmp;
	tmp = __raw_read(0x1000D008);
	tmp &= ~0x1;
	__raw_write(0x1000D008, tmp);
}

void bsp_init(void)
{
	disable_fiq();
	disable_irq();
	cpu_init();
	dtcm_clear();
	apb_init();
	enable_irq();
	disable_soc();
}
