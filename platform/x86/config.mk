PRODUCT_ID := PC001

GLOBAL_FLAGS := \
	$(VERFLAGS) \
	-DOUSIA \
	-D__PLATFORM_X86__

GLOBAL_CFLAGS := \
	-g3 -nostdlib -ffunction-sections -fdata-sections \
	-Wl,--gc-sections -Wno-unused-function $(GLOBAL_FLAGS)

GLOBAL_ASFLAGS := $(GLOBAL_FLAGS)

# Build Environment
SHELL	:= /bin/bash
PYTHON	:= /usr/bin/python
CROSS   :=
