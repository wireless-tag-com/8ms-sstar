#ifndef __LV_8MS_H
#define __LV_8MS_H

#include <libubox/blobmsg_json.h>
#include <libubus.h>
#include "cJSON.h"

#define LV_8MS_EVENT "/local/8ms/*"

typedef void (*lv_8ms_api_cb)(char *msg);
typedef void (*lv_8ms_event_cb)(const char *event, char *msg);
typedef void (*lv_8ms_loop_cb)(void);

void lv_8ms_init(void);
void lv_8ms_deinit(void);
void lv_8ms_start(void);
void lv_8ms_stop(void);

int lv_8ms_send_event(const char *event, const char *msg);
int lv_8ms_call_api(const char *api_path, const char *api_name, const char *param);
int lv_8ms_call_wapi(const char *api_path, const char *api_name, cJSON *obj);
void lv_8ms_set_api_cb(lv_8ms_api_cb cb);
void lv_8ms_set_event_cb(lv_8ms_event_cb cb);
void lv_8ms_set_loop_cb(lv_8ms_loop_cb cb);
int lv_8ms_add_object(struct ubus_object *obj);

#endif
