DRIVER_INCLUDES := -I$(DRIVER_PATH)

BUILDDIRS       += $(BUILD_PATH)/$(d)

# char
sp              := $(sp).x
dirstack_$(sp)  := $(d)
d               := $(dir)
BUILDDIRS       += $(BUILD_PATH)/$(d)/char
include $(DRIVER_PATH)/char/rules.mk

# block
sp              := $(sp).x
dirstack_$(sp)  := $(d)
d               := $(dir)
BUILDDIRS       += $(BUILD_PATH)/$(d)/block
include $(DRIVER_PATH)/block/rules.mk
