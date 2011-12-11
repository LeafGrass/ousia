# Standard things
sp              := $(sp).x
dirstack_$(sp)  := $(d)
d               := $(dir)
BUILDDIRS       += $(BUILD_PATH)/$(d)/char/i2c

# Local flags
CFLAGS_$(d) = -I$(d) $(DRIVER_INCLUDES) $(PLATFORM_INCLUDES) $(CORE_INCLUDES) $(LIBMAPLE_INCLUDES) -Wall -Werror

# Local rules and targets
# libmaple
cSRCS_$(d) := \
	char/i2c/i2c_soft.c

cFILES_$(d) := $(cSRCS_$(d):%=$(d)/%)
sFILES_$(d) := $(sSRCS_$(d):%=$(d)/%)

OBJS_$(d) := $(cFILES_$(d):%.c=$(BUILD_PATH)/%.o) $(sFILES_$(d):%.S=$(BUILD_PATH)/%.o)
DEPS_$(d) := $(OBJS_$(d):%.o=%.d)

$(OBJS_$(d)): TGT_CFLAGS := $(CFLAGS_$(d))
$(OBJS_$(d)): TGT_ASFLAGS :=

TGT_BIN += $(OBJS_$(d))

# Standard things
-include $(DEPS_$(d))
d := $(dirstack_$(sp))
sp := $(basename $(sp))
