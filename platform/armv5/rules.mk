# Standard things
sp              := $(sp).x
dirstack_$(sp)  := $(d)
d               := $(dir)
BUILDDIRS       += $(BUILD_PATH)/$(d)/$(TARGET_PLATFORM)/common
BUILDDIRS       += $(BUILD_PATH)/$(d)/$(TARGET_PLATFORM)/utils
BUILDDIRS       += $(BUILD_PATH)/$(d)/$(TARGET_PLATFORM)/port

COMMON_INCLUDE := \
	-I$(PLATFORM_PATH)/$(TARGET_PLATFORM)/common

# Local flags
CFLAGS_$(d) = -I$(d) -I$(d)/$(TARGET_PLATFORM) -I$(INCLUDE_PATH) $(COMMON_INCLUDE) -Wall -Werror

# Local rules and targets
# libmaple
cSRCS_$(d) := \
	$(TARGET_PLATFORM)/common/exc.c \
	$(TARGET_PLATFORM)/utils/utils.c
sSRCS_$(d) += \
	$(TARGET_PLATFORM)/common/entry.S

# porting
cSRCS_$(d) += \
	$(TARGET_PLATFORM)/port/ousia_port.c

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
