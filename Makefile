VERSION = 0
SUBLEVEL = 0
DEVNUM = 0
NAME = Chinese Abelia

.DEFAULT_GOAL := sketch
OUSIA_TARGET :=	ousia
OUSIA_PLATFORM := arm-v7m
BOARD := leach
MEMORY_TARGET := jtag

# Some target specific things
ifeq ($(MEMORY_TARGET), ram)
	LDSCRIPT := $(BOARD)/ram.ld
	VECT_BASE_ADDR := VECT_TAB_RAM
endif
ifeq ($(MEMORY_TARGET), flash)
	LDSCRIPT := $(BOARD)/flash.ld
	VECT_BASE_ADDR := VECT_TAB_FLASH
endif
ifeq ($(MEMORY_TARGET), jtag)
	LDSCRIPT := $(BOARD)/jtag.ld
	VECT_BASE_ADDR := VECT_TAB_BASE
endif

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
SAMPLE_PATH := $(SRCROOT)/sample

# Compilation flags.
GLOBAL_FLAGS :=	-DOUSIA
#GLOBAL_CFLAGS := -Os -g3 -gdwarf-2 -mcpu=cortex-m3 -mthumb -march=armv7-m \
				 -nostdlib -ffunction-sections -fdata-sections -I$(SRCROOT)/include \
				 -Wl,--gc-sections $(GLOBAL_FLAGS)
GLOBAL_CFLAGS := -Os -ffunction-sections -fdata-sections -I$(SRCROOT)/include

GLOBAL_ASFLAGS  := -mcpu=cortex-m3 -march=armv7-m -mthumb \
                   -x assembler-with-cpp $(GLOBAL_FLAGS)

LDDIR    := $(PLATFORM_PATH)/ld
LDFLAGS  = -T$(LDDIR)/$(LDSCRIPT) -L$(LDDIR) \
#           -mcpu=cortex-m3 -mthumb -Xlinker \
           --gc-sections --print-gc-sections --march=armv7-m -Wall

# Set up build rules and some useful templates
include $(SUPPORT_PATH)/make/build-rules.mk
include $(SUPPORT_PATH)/make/build-templates.mk

# Set all modules here
MODULES := $(PLATFORM)/stm32
MODULES	:= $(CORE_PATH)

# call each module rules.mk
$(foreach m,$(MODULES),$(eval $(call MODULE_template,$(m))))

# Main target
include $(SUPPORT_PATH)/make/build-targets.mk

.PHONY: install sketch clean help debug cscope tags ctags ram flash jtag

# Target upload commands

# conditionally upload to whatever the last build was
install: INSTALL_TARGET = $(shell cat $(BUILD_PATH)/build-type 2>/dev/null)
install: $(BUILD_PATH)/$(OUSIA_TARGET)
	@echo Install target: $(INSTALL_TARGET)
	$(UPLOAD_$(INSTALL_TARGET))

# Force a rebuild if the maple target changed
PREV_BUILD_TYPE = $(shell cat $(BUILD_PATH)/build-type 2>/dev/null)
build-check:
ifneq ($(PREV_BUILD_TYPE), $(MEMORY_TARGET))
	$(shell rm -rf $(BUILD_PATH))
endif

sketch: MSG_INFO build-check $(BUILD_PATH)/$(OUSIA_TARGET)

clean:
	rm -rf build

help:
	@echo ""
	@echo "  libmaple Makefile help"
	@echo "  ----------------------"
	@echo "  Compile targets (default MEMORY_TARGET=flash):"
	@echo "      ram:    Compile sketch code to ram"
	@echo "      flash:  Compile sketch code to flash"
	@echo "      jtag:   Compile sketch code to jtag"
	@echo "      sketch: Compile sketch code to target MEMORY_TARGET"
	@echo "  "
	@echo "  Programming targets:"
	@echo "      install:  Upload code to target"
	@echo "  "
	@echo "  Other targets:"
	@echo "      debug:  Start an openocd gdb server, port 3333"
	@echo "      clean: Remove all build and object files"
	@echo "      help: Show this message"
	@echo "  "

debug:
	$(OPENOCD_WRAPPER) debug

cscope:
	rm -rf *.cscope
	find . -name '*.[hcS]' -o -name '*.cpp' | xargs cscope -b

tags:
	etags `find . -name "*.c" -o -name "*.cpp" -o -name "*.h"`
	@echo "Made TAGS file for EMACS code browsing"

ctags:
	ctags -R *
	@echo "Made tags file for VIM code browsing"

ram:
	@$(MAKE) MEMORY_TARGET=ram --no-print-directory sketch

flash:
	@$(MAKE) MEMORY_TARGET=flash --no-print-directory sketch

jtag:
	@$(MAKE) MEMORY_TARGET=jtag --no-print-directory sketch

doxygen:
	doxygen $(SUPPORT_PATH)/doxygen/Doxyfile
