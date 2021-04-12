CSRCS += lv_freetype.c

DEPPATH += --dep-path $(LVGL_DIR)/$(LVGL_DIR_NAME)/src/lv_lib_freetype
DEPPATH += --dep-path $(LVGL_DIR)/$(LVGL_DIR_NAME)/src/lv_lib_freetype/freetype
DEPPATH += --dep-path $(LVGL_DIR)/$(LVGL_DIR_NAME)/src/lv_lib_freetype/freetype/config
VPATH += :$(LVGL_DIR)/$(LVGL_DIR_NAME)/src/lv_lib_freetype

CFLAGS += "-I$(LVGL_DIR)/$(LVGL_DIR_NAME)/src/lv_lib_freetype"
CFLAGS += "-I$(LVGL_DIR)/$(LVGL_DIR_NAME)/src/lv_lib_freetype/freetype"
CFLAGS += "-I$(LVGL_DIR)/$(LVGL_DIR_NAME)/src/lv_lib_freetype/freetype/config"