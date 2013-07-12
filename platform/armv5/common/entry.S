#define RAM_CODE_START 0x000000

#define USR_MODE 0x10
#define FIQ_MODE 0x11
#define IRQ_MODE 0x12
#define SVC_MODE 0x13
#define ABT_MODE 0x17
#define UND_MODE 0x1B
#define SYS_MODE 0x1F

#define IRQ_BIT 0x80
#define FIQ_BIT 0x40

#if 0

#define RAM_RES_STACK 0x4001000
#define RAM_IRQ_STACK 0x4002000
#define RAM_SVC_STACK 0x4003000

#else

	.align 5
	.section ".stack", "w"
stack_spaces:
	.space	1024
RAM_RES_STACK:
	.space	1024
RAM_IRQ_STACK:
	.space	1024
RAM_SVC_STACK:

#endif

/*
 * The __start code may be probrammed into ROM/Flash
 * after reset, CPU jumps to __start
 */
		.section ".start", "ax"
		.align
.globl __start
__start:
	b hardware_reset
	ldr	pc, _undintr_vector
	ldr	pc, _swi_vector
	ldr	pc, _preabt_vector
	ldr	pc, _dataabt_vector
	ldr	pc, _notused_vector
	ldr	pc, _irq_vector
	ldr	pc, _fiq_vector

_undintr_vector:
	.word undintr_vector
_swi_vector:
	.word swi_vector
_preabt_vector:
	.word preabt_vector
_dataabt_vector:
	.word dataabt_vector
_notused_vector:
	.word notused_vector
_irq_vector:
	.word irq_vector
_fiq_vector:
	.word fiq_vector

	/*
	 * Padding with some special number
	 */
	.balignl 16,0x19801111

	/*
	 * Global variables
	 */
_RAM_CODE_START:
	.word RAM_CODE_START	@ defined

.globl _ROM_CODE_START
_ROM_CODE_START:
	.word __start			@ See above

.globl _ROM_CODE_END
_ROM_CODE_END:
	.word __end				@ Setup in linker scripts

.globl _bss_start
_bss_start:
	.word __bss_start		@ Setup in linker scripts

.globl _bss_end
_bss_end:
	.word __bss_end				@ Setup in linker scripts

.globl _IRQ_STACK
_IRQ_STACK:
	.word RAM_IRQ_STACK		@ defined

.globl _RES_STACK
_RES_STACK:
	.word RAM_RES_STACK		@ defined

.globl _SVC_STACK
_SVC_STACK:
	.word RAM_SVC_STACK		@ defined

	/*
	 * Real code starts here
	 */
hardware_reset:
	/* Set to SVC32 */
	mrs r0, cpsr
	bic r0, #0x1f
	orr r0, #0xd3	@ 1101 0011: IRQ=off, FIQ=off, Mode=SVC32
	msr cpsr, r0

	/* Other settings? */
	@ platform
	@ cp15

	/*
	 * Relocate code from ROM to RAM
	 */
relocate_code:
	adr r0, __start			@ pc + __start's offset, ROM code start: src address
	ldr	r1, _RAM_CODE_START	@ RAM code start: dest address
	cmp     r0, r1
	beq     stack_setup     @codes is in ram,don't need to copy codes

	ldr	r2, _ROM_CODE_START
	ldr	r3, _ROM_CODE_END
	sub	r2, r3, r2
	add	r2, r0, r2			@ ROM code end: src size r2
1:
	ldmia	r0!, {r4-r11}
	stmia	r1!, {r4-r11}
	cmp	r0, r2
	ble	1b

	/*
	 * Setup stack, don't setup for undef,abort,user mode
	 */
stack_setup:
	@ CPU has been in SVC mode.
	mrs r1, cpsr	@ Save cpsr to r1

	ldr	r0, _SVC_STACK
	mov sp, r0		@ Setup SVC stack

	mov r0, #0xd7	@ 1101 0111: IRQ=off, FIQ=off, Mode=Abort
	msr cpsr, r0
	ldr r0, _RES_STACK
	mov sp, r0

	mov r0, #0xdb	@ 1101 1011: IRQ=off, FIQ=off, Mode=Undef
	msr cpsr, r0
	ldr r0, _RES_STACK
	mov sp, r0

	mov r0, #0xd1	@ 1101 0001: IRQ=off, FIQ=off, Mode=FIQ
	msr cpsr, r0
	ldr r0, _RES_STACK
	mov sp, r0

	mov r0, #0x92	@ 1001 0010: IRQ=off, FIQ=on, Mode=IRQ
	msr cpsr, r0
	ldr r0, _IRQ_STACK
	mov sp, r0

	msr cpsr, r1	@ Restore to SVC32 mode, IRQ=off, FIQ=off

	/*bl dump_sm_trace*/

zero_bss:
	ldr	r0, _bss_start
	ldr	r1, _bss_end
	mov r2, #0x0
1:
	str	r2, [r0]
	add	r0, r0, #4
	cmp	r0, r1
	ble	1b

	/*
	 * Jump to RAM code
	 */
	ldr	pc, __entry
__entry:
	.word __os_main

/*
 * exception handlers
 */
#define ARM_FRAME_SIZE	72
#define ARM_R0		0
#define ARM_R1		4
#define ARM_R2		8
#define ARM_R3		12
#define ARM_R4		16
#define ARM_R5		20
#define ARM_R6		24
#define ARM_R7		28
#define ARM_R8		32
#define ARM_R9		36
#define ARM_R10		40
#define ARM_FP		44
#define ARM_IP		48
#define ARM_SP		52
#define ARM_LR		56
#define ARM_PC		60
#define ARM_PSR		64
#define ARM_OLD_R0	68

	.align  5
undintr_vector:
	sub sp, #ARM_FRAME_SIZE
	stmia sp, {r0-r15}
	add r1, sp, #ARM_PSR
	mrs r0, spsr
	str r0, [r1]
	mov r0, sp
	bl do_undintr_vector

	.align 5
swi_vector:
	sub sp, #ARM_FRAME_SIZE
	stmia sp, {r0-r15}
	add r1, sp, #ARM_PSR
	mrs r0, spsr
	str r0, [r1]
	mov r0, sp
	bl do_swi_vector

	.align 5
preabt_vector:
	sub sp, #ARM_FRAME_SIZE
	stmia sp, {r0-r15}
	add r1, sp, #ARM_PSR
	mrs r0, spsr
	str r0, [r1]
	mov r0, sp
	bl do_preabt_vector

	.align 5
dataabt_vector:
	sub sp, #ARM_FRAME_SIZE
	stmia sp, {r0-r15}
	add r1, sp, #ARM_PSR
	mrs r0, spsr
	str r0, [r1]
	mov r0, sp
	bl do_dataabt_vector

	.align 5
notused_vector:
	sub sp, #ARM_FRAME_SIZE
	stmia sp, {r0-r15}
	add r1, sp, #ARM_PSR
	mrs r0, spsr
	str r0, [r1]
	mov r0, sp
	bl do_notused_vector

/*
 * non-nested interrupt handling currently.
 */
	.align 5
irq_vector:
	sub sp, #ARM_FRAME_SIZE
	stmia sp, {r0-r12}		/* Save previous mode r0-r12 */
	add r1, sp, #ARM_SP
	stmia r1, {sp, lr}^		/* Save user mode [sp and lr] ? */
	add r1, r1, #8
	str lr, [r1], #4		/* Save previous mode pc */
	mrs r2, spsr
	str r2, [r1], #4		/* Save previous mode cpsr */
	str r0, [r1]			/* Save old r0 */
	mov r0, sp
	bl do_irq_vector
	ldmia sp, {r0-lr}^		/* Load to user mode r0-r12, [sp, lr] */
	mov r0, r0
	ldr lr, [sp, #ARM_PC]
	add sp, #ARM_FRAME_SIZE
	subs pc, lr, #4

	.align 5
fiq_vector:
	sub lr, lr, #4
	stmfd sp!, {r0-r7,lr}
	mov r0, sp
	bl do_fiq_vector
	ldmfd sp!, {r0-r7,pc}^


