# Standard things
sp := $(sp).x
dirstack_$(sp) := $(d)
d := $(dir)
BUILDDIRS += $(BUILD_PATH)/$(d)
BUILDDIRS += $(BUILD_PATH)/$(d)/cds
BUILDDIRS += $(BUILD_PATH)/$(d)/port
BUILDDIRS += $(BUILD_PATH)/$(d)/ulib

CORE_INCLUDES := -I$(INCLUDE_PATH) -I$(CORE_PATH) -I$(PLATFORM_PATH)

# Local flags /* here need more INCLUDES if more sub dirs */
CFLAGS_$(d) := -I$(d) -I$(d)/port -I$(d)/ulib $(CORE_INCLUDES) -Wall

# Local rules and targets (add source files here)
cSRCS_$(d) := port/ousia_port.c \
              sys_print.c \
              sys_sched.c \
              sys_time.c \
              sys_utils.c \
              sys_ictl.c \
              sys_debug.c
sSRCS_$(d) :=

cFILES_$(d) := $(cSRCS_$(d):%=$(d)/%)
sFILES_$(d) := $(sSRCS_$(d):%=$(d)/%)

OBJS_$(d) := $(cFILES_$(d):%.c=$(BUILD_PATH)/%.o)
DEPS_$(d) := $(OBJS_$(d):%.o=%.d)

$(OBJS_$(d)): TGT_CFLAGS := $(CFLAGS_$(d))
$(OBJS_$(d)): TGT_ASFLAGS :=

TGT_BIN += $(OBJS_$(d))

# Standard things
-include $(DEPS_$(d))
d := $(dirstack_$(sp))
sp := $(basename $(sp))

