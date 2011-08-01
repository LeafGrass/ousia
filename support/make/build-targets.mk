# main project target

# Here adds includes' searching paths of sample code
INCLUDES := $(CORE_INCLUDES) \
            $(LIBMAPLE_INCLUDES)

$(BUILD_PATH)/main.o: $(SAMPLE_PATH)/main.c
	$(SILENT_CC) $(CC) $(CFLAGS) $(INCLUDES) -o $@ -c $<

# TODO Now this is not implemented actually.
$(BUILD_PATH)/(OUSIA_TARGET).a: $(BUILDDIRS) $(TGT_BIN)
	- rm -f $@
	$(AR) crv $(BUILD_PATH)/(OUSIA_TARGET).a $(TGT_BIN)

library: $(BUILD_PATH)/$(OUSIA_TARGET).a

.PHONY: library

$(BUILD_PATH)/$(OUSIA_TARGET).elf: $(BUILDDIRS) $(TGT_BIN) $(BUILD_PATH)/main.o
	$(SILENT_LD) $(CC) $(LDFLAGS) -o $@ $(TGT_BIN) $(BUILD_PATH)/main.o -Wl,-Map,$(BUILD_PATH)/$(OUSIA_TARGET).map

$(BUILD_PATH)/$(OUSIA_TARGET): $(BUILD_PATH)/$(OUSIA_TARGET).bin

# TODO Here needs improve, for not only depend on this specific hardware.
$(BUILD_PATH)/$(OUSIA_TARGET).bin: $(BUILD_PATH)/$(OUSIA_TARGET).elf
	$(SILENT_OBJCOPY) $(OBJCOPY) -v -Obinary $(BUILD_PATH)/$(OUSIA_TARGET).elf $@ 1>/dev/null
	$(SILENT_DISAS) $(DISAS) -d $(BUILD_PATH)/$(OUSIA_TARGET).elf > $(BUILD_PATH)/$(OUSIA_TARGET).disas
#$(BUILD_PATH)/$(OUSIA_TARGET): $(BUILDDIRS) $(TGT_BIN) $(BUILD_PATH)/main.o
#	$(SILENT_LD) $(CC) $(LDFLAGS) -o $@ $(TGT_BIN) $(BUILD_PATH)/main.o -Wl,-Map,$(BUILD_PATH)/$(OUSIA_TARGET).map
	@echo " "
	@echo "Object file sizes:"
	@find $(BUILD_PATH) -iname *.o | xargs $(SIZE) -t > $(BUILD_PATH)/$(OUSIA_TARGET).sizes
	@cat $(BUILD_PATH)/$(OUSIA_TARGET).sizes
	@echo " "
	@echo "Final Size:"
#	@$(SIZE) $@
	@$(SIZE) $<
	@echo $(MEMORY_TARGET) > $(BUILD_PATH)/build-type

$(BUILDDIRS):
	@mkdir -p $@

MSG_INFO:
	@echo "======================================="
	@echo "[Build info]"
	@echo ""
	@echo "TARGET: "$(OUSIA_TARGET)
	@echo "NAME: "$(NAME)
	@echo "PLATFORM: "$(OUSIA_PLATFORM)
	@echo "MEMORY_TARGET: "$(MEMORY_TARGET)
	@echo ""
	@echo "  See 'make help' for more information"
	@echo "======================================="
	@echo

