# Useful tools
CC      := $(CROSS)gcc
CXX     := $(CROSS)g++
LD      := $(CROSS)ld
AR      := $(CROSS)ar
AS      := $(CROSS)gcc
OBJCOPY := $(CROSS)objcopy
DISAS   := $(CROSS)objdump
OBJDUMP := $(CROSS)objdump
SIZE    := $(CROSS)size
NM      := $(CROSS)nm

# Suppress annoying output unless V is set
#define V
ifndef V
	SILENT_CC       = @echo '  [CC]       ' $(<);
	SILENT_AS       = @echo '  [AS]       ' $(<);
	SILENT_CXX      = @echo '  [CXX]      ' $(<);
	SILENT_LD       = @echo '  [LD]       ' $(@);
	SILENT_AR       = @echo '  [AR]       ' $(@);
	SILENT_OBJCOPY  = @echo '  [OBJCOPY]  ' $(@);
	SILENT_DISAS    = @echo '  [DISAS]    ' $(@);
	SILENT_OBJDUMP  = @echo '  [OBJDUMP]  ' $(@);
	SILENT_NM       = @echo '  [NM]       ' $(@);
endif
#endef

BUILDDIRS     :=
TGT_BIN       :=
USR_BUILDDIRS :=
USR_TGT_BIN   :=

CFLAGS   = $(GLOBAL_CFLAGS) $(TGT_CFLAGS)
CXXFLAGS = $(GLOBAL_CXXFLAGS) $(TGT_CXXFLAGS)
ASFLAGS  = $(GLOBAL_ASFLAGS) $(TGT_ASFLAGS)

# General directory independent build rules, generate dependency information
$(BUILD_PATH)/%.o: %.c
	$(SILENT_CC) $(CC) $(CFLAGS) -MMD -MP -MF $(@:%.o=%.d) -MT $@ -o $@ -c $<

$(BUILD_PATH)/%.o: %.cpp
	$(SILENT_CXX) $(CXX) $(CFLAGS) $(CXXFLAGS) -MMD -MP -MF $(@:%.o=%.d) -MT $@ -o $@ -c $<

$(BUILD_PATH)/%.o: %.S
	$(SILENT_AS) $(AS) $(ASFLAGS) -MMD -MP -MF $(@:%.o=%.d) -MT $@ -o $@ -c $<
