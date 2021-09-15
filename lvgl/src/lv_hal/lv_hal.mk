
DEPPATH += --dep-path $(LVGL_DIR)/$(LVGL_DIR_NAME)/src/lv_hal
VPATH += :$(LVGL_DIR)/$(LVGL_DIR_NAME)/src/lv_hal

CFLAGS += "-I$(LVGL_DIR)/$(LVGL_DIR_NAME)/src/lv_hal"
