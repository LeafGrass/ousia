FRAMEWORK_INCLUDES := -I$(FRAMEWORK_PATH)

BUILDDIRS       += $(BUILD_PATH)/$(d)

# ulib
sp              := $(sp).x
dirstack_$(sp)  := $(d)
d               := $(dir)
BUILDDIRS       += $(BUILD_PATH)/$(d)/ulib
BUILDDIRS       += $(BUILD_PATH)/$(d)/ulib/tprintf
include $(FRAMEWORK_PATH)/ulib/rules.mk

# serial shell
sp              := $(sp).x
dirstack_$(sp)  := $(d)
d               := $(dir)
BUILDDIRS       += $(BUILD_PATH)/$(d)/ss
include $(FRAMEWORK_PATH)/ss/rules.mk

