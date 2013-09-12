#include <ousia/ousia.h>
#include <ousia/ousia_type.h>
#include <sys/debug.h>
#include <sys/print.h>
#include <sys/time.h>
#include "port/ousia_port.h"

#if 0
typedef struct pt_regs {
	INT32 arm_r0;
	INT32 arm_r1;
	INT32 arm_r2;
	INT32 arm_r3;
	INT32 arm_r4;
	INT32 arm_r5;
	INT32 arm_r6;
	INT32 arm_r7;
	INT32 arm_r8;
	INT32 arm_r9;
	INT32 arm_r10;
	INT32 arm_fp;
	INT32 arm_ip;
	INT32 arm_sp;
	INT32 arm_lr;
	INT32 arm_pc;
	INT32 arm_psr;
	INT32 arm_old_r0;
} arm_regs;
#endif

void show_regs(struct pt_regs *regs)
{
}

void halt_cpu(void)
{
	os_printk(LOG_ERROR, "Halt CPU ...\n");
	asm volatile ("1: b 1b\n");
}

void do_undintr_vector(struct pt_regs *regs)
{
	os_printk(LOG_ERROR, "undefined instruction\n");
	show_regs(regs);
	halt_cpu();
}

void do_swi_vector(struct pt_regs *regs)
{
	os_printk(LOG_ERROR, "software interrupt\n");
	show_regs(regs);
	halt_cpu();
}

void do_preabt_vector(struct pt_regs *regs)
{
	os_printk(LOG_ERROR, "prefetch abort\n");
	show_regs(regs);
	halt_cpu();
}

void do_dataabt_vector(struct pt_regs *regs)
{
	os_printk(LOG_ERROR, "data abort\n");
	show_regs(regs);
	halt_cpu();
}

void do_notused_vector(struct pt_regs *regs)
{
	os_printk(LOG_ERROR, "not used vector\n");
	show_regs(regs);
	halt_cpu();
}

void do_irq_vector(void)
{
#define APB_ICTL_IRQ_FINALSTATUS_L	0x10000000 + 0x30
	uint32 irqreg = 0;
	irqreg = *(uint32 *) (APB_ICTL_IRQ_FINALSTATUS_L);
	return;
}

void do_fiq_vector(struct pt_regs *regs)
{
	os_printk(LOG_ERROR, "fiq vetor\n");
	halt_cpu();
}
