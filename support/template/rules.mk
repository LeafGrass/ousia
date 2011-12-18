# Standard things
sp := $(sp).x
dirstack_$(sp) := $(d)
d := $(dir)
BUILDDIRS += $(BUILD_PATH)/$(d)
#BUILDDIRS += $(BUILD_PATH)/$(d)/xxx

XXX_INCLUDES :=

# Local flags
CFLAGS_$(d) := -I$(d) $(XXX_INCLUDES) -Wall -Werror

# Local rules and targets /* add source files here */
cSRCS_$(d) :=
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
