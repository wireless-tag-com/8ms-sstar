MSRCS += $(shell find -L $(TOPDIR)/tty -name \*.c -exec basename {} \;)

DEPPATH += --dep-path $(TOPDIR)/tty
VPATH += :$(TOPDIR)/tty
CFLAGS += "-I$(TOPDIR)/tty"

