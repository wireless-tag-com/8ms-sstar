/**
* author:xnby
* platform: esp
* version: 1.0.0
* UIResolution: 480*320
*/

#include "qm_ui_entry.h"
#include <stdio.h>
#include <stdlib.h>

/*
* includes
*/
/*
* declear screens
*/
lv_obj_t * main_screen;

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

void lv_qm_ui_entry(void)
{
    LV_FONT_DECLARE(ali_font_16);
    lv_font_montserrat_16 = ali_font_16;

    /*
    * create screens
    */
    main_screen = lv_obj_create(NULL, NULL);

    /*
    * ui-element begins
    */

    init_function();
}
