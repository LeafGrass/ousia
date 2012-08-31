# Standard things
sp              := $(sp).x
dirstack_$(sp)  := $(d)
d               := $(dir)
BUILDDIRS       += $(BUILD_PATH)/$(d)/console

# Local flags
# FIXME should not add $(PLATFORM_INCLUDES), instead, we need a cfg file parser
#       called in Makefile then define those configs while compiling
CFLAGS_$(d) = -I$(d) $(PLATFORM_INCLUDES) $(CORE_INCLUDES) -Wall -Werror

# Local rules and targets
cSRCS_$(d) := console/console.c

cFILES_$(d) := $(cSRCS_$(d):%=$(d)/%)

OBJS_$(d) := $(cFILES_$(d):%.c=$(BUILD_PATH)/%.o)
DEPS_$(d) := $(OBJS_$(d):%.o=%.d)

$(OBJS_$(d)): TGT_CFLAGS := $(CFLAGS_$(d))

TGT_BIN += $(OBJS_$(d))

# Standard things
-include $(DEPS_$(d))
d := $(dirstack_$(sp))
sp := $(basename $(sp))
