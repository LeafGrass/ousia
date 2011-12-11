DRIVER_INCLUDES := -I$(DRIVER_PATH)

include $(DRIVER_PATH)/char/rules.mk
include $(DRIVER_PATH)/block/rules.mk
