MSRCS += $(shell find -L $(TOPDIR)/control -name \*.c -exec basename {} \;)

DEPPATH += --dep-path $(TOPDIR)/control
VPATH += :$(TOPDIR)/control
CFLAGS += "-I$(TOPDIR)/control"

