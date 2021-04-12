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
#include <linux/i2c.h> 
#include <linux/i2c-dev.h>

#include "lv_8ms_i2c.h"

int lv_8ms_i2c_init(const char *devname)
{
	if (!devname)
		return -1;

	int fd = open(devname, O_RDWR);
	if (fd < 0) {
		syslog(LOG_ERR, "i2c_init: open %s failed(%s)", devname, strerror(errno));
		return -1;
	}

	return fd;
}

void lv_8ms_i2c_uninit(int fd)
{
	if (fd > 0)
		close(fd);
}

int lv_8ms_i2c_write(int fd, unsigned char slave_addr, unsigned char reg_addr, unsigned char value)
{
	if (fd <= 0)
		return -1;

	unsigned char outbuf[2];
	struct i2c_rdwr_ioctl_data packets;
	struct i2c_msg messages[1];

	messages[0].addr = slave_addr;
	messages[0].flags = 0;
	messages[0].len = sizeof(outbuf);
	messages[0].buf = outbuf;

	outbuf[0] = reg_addr;
	outbuf[1] = value;

	packets.msgs = messages;
	packets.nmsgs = 1;

	if(ioctl(fd, I2C_RDWR, &packets) < 0)
	{
		syslog(LOG_ERR, "i2c_write failed(%s)", strerror(errno));
		return 1;
	}

	return 0;
}

int lv_8ms_i2c_read(int fd, unsigned char slave_addr, unsigned char reg_addr, unsigned char *value)
{
	if (fd <= 0)
		return -1;

	unsigned char inbuf, outbuf;
	struct i2c_rdwr_ioctl_data packets;
	struct i2c_msg messages[2];

	outbuf = reg_addr;
	messages[0].addr = slave_addr;
	messages[0].flags = 0;
	messages[0].len = sizeof(outbuf);
	messages[0].buf = &outbuf;

	messages[1].addr = slave_addr;
	messages[1].flags = I2C_M_RD;
	messages[1].len = sizeof(inbuf);
	messages[1].buf = &inbuf;

	packets.msgs = messages;
	packets.nmsgs = 2;

	if(ioctl(fd, I2C_RDWR, &packets) < 0)
	{
		syslog(LOG_ERR, "i2c_read failed(%s)", strerror(errno));
		return 1;
	}
	*value = inbuf;

	return 0;
}