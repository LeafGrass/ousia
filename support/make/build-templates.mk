define MODULE_template
dir := $(1)
#LIBMAPLE_INCLUDES += -I$$(dir)
INCLUDES += -I$$(dir)
include $$(dir)/rules.mk
endef

