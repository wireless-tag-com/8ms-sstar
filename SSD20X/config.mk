CC := arm-linux-gnueabihf-gcc
STRIP := arm-linux-gnueabihf-strip
CFLAGS := -O3 -g -I$(LVGL_DIR)/

CSRCS += $(shell find -L $(TOPDIR)/SSD20X -name \*.c -exec basename {} \;)

DEPPATH += --dep-path $(TOPDIR)/SSD20X
VPATH += :$(TOPDIR)/SSD20X
CFLAGS += "-I$(TOPDIR)/SSD20X"
