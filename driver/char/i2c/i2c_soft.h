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
 * @file    driver/char/i2c/i2c_soft.h
 * @brief   gpio operations as software implemented i2c bus
 * @log     2011.12 initial revision
 */

#ifndef __I2C_SOFT_H__
#define __I2C_SOFT_H__

#define I2C_SOFT_RET_OK		0
#define I2C_SOFT_RET_ERR	-1
#define I2C_SOFT_RET_ENACK	-2

void i2c_soft_init(void);
int8 i2c_soft_eeprom_write(uint8 addr, uint8 * pbuf, uint32 nb);
int8 i2c_soft_eeprom_read(uint8 addr, uint8 * pbuf, uint32 nb);

#endif /* __I2C_SOFT_H__ */
