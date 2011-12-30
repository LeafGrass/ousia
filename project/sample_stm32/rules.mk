# Standard things
sp              := $(sp).x
dirstack_$(sp)  := $(d)
d               := $(dir)
USR_BUILDDIRS   += $(BUILD_PATH)/$(d)

# Local flags
CFLAGS_$(d) = -I$(d) $(PROJECT_INCLUDES) $(PLATFORM_INCLUDES) $(CORE_INCLUDES) $(DRIVER_INCLUDES) $(FRAMEWORK_INCLUDES) -Wall

# Local rules and targets
# libmaple
cSRCS_$(d) := main.c
sSRCS_$(d) :=

cFILES_$(d) := $(cSRCS_$(d):%=$(d)/%)
sFILES_$(d) := $(sSRCS_$(d):%=$(d)/%)

OBJS_$(d) := $(cFILES_$(d):%.c=$(BUILD_PATH)/%.o) $(sFILES_$(d):%.S=$(BUILD_PATH)/%.o)
DEPS_$(d) := $(OBJS_$(d):%.o=%.d)

$(OBJS_$(d)): TGT_CFLAGS := $(CFLAGS_$(d))
$(OBJS_$(d)): TGT_ASFLAGS :=

USR_TGT_BIN += $(OBJS_$(d))

# Standard things
-include $(DEPS_$(d))
d := $(dirstack_$(sp))
sp := $(basename $(sp))

