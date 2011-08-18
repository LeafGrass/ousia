PROJECT_INCLUDES := -I$(PROJECT_PATH)

# TODO Add specific project related module.
#      Should only include one module each time.

include $(PROJECT_PATH)/$(PROJECT_NAME)/rules.mk

