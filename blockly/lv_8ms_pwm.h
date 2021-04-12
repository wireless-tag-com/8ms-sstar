#ifndef __LV_8MS_PWM_H
#define __LV_8MS_PWM_H

int lv_8ms_pwm_init(int chipid, int pin);
int lv_8ms_pwm_uninit(int chipid, int pin);
int lv_8ms_pwm_read_period(int chidid, int pin);
int lv_8ms_pwm_write_period(int chidid, int pin, int s);
int lv_8ms_pwm_write_period_ms(int chidid, int pin, int ms);
int lv_8ms_pwm_write_period_us(int chidid, int pin, int us);
int lv_8ms_pwm_read_duty(int chipid, int pin, int duty);
int lv_8ms_pwm_write_duty(int chipid, int pin, int duty);
int lv_8ms_pwm_set_status(int chidid, int pin, int status);
int lv_8ms_pwm_get_status(int chipid, int pin);

#endif
