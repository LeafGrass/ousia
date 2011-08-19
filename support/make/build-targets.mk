# main project target

lib: $(BUILD_PATH)/lib$(OUSIA_TARGET).a
	@find $(BUILD_PATH) -iname *.o | xargs $(SIZE) -t > $(BUILD_PATH)/$(OUSIA_TARGET).sizes
#	@cat $(BUILD_PATH)/$(OUSIA_TARGET).sizes

$(BUILD_PATH)/lib$(OUSIA_TARGET).a: MSG_INFO $(BUILDDIRS) $(TGT_BIN)
	- rm -f $@
	$(SILENT_AR) $(AR) cr $(BUILD_PATH)/lib$(OUSIA_TARGET).a $(TGT_BIN)

.PHONY: lib

include $(PLATFORM_PATH)/$(TARGET_PLATFORM)/target.mk

$(BUILDDIRS):
	@mkdir -p $@

$(USR_BUILDDIRS):
	@mkdir -p $@
