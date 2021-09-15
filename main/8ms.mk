MSRCS += $(shell find -L $(TOPDIR)/main -name \*.c -exec basename {} \;)

DEPPATH += --dep-path $(TOPDIR)/main
VPATH += :$(TOPDIR)/main
VPATH += :$(TOPDIR)/main/custom
CFLAGS += "-I$(TOPDIR)/main"
CFLAGS += "-I$(TOPDIR)/main/custom"

