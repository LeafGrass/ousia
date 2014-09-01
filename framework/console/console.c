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
#define MAX_ARGV_ENTRIES	4 /* 1 cmd word and MAX_ARGV_ENTRIES - 1 args */

static struct console_cmd concmd;
static char *argv[MAX_ARGV_ENTRIES];
static int32 argc;

static void ps_cmdexec(void *args)
{
	int32 ret = 0;
	struct console_cmd *conc = (struct console_cmd *)args;

	ret = conc->hcmd_arr[conc->index].cmd_fn(argc, argv);
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

static int32 parse_cmd(struct console_cmd *conc)
{
	char str[32] = { 0 };
	char *args = NULL;
	char *tok = NULL;
	static char *cmd_saveptr = NULL;
	int32 i;

	for (i = 0; i < conc->ccs.nc; i++)
		str[i] = conc->cbuf[i].c;

	tok = strtok_r(str, " ", &cmd_saveptr);

	for (i = 0; conc->hcmd_arr[i].cmd_word != NULL; i++) {
		if (strcmp(tok, conc->hcmd_arr[i].cmd_word) == 0) {
			conc->index = i;
			break;
		}
	}
	if (conc->index < 0) {
		os_printf("%s: command not found\n", tok);
		return 0;
	}

	memset(argv, 0, MAX_ARGV_ENTRIES * sizeof(char *));
	argv[0] = conc->hcmd_arr[conc->index].cmd_word;

	args = strtok_r(NULL, "\r", &cmd_saveptr);

	memset(conc->args, 0, CMD_CHAR_BUF_SIZE);
	i = 0;
	if (args != NULL) {
		strcpy(conc->args, args);
		cmd_saveptr = NULL;
		tok = strtok_r((char *)conc->args, " ", &cmd_saveptr);
		while (tok != NULL) {
			os_printf("argv[%d]: %s\n", ++i, tok);
			argv[i] = tok;
			if (i == MAX_ARGV_ENTRIES - 1)
				break;
			tok = strtok_r(NULL, " ", &cmd_saveptr);
		}
	}
	argc = i + 1;
	if (argc > 1)
		os_printf("argc: %d\n", argc);

	return strchr(args, '&') == NULL ? 0 : 1;
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
		/* FIXME Create a process like will still crash */
		if (backend) {
			ret = os_process_create(ps_cmdexec, conc,
						PS_CMDEXEC_STACK_SIZE);
			if (ret < 0)
				os_printf("command exec failed %d\n", ret);
		} else {
			conc->hcmd_arr[conc->index].cmd_fn(argc, argv);
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
