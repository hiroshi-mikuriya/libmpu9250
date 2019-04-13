#include "spi.h"
#include <linux/spi/spidev.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>

#define SPEED_HZ		2 * 1000 * 1000
#define BITS_PER_WORD	8

static int s_fd = 0;

int spi_init(void)
{
	spi_deinit();
	const char * dev = "/dev/spidev0.0";
	int res = open(dev, O_RDWR);
	if (res < 0) {
		fprintf(stderr, "error open(%s) %s\n", dev, strerror(errno));
		return errno;
	}
	s_fd = res;
	return 0;
}

int spi_deinit(void)
{
	if (s_fd != 0 && close(s_fd) < 0) {
		fprintf(stderr, "error close %s\n", strerror(errno));
		return errno;
	}
	s_fd = 0;
	return 0;
}

int spi_write(uint8_t * buf, uint16_t len, int cs)
{
	struct spi_ioc_transfer mesgs[] = {
		{
			.tx_buf = (unsigned long)buf,
			.len = len,
			.speed_hz = SPEED_HZ,
			.bits_per_word = BITS_PER_WORD,
			.cs_change = cs,
		}
	};
	if (ioctl(s_fd, SPI_IOC_MESSAGE(1), &mesgs) < 0) {
		fprintf(stderr, "error ioctl(SPI_IOC_MESSAGE) %s\n", strerror(errno));
		return errno;
	}
	return 0;
}

int spi_read(uint8_t * buf, uint16_t len, int cs)
{
	struct spi_ioc_transfer mesgs[] = {
		{
			.rx_buf = (unsigned long)buf,
			.len = len,
			.speed_hz = SPEED_HZ,
			.bits_per_word = BITS_PER_WORD,
			.cs_change = cs,
		}
	};
	if (ioctl(s_fd, SPI_IOC_MESSAGE(1), &mesgs) < 0) {
		fprintf(stderr, "error ioctl(SPI_IOC_MESSAGE) %s\n", strerror(errno));
		return errno;
	}
	return 0;
}

int spi_write_read(uint8_t * wbuf, uint16_t wlen, uint8_t * rbuf, uint16_t rlen, int cs)
{
	struct spi_ioc_transfer mesgs[] = {
		{
			.tx_buf = (unsigned long)wbuf,
			.len = wlen,
			.speed_hz = SPEED_HZ,
			.bits_per_word = BITS_PER_WORD,
			.cs_change = cs,
		},
		{
			.rx_buf = (unsigned long)rbuf,
			.len = rlen,
			.speed_hz = SPEED_HZ,
			.bits_per_word = BITS_PER_WORD,
			.cs_change = cs,
		}
	};
	if (ioctl(s_fd, SPI_IOC_MESSAGE(2), &mesgs) < 0) {
		fprintf(stderr, "error ioctl(SPI_IOC_MESSAGE) %s\n", strerror(errno));
		return errno;
	}
	return 0;
}