/***
* author:xnby
* platform: esp
* version: 1.0.0
* UIResolution: 480*480
*/

#include "qm_ui_entry.h"
#include <stdio.h>
#include <stdlib.h>

    /*
    * includes
    */
    /*
    * declear main_screen
    */
lv_obj_t * main_screen;
lv_obj_t * label_34ea;

    LV_FONT_DECLARE(ali_font_16);
    LV_FONT_DECLARE(ali_font_41);

    void show_screen_main_screen();

 /*
    * keyboard
    */
    lv_obj_t * g_kb = NULL;

static void g_kb_event_cb(lv_obj_t * event_kb, lv_event_t event)
{
    /* Just call the regular event handler */
    if (event == LV_EVENT_APPLY)
    {
        lv_obj_set_hidden(event_kb, true);
    }
    else if (event == LV_EVENT_CANCEL)
    {
        lv_obj_set_hidden(event_kb, true);
    }
    else
    {
        lv_keyboard_def_event_cb(event_kb, event);
    }
}
static void g_create_kb_ifneeded()
{
    if (g_kb == NULL)
    {
        g_kb = lv_keyboard_create(lv_scr_act(), NULL);
        lv_obj_set_pos(g_kb, 5, 90);
        lv_obj_set_event_cb(g_kb, g_kb_event_cb); /* Setting a custom event handler stops the keyboard from closing automatically */
        lv_obj_set_size(g_kb, LV_HOR_RES - 10, 140);
        lv_keyboard_set_cursor_manage(g_kb, true); /* Automatically show/hide cursors on text areas */
        lv_obj_set_hidden(g_kb, true);
    }
}
static void g_show_kb()
{
    lv_obj_set_parent(g_kb, lv_scr_act());
    lv_obj_set_hidden(g_kb, false);
    lv_obj_align(g_kb, NULL, LV_ALIGN_IN_BOTTOM_MID, 0, 0);
}
 /**
    *开机启动屏相关
    */

int timer_cnt = 0;

/*
    * declear codes
    */



/*
 * callback functions begins
 */
/*
 * callback functions ends
 */
    /*
    * init codes
    */
void init_function()
{
#ifdef BLOCKLY_init
    blockly_init();
#endif
    }

void lv_8ms_label_34ea_create()
{
	label_34ea = lv_label_create(main_screen, NULL);
	lv_label_set_long_mode(label_34ea,LV_LABEL_LONG_BREAK);
	lv_obj_set_size(label_34ea, 211, 166);
//set label style
	lv_obj_set_style_local_text_font(label_34ea,LV_LABEL_PART_MAIN,LV_STATE_DEFAULT,&ali_font_41);

	lv_label_set_text(label_34ea, "helloworld");
	lv_label_set_align(label_34ea, LV_LABEL_ALIGN_CENTER);
#ifdef BLOCKLY_label_34ea_EVENT_HANDLER
	lv_obj_set_event_cb(label_34ea, label_34ea_event_handler);
#endif
	if(label_34ea->coords.x2-label_34ea->coords.x1<211)
	{
		int x_offset=(211-(label_34ea->coords.x2-label_34ea->coords.x1))/2;
		lv_obj_set_style_local_pad_left(label_34ea,LV_LABEL_PART_MAIN,LV_STATE_DEFAULT,x_offset);
		lv_obj_set_style_local_pad_right(label_34ea,LV_LABEL_PART_MAIN,LV_STATE_DEFAULT,x_offset);
	}
	if(label_34ea->coords.y2-label_34ea->coords.y1<166)
	{
		int y_offset=(166-(label_34ea->coords.y2-label_34ea->coords.y1))/2;
		lv_obj_set_style_local_pad_bottom(label_34ea,LV_LABEL_PART_MAIN,LV_STATE_DEFAULT,y_offset);
		lv_obj_set_style_local_pad_top(label_34ea,LV_LABEL_PART_MAIN,LV_STATE_DEFAULT,y_offset);
	}
		lv_obj_set_pos(label_34ea, 33, 35);

}

void show_screen_main_screen()
{
        lv_scr_load(main_screen);

    
}

int screen_loop_enter = 0;
int _delay = 0;
void lv_qm_ui_loop(void)
{
    #ifdef BLOCKLY_loop
    blockly_loop();
    #endif
}
bool g_logo_inited = false;

void lv_qm_ui_entry(void)
{
/*
    * create screens
    */
    main_screen = lv_obj_create(NULL, NULL);

//CREATE STATIC OBJ
    lv_8ms_label_34ea_create();
  
    static lv_style_t style_main_main_screen;
    lv_style_init(&style_main_main_screen);
    lv_style_set_bg_color(&style_main_main_screen,LV_STATE_DEFAULT,LV_COLOR_WHITE);
    lv_obj_add_style(main_screen,LV_CONT_PART_MAIN,&style_main_main_screen);
    lv_scr_load(main_screen);
    init_function();
}
