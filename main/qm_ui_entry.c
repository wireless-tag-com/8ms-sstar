/**
* author:xnby
* platform: esp
* version: 1.0.0
* UIResolution: 480*320
*/

#include "qm_ui_entry.h"
#include <stdio.h>
#include <stdlib.h>

#include "lv_8ms.h"

typedef struct
{
    char *name;
    lv_obj_t **id;
} lv_widget_t;

typedef struct
{
    char *name;
    lv_obj_t **id;
    lv_widget_t *wids;
    int len;
} lv_screen_t;

/*
* includes
*/
/*
* declear screens
*/
lv_obj_t *main_screen;
lv_obj_t *p1;
lv_obj_t *p2;
lv_obj_t *p3;
lv_obj_t *sw1;

lv_widget_t g_main_screen_ids[] =
{
    {"p2", &p2},
    {"s1", &sw1},
};

lv_widget_t g_p1_screen_ids[] =
{
    {"p3", &p3},
};

lv_screen_t g_screens[] =
{
    {"main_screen", &main_screen, g_main_screen_ids, (sizeof(g_main_screen_ids) / sizeof(g_main_screen_ids[0]))},
    {"p1", &p1, g_p1_screen_ids, (sizeof(g_p1_screen_ids) / sizeof(g_p1_screen_ids[0]))},
};

/*
 * callback functions begins
 */
/*
 * callback functions ends
 */
/*
* init codes
*/
static void init_function(void)
{
#ifdef BLOCKLY_init
    blockly_init();
#endif
}

void lv_qm_ui_loop(void)
{
    #ifdef BLOCKLY_loop
    blockly_loop();
    #endif
}

void lv_qm_api_cb(char *msg)
{
    if (!msg)
        return;

    printf("api cb: %s\n", msg);
}

void go2(lv_obj_t *obj, lv_event_t event)
{
    if (event == LV_EVENT_CLICKED)
    {
        cJSON *param = cJSON_CreateObject();

        lv_scr_load(p1);
        lv_8ms_call_api("system", "board", NULL);

        if (param) {
            cJSON_AddItemToObject(param, "brightness", cJSON_CreateString("50"));
            lv_8ms_call_wapi("lcd", "set_brightness", param);
        }
    }
}

void go1(lv_obj_t* obj, lv_event_t event)
{
    if (event == LV_EVENT_CLICKED)
    {
        lv_scr_load(main_screen);
    }
}

void lv_qm_ui_entry(void)
{
    LV_FONT_DECLARE(ali_font_16);
    lv_font_montserrat_16 = ali_font_16;

    lv_8ms_set_api_cb(lv_qm_api_cb);

    /*
    * create screens
    */
    main_screen = lv_obj_create(NULL, NULL);

    p1 = lv_obj_create(NULL,NULL);
    p2 = lv_btn_create(main_screen,NULL);
    sw1 = lv_switch_create(main_screen, NULL);
    p3 = lv_btn_create(p1,NULL);
    lv_obj_set_style_local_bg_color(main_screen, 0, LV_STATE_DEFAULT, LV_COLOR_GREEN);
    lv_obj_set_style_local_bg_color(p1, 0, LV_STATE_DEFAULT, LV_COLOR_BLUE);
    lv_obj_t* label = lv_label_create(p2, NULL);
    lv_label_set_text(label, "p1");
    label = lv_label_create(p3, NULL);
    lv_label_set_text(label, "p2");
    lv_obj_align(p2, NULL, LV_ALIGN_CENTER, -50, 0);
    lv_obj_align(p3, NULL, LV_ALIGN_CENTER, 50, 0);
    lv_obj_set_event_cb(p2, go2);
    lv_obj_set_event_cb(p3, go1);

    lv_obj_set_pos(sw1, LV_DPX(10), LV_DPX(10));
    lv_obj_set_style_local_value_str(sw1, LV_SWITCH_PART_BG, LV_STATE_DEFAULT, "Dark");

    lv_scr_load(main_screen);

    /*
    * ui-element begins
    */

    init_function();
}

lv_obj_t *lv_8ms_get_screen(char *name)
{
    int i = 0, j = 0;
    for (i = 0; i < (sizeof(g_screens) / sizeof(g_screens[0])); i++)
    {
        if (!strcmp(name, g_screens[i].name))
        {
            return *(g_screens[i].id);
        }
    }
    return NULL;
}

lv_obj_t *lv_8ms_get_widget(char *w_name)
{
    int i = 0, j = 0;

    for (i = 0; i < (sizeof(g_screens) / sizeof(g_screens[0])); i++)
    {
        for (j = 0; j <  g_screens[i].len; j++)
        {
            if (!strcmp(w_name, g_screens[i].wids[j].name))
            {
                return *(g_screens[i].wids[j].id);
            }
        }
    }

    return NULL;
}

const char *lv_8ms_get_obj_name(lv_obj_t *obj)
{
    if (!obj) {
        return NULL;
    }

    int i = 0, j = 0;
    for (i = 0; i < (sizeof(g_screens) / sizeof(g_screens[0])); i++)
    {
        if (*(g_screens[i].id) == obj) {
            return g_screens[i].name;
        }

        for (j = 0; j <  g_screens[i].len; j++)
        {
            if (*(g_screens[i].wids[j].id) == obj)
            {
                return g_screens[i].wids[j].name;
            }
        }
    }

    return NULL;
}
