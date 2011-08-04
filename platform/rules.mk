PLATFORM_INCLUDES := -I$(PLATFORM_PATH)

# TODO Add specific platform related module
ifeq ($(MCU), STM32F103RB)
include $(PLATFORM_PATH)/stm32/rules.mk
endif

