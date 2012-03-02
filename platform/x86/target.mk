$(BUILD_PATH)/$(OUSIA_TARGET): $(BUILDDIRS) $(TGT_BIN) $(USR_BUILDDIRS) $(USR_TGT_BIN)
	$(SILENT_LD) $(CC) $(LDFLAGS) -o $@ $(TGT_BIN) $(USR_TGT_BIN) -Wl,-Map,$(BUILD_PATH)/$(OUSIA_TARGET).map
	$(SILENT_DISAS) $(DISAS) -d $(BUILD_PATH)/$(OUSIA_TARGET) > $(BUILD_PATH)/$(OUSIA_TARGET).disas
	@echo " "
	@echo "Object file sizes:"
	@find $(BUILD_PATH) -iname *.o | xargs $(SIZE) -t > $(BUILD_PATH)/$(OUSIA_TARGET).sizes
	@cat $(BUILD_PATH)/$(OUSIA_TARGET).sizes
	@echo " "
	@echo "Final Size:"
	@$(SIZE) $(BUILD_PATH)/$(OUSIA_TARGET)
	@echo $(TARGET_PLATFORM) > $(BUILD_PATH)/build-type
