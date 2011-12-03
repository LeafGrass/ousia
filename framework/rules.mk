FRAMEWORK_INCLUDES := -I$(FRAMEWORK_PATH)

include $(FRAMEWORK_PATH)/mem/rules.mk
include $(FRAMEWORK_PATH)/console/rules.mk
include $(FRAMEWORK_PATH)/vfs/rules.mk

