#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <termios.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <libubox/uloop.h>

#include "ringbuf.h"
#include "wtctrl.h"
#include "qm_ui_entry.h"

#define RINGBUF_SIZE 2048

static ringbuf_t g_tty_rb;
static struct uloop_fd g_tty_fd;
static char g_json_buf[RINGBUF_SIZE];

static ssize_t __wttty_swriten(int fd, const void *vptr, size_t n);

static int __wttty_event_cb(lv_obj_t *obj, lv_event_t event)
{
	const char *name = lv_8ms_get_obj_name(obj);
	cJSON *cres;
	char *json_str;

	/*
	 * if event is not set
	 */
	if (event != LV_EVENT_CLICKED) {
		return 0;
	}

	name = lv_8ms_get_obj_name(obj);
	if (name == NULL) {
		return -1;
	}

	cres = cJSON_CreateObject();
	if (!cres) {
		return -1;
	}

	cJSON_AddItemToObject(cres, WTCTRL_WID_NAME, cJSON_CreateString(name));
	cJSON_AddItemToObject(cres, WTCTRL_EVENT_NAME, cJSON_CreateNumber(event));
	json_str = cJSON_PrintUnformatted(cres);
	if (json_str) {
		__wttty_swriten(g_tty_fd.fd, json_str, strlen(json_str));
		__wttty_swriten(g_tty_fd.fd, "\n", 1);
		free(json_str);
		cJSON_Delete(cres);
		return 0;
	}

	cJSON_Delete(cres);

	return -1;
}

static int __wttty_set_opt(int fd, int c_flow, int nSpeed, int nBits, char nEvent, int nStop)
{
	struct termios newtio,oldtio;

	if(tcgetattr(fd,&oldtio)!=0) {
		printf("tcgetattr (%s) failed\n", strerror(errno));
		return -1;
	}

	bzero(&newtio,sizeof(newtio));
	newtio.c_cflag |= CLOCAL | CREAD;
	newtio.c_cflag &= ~CSIZE;

	newtio.c_lflag &=~ICANON;

	switch(nBits)
	{
	case 7:
		newtio.c_cflag |= CS7;
		break;
	case 8:
		newtio.c_cflag |=CS8;
		break;
	}


	switch(c_flow)
	{
	case 0:
		newtio.c_cflag &= ~CRTSCTS;
		break;
	case 1:
		newtio.c_cflag |= CRTSCTS;
		break;
	case 2:
		newtio.c_cflag |= IXON|IXOFF|IXANY;
		break;
	default:
		newtio.c_cflag &= ~CRTSCTS;
		return -1;
	}

	switch(nEvent)
	{
	case 'O':
		newtio.c_cflag |= PARENB;
		newtio.c_cflag |= PARODD;
		newtio.c_iflag |= (INPCK | ISTRIP);
		break;
	case 'E':
		newtio.c_iflag |= (INPCK | ISTRIP);
		newtio.c_cflag |= PARENB;
		newtio.c_cflag &= ~PARODD;
		break;
	case 'N':
		newtio.c_cflag &=~PARENB;
		break;
	}

	switch(nSpeed)
	{
	case 2400:
		cfsetispeed(&newtio,B2400);
		cfsetospeed(&newtio,B2400);
		break;
	case 4800:
		cfsetispeed(&newtio,B4800);
		cfsetospeed(&newtio,B4800);
		break;
	case 9600:
		cfsetispeed(&newtio,B9600);
		cfsetospeed(&newtio,B9600);
		break;
	case 57600:
		cfsetispeed(&newtio,B57600);
		cfsetospeed(&newtio,B57600);
		break;
	case 115200:
		cfsetispeed(&newtio,B115200);
		cfsetospeed(&newtio,B115200);
		break;
	case 460800:
		cfsetispeed(&newtio,B460800);
		cfsetospeed(&newtio,B460800);
		break;
	default:
		cfsetispeed(&newtio,B9600);
		cfsetospeed(&newtio,B9600);
		break;
	}

	if(nStop == 1)
		newtio.c_cflag &= ~CSTOPB;
	else if(nStop == 2)
		newtio.c_cflag |= CSTOPB;

	newtio.c_oflag &= ~OPOST;
	newtio.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);

	newtio.c_cc[VTIME] = 1;
	newtio.c_cc[VMIN] = 0;
	tcflush(fd,TCIFLUSH);

	if(tcsetattr(fd,TCSANOW,&newtio)!=0) {
		printf("tcsetattr (%s) failed\n", strerror(errno));
		return -1;
	}

	return 0;
}

static ssize_t __wttty_safe_read(int fd, void *buf, size_t count)
{
	ssize_t n;

	do {
		n = read(fd, buf, count);
	} while(n < 0 && errno == EINTR);

	return n;
}

static ssize_t __wttty_swriten(int fd, const void *vptr, size_t n)
{
	size_t  nleft;
	ssize_t nwritten;
	const char *ptr;

	ptr = vptr;
	nleft = n;

	while(nleft > 0) {
		if ((nwritten = write(fd, ptr, nleft)) <= 0) {
			if(nwritten < 0 && errno == EINTR)
				nwritten = 0;
			else
				return -1;
		}

		nleft -= nwritten;
		ptr   += nwritten;
	}

	return(n);
}

static void __wttty_loop_cb(struct uloop_fd *u, unsigned int events)
{
	if (events & ULOOP_READ) {
		unsigned char buf[128];
		ssize_t ret;

		memset(buf, 0, sizeof(buf));
		ret = __wttty_safe_read(u->fd, &buf, sizeof(buf));
		if (ret < 0) {
			printf("read header error(%s)\n", strerror(errno));
			return;
		} else if (ret > 0) {
			ssize_t offset;

			ringbuf_memcpy_into(g_tty_rb, buf, ret);

			offset = ringbuf_findchr(g_tty_rb, '\n', 0);

			if (offset > 0) {
				char *rbuf;

				memset(g_json_buf, 0, sizeof(g_json_buf));
				ringbuf_memcpy_from(g_json_buf, g_tty_rb, offset + 1);
				g_json_buf[offset] = 0;
				rbuf = wtctrl_json_parse(g_json_buf);
				if (rbuf) {
					ret = __wttty_swriten(u->fd, rbuf, strlen(rbuf));
					ret = __wttty_swriten(u->fd, "\n", 1);
					free(rbuf);
				}
			}
		}
	}
}

int wttty_init(const char *tty_dev)
{
	int fd;
	int ret;

	fd = open(tty_dev, O_RDWR | O_NOCTTY | O_NONBLOCK);

	if (fd < 0) {
		printf("open %s(%s) failed\n", tty_dev, strerror(errno));
		return -1;
	}

	ret = __wttty_set_opt(fd, 0, 115200, 8, 'N', 1);

	if(ret == -1) {
		printf("set %s(%s) opt failed\n", tty_dev, strerror(errno));
		close(fd);
		return -1;
	}

	printf("init %s ok\n", tty_dev);

	g_tty_rb = ringbuf_new(RINGBUF_SIZE - 1);

	g_tty_fd.fd = fd;
	g_tty_fd.cb = __wttty_loop_cb;
	uloop_fd_add(&g_tty_fd, ULOOP_READ);

	lv_obj_8ms_set_event_cb(__wttty_event_cb);

	return fd;
}

void wttty_close(void)
{
	if (g_tty_fd.fd) {
		close(g_tty_fd.fd);
	}

	if (g_tty_rb) {
		ringbuf_free(&g_tty_rb);
	}
}
