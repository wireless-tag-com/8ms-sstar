MSRCS += $(shell find -L $(TOPDIR)/blockly -name \*.c -exec basename {} \;)

DEPPATH += --dep-path $(TOPDIR)/blockly
VPATH += :$(TOPDIR)/blockly
CFLAGS += "-I$(TOPDIR)/blockly"

