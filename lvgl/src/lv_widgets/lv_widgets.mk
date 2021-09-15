DEPPATH += --dep-path $(LVGL_DIR)/$(LVGL_DIR_NAME)/src/lv_widgets
VPATH += :$(LVGL_DIR)/$(LVGL_DIR_NAME)/src/lv_widgets

CFLAGS += "-I$(LVGL_DIR)/$(LVGL_DIR_NAME)/src/lv_widgets"
