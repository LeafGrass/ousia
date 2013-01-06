########################################
# User Customization Items
########################################
BOARD = mini
MEMORY_TARGET = flash

########################################
# System Configurations
########################################
# NOTE!
# In USB configuration, BOARD_USB_DISC_DEV is the GPIO port containing
# the USB_DISC pin, and BOARD_USB_DISC_BIT is that pin's bit.
# These are specialized for stm32.

ifeq ($(BOARD), mini)
BOOT_BIN := bootloader/prebuilt/mini_boot.bin
MCU := STM32F103CB
MCU_SERIES := stm32f1
MCU_F1_LINE := performance
LD_MEM_DIR := sram_20k_flash_128k
VENDOR_ID := 1EAF
PRODUCT_ID := 0003
ERROR_LED_PORT := GPIOB
ERROR_LED_PIN := 1
USR_BUT_PORT := GPIOB
USR_BUT_PIN := 8
USART_CONSOLE_BANK := USART1
VCOM_ID_PRODUCT	:= 0004
USB_DISC_DEV := GPIOB
USB_DISC_BIT := 9
endif
ifeq ($(BOARD), leach)
BOOT_BIN := bootloader/prebuilt/maple_boot.bin
MCU = STM32F103RB
MCU_SERIES := stm32f1
MCU_F1_LINE := performance
LD_MEM_DIR := sram_20k_flash_128k
VENDOR_ID = 1EAF
PRODUCT_ID = 0003
ERROR_LED_PORT = GPIOA
ERROR_LED_PIN  = 5
USR_BUT_PORT := GPIOC
USR_BUT_PIN := 9
USART_CONSOLE_BANK = USART1
VCOM_ID_PRODUCT	= 0004
USB_DISC_DEV = GPIOC
USB_DISC_BIT = 12
endif
ifeq ($(BOARD), leach_h)
BOOT_BIN := bootloader/prebuilt/maple_boot.bin
MCU = STM32F103VC
MCU_SERIES := stm32f1
MCU_F1_LINE := performance
LD_MEM_DIR := sram_64k_flash_512k
VENDOR_ID = 1EAF
PRODUCT_ID = 0003
ERROR_LED_PORT = GPIOA
ERROR_LED_PIN  = 5
USART_CONSOLE_BANK = USART1
VCOM_ID_PRODUCT	= 0004
USB_DISC_DEV = GPIOC
USB_DISC_BIT = 12
endif

ifeq ($(MCU_SERIES), stm32f1)
	MCU_LINE := $(MCU_F1_LINE)
endif
ifeq ($(MCU_SERIES), stm32f2)
	MCU_LINE := $(MCU_F2_LINE)
endif
LDDIR := $(PLATFORM_PATH)/$(TARGET_PLATFORM)/ld
LD_SERIES_PATH := $(LDDIR)/stm32/series/$(MCU_SERIES)
LD_MEM_PATH := $(LDDIR)/stm32/mem/$(LD_MEM_DIR)
LD_SERIES_LINE_PATH := $(LDDIR)/stm32/series/$(MCU_SERIES)/$(MCU_LINE)

# Some target specific things
ifeq ($(MEMORY_TARGET), ram)
	LD_SCRIPT_PATH := $(LDDIR)/$(MEMORY_TARGET).ld
	VECT_BASE_ADDR := VECT_TAB_RAM
endif
ifeq ($(MEMORY_TARGET), flash)
	LD_SCRIPT_PATH := $(LDDIR)/$(MEMORY_TARGET).ld
	VECT_BASE_ADDR := VECT_TAB_FLASH
endif
ifeq ($(MEMORY_TARGET), jtag)
	LD_SCRIPT_PATH := $(LDDIR)/$(MEMORY_TARGET).ld
	VECT_BASE_ADDR := VECT_TAB_BASE
endif

GLOBAL_FLAGS := \
	$(VERFLAGS) \
	-DOUSIA \
	-D__PLATFORM_STM32__ \
	-D$(VECT_BASE_ADDR) \
	-DBOARD_$(BOARD) \
	-DMCU_$(MCU) \
	-DERROR_LED_PORT=$(ERROR_LED_PORT) \
	-DERROR_LED_PIN=$(ERROR_LED_PIN) \
	-DUSR_BUT_PORT=$(USR_BUT_PORT) \
	-DUSR_BUT_PIN=$(USR_BUT_PIN) \
	-DUSART_CONSOLE_BANK=$(USART_CONSOLE_BANK) \
	-DVCOM_ID_PRODUCT=$(VCOM_ID_PRODUCT) \
	-DUSB_DISC_DEV=$(USB_DISC_DEV) \
	-DUSB_DISC_BIT=$(USB_DISC_BIT)

GLOBAL_CFLAGS := \
	-g3 -gdwarf-2 -mcpu=cortex-m3 -mthumb -march=armv7-m -O2 \
	-nostdlib -nostdinc -ffunction-sections -fdata-sections \
	-Wno-unused-function $(GLOBAL_FLAGS) \

GLOBAL_ASFLAGS := \
	-mcpu=cortex-m3 -march=armv7-m -mthumb \
	-x assembler-with-cpp $(GLOBAL_FLAGS)

LDFLAGS := \
	-L$(LDDIR) \
	-L$(LD_SERIES_PATH) \
	-L$(LD_MEM_PATH) \
	-L$(LD_SERIES_LINE_PATH) \
	-T$(LD_SCRIPT_PATH) \
	--gc-sections -nostartfiles

# Build Environment
SHELL	:= /bin/bash
PYTHON	:= /usr/bin/python
CROSS   := arm-none-eabi-
