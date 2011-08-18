# main project target

# Here adds includes' searching paths of sample code
INCLUDES := $(CORE_INCLUDES) \
            $(PLATFORM_INCLUDES) \
			$(FRAMEWORK_INCLUDES)

$(BUILD_PATH)/lib$(OUSIA_TARGET).a: MSG_INFO $(BUILDDIRS) $(TGT_BIN)
	- rm -f $@
	$(SILENT_AR) $(AR) crv $(BUILD_PATH)/lib$(OUSIA_TARGET).a $(TGT_BIN)

# FIXME Should not include user modules while make library
library: $(BUILD_PATH)/lib$(OUSIA_TARGET).a
	@find $(BUILD_PATH) -iname *.o | xargs $(SIZE) -t > $(BUILD_PATH)/$(OUSIA_TARGET).sizes
	@cat $(BUILD_PATH)/$(OUSIA_TARGET).sizes

.PHONY: library

$(BUILD_PATH)/$(OUSIA_TARGET).elf: $(BUILDDIRS) $(TGT_BIN)
	$(SILENT_LD) $(CC) $(LDFLAGS) -o $@ $(TGT_BIN) -Wl,-Map,$(BUILD_PATH)/$(OUSIA_TARGET).map

$(BUILD_PATH)/$(OUSIA_TARGET): $(BUILD_PATH)/$(OUSIA_TARGET).bin

# TODO Here needs improve, for not only depend on this specific hardware.
$(BUILD_PATH)/$(OUSIA_TARGET).bin: $(BUILD_PATH)/$(OUSIA_TARGET).elf
	$(SILENT_OBJCOPY) $(OBJCOPY) -v -Obinary $(BUILD_PATH)/$(OUSIA_TARGET).elf $@ 1>/dev/null
	$(SILENT_DISAS) $(DISAS) -d $(BUILD_PATH)/$(OUSIA_TARGET).elf > $(BUILD_PATH)/$(OUSIA_TARGET).disas
	@echo " "
	@echo "Object file sizes:"
	@find $(BUILD_PATH) -iname *.o | xargs $(SIZE) -t > $(BUILD_PATH)/$(OUSIA_TARGET).sizes
	@cat $(BUILD_PATH)/$(OUSIA_TARGET).sizes
	@echo " "
	@echo "Final Size:"
	@$(SIZE) $<
	@echo $(MEMORY_TARGET) > $(BUILD_PATH)/build-type

$(BUILDDIRS):
	@mkdir -p $@

MSG_INFO:
	@echo ""
	@echo "========================================"
	@echo "[Build Information]"
	@echo ""
	@echo "TARGET: "$(OUSIA_TARGET)
	@echo "VERSION: "$(VERSION)"."$(SUBLEVEL)"."$(DEVNUM)
	@echo "NAME: "$(NAME)
	@echo "PLATFORM: "$(TARGET_PLATFORM)
	@echo "MEMORY_TARGET: "$(MEMORY_TARGET)
	@echo "PROJECT: "$(PROJECT_NAME)
	@echo ""
	@echo "  See 'make help' for more information"
	@echo "========================================"
	@echo ""

