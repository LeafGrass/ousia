# Choose specific platform related module.
PLATFORM_INCLUDES := -I$(PLATFORM_PATH)/$(TARGET_PLATFORM)
include $(PLATFORM_PATH)/$(TARGET_PLATFORM)/rules.mk
