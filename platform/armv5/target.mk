LIBGCC := $(shell $(CC) --print-libgcc-file-name)

$(BUILD_PATH)/$(OUSIA_TARGET): $(BUILD_PATH)/$(OUSIA_TARGET).bin

$(BUILD_PATH)/$(OUSIA_TARGET).bin: $(BUILD_PATH)/$(OUSIA_TARGET).elf
	$(SILENT_OBJCOPY) $(OBJCOPY) -v -Obinary $(BUILD_PATH)/$(OUSIA_TARGET).elf $@ 1>/dev/null
	$(SILENT_DISAS) $(DISAS) -d -x $(BUILD_PATH)/$(OUSIA_TARGET).elf > $(BUILD_PATH)/$(OUSIA_TARGET).lss
	$(SILENT_NM) $(NM) -n $(BUILD_PATH)/$(OUSIA_TARGET).elf > $(BUILD_PATH)/$(OUSIA_TARGET).sym
	@find $(BUILD_PATH) -iname *.o | xargs $(SIZE) -t > $(BUILD_PATH)/$(OUSIA_TARGET).sizes
	@echo " "
	@echo "Final Size:"
	@$(SIZE) $<
	@echo $(TARGET_PLATFORM) > $(BUILD_PATH)/build-type

$(BUILD_PATH)/$(OUSIA_TARGET).elf: $(BUILDDIRS) $(TGT_BIN) $(USR_BUILDDIRS) $(USR_TGT_BIN)
	$(SILENT_LD) $(LD) $(LDFLAGS) -o $@ $(TGT_BIN) $(USR_TGT_BIN) $(LIBGCC) -Map=$(BUILD_PATH)/$(OUSIA_TARGET).map
