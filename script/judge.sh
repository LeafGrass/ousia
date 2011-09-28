#!/bin/bash

# Ugly bash code :P
# Judge if files in "core/port/" and "platform/$(TARGET_PLATFORM)/port/"
# is the same.

flag=0

fileA1=./core/port/ousia_port.c
fileA2=./platform/stm32/port/ousia_port.c
fileB1=./core/port/ousia_port.h
fileB2=./platform/stm32/port/ousia_port.h
fileC1=./core/port/ousia_cfg.h
fileC2=./platform/stm32/port/ousia_cfg.h

if diff $fileA1 $fileA2 > /dev/null; then
	flag=0
else
	flag=1
fi
if diff $fileA1 $fileA2 > /dev/null; then
	flag=0
else
	flag=1
fi
if diff $fileA1 $fileA2 > /dev/null; then
	flag=0
else
	flag=1
fi

if [ $flag == 1 ]; then
	echo flag=1
else
	echo flag=0
fi

