# Standard things
sp              := $(sp).x
dirstack_$(sp)  := $(d)
d               := $(dir)
BUILDDIRS       += $(BUILD_PATH)/$(d)/stm32/libmaple
BUILDDIRS       += $(BUILD_PATH)/$(d)/stm32/libmaple/$(MCU_SERIES)
BUILDDIRS       += $(BUILD_PATH)/$(d)/stm32/libmaple/$(MCU_SERIES)/$(MCU_F1_LINE)
BUILDDIRS       += $(BUILD_PATH)/$(d)/stm32/libmaple/usb/
BUILDDIRS       += $(BUILD_PATH)/$(d)/stm32/libmaple/usb/usb_lib
BUILDDIRS       += $(BUILD_PATH)/$(d)/stm32/libmaple/usb/$(MCU_SERIES)
BUILDDIRS       += $(BUILD_PATH)/$(d)/stm32/stm32utils
BUILDDIRS       += $(BUILD_PATH)/$(d)/stm32/port

LIBMAPLE_INCLUDES := \
	-I$(PLATFORM_PATH)/stm32/libmaple \
	-I$(PLATFORM_PATH)/stm32/libmaple/include \
	-I$(PLATFORM_PATH)/stm32/libmaple/$(MCU_SERIES)/include \
	-I$(PLATFORM_PATH)/stm32/libmaple/usb \
	-I$(PLATFORM_PATH)/stm32/libmaple/usb/usb_lib

# Local flags
CFLAGS_$(d) = -I$(d) -I$(d)/$(TARGET_PLATFORM) -I$(INCLUDE_PATH) $(LIBMAPLE_INCLUDES) -Wall -Werror

# Local rules and targets
# libmaple
cSRCS_$(d) := \
	stm32/libmaple/adc.c \
	stm32/libmaple/dac.c \
	stm32/libmaple/dma.c \
	stm32/libmaple/exti.c \
	stm32/libmaple/flash.c \
	stm32/libmaple/gpio.c \
	stm32/libmaple/iwdg.c \
	stm32/libmaple/nvic.c \
	stm32/libmaple/pwr.c \
	stm32/libmaple/rcc.c \
	stm32/libmaple/spi.c \
	stm32/libmaple/start_c.c \
	stm32/libmaple/systick.c \
	stm32/libmaple/timer.c \
	stm32/libmaple/usart.c \
	stm32/libmaple/usart_private.c \
	stm32/libmaple/util.c
ifeq ($(MCU_SERIES), stm32f1)
cSRCS_$(d) += \
	stm32/libmaple/$(MCU_SERIES)/adc.c \
	stm32/libmaple/$(MCU_SERIES)/bkp.c \
	stm32/libmaple/$(MCU_SERIES)/dma.c \
	stm32/libmaple/$(MCU_SERIES)/exti.c \
	stm32/libmaple/$(MCU_SERIES)/fsmc.c \
	stm32/libmaple/$(MCU_SERIES)/gpio.c \
	stm32/libmaple/i2c.c \
	stm32/libmaple/$(MCU_SERIES)/i2c.c \
	stm32/libmaple/$(MCU_SERIES)/rcc.c \
	stm32/libmaple/$(MCU_SERIES)/spi.c \
	stm32/libmaple/$(MCU_SERIES)/timer.c \
	stm32/libmaple/$(MCU_SERIES)/usart.c \
	stm32/libmaple/util_hooks.c \
	stm32/libmaple/usb/$(MCU_SERIES)/usb.c \
	stm32/libmaple/usb/$(MCU_SERIES)/usb_reg_map.c \
	stm32/libmaple/usb/$(MCU_SERIES)/usb_cdcacm.c \
	stm32/libmaple/usb/usb_lib/usb_core.c \
	stm32/libmaple/usb/usb_lib/usb_init.c \
	stm32/libmaple/usb/usb_lib/usb_mem.c \
	stm32/libmaple/usb/usb_lib/usb_regs.c
endif
ifeq ($(MCU_SERIES), stm32f2)
cSRCS_$(d) += \
	stm32/libmaple/$(MCU_SERIES)/adc.c \
	stm32/libmaple/$(MCU_SERIES)/dma.c \
	stm32/libmaple/$(MCU_SERIES)/exti.c \
	stm32/libmaple/$(MCU_SERIES)/fsmc.c \
	stm32/libmaple/$(MCU_SERIES)/gpio.c \
	stm32/libmaple/$(MCU_SERIES)/rcc.c \
	stm32/libmaple/$(MCU_SERIES)/spi.c \
	stm32/libmaple/$(MCU_SERIES)/syscfg.c \
	stm32/libmaple/$(MCU_SERIES)/timer.c \
	stm32/libmaple/$(MCU_SERIES)/usart.c
endif
sSRCS_$(d) += \
	stm32/libmaple/exc.S \
	stm32/libmaple/start.S \
	stm32/libmaple/$(MCU_SERIES)/$(MCU_F1_LINE)/isrs.S \
	stm32/libmaple/$(MCU_SERIES)/$(MCU_F1_LINE)/vector_table.S

ifeq ($(MCU_SERIES), stm32f1)
# stm32utils
cSRCS_$(d) += stm32/stm32utils/stm32utils.c
endif

# porting
cSRCS_$(d) += \
	stm32/port/ousia_port.c

cFILES_$(d) := $(cSRCS_$(d):%=$(d)/%)
sFILES_$(d) := $(sSRCS_$(d):%=$(d)/%)

OBJS_$(d) := $(sFILES_$(d):%.S=$(BUILD_PATH)/%.o) $(cFILES_$(d):%.c=$(BUILD_PATH)/%.o)
DEPS_$(d) := $(OBJS_$(d):%.o=%.d)

$(OBJS_$(d)): TGT_CFLAGS := $(CFLAGS_$(d))
$(OBJS_$(d)): TGT_ASFLAGS :=

TGT_BIN += $(OBJS_$(d))

# Standard things
-include $(DEPS_$(d))
d := $(dirstack_$(sp))
sp := $(basename $(sp))
