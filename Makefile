#
# 8ms Makefile
#
TOPDIR := ${shell pwd}
LVGL_DIR_NAME := lvgl
LVGL_DIR := ${shell pwd}

include SSD20X/config.mk

CFLAGS += -I$(LVGL_DIR)/
CFLAGS += -DLV_CONF_INCLUDE_SIMPLE -DLV_EX_CONF_INCLUDE_SIMPLE
CFLAGS += -I$(TOPDIR)/include/ -I$(TOPDIR)/include/lvgl/
CFLAGS += -I$(TOPDIR)/extra/include/

MSRCS += $(shell find . -maxdepth 1 -name \*.c)

include $(LVGL_DIR)/main/8ms.mk

OBJEXT ?= .o
MOBJS = $(MSRCS:.c=$(OBJEXT))

all: clean prepare demo

prepare:
	@mkdir -p build bin

%.o: %.c
	@$(CC) $(CFLAGS) -c -o build/$@ $^
	@echo "CC $^"

demo: $(MOBJS)
	$(CC) -o bin/$@ build/*.o -L./lib -L$(TOPDIR)/extra/lib/ -llvgl -lubus -lubox -lblobmsg_json -ljson-c -lm
	cp bin/$@ bin/$@_debug
	$(STRIP) --strip-all bin/$@

clean:
	@rm -rf build bin

