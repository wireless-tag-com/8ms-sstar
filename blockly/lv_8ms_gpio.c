#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <poll.h>
#include <pthread.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/time.h>
#include <syslog.h>
#include <stdio.h>

#include "lv_8ms_gpio.h"

#define SYSFS_CLASS_GPIO "/sys/class/gpio"

int lv_8ms_gpio_init(int pin, int dir)
{
	char bu[PATH_MAX];
	int length;
	int ret = -1;

	char directory[PATH_MAX];
	snprintf(directory, PATH_MAX, SYSFS_CLASS_GPIO "/gpio%d/", pin);
	struct stat idir;
	if (stat(directory, &idir) != 0) {
		int export = open(SYSFS_CLASS_GPIO "/export", O_WRONLY);
		if (export == -1) {
			syslog(LOG_ERR, "gpio%i: init: Failed to open 'export' for writing: %s", pin, strerror(errno));
			ret = -1;
			goto err_out;
		}

		length = snprintf(bu, sizeof(bu), "%d", pin);
		if (write(export, bu, length * sizeof(char)) == -1) {
			syslog(LOG_ERR, "gpio%i: init: Failed to write to 'export': %s", pin, strerror(errno));
			close(export);
			ret = -1;
			goto err_out;
		}
		close(export);
	}
	lv_8ms_gpio_set_dir( pin,  dir);
err_out:
	return ret;
}

int lv_8ms_gpio_uninit(int pin)
{
	char bu[PATH_MAX];
	int length;
	int ret = -1;

	int export = open(SYSFS_CLASS_GPIO "/unexport", O_WRONLY);
	if (export == -1) {
		syslog(LOG_ERR, "gpio%i: uninit: Failed to open 'unexport' for writing: %s", pin, strerror(errno));
		ret = -1;
		goto err_out;
	}

	length = snprintf(bu, sizeof(bu), "%d", pin);
	if (write(export, bu, length * sizeof(char)) == -1) {
		syslog(LOG_ERR, "gpio%i: uninit: Failed to write to 'unexport': %s", pin, strerror(errno));
		close(export);
		ret = -1;
		goto err_out;
	}

	close(export);

err_out:
	return ret;
}

int lv_8ms_gpio_set_dir(int pin, int dir)
{
	char bu[PATH_MAX];
	int fd;
	int length;

	snprintf(bu, PATH_MAX, SYSFS_CLASS_GPIO "/gpio%d/direction", pin);

	fd = open(bu, O_RDWR);
	if (fd == -1) {
		syslog(LOG_ERR, "gpio%i: Failed to open 'value': %s", pin, strerror(errno));
		return -1;
	}

	switch (dir) {
	case LV_8MS_GPIO_OUT:
		length = snprintf(bu, sizeof(bu), "out");
		break;
	case LV_8MS_GPIO_IN:
		length = snprintf(bu, sizeof(bu), "in");
		break;
	case LV_8MS_GPIO_OUT_HIGH:
		length = snprintf(bu, sizeof(bu), "high");
		break;
	case LV_8MS_GPIO_OUT_LOW:
		length = snprintf(bu, sizeof(bu), "low");
		break;
	default:
		close(fd);
		return -1;
	}

	if (write(fd, bu, length * sizeof(char)) == -1) {
		close(fd);
		syslog(LOG_ERR, "gpio%i: dir: Failed to write to 'direction': %s", pin, strerror(errno));
		return -1;
	}

	close(fd);
	return 0;
}

int lv_8ms_gpio_get_dir(int pin, int *dir)
{
	char bu[PATH_MAX];
	int fd;
	char value[5];
	int rc;

	snprintf(bu, PATH_MAX, SYSFS_CLASS_GPIO "/gpio%d/direction", pin);

	fd = open(bu, O_RDWR);
	if (fd == -1) {
		syslog(LOG_ERR, "gpio%i: Failed to open 'value': %s", pin, strerror(errno));
		return -1;
	}

	memset(value, '\0', sizeof(value));
	rc = read(fd, value, sizeof(value));
	close(fd);
	if (rc <= 0) {
		syslog(LOG_ERR, "gpio%i: read_dir: Failed to read 'direction': %s", pin, strerror(errno));
		return -1;
	}

	if (strcmp(value, "out\n") == 0) {
		*dir = LV_8MS_GPIO_OUT;
	} else if (strcmp(value, "in\n") == 0) {
		*dir = LV_8MS_GPIO_IN;
	} else {
		syslog(LOG_ERR, "gpio%i: read_dir: unknown direction: %s", pin, value);
		return -1;
	}
}

int lv_8ms_gpio_read(int pin)
{
	char bu[PATH_MAX];
	int fd;

	snprintf(bu, PATH_MAX, SYSFS_CLASS_GPIO "/gpio%d/value", pin);

	fd = open(bu, O_RDWR);
	if (fd == -1) {
		syslog(LOG_ERR, "gpio%i: Failed to open 'value': %s", pin, strerror(errno));
		return -1;
	}

	if (lseek(fd, 0, SEEK_SET) == -1) {
		syslog(LOG_ERR, "gpio%i: write: Failed to lseek 'value': %s", pin, strerror(errno));
		close(fd);
		return -1;
	}

	char res[2];
	if (read(fd, res, 2 * sizeof(char)) != 2) {
		syslog(LOG_ERR, "gpio%i: read: Failed to read a sensible value from sysfs: %s",
						pin, strerror(errno));
		lseek(fd, 0, SEEK_SET);
		close(fd);
		return -1;
	}
	lseek(fd, 0, SEEK_SET);
	close(fd);

	return (int) strtol(res, NULL, 10);
}

int lv_8ms_gpio_write(int pin, int value)
{
	char bu[PATH_MAX];
	int fd;

	snprintf(bu, PATH_MAX, SYSFS_CLASS_GPIO "/gpio%d/value", pin);

	fd = open(bu, O_RDWR);
	if (fd == -1) {
		syslog(LOG_ERR, "gpio%i: Failed to open 'value': %s", pin, strerror(errno));
		return -1;
	}

	if (lseek(fd, 0, SEEK_SET) == -1) {
		syslog(LOG_ERR, "gpio%i: write: Failed to lseek 'value': %s", pin, strerror(errno));
		close(fd);
		return -1;
	}

	int length = snprintf(bu, sizeof(bu), "%d", value);
	if (write(fd, bu, length * sizeof(char)) == -1) {
		syslog(LOG_ERR, "gpio%i: write: Failed to write to 'value': %s", pin, strerror(errno));
		close(fd);
		return -1;
	}

	close(fd);
	return 0;
}