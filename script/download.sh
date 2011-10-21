#!/bin/bash

# by librae
# Temporarily only support medium-density, i.e. 128K Flash.

if [ -n "$1" ]; then
        echo ''
        echo "download specified image $1."
        echo ''
        openocd -f ./support/openocd/stm32.cfg -c "init" -c "targets" -c "reset halt" -c "targets" -c "flash erase_address 0x08000000 0x20000" -c "flash write_bank 0 $1 0" -c "verify_image $1 0x08000000" -c "reset run" -c "shutdown"
else
        echo ''
        echo "download default image.."
        echo ''
        openocd -f ./support/openocd/stm32.cfg -c "init" -c "targets" -c "reset halt" -c "targets" -c "flash erase_address 0x08000000 0x20000" -c "flash write_bank 0 ./build/ousia.bin 0" -c "verify_image ./build/ousia.bin 0x08000000 bin" -c "reset run" -c "shutdown"
fi

