CSRCS += lv_png.c
CSRCS += lodepng.c
CSRCS += png_decoder_test.c

DEPPATH += --dep-path $(LVGL_DIR)/$(LVGL_DIR_NAME)/src/lv_lib_png
VPATH += :$(LVGL_DIR)/$(LVGL_DIR_NAME)/src/lv_lib_png

CFLAGS += "-I$(LVGL_DIR)/$(LVGL_DIR_NAME)/src/lv_lib_png"