#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <syslog.h>
#include <stdio.h>
#include <limits.h>
#include <fcntl.h>
#include <termios.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "lv_8ms_uart.h"

static speed_t
uint2speed(unsigned int speed)
{
    switch (speed) {
        case 0:
            return B0; // hangup, not too useful otherwise
        case 50:
            return B50;
        case 75:
            return B75;
        case 110:
            return B110;
        case 150:
            return B150;
        case 200:
            return B200;
        case 300:
            return B300;
        case 600:
            return B600;
        case 1200:
            return B1200;
        case 1800:
            return B1800;
        case 2400:
            return B2400;
        case 4800:
            return B4800;
        case 9600:
            return B9600;
        case 19200:
            return B19200;
        case 38400:
            return B38400;
        case 57600:
            return B57600;
        case 115200:
            return B115200;
        case 230400:
            return B230400;
        case 460800:
            return B460800;
        case 500000:
            return B500000;
        case 576000:
            return B576000;
        case 921600:
            return B921600;
        case 1000000:
            return B1000000;
        case 1152000:
            return B1152000;
        case 1500000:
            return B1500000;
        case 2000000:
            return B2000000;
        case 2500000:
            return B2500000;
        case 3000000:
            return B3000000;
        case 3500000:
            return B3500000;
        case 4000000:
            return B4000000;
        default:
            // if we are here, then an unsupported baudrate was selected.
            return B0;
    }
}

static unsigned int speed_to_uint(speed_t speedt) {
    struct baud_table {
        speed_t speedt;
        unsigned int baudrate;
    };
    static const struct baud_table bauds[] = {
        { B50, 50 },
        { B75, 75 },
        { B110, 110 },
        { B150, 150 },
        { B200, 200 },
        { B300, 300 },
        { B600, 600 },
        { B1200, 1200 },
        { B1800, 1800 },
        { B2400, 2400 },
        { B9600, 9600 },
        { B19200, 19200 },
        { B38400, 38400 },
        { B57600, 57600 },
        { B115200, 115200 },
        { B230400, 230400 },
        { B460800, 460800 },
        { B500000, 500000 },
        { B576000, 576000 },
        { B921600, 921600 },
        { B1000000, 1000000 },
        { B1152000, 1152000 },
        { B1500000, 1500000 },
        { B2000000, 2000000 },
        { B2500000, 2500000 },
        { B3000000, 3000000 },
#if !defined(MSYS)
        { B3500000, 3500000 },
        { B4000000, 4000000 },
#endif
        { B0, 0} /* Must be last in this table */
    };
    int i = 0;

    while (bauds[i].baudrate > 0) {
        if (speedt == bauds[i].speedt) {
            return bauds[i].baudrate;
        }
        i++;
    }
    return 0;
}

int lv_8ms_uart_init(const char *devname)
{
	if (!devname)
		return -1;

	int fd = open(devname, O_RDWR | O_NOCTTY);
	if (fd < 0) {
		syslog(LOG_ERR, "uart_init: open %s failed(%s)", devname, strerror(errno));
		return -1;
	}
    // now setup the tty and the selected baud rate
    struct termios termio;

    // get current modes
    if (tcgetattr(fd, &termio)) {
        syslog(LOG_ERR, "uart: tcgetattr(%s) failed: %s", devname, strerror(errno));
        goto err_out;
    }

    cfmakeraw(&termio);
    if (tcsetattr(fd, TCSAFLUSH, &termio) < 0) {
        syslog(LOG_ERR, "uart: tcsetattr(%s) failed after cfmakeraw(): %s", devname, strerror(errno));
        goto err_out;
    }

	return fd;

err_out:
	close(fd);
	return -1;
}

int lv_8ms_uart_settings(int fd, int* baudrate, int* databits, int* stopbits, lv_8ms_uart_parity_t* parity, unsigned int* ctsrts, unsigned int* xonxoff)
{
    struct termios term;

    /* is any information that requires opening the device requested?  */
    if (baudrate != NULL || stopbits != NULL || parity != NULL || databits != NULL || ctsrts != NULL || xonxoff != NULL) {
       if (tcgetattr(fd, &term)) {
           close(fd);
           return -1;
       }

       if (databits != NULL) {
           switch (term.c_cflag & CSIZE) {
           case CS8:
               *databits = 8;
               break;
           case CS7:
               *databits = 7;
               break;
           case CS6:
               *databits = 6;
               break;
           case CS5:
               *databits = 5;
           default: /* Cannot happen? Linux kernel CSIZE mask is exactly two bits wide */
               break;
           }
       }

       if (stopbits != NULL) {
           *stopbits = term.c_cflag & CSTOPB ? 2 : 1;
       }

       if (parity != NULL) {
           if (term.c_cflag & PARODD) *parity = LV_8MS_UART_PARITY_ODD;
           else
           if (term.c_cflag & PARENB) *parity = LV_8MS_UART_PARITY_EVEN;
           else
           *parity = LV_8MS_UART_PARITY_NONE;
       }

       if (baudrate != NULL) {
           *baudrate = speed_to_uint(cfgetospeed(&term));
       }

       if (ctsrts != NULL) {
           *ctsrts = term.c_cflag & CRTSCTS;
       }

       if (xonxoff != NULL) {
           *xonxoff = term.c_cflag & (IXON|IXOFF);
       }
    }

    return 0;
}

int lv_8ms_uart_flush(int fd)
{
    if (tcdrain(fd) == -1) {
        return -1;
    }

    return 0;
}

int lv_8ms_uart_sendbreak(int fd, int duration)
{
    if (tcsendbreak(fd, duration) == -1) {
        return -1;
    }

    return 0;
}

int lv_8ms_uart_set_baudrate(int fd, unsigned int baud)
{
    struct termios termio;
    if (tcgetattr(fd, &termio)) {
        syslog(LOG_ERR, "uart: set_baudrate: tcgetattr() failed: %s", strerror(errno));
        return -1;
    }

    // set our baud rates
    speed_t speed = uint2speed(baud);
    if (speed == B0)
    {
        syslog(LOG_ERR, "uart: set_baudrate: invalid baudrate: %i", baud);
        return -1;
    }
    cfsetispeed(&termio, speed);
    cfsetospeed(&termio, speed);

    // make it so
    if (tcsetattr(fd, TCSAFLUSH, &termio) < 0) {
        syslog(LOG_ERR, "uart: set_baudrate: tcsetattr() failed: %s", strerror(errno));
        return -1;
    }
    return 0;
}

int lv_8ms_uart_set_mode(int fd, int bytesize, lv_8ms_uart_parity_t parity, int stopbits)
{
    struct termios termio;
    if (tcgetattr(fd, &termio)) {
        syslog(LOG_ERR, "uart: set_mode: tcgetattr() failed: %s", strerror(errno));
        return -1;
    }

    termio.c_cflag &= ~CSIZE;
    switch (bytesize) {
        case 8:
            termio.c_cflag |= CS8;
            break;
        case 7:
            termio.c_cflag |= CS7;
            break;
        case 6:
            termio.c_cflag |= CS6;
            break;
        case 5:
            termio.c_cflag |= CS5;
            break;
        default:
            termio.c_cflag |= CS8;
            break;
    }

    // POSIX & linux doesn't support 1.5 and I've got bigger fish to fry
    switch (stopbits) {
        case 1:
            termio.c_cflag &= ~CSTOPB;
            break;
        case 2:
            termio.c_cflag |= CSTOPB;
        default:
            break;
    }

    switch (parity) {
        case LV_8MS_UART_PARITY_NONE:
            termio.c_cflag &= ~(PARENB | PARODD);
            break;
        case LV_8MS_UART_PARITY_EVEN:
            termio.c_cflag |= PARENB;
            termio.c_cflag &= ~PARODD;
            break;
        case LV_8MS_UART_PARITY_ODD:
            termio.c_cflag |= PARENB | PARODD;
            break;
        case LV_8MS_UART_PARITY_MARK: // not POSIX
            termio.c_cflag |= PARENB | CMSPAR | PARODD;
            break;
        case LV_8MS_UART_PARITY_SPACE: // not POSIX
            termio.c_cflag |= PARENB | CMSPAR;
            termio.c_cflag &= ~PARODD;
            break;
    }

    if (tcsetattr(fd, TCSAFLUSH, &termio) < 0) {
        syslog(LOG_ERR, "uart: set_mode: tcsetattr() failed: %s", strerror(errno));
        return -1;
    }

    return 0;
}

int lv_8ms_uart_set_flowcontrol(int fd, int xonxoff, int rtscts)
{
    // hardware flow control
    int action = TCIOFF;
    if (xonxoff) {
        action = TCION;
    }
    if (tcflow(fd, action)) {
        return -1;
    }

    // rtscts
    struct termios termio;

    // get current modes
    if (tcgetattr(fd, &termio)) {
        syslog(LOG_ERR, "uart: set_flowcontrol: tcgetattr() failed: %s", strerror(errno));
         return -1;
    }

    if (rtscts) {
        termio.c_cflag |= CRTSCTS;
    } else {
        termio.c_cflag &= ~CRTSCTS;
    }

    if (tcsetattr(fd, TCSAFLUSH, &termio) < 0) {
        syslog(LOG_ERR, "uart: set_flowcontrol: tcsetattr() failed: %s", strerror(errno));
        return -1;
    }

    return 0;
}

int lv_8ms_uart_set_timeout(int fd, int readtimeout)
{
    struct termios termio;
    // get current modes
    if (tcgetattr(fd, &termio)) {
        syslog(LOG_ERR, "uart: set_timeout: tcgetattr() failed: %s", strerror(errno));
        return -1;
    }
    if (readtimeout > 0) {
        readtimeout = readtimeout / 100;
        if (readtimeout == 0)
            readtimeout = 1;
    }
    termio.c_lflag &= ~ICANON; /* Set non-canonical mode */
    termio.c_cc[VTIME] = readtimeout; /* Set timeout in tenth seconds */
    if (tcsetattr(fd, TCSANOW, &termio) < 0) {
        syslog(LOG_ERR, "uart: set_timeout: tcsetattr() failed: %s", strerror(errno));
        return -1;
    }

    return 0;
}

int lv_8ms_uart_set_non_blocking(int fd, int nonblock)
{
    // get current flags
    int flags = fcntl(fd, F_GETFL);

    // update flags with new blocking state according to nonblock bool
    if (nonblock) {
        flags |= O_NONBLOCK;
    } else {
        flags &= ~O_NONBLOCK;
    }

    // set new flags
    if (fcntl(fd, F_SETFL, flags) < 0) {
        syslog(LOG_ERR, "uart: non_blocking: failed changing fd blocking state: %s", strerror(errno));
        return -1;
    }

    return 0;
}


void lv_8ms_uart_uninit(int fd)
{
	if (fd > 0)
		close(fd);
}

int lv_8ms_uart_write(int fd, const char* buf, size_t len)
{
	if (fd <= 0)
		return -1;

	return write(fd, buf, len);
}

int lv_8ms_uart_read(int fd, char* buf, size_t len)
{
	if (fd <= 0)
		return -1;

	return read(fd, buf, len);
}

int lv_8ms_uart_data_available(int fd, unsigned int millis)
{
    struct timeval timeout;

    if (millis == 0) {
        // no waiting
        timeout.tv_sec = 0;
        timeout.tv_usec = 0;
    } else {
        timeout.tv_sec = millis / 1000;
        timeout.tv_usec = (millis % 1000) * 1000;
    }

    fd_set readfds;

    FD_ZERO(&readfds);
    FD_SET(fd, &readfds);

    if (select(fd + 1, &readfds, NULL, NULL, &timeout) > 0) {
        return 1; // data is ready
    } else {
        return 0;
    }
}
