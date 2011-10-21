MCU = STM32F103RB
BOARD = leach
#BOARD = leach_high
MEMORY_TARGET = flash
DENSITY = STM32_MEDIUM_DENSITY
#DENSITY = STM32_HIGH_DENSITY
VENDOR_ID = 1EAF
PRODUCT_ID = 0003
ERROR_LED_PORT = GPIOA
ERROR_LED_PIN  = 5
USART_CONSOLE_BANK = USART1

# Some target specific things
ifeq ($(MEMORY_TARGET), ram)
	LDSCRIPT := $(BOARD)/ram.ld
	VECT_BASE_ADDR := VECT_TAB_RAM
endif
ifeq ($(MEMORY_TARGET), flash)
	LDSCRIPT := $(BOARD)/flash.ld
	VECT_BASE_ADDR := VECT_TAB_FLASH
endif
ifeq ($(MEMORY_TARGET), jtag)
	LDSCRIPT := $(BOARD)/jtag.ld
	VECT_BASE_ADDR := VECT_TAB_BASE
endif

GLOBAL_FLAGS := \
	-DOUSIA \
	-D__PLATFORM__=__STM32__ \
	-D$(DENSITY) \
	-D$(VECT_BASE_ADDR) \
	-DBOARD_$(BOARD) \
	-DMCU_$(MCU) \
	-DERROR_LED_PORT=$(ERROR_LED_PORT) \
	-DERROR_LED_PIN=$(ERROR_LED_PIN) \
	-DUSART_CONSOLE_BANK=$(USART_CONSOLE_BANK)

GLOBAL_CFLAGS := \
	-O2 -g3 -gdwarf-2 -mcpu=cortex-m3 -mthumb -march=armv7-m \
	-nostdlib -ffunction-sections -fdata-sections \
	-Wl,--gc-sections $(GLOBAL_FLAGS) \

GLOBAL_ASFLAGS := \
	-mcpu=cortex-m3 -march=armv7-m -mthumb \
	-x assembler-with-cpp $(GLOBAL_FLAGS)

LDDIR := $(PLATFORM_PATH)/$(TARGET_PLATFORM)/ld
LDFLAGS := \
	-T$(LDDIR)/$(LDSCRIPT) -L$(LDDIR) \
	-mcpu=cortex-m3 -mthumb -Xlinker \
	--gc-sections --print-gc-sections --march=armv7-m -Wall

# Build Environment
SHELL	:= /bin/bash
PYTHON	:= /usr/bin/python
CROSS   := arm-none-eabi-

