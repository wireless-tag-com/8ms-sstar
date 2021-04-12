#ifndef __LV_8MS_GPIO_H
#define __LV_8MS_GPIO_H

typedef enum {
	LV_8MS_GPIO_OUT = 0,      /**< Output. A Mode can also be set */
	LV_8MS_GPIO_IN = 1,       /**< Input */
	LV_8MS_GPIO_OUT_HIGH = 2, /**< Output. Init High */
	LV_8MS_GPIO_OUT_LOW = 3   /**< Output. Init Low */
} lv_8ms_gpio_dir_t;

int lv_8ms_gpio_init(int pin, int dir);
int lv_8ms_gpio_uninit(int pin);
int lv_8ms_gpio_set_dir(int pin, int dir);
int lv_8ms_gpio_get_dir(int pin, int *dir);
int lv_8ms_gpio_read(int pin);
int lv_8ms_gpio_write(int pin, int value);

#endif
