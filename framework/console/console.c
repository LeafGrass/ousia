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

static struct console_cmd concmd;

static void ps_cmdexec(void *args)
{
	int32 ret = 0;
	struct console_cmd *conc = (struct console_cmd *)args;

	ret = conc->hcmd_arr[conc->index].cmd_fn(conc->args);
	if (ret != 0)
		os_printf("exec fail %d\n", conc->index);
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

static char *cmd_saveptr = NULL;

static int32 parse_cmd(struct console_cmd *conc)
{
	int32 index;
	int32 i;
	int32 ret;
	char str[32] = { 0 };
	char *cmd = NULL;
	char *args = NULL;

	for (i = 0; i < conc->ccs.nc; i++)
		str[i] = conc->cbuf[i].c;

	cmd = strtok_r(str, " ", &cmd_saveptr);

	for (index = 0; conc->hcmd_arr[index].cmd_word != NULL; index++) {
		if (strcmp(cmd, conc->hcmd_arr[index].cmd_word) == 0) {
			conc->index = index;
		} else {
			os_printf("%s: command not found\n", cmd);
			return -1;
		}
	}

	args = strtok_r(NULL, "&", &cmd_saveptr);
	if (args == NULL) {
		args = strtok_r(NULL, "\r", &cmd_saveptr);
		ret = 0;
	} else {
		ret = 1;
	}
	os_printf("args: %s\n", args);
	strcpy(conc->args, args);

	return ret;
}

static int32 process_enter(struct console_cmd *conc)
{
#ifndef DEBUG_CONSOLE
	int32 backend = 0;
	int32 ret;

	conc->index = -1;
	if (conc->ccs.nc > 0)
		backend = parse_cmd(conc);

	if (conc->index >= 0) {
		if (backend) {
			/* FIXME Create a process like will still crash */
			ret = os_process_create(ps_cmdexec, conc,
						PS_CMDEXEC_STACK_SIZE);
			if (ret < 0)
				os_printf("command exec failed %d\n", ret);
		} else {
			conc->hcmd_arr[conc->index].cmd_fn(NULL);
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
	memset(&concmd, 0, sizeof(struct console_cmd));
	INIT_LIST_HEAD(&concmd.ccs.s);
	commands_register(&concmd);
	os_printf("\nWelcome to Ousia console. :)\n");
	os_printf("Please press ENTER to activate it.\n");
	for (;;) {
		console_echo(&concmd);
		os_process_sleep(10);
	}
}
