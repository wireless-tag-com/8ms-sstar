/**
 * @file lv_tutorial_hello_world
 *
 */

#ifndef LV_TUTORIAL_HELLO_WORLD_H
#define LV_TUTORIAL_HELLO_WORLD_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/

#include "lvgl/lvgl.h"


/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 * GLOBAL PROTOTYPES
 **********************/
void lv_qm_ui_entry(void);
void lv_qm_ui_loop(void);
 
lv_obj_t *lv_8ms_get_screen(char *name);
lv_obj_t *lv_8ms_get_widget(char *w_name);
const char *lv_8ms_get_obj_name(lv_obj_t *obj);

/**********************
 *      MACROS
 **********************/


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*LV_TUTORIAL_HELLO_WORLD_H*/
