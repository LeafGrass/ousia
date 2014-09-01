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
 * @file    framework/console/cmd.c
 * @brief   various definitions and implementations of commands
 * @log     2012.9 first revision
 */

#include <ousia/ousia.h>
#include <ousia/ousia_type.h>
#include <ulib/stdlib.h>
#include <ulib/string.h>
#include <sys/time.h>
#include <sys/sched.h>
#include <sys/print.h>
#include <sys/mm.h>

#include "console/console.h"
#include "console/cmd.h"

static int32 cmd_help(int argc, char **argv);
static int32 cmd_debug(int argc, char **argv);
static int32 cmd_reboot(int argc, char **argv);
static int32 cmd_free(int argc, char **argv);
static int32 cmd_xd(int argc, char **argv);
static int32 cmd_mr(int argc, char **argv);
static int32 cmd_mw(int argc, char **argv);

const struct cmd_handle hcmd_arr[] = {
	{
		.cmd_word = "help",
		.cmd_fn = cmd_help,
	},
	{
		.cmd_word = "?",
		.cmd_fn = cmd_help,
	},
	{
		.cmd_word = "debug",
		.cmd_fn = cmd_debug,
	},
	{
		.cmd_word = "reboot",
		.cmd_fn = cmd_reboot,
	},
	{
		.cmd_word = "free",
		.cmd_fn = cmd_free,
	},
	{
		.cmd_word = "xd",
		.cmd_fn = cmd_xd,
	},
	{
		.cmd_word = "mr",
		.cmd_fn = cmd_mr,
	},
	{
		.cmd_word = "mw",
		.cmd_fn = cmd_mw,
	},
	{
		.cmd_word = NULL,
		.cmd_fn = NULL,
	}
};

void commands_register(struct console_cmd *conc)
{
	conc->hcmd_arr = hcmd_arr;
}

static int32 cmd_help(int argc, char **argv)
{
	const struct cmd_handle *c;
	os_printf("available commands:\n");
	for (c = &hcmd_arr[0]; c->cmd_word != NULL; c++)
		os_printf("\t%s\n", c->cmd_word);
	return 0;
}

static int32 cmd_debug(int argc, char **argv)
{
	_sched_dump_pq();
	return 0;
}

static int32 cmd_reboot(int argc, char **argv)
{
#define DO_SOMETHING()	os_process_sleep(1000)
	/* TODO Prepare things before reboot */
	os_printf("Rebooting...\n");
	/* TODO Do real hardware reset here */
	DO_SOMETHING();
	return 0;
}

static int32 cmd_free(int argc, char **argv)
{
	struct mallinfo data;
	mallinfo(&data);
	os_printf("              total       used       free    largest\n");
	os_printf("Mem:    %11d%11d%11d%11d\n",
		  data.arena, data.uordblks, data.fordblks, data.mxordblk);
	return 0;
}

static int32 cmd_xd(int argc, char **argv)
{
	char *addr;
	int32 nbytes;

	if (argv[1] == NULL || argv[2] == NULL)
		return -1;

	addr = (char *)((uintptr_t)atol(argv[1]));
	os_printf("addr: 0x%x\n", addr);

	nbytes = (int)atol(argv[2]);
	os_printf("nb: %d\n", nbytes);

	char line[128];
	unsigned char *buffer = (unsigned char *)addr;
	int ch;
	int i;
	int j;

	os_printf("memory dump\n");
	for (i = 0; i < nbytes; i += 16) {
		os_sprintf(line, "%04x: ", i);

		for ( j = 0; j < 16; j++) {
			if (i + j < nbytes)
				os_sprintf(&line[strlen(line)], "%02x ", buffer[i+j] );
			else
				strcpy(&line[strlen(line)], "   ");
		}

		for ( j = 0; j < 16; j++) {
			if (i + j < nbytes) {
				ch = buffer[i+j];
				os_sprintf(&line[strlen(line)], "%c", ch >= 0x20 && ch <= 0x7e ? ch : '.');
			}
		}

		os_printf("%s\n", line);
	}
	return 0;
}

static int32 cmd_mr(int argc, char **argv)
{
	return 0;
}

static int32 cmd_mw(int argc, char **argv)
{
	return 0;
}
