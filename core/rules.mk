# Standard things
sp := $(sp).x
dirstack_$(sp) := $(d)
d := $(dir)
BUILDDIRS += $(BUILD_PATH)/$(d)
# TODO here goes on if more sub dirs
#BUILDDIRS += ... 

CORE_INCLUDES := -I$(INCLUDE_PATH)

# Local flags /* here need more INCLUDES if more sub dirs */
CFLAGS_$(d) := -I$(d) $(CORE_INCLUDES) -Wall -Werror

# Local rules and targets (add source files here)
cSRCS_$(d) := scheduler.c \
              sysutils.c

cFILES_$(d) := $(cSRCS_$(d):%=$(d)/%)

OBJS_$(d) := $(cFILES_$(d):%.c=$(BUILD_PATH)/%.o)
DEPS_$(d) := $(OBJS_$(d):%.o=%.d)

$(OBJS_$(d)): TGT_CFLAGS := $(CFLAGS_$(d))

TGT_BIN += $(OBJS_$(d))

# Standard things
-include $(DEPS_$(d))
d := $(dirstack_$(sp))
sp := $(basename $(sp))
