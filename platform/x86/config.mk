PRODUCT_ID := PC001

GLOBAL_FLAGS := \
	$(VERFLAGS) \
	-DOUSIA \
	-D__PLATFORM__=__X86__

GLOBAL_CFLAGS := \
	-g3 -nostdlib -ffunction-sections -fdata-sections \
	-Wl,--gc-sections $(GLOBAL_FLAGS)

GLOBAL_ASFLAGS := $(GLOBAL_FLAGS)

# Build Environment
SHELL	:= /bin/bash
PYTHON	:= /usr/bin/python
CROSS   :=
