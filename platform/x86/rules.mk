# Standard things
sp              := $(sp).x
dirstack_$(sp)  := $(d)
d               := $(dir)
#BUILDDIRS       += $(BUILD_PATH)/$(d)
BUILDDIRS       += $(BUILD_PATH)/$(d)/x86
BUILDDIRS       += $(BUILD_PATH)/$(d)/x86/utils
BUILDDIRS       += $(BUILD_PATH)/$(d)/x86/port

# Local flags
CFLAGS_$(d) = -I$(d) -I$(d)/$(TARGET_PLATFORM) -I$(INCLUDE_PATH) -Wall -Werror

# Local rules and targets
# x86utils
cSRCS_$(d) := x86/utils/utils.c
# porting
cSRCS_$(d) += x86/port/ousia_port.c

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
