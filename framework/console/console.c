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
#include <port/ousia_cfg.h>
#include <ulib/string.h>
#include <sys/time.h>
#include <sys/sched.h>
#include <sys/print.h>
#include <sys/ds.h>

#include "console/cmd.h"
#include "console/console.h"

#define PS_CMDEXEC_STACK_SIZE	512

static struct console_cmd conc;

static void ps_cmdexec(void *args)
{
	int32 ret = 0;
	int32 index = (int32) args;

	ret = conc.hcmd_arr[index].cmd_fn(NULL);
	if (ret != 0)
		os_printf("exec fail %d\n", index);
	os_process_delete(0);
}

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

static int32 parse_cmd(struct console_cmd *conc)
{
	int32 index;
	int32 i;
	char cw[16] = { 0 };

	for (i = 0; i < conc->ccs.nc; i++) {
		cw[i] = conc->cbuf[i].c;
		if (cw[i] == ' ' || cw[i] == '\0')
			break;
	}

	for (index = 0; conc->hcmd_arr[index].cmd_word != NULL; index++) {
		if (strcmp(cw, conc->hcmd_arr[index].cmd_word) == 0)
			return index;
		else
			continue;
	}

	return -1;
}

static int32 parse_args(struct console_cmd *conc)
{
	return 0;
}

static int32 process_enter(struct console_cmd *conc)
{
#ifndef DEBUG_CONSOLE
	int32 cmd_index = 0;
	int32 backend = 0;
	int32 ret;

	cmd_index = parse_cmd(conc);
	backend = parse_args(conc);
	if (cmd_index >= 0) {
		if (backend) {
			/* FIXME Create a process like will still crash */
			ret = os_process_create(ps_cmdexec, (void *)cmd_index,
						PS_CMDEXEC_STACK_SIZE);
			if (ret < 0)
				os_printf("command exec failed %d\n", ret);
		} else {
			/* TODO Pass in buffer of args instead of NULL */
			conc->hcmd_arr[cmd_index].cmd_fn(NULL);
		}
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

	ch = os_getchar();
	switch (ch) {
	case '\r':
		os_putchar('\n');
		os_putchar('\r');
		process_enter(conc);
		break;
	case '\b':
		if (conc->ccs.nc != 0) {
			os_putchar('\b');
			os_putchar(' ');
			os_putchar('\b');
			__cmd_char_pop(conc);
		}
		break;
	default:
		os_putchar(ch);
		__cmd_char_push(conc, ch);
		break;
	}
}

void ps_console(void *args)
{
	INIT_LIST_HEAD(&conc.ccs.s);
	commands_register(&conc);
	os_printf("\nWelcome to Ousia console. :)\n");
	os_printf("Please press ENTER to activate it.\n");
	for (;;) {
		console_echo(&conc);
		os_process_sleep(10);
	}
}
