#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/time.h>
#include <syslog.h>
#include <stdio.h>
#include <limits.h>
#include <linux/spi/spidev.h>

#include "lv_8ms_spi.h"

int lv_8ms_spi_init(const char *devname, int mode, int lsbmode, int bits, int speed)
{
	int fd;
	int ret;

	if (!devname)
		return -1;

	fd = open(devname, O_RDWR);
	if (fd < 0) {
		syslog(LOG_ERR, "spi_init: open %s failed(%s)", devname, strerror(errno));
		return -1;
	}

	ret = ioctl(fd, SPI_IOC_WR_MODE, &mode);
	if (ret < 0) {
		goto err_out;
	}

	ret = ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits);
	if (ret < 0) {
		goto err_out;
	}

	ret = ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
	if (ret < 0) {
		goto err_out;
	}

	return fd;

err_out:
	close(fd);
	return -1;
}

void lv_8ms_spi_uninit(int fd)
{
	if (fd > 0)
		close(fd);
}

int lv_8ms_spi_write(int fd, unsigned char *buf, int len)
{
	int ret;

	if (fd <= 0)
		return -1;

	ret = write(fd, buf, len);

	return ret;
}

int lv_8ms_spi_read(int fd, unsigned char *buf, int len)
{
	int ret;

	if (fd <= 0)
		return -1;

	ret = read(fd, buf, len);

	return ret;
}

int lv_8ms_spi_trnasfer(int fd, unsigned char *tbuf, unsigned char *rbuf, int len, int delay)
{
	int ret;

	if (fd <= 0)
		return -1;

	struct spi_ioc_transfer tr ={
		.tx_buf = (unsigned long) tbuf,
		.rx_buf = (unsigned long) rbuf,
		.len = len,
		.delay_usecs = delay,
	};

	ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);

	return ret;
}