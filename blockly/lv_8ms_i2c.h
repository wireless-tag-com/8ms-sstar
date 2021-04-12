#ifndef __LV_8MS_I2C_H
#define __LV_8MS_I2C_H

int lv_8ms_i2c_init(const char *devname);
void lv_8ms_i2c_uninit(int fd);
int lv_8ms_i2c_write(int fd, unsigned char slave_addr, unsigned char reg_addr, unsigned char value);
int lv_8ms_i2c_read(int fd, unsigned char slave_addr, unsigned char reg_addr, unsigned char *value);

#endif
