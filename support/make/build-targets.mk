# main project target

include $(PLATFORM_PATH)/$(TARGET_PLATFORM)/target.mk

$(BUILDDIRS):
	@mkdir -p $@

$(USR_BUILDDIRS):
	@mkdir -p $@
