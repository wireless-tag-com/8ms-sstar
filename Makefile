#
# Makefile
#
BOARD ?=WT1280

TOPDIR := ${shell pwd}
LVGL_DIR_NAME := lvgl
LVGL_DIR := ${shell pwd}

include board/$(BOARD)/config.mk

BIN = demo

CFLAGS += -I$(LVGL_DIR)/ -I$(LVGL_DIR)/gfx 
CFLAGS += -DLV_CONF_INCLUDE_SIMPLE -DLV_EX_CONF_INCLUDE_SIMPLE
CFLAGS += -I$(TOPDIR)/board/$(BOARD)/lvgl -I$(LVGL_DIR)/freetype/include -I$(LVGL_DIR)/ssd20x/include
#CFLAGS += -L$(LVGL_DIR)/gfx -L$(LVGL_DIR)/freetype/ -L$(LVGL_DIR)/ssd20x/lib
MAINSRC += $(shell find . -maxdepth 1 -name \*.c)
LDFLAGS += -L$(LVGL_DIR)/extra/lib -L$(LVGL_DIR)/extra/include -L$(LVGL_DIR)/gfx -L$(LVGL_DIR)/freetype/ -L$(LVGL_DIR)/ssd20x/lib -lmi_common -l8ms -lmi_disp -lmi_gfx -lmi_sys -lmi_venc -lmi_ai -lmi_ao -lmi_divp -lmi_ipu -lmi_panel -lmi_venc -lpthread -ldl -lm -lfreetype -lz
include $(LVGL_DIR)/$(LVGL_DIR_NAME)/lvgl.mk
include $(LVGL_DIR)/$(LVGL_DIR_NAME)/src/lv_lib_png/lv_lib_png.mk
include $(LVGL_DIR)/common/common.mk
include $(LVGL_DIR)/main/8ms.mk
include $(LVGL_DIR)/tty/tty.mk
#include $(LVGL_DIR)/lv_demo_widgets/lv_demo_widgets.mk
#CFLAGS += -DLV_DEMO_TEST

OBJEXT ?= .o

AOBJS = $(ASRCS:.S=$(OBJEXT))
COBJS = $(CSRCS:.c=$(OBJEXT))
MCOBJS = $(MSRCS:.c=$(OBJEXT))

MAINOBJ = $(MAINSRC:.c=$(OBJEXT))

SRCS = $(ASRCS) $(CSRCS) $(MAINSRC) $(MSRCS)
OBJS = $(AOBJS) $(COBJS) $(MCOBJS)

all: clean prepare default

prepare:
	@mkdir -p build bin

%.o: %.c
	@$(CC)  $(CFLAGS) -c $< -o build/$@
	@echo "CC $<"

default: $(AOBJS) $(COBJS) $(MAINOBJ) $(MCOBJS)
	$(CC) -o bin/$(BIN) build/*.o $(LDFLAGS) 
	cp bin/$(BIN) bin/$(BIN)_debug
	$(STRIP) --strip-all bin/$(BIN)

clean:
	@rm -rf build bin

