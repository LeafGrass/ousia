# Standard things
sp              := $(sp).x
dirstack_$(sp)  := $(d)
d               := $(dir)
BUILDDIRS       += $(BUILD_PATH)/$(d)
BUILDDIRS       += $(BUILD_PATH)/$(d)/libmaple
BUILDDIRS       += $(BUILD_PATH)/$(d)/libmaple/usb
BUILDDIRS       += $(BUILD_PATH)/$(d)/libmaple/usb/usb_lib

LIBMAPLE_PATH	:= $(BUILD_PATH)/$(d)/libmaple

LIBMAPLE_INCLUDES := -I$(LIBMAPLE_PATH) -I$(LIBMAPLE_PATH)/usb -I$(LIBMAPLE_PATH)/usb/usb_lib

# Local flags
CFLAGS_$(d) = -I$(d) $(LIBMAPLE_INCLUDES) -Wall -Werror

# Local rules and targets
cSRCS_$(d) := libmaple/adc.c                    \
              libmaple/bkp.c                    \
              libmaple/dac.c                    \
              libmaple/dma.c                    \
              libmaple/exti.c                   \
              libmaple/flash.c                  \
              libmaple/fsmc.c                   \
              libmaple/gpio.c                   \
              libmaple/iwdg.c                   \
              libmaple/nvic.c                   \
              libmaple/pwr.c	                \
              libmaple/i2c.c                    \
              libmaple/rcc.c                    \
              libmaple/spi.c                    \
              libmaple/syscalls.c               \
              libmaple/systick.c                \
              libmaple/timer.c                  \
              libmaple/usart.c                  \
              libmaple/util.c
#             libmaple/usb/descriptors.c        \
#             libmaple/usb/usb.c                \
#             libmaple/usb/usb_callbacks.c      \
#             libmaple/usb/usb_hardware.c       \
#             libmaple/usb/usb_lib/usb_core.c   \
#             libmaple/usb/usb_lib/usb_init.c   \
#             libmaple/usb/usb_lib/usb_int.c    \
#             libmaple/usb/usb_lib/usb_mem.c    \
#             libmaple/usb/usb_lib/usb_regs.c

sSRCS_$(d) := exc.S

cFILES_$(d) := $(cSRCS_$(d):%=$(d)/%)
sFILES_$(d) := $(sSRCS_$(d):%=$(d)/%)

OBJS_$(d) := $(cFILES_$(d):%.c=$(BUILD_PATH)/%.o) $(sFILES_$(d):%.S=$(BUILD_PATH)/%.o)
DEPS_$(d) := $(OBJS_$(d):%.o=%.d)

$(OBJS_$(d)): TGT_CFLAGS := $(CFLAGS_$(d))
$(OBJS_$(d)): TGT_ASFLAGS :=

TGT_BIN += $(OBJS_$(d))

# Standard things
-include        $(DEPS_$(d))
d               := $(dirstack_$(sp))
sp              := $(basename $(sp))
