# Standard things
sp              := $(sp).x
dirstack_$(sp)  := $(d)
d               := $(dir)
#BUILDDIRS       += $(BUILD_PATH)/$(d)
BUILDDIRS       += $(BUILD_PATH)/$(d)/ulib
BUILDDIRS       += $(BUILD_PATH)/$(d)/ulib/tprintf

# Local flags
CFLAGS_$(d) = -I$(d) -Wall -Werror

# Local rules and targets
cSRCS_$(d) := ulib/tprintf/tprintf.c

cFILES_$(d) := $(cSRCS_$(d):%=$(d)/%)

OBJS_$(d) := $(cFILES_$(d):%.c=$(BUILD_PATH)/%.o)
DEPS_$(d) := $(OBJS_$(d):%.o=%.d)

$(OBJS_$(d)): TGT_CFLAGS := $(CFLAGS_$(d))

TGT_BIN += $(OBJS_$(d))

# Standard things
-include $(DEPS_$(d))
d := $(dirstack_$(sp))
sp := $(basename $(sp))

