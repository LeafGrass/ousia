/*
 * Copyright (c) 2011-2013 LeafGrass <leafgrass.g@gmail.com>
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
 * @file    platform/sim/utils/entry.c
 * @brief   sim entry
 * @log     2013.7 initial revision
 */

#include <ousia/ousia.h>
#include <ousia/ousia_type.h>

#include <stdlib.h>
#include <unistd.h>
#include <sys/print.h>
#include <sys/time.h>
#include <sys/utils.h>

#include "utils/utils.h"

void __os_main(void);

int main(void)
{
	__os_main();

	while (1)
		sleep(1);
	return 0;
}
