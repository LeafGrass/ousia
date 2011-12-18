# Ousia Makefile
# Contact - leafgrass.g@gmail.com

VERSION = 0
SUBLEVEL = 0
DEVNUM = 4
NAME = Delavay Fir

########################################
# User Customization Items
########################################
# TODO @OUSIA_TARGET the name of target binary file
#      @TARGET_PLATFORM specific directory name in folder $(PLATFORM_PATH)
#      @PROJECT_NAME specific directory name in folder $(PROJECT_PATH)
#      @DOWNLOAD_MODE jtag, serial or dfu
OUSIA_TARGET = ousia
TARGET_PLATFORM = stm32
PROJECT_NAME = sample_$(TARGET_PLATFORM)
DOWNLOAD_MODE = dfu

########################################
# System Rules and Configurations
########################################
# Useful paths
ifeq ($(OUSIA_HOME),)
	SRCROOT := .
else
	SRCROOT := $(OUSIA_HOME)
endif
BUILD_PATH = build
INCLUDE_PATH := $(SRCROOT)/include
PLATFORM_PATH := $(SRCROOT)/platform
CORE_PATH := $(SRCROOT)/core
DRIVER_PATH := $(SRCROOT)/driver
FRAMEWORK_PATH := $(SRCROOT)/framework
SUPPORT_PATH := $(SRCROOT)/support
SCRIPT_PATH := $(SRCROOT)/script
PROJECT_PATH := $(SRCROOT)/project

# FIXME This solution results to a problem.
#       In target platform directory, we must use too many .mk files.
#       i.e. Three files for now: rules.mk, config.mk and target.mk.
include $(PLATFORM_PATH)/$(TARGET_PLATFORM)/config.mk

.DEFAULT_GOAL := sketch

# Set up build rules and some useful templates
include $(SUPPORT_PATH)/make/build-rules.mk
include $(SUPPORT_PATH)/make/build-templates.mk

# Ousia source modules
MODULES	:= $(CORE_PATH)
MODULES += $(PLATFORM_PATH)
MODULES += $(DRIVER_PATH)
MODULES += $(FRAMEWORK_PATH)
# User application code
MODULES += $(PROJECT_PATH)/$(PROJECT_NAME)

# call each module rules.mk
$(foreach m,$(MODULES),$(eval $(call MODULE_template,$(m))))

# Main target
include $(SUPPORT_PATH)/make/build-targets.mk

.PHONY: MSG_INFO build-check install bootloader sketch lib clean distclean tarball help

# Download code to target device
install: $(BUILD_PATH)/$(OUSIA_TARGET).bin
ifeq ($(DOWNLOAD_MODE), jtag)
	$(SHELL) $(SCRIPT_PATH)/download.sh
endif
ifeq ($(DOWNLOAD_MODE), serial)
	$(PYTHON) $(SCRIPT_PATH)/stm32loader.py -p/dev/ttyUSB0 -a0x08000000 -evw $(BUILD_PATH)/$(OUSIA_TARGET).bin
endif
ifeq ($(DOWNLOAD_MODE), dfu)
	$(SCRIPT_PATH)/reset.py && sleep 1 && \
	dfu-util -a1 -d $(VENDOR_ID):$(PRODUCT_ID) -D $(BUILD_PATH)/$(OUSIA_TARGET).bin -R
endif

BOOTLOADER_BIN = $(PLATFORM_PATH)/$(TARGET_PLATFORM)/bootloader/build/maple_boot.bin

bootloader: $(BOOTLOADER_BIN)
	$(PYTHON) $(SCRIPT_PATH)/stm32loader.py -p/dev/ttyUSB0 -a0x08000000 -evw $^

# Force a rebuild if the maple target changed
PREV_BUILD_TYPE = $(shell cat $(BUILD_PATH)/build-type 2>/dev/null)
build-check:
ifneq ($(PREV_BUILD_TYPE), $(TARGET_PLATFORM))
	$(shell rm -rf $(BUILD_PATH))
	$(shell cp $(PLATFORM_PATH)/$(TARGET_PLATFORM)/port/ousia_*.* $(CORE_PATH)/port/)
else
	$(shell $(SCRIPT_PATH)/check_port.sh)
endif

sketch: MSG_INFO build-check $(BUILD_PATH)/$(OUSIA_TARGET)

lib: $(BUILD_PATH)/lib$(OUSIA_TARGET).a
	@find $(BUILD_PATH) -iname *.o | xargs $(SIZE) -t > $(BUILD_PATH)/$(OUSIA_TARGET).sizes

$(BUILD_PATH)/lib$(OUSIA_TARGET).a: MSG_INFO build-check $(BUILDDIRS) $(TGT_BIN)
	$(shell rm -f $@)
	$(SILENT_AR) $(AR) cr $(BUILD_PATH)/lib$(OUSIA_TARGET).a $(TGT_BIN)

clean:
	$(shell rm -rf $(BUILD_PATH))

distclean:
	$(shell rm -rf $(BUILD_PATH))
	$(shell rm -rf tarball)
	$(shell rm -f tags tags.ut tags.fn cscope.out)
	$(shell find . -name "*.swp" -exec rm -rf {} \;)
	@echo "clean up completely"

tarball:
	mkdir -p tarball
	$(SHELL) $(SCRIPT_PATH)/tarball.sh

help:
	@echo ""
	@echo "==========================================================="
	@echo "[Ousia Make Help]"
	@echo ""
	@echo "Build targets:"
	@echo "  sketch:     default build"
	@echo "  lib:        build ousia into a static library"
	@echo ""
	@echo "Downloading targets:"
	@echo "  install:    download binary image to target device"
	@echo "  bootloader: download bootloader via serial (stm32 peculiar)"
	@echo ""
	@echo "Debug targets:"
	@echo "  debug:      start an openocd server"
	@echo ""
	@echo "Other targets:"
	@echo "  clean:      remove all build files"
	@echo "  distclean:  remove all builds tarballs, and other misc"
	@echo "  tarball:    package current revision into a tarball"
	@echo "  help:       show this message"
	@echo "==========================================================="
	@echo ""

MSG_INFO:
	@echo ""
	@echo "========================================"
	@echo "[Build Information]"
	@echo ""
	@echo "TARGET: "$(OUSIA_TARGET)
	@echo "VERSION: "$(VERSION)"."$(SUBLEVEL)"."$(DEVNUM)
	@echo "NAME: "$(NAME)
	@echo "PLATFORM: "$(TARGET_PLATFORM)
	@echo "PROJECT: "$(PROJECT_NAME)
	@echo ""
	@echo "  See 'make help' for more information"
	@echo "========================================"
	@echo ""
