/*
 * Copyright (c) 2011-2012 LeafGrass <leafgrass.g@gmail.com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 * ARM SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR
 * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 */

/*
 * @file    framework/console/console.c
 * @brief   basic implementation of console as a shell like user interface
 * @log     2011.11 first revision
 */

#include <ousia/ousia.h>
#include <ousia/ousia_type.h>
#include <ulib/string.h>
#include <sys/time.h>
#include <sys/sched.h>
#include <sys/print.h>
#include <sys/ds.h>
#include <console/console.h>
#include <stm32/libmaple/include/libmaple/libmaple.h>
#include <stm32/stm32utils/stm32utils.h>

/*#define DEBUG_CONSOLE*/
#define USB_SERIAL

#define CMD_CHAR_BUF_SIZE	32
#define CMD_FN_ARGS_SIZE	16

struct cmd_handle {
	char *cmd_word;
	int32 (*cmd_fn)(void *args);
};

struct cmd_char_stack {
	int32 nc;
	int32 nc_tot;
	struct list_head s;
};

struct cmd_char {
	char c;
	struct list_head list;
};

struct console_cmd {
	struct cmd_char_stack	ccs;
	struct cmd_char		cbuf[CMD_CHAR_BUF_SIZE];
#if 0
	char			cargs[CMD_FN_ARGS_SIZE];
#endif
};

/*
 * @brief   command - reboot
 * @param   args -i- arguments for command
 * @return  status code
 */
static int32 cmd_reboot(void *args)
{
	os_printf("reboot command.\n");
	return 0;
}

/*
 * @brief   command - reserved
 * @param   args -i- arguments for command
 * @return  status code
 */
static int32 cmd_reserved(void *args)
{
	return 0;
}

/*
 * FIXME
 * Should not be placed here, not convenient to update.
 */
static const struct cmd_handle hcmd_arr[] = {
	{
		.cmd_word = "reboot",
		.cmd_fn = cmd_reboot,
	},
	{
		.cmd_word = "reserved",
		.cmd_fn = cmd_reserved,
	},
	{
		.cmd_word = NULL,
		.cmd_fn = NULL,
	}
};

static void ps_cmdexec(void *args)
{
	int32 ret = 0;
	int32 index = (int32)args;

	ret = hcmd_arr[index].cmd_fn(NULL);
	if (ret != 0)
		os_printf("exec fail %d\n", index);
	os_process_delete(0);
}

static struct console_cmd conc;

#define PS_CMDEXEC_STACK_SIZE	512
static uint8 ps_cmdexec_stack[PS_CMDEXEC_STACK_SIZE] = {0};
static struct _pcb_t ps_cmdexec_pcb;

static void __cmd_char_push(struct console_cmd *conc, char ch)
{
	struct cmd_char *cc;

	if (conc->ccs.nc >= 0 && conc->ccs.nc < CMD_CHAR_BUF_SIZE) {
		cc = &conc->cbuf[conc->ccs.nc];
		cc->c = ch;
		list_add(&cc->list, &conc->ccs.s);
		conc->ccs.nc++;
	}
	conc->ccs.nc_tot++;
}

static void __cmd_char_pop(struct console_cmd *conc)
{
	struct cmd_char *cc;

	if (conc->ccs.nc_tot > conc->ccs.nc) {
		conc->ccs.nc_tot--;
		return;
	}
	if (conc->ccs.nc > 0 && conc->ccs.nc <= CMD_CHAR_BUF_SIZE) {
		cc = &conc->cbuf[conc->ccs.nc - 1];
		cc->c = 0;
		list_del(&cc->list);
		conc->ccs.nc--;
	}
	if (conc->ccs.nc_tot > 0)
		conc->ccs.nc_tot--;
}

static void __cmd_char_stack_flush(struct console_cmd *conc)
{
	struct cmd_char *cc, *cc_tmp;
	list_for_each_entry_safe(cc, cc_tmp, &conc->ccs.s, list) {
		list_del(&cc->list);
		cc->c = 0;
	}
	conc->ccs.nc = 0;
	conc->ccs.nc_tot = 0;
	memset(conc->cbuf, 0, sizeof(conc->cbuf));
}

static int32 seek_cmd_function(struct console_cmd *conc)
{
	int32 index;
	int32 i;
	char cw[16] = {0};

	for (i = 0; i < conc->ccs.nc; i++) {
		cw[i] = conc->cbuf[i].c;
		if (cw[i] == ' ' || cw[i] == '\0')
			break;
	}

	for (index = 0; hcmd_arr[index].cmd_word != NULL; index++) {
		if (strcmp(cw, hcmd_arr[index].cmd_word) == 0)
			return index;
		else
			continue;
	}

	return -1;
}

static int32 process_enter(struct console_cmd *conc)
{
#ifndef DEBUG_CONSOLE
	int32 cmd_index = -1;
	cmd_index = seek_cmd_function(conc);
	if (cmd_index >= 0) {
		os_process_create(&ps_cmdexec_pcb, ps_cmdexec, (void *)cmd_index,
				  ps_cmdexec_stack, PS_CMDEXEC_STACK_SIZE);
	}
#else
	struct cmd_char *cc;
	list_for_each_entry_reverse(cc, &conc->ccs.s, list) {
		os_printf("%c", cc->c);
	}
	os_printf("\nnc: %d, tot: %d\n", conc->ccs.nc, conc->ccs.nc_tot);
#endif
	__cmd_char_stack_flush(conc);
	os_printf("$ ");

	return 0;
}

static void console_echo(struct console_cmd *conc)
{
	char ch = 0;

#ifdef USB_SERIAL
	if (stm32utils_usb_getc(NULL, &ch) == 0) {
		switch (ch) {
		case '\r':
			os_printf("\n");
			process_enter(conc);
			break;
		case '\b':
			os_printf("\b \b");
			__cmd_char_pop(conc);
			break;
		default:
			os_printf("%c", ch);
			__cmd_char_push(conc, ch);
			break;
		}
	}
#else
	if (USART_CONSOLE_BANK->flag_trigger) {
		for (i = 0; i < USART_CONSOLE_BANK->cnt_trigger; i++) {
			stm32utils_io_getc(USART_CONSOLE_BANK, &ch);
			switch (ch) {
			case 0:
				break;
			case '\r':
				os_printf( "\r\n" );
				break;
			case '\b':
				os_printf( "\b \b" );
				break;
			default:
				os_printf( "%c", ch );
				break;
			}
		}
	}
#endif
}

void ps_console(void *args)
{
	INIT_LIST_HEAD(&conc.ccs.s);
	for (;;) {
		console_echo(&conc);
		os_process_sleep(10);
	}
}
