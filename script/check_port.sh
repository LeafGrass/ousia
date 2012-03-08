#!/bin/bash

# ** NOTICE ** This is a legacy file, and it won't be used during build

# ugly bash code, can only work depend on this specific directory structure :P
# check if files in "core/port/" and "platform/$(TARGET_PLATFORM)/port/" is the same
# if not, copy the latest code into "core/port/"

flag_different=0
build_type=`cat ./build/build-type`

fileA1=./core/port/ousia_port.c
fileA2=./platform/$build_type/port/ousia_port.c
fileB1=./core/port/ousia_port.h
fileB2=./platform/$build_type/port/ousia_port.h
fileC1=./core/port/ousia_cfg.h
fileC2=./platform/$build_type/port/ousia_cfg.h

if diff $fileA1 $fileA2 > /dev/null; then
        flag_different=0
else
        flag_different=1
fi
if diff $fileA1 $fileA2 > /dev/null; then
        flag_different=0
else
        flag_different=1
fi
if diff $fileA1 $fileA2 > /dev/null; then
        flag_different=0
else
        flag_different=1
fi

if [ $flag_different == 1 ]; then
        rm -rf ./core/port/*
        cp ./platform/$build_type/port/ousia_*.* ./core/port/
else
        echo
fi

