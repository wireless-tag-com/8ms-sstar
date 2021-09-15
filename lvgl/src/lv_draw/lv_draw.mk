

DEPPATH += --dep-path $(LVGL_DIR)/$(LVGL_DIR_NAME)/src/lv_draw
VPATH += :$(LVGL_DIR)/$(LVGL_DIR_NAME)/src/lv_draw

CFLAGS += "-I$(LVGL_DIR)/$(LVGL_DIR_NAME)/src/lv_draw"
