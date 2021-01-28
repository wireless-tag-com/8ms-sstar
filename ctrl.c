#include <unistd.h>
#include <stdio.h>

#include <libubox/blobmsg_json.h>
#include <libubus.h>

#include "lv_8ms.h"
#include "wtctrl.h"

static struct blob_buf g_ctrl_bbuf;

static int lv_ubus_ctrl(struct ubus_context *ctx, struct ubus_object *obj,
    struct ubus_request_data *req, const char *method, struct blob_attr *msg)
{
    char *str;
    char *res;

    blob_buf_init(&g_ctrl_bbuf, 0);

    str = blobmsg_format_json(msg, true);

    if (str) {
        res = wtctrl_json_parse(str);
        if (res) {
            blobmsg_add_json_from_string(&g_ctrl_bbuf, res);
            free(res);
        }
    }

    ubus_send_reply(ctx, req, g_ctrl_bbuf.head);

    return 0;
}

static const struct ubus_method lv_ubus_methods[] = {
    { .name = "ctrl", .handler = lv_ubus_ctrl },
};

static struct ubus_object_type lv_ubus_object_type =
    UBUS_OBJECT_TYPE("8ms", lv_ubus_methods);

static struct ubus_object lv_ubus_object = {
    .name = "8ms",
    .type = &lv_ubus_object_type,
    .methods = lv_ubus_methods,
    .n_methods = ARRAY_SIZE(lv_ubus_methods),
};

int lv_8ms_ctrl_init(void)
{
    return lv_8ms_add_object(&lv_ubus_object);
}
