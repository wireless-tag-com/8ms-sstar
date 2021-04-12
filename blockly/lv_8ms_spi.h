#ifndef __LV_8MS_SPI_H
#define __LV_8MS_SPI_H

int lv_8ms_spi_init(const char *devname, int mode, int lsbmode, int bits, int speed);
void lv_8ms_spi_uninit(int fd);

int lv_8ms_spi_write(int fd, unsigned char *buf, int len);
int lv_8ms_spi_read(int fd, unsigned char *buf, int len);
int lv_8ms_spi_trnasfer(int fd, unsigned char *tbuf, unsigned char *rbuf, int len, int delay);

#endif
