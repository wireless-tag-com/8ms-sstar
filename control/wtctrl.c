#include <stdio.h>
#include <stdlib.h>
#include "cJSON.h"
#include "wtctrl.h"
#include "lvgl/lvgl.h"

#include "qm_ui_entry.h"

static char *lv_ctrl_handle(lv_obj_t *lobj, cJSON *attr)
{
	return lv_obj_8ms_call_ctrl_cb(lobj, attr);
}

static char *__wtctrl_json_make(int code)
{
	cJSON *result;
	char *res = NULL;
	result = cJSON_CreateObject();

	if (result) {
		cJSON_AddItemToObject(result, "code", cJSON_CreateNumber(code));
		res = cJSON_PrintUnformatted(result);
		cJSON_Delete(result);
	}

	return res;
}

char *wtctrl_json_parse(const char *json_str)
{
	cJSON *root = NULL;
	cJSON *wid = NULL;
	cJSON *cmd = NULL;
	lv_obj_t *screen;
	lv_obj_t *widget;
	char *res = NULL;

	root = cJSON_Parse(json_str);
	if (!root) {
		res = __wtctrl_json_make(WTCTRL_CODE_PARAM_ERROR);
		goto err_out;
	}

	cmd = cJSON_GetObjectItem(root, WTCTRL_CMD_NAME);
	if (!cmd) {
		res = __wtctrl_json_make(WTCTRL_CODE_PARAM_ERROR);
		goto err_out;
	}

	if ((cmd->type)&255 != cJSON_String) {
		res = __wtctrl_json_make(WTCTRL_CODE_PARAM_ERROR);
		goto err_out;
	}

	wid = cJSON_GetObjectItem(root, WTCTRL_WID_NAME);

	if (!wid) {
		res = __wtctrl_json_make(WTCTRL_CODE_PARAM_ERROR);
		goto err_out;
	}

	if ((wid->type)&255 != cJSON_String) {
		res = __wtctrl_json_make(WTCTRL_CODE_PARAM_ERROR);
		goto err_out;
	}

	screen = lv_8ms_get_screen(wid->valuestring);

	if (screen) {
		res = lv_ctrl_handle(screen, root);
		goto err_out;
	}

	widget = lv_8ms_get_widget(wid->valuestring);
	if (!widget) {
		res = __wtctrl_json_make(WTCTRL_CODE_WID_NOT_FOUND);
		goto err_out;
	}

	res = lv_ctrl_handle(widget, root);

err_out:
	if (root)
		cJSON_Delete(root);

	return res;
}
