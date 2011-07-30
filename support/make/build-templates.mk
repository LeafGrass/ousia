define MODULE_template
dir := $(1)
INCLUDES += -I$$(dir)
include $$(dir)/rules.mk
endef

