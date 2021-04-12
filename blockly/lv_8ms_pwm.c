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

#include "lv_8ms_pwm.h"

#define SYSFS_CLASS_PWM "/sys/class/pwm"

int lv_8ms_pwm_init(int chipid, int pin)
{
	char directory[PATH_MAX];
	snprintf(directory, PATH_MAX, SYSFS_CLASS_PWM "/pwmchip%d/pwm%d", chipid, pin);
	struct stat dir;
	if (stat(directory, &dir) == 0) {
		syslog(LOG_NOTICE, "pwm_init: pwm%i already exported, continuing", pin);
	} else {
		char buffer[PATH_MAX];
		snprintf(buffer, PATH_MAX, "/sys/class/pwm/pwmchip%d/export", chipid);
		int export_f = open(buffer, O_WRONLY);
		if (export_f == -1) {
			syslog(LOG_ERR, "pwm_init: pwm%i. Failed to open export for writing: %s", pin, strerror(errno));
			return -1;
		}

		char out[PATH_MAX];
		int size = snprintf(out, PATH_MAX, "%d", pin);
		if (write(export_f, out, size * sizeof(char)) == -1) {
			syslog(LOG_WARNING, "pwm_init: pwm%i. Failed to write to export! (%s) Potentially already in use.", pin, strerror(errno));
			close(export_f);
			return -1;
		}
		close(export_f);
	}

	return 0;
}

int lv_8ms_pwm_uninit(int chipid, int pin)
{
	char bu[PATH_MAX];
	int length;
	int ret = -1;

	lv_8ms_pwm_set_status(chipid, pin , 0);
	snprintf(bu, PATH_MAX, SYSFS_CLASS_PWM "/pwmchip%d/unexport", chipid);

	int export = open(bu, O_WRONLY);
	if (export == -1) {
		syslog(LOG_ERR, "pwm%i: uninit: Failed to open 'unexport' for writing: %s", pin, strerror(errno));
		ret = -1;
		goto err_out;
	}

	length = snprintf(bu, sizeof(bu), "%d", pin);
	if (write(export, bu, length * sizeof(char)) == -1) {
		syslog(LOG_ERR, "pwm%i: uninit: Failed to write to 'unexport': %s", pin, strerror(errno));
		close(export);
		ret = -1;
		goto err_out;
	}

	close(export);

err_out:
	return ret;
}

static int __lv_8ms_pwm_write_period(int chipid, int pin, int period)
{
	char bu[PATH_MAX];
	int duty_fp;
	snprintf(bu, PATH_MAX, SYSFS_CLASS_PWM "/pwmchip%d/pwm%d/period", chipid, pin);

	duty_fp = open(bu, O_RDWR);
	if (duty_fp == -1) {
		return -1;
	}

	int length = snprintf(bu, PATH_MAX, "%d", period);
	if (write(duty_fp, bu, length * sizeof(char)) == -1)
	{
		syslog(LOG_ERR, "pwm%i write_period: Failed to write to period: %s", pin, strerror(errno));
		return -1;
	}
	return 0;
}

int lv_8ms_pwm_write_period_us(int chipid, int pin, int us)
{
	return __lv_8ms_pwm_write_period(chipid, pin, us * 1000);
}

int lv_8ms_pwm_write_period_ms(int chipid, int pin, int ms)
{
	return lv_8ms_pwm_write_period_us(chipid, pin, ms * 1000);
}

int lv_8ms_pwm_write_period(int chipid, int pin, int s)
{
	return lv_8ms_pwm_write_period_ms(chipid, pin, s * 1000);
}

int lv_8ms_pwm_read_period(int chipid, int pin)
{
	char bu[PATH_MAX];
	char output[PATH_MAX];

	snprintf(bu, PATH_MAX, SYSFS_CLASS_PWM "/pwmchip%d/pwm%d/period", chipid, pin);

	int period_f = open(bu, O_RDWR);
	if (period_f == -1) {
		syslog(LOG_ERR, "pwm%i read_period: Failed to open period for reading: %s", pin, strerror(errno));
		return 0;
	}

	lseek(period_f, 0, SEEK_SET);
	ssize_t rb = read(period_f, output, PATH_MAX);
	close(period_f);

	if (rb < 0) {
		syslog(LOG_ERR, "pwm%i read_period: Failed to read period: %s", pin, strerror(errno));
		return -1;
	}

	char* endptr;
	long int ret = strtol(output, &endptr, 10);
	if ('\0' != *endptr && '\n' != *endptr) {
		syslog(LOG_ERR, "pwm%i read_period: Error in string conversion", pin);
		return -1;
	} else if (ret > INT_MAX || ret < INT_MIN) {
		syslog(LOG_ERR, "pwm%i read_period: Number is invalid", pin);
		return -1;
	}
	return (int) ret;
}

int lv_8ms_pwm_write_duty(int chipid, int pin, int duty)
{
	char bu[PATH_MAX];
	int duty_fp;
	snprintf(bu, PATH_MAX, SYSFS_CLASS_PWM "/pwmchip%d/pwm%d/duty_cycle", chipid, pin);

	duty_fp = open(bu, O_RDWR);
	if (duty_fp == -1) {
		return -1;
	}

	int length = snprintf(bu, PATH_MAX, "%d", duty);
	if (write(duty_fp, bu, length * sizeof(char)) == -1)
	{
		syslog(LOG_ERR, "pwm%i write_duty: Failed to write to duty_cycle: %s", pin, strerror(errno));
		return -1;
	}
	return 0;
}

int lv_8ms_pwm_read_duty(int chipid, int pin, int duty)
{
	char bu[PATH_MAX];
	int duty_fp;
	snprintf(bu, PATH_MAX, SYSFS_CLASS_PWM "/pwmchip%d/pwm%d/duty_cycle", chipid, pin);

	duty_fp = open(bu, O_RDWR);
	if (duty_fp == -1) {
		return -1;
	}

	lseek(duty_fp, 0, SEEK_SET);

	char output[PATH_MAX];
	ssize_t rb = read(duty_fp, output, PATH_MAX);
	if (rb < 0) {
		syslog(LOG_ERR, "pwm%i read_duty: Failed to read duty_cycle: %s", pin, strerror(errno));
		return -1;
	}

	char* endptr;
	long int ret = strtol(output, &endptr, 10);
	if ('\0' != *endptr && '\n' != *endptr) {
		syslog(LOG_ERR, "pwm%i read_duty: Error in string conversion", pin);
		return -1;
	} else if (ret > INT_MAX || ret < INT_MIN) {
		syslog(LOG_ERR, "pwm%i read_duty: Number is invalid", pin);
		return -1;
	}
	return (int) ret;
}

int lv_8ms_pwm_set_status(int chipid, int pin, int status)
{
	char bu[PATH_MAX];
	int enable_fp;
	snprintf(bu, PATH_MAX, SYSFS_CLASS_PWM "/pwmchip%d/pwm%d/enable", chipid, pin);

	enable_fp = open(bu, O_RDWR);
	if (enable_fp == -1) {
		return -1;
	}

	int length = snprintf(bu, PATH_MAX, "%d", status);
	if (write(enable_fp, bu, length * sizeof(char)) == -1)
	{
		syslog(LOG_ERR, "pwm%i write_period: Failed to write to period: %s", pin, strerror(errno));
		return -1;
	}
	return 0;
}

int lv_8ms_pwm_get_status(int chipid, int pin)
{
	char bu[PATH_MAX];
	int fd;
	snprintf(bu, PATH_MAX, SYSFS_CLASS_PWM "/pwmchip%d/pwm%d/enable", chipid, pin);

	fd = open(bu, O_RDWR);
	if (fd == -1) {
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