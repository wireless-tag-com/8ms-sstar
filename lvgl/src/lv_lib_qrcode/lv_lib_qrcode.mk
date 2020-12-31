CSRCS += lv_qrcode.c
CSRCS += qrcodegen.c

DEPPATH += --dep-path $(LVGL_DIR)/$(LVGL_DIR_NAME)/src/lv_lib_qrcode
VPATH += :$(LVGL_DIR)/$(LVGL_DIR_NAME)/src/lv_lib_qrcode

CFLAGS += "-I$(LVGL_DIR)/$(LVGL_DIR_NAME)/src/lv_lib_qrcode"