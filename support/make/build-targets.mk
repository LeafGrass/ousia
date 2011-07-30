# main project target

# Here adds includes' searching paths
#INCLUDES := 

#$(BUILD_PATH)/main.o: $(APP_PATH)/main.c
#	$(SILENT_CC) $(CC) $(CFLAGS) $(INCLUDES) -o $@ -c $<

$(BUILD_PATH)/main.o: $(SAMPLE_PATH)/main.c
	$(SILENT_CC) $(CC) $(CFLAGS) $(INCLUDES) -o $@ -c $<

#$(BUILD_PATH)/(OUSIA_TARGET).a: $(BUILDDIRS) $(TGT_BIN)
#	@echo $(BUILDDIRS)
#	- rm -f $@
#	$(AR) crv $(BUILD_PATH)/(OUSIA_TARGET).a $(TGT_BIN)

$(BUILD_PATH)/(OUSIA_TARGET).a: $(BUILDDIRS) $(TGT_BIN)
	- rm -f $@
	$(AR) crv $(BUILD_PATH)/(OUSIA_TARGET).a $(TGT_BIN)

library: $(BUILD_PATH)/$(OUSIA_TARGET).a

.PHONY: library


#$(BUILD_PATH)/$(BOARD).elf: $(BUILDDIRS) $(TGT_BIN) $(BUILD_PATH)/main.o
#	$(SILENT_LD) $(CC) $(LDFLAGS) -o $@ $(TGT_BIN) $(BUILD_PATH)/main.o -Wl,-Map,$(BUILD_PATH)/$(BOARD).map

#$(BUILD_PATH)/$(BOARD).bin: $(BUILD_PATH)/$(BOARD).elf
#	$(SILENT_OBJCOPY) $(OBJCOPY) -v -Obinary $(BUILD_PATH)/$(BOARD).elf $@ 1>/dev/null
#	$(SILENT_DISAS) $(DISAS) -d $(BUILD_PATH)/$(BOARD).elf > $(BUILD_PATH)/$(BOARD).disas
#
$(BUILD_PATH)/$(OUSIA_TARGET): $(BUILDDIRS) $(TGT_BIN) $(BUILD_PATH)/main.o
	$(SILENT_LD) $(CC) $(LDFLAGS) -o $@ $(TGT_BIN) $(BUILD_PATH)/main.o -Wl,-Map,$(BUILD_PATH)/$(OUSIA_TARGET).map
	@echo " "
	@echo "Object file sizes:"
	@find $(BUILD_PATH) -iname *.o | xargs $(SIZE) -t > $(BUILD_PATH)/$(OUSIA_TARGET).sizes
	@cat $(BUILD_PATH)/$(OUSIA_TARGET).sizes
	@echo " "
	@echo "Final Size:"
	@$(SIZE) $@
	@echo $(MEMORY_TARGET) > $(BUILD_PATH)/build-type

$(BUILDDIRS):
	@mkdir -p $@

MSG_INFO:
	@echo "======================================="
	@echo "[Build info]"
	@echo ""
	@echo "TARGET:\t\t"$(OUSIA_TARGET)
	@echo "NAME:\t\t"$(NAME)
	@echo "PLATFORM:\t"$(OUSIA_PLATFORM)
	@echo "MEMORY_TARGET:\t"$(MEMORY_TARGET)
	@echo ""
	@echo "  See 'make help' for more information"
	@echo "======================================="
	@echo

