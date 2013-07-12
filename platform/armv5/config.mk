########################################
# User Customization Items
########################################
BOARD = rdk
MEMORY_TARGET = ram

########################################
# System Configurations
########################################
ERROR_LED_PORT = 13
UART_CONSOLE_BANK = 0

LDDIR := $(PLATFORM_PATH)/$(TARGET_PLATFORM)/ld

LD_SCRIPT_PATH := $(LDDIR)/$(MEMORY_TARGET).ld

GLOBAL_FLAGS := \
	$(VERFLAGS) \
	-DOUSIA \
	-D__PLATFORM_ARMV5__ \
	-DBOARD_$(BOARD) \
	-DERROR_LED_PORT=$(ERROR_LED_PORT) \
	-DUART_CONSOLE_BANK=$(UART_CONSOLE_BANK)

GLOBAL_CFLAGS := \
	-fno-optimize-sibling-calls -nostdinc -mlittle-endian -march=armv5te \
	-mapcs -mabi=aapcs-linux -mthumb-interwork -msoft-float -g -ggdb \
	-fno-strict-aliasing -fsigned-char -fsigned-bitfields -O0 \
	$(GLOBAL_FLAGS)

GLOBAL_ASFLAGS := \
	-fno-optimize-sibling-calls -nostdinc -mlittle-endian -march=armv5te \
	-mapcs -mabi=aapcs-linux -mthumb-interwork -msoft-float -g -ggdb \
	-fno-strict-aliasing -fsigned-char -fsigned-bitfields -O0 \
	$(GLOBAL_FLAGS)

LDFLAGS := \
	-L$(LDDIR) \
	-T$(LD_SCRIPT_PATH) \
	-nostdlib -nostartfiles

# Build Environment
SHELL	:= /bin/bash
CROSS   := arm-marvell-eabi-
