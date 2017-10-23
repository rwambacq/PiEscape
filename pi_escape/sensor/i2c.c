#include <i2c.h>
#include <linux/i2c-dev.h>
#include <fcntl.h>
#include <stdint.h>


int i2c_init_adapter(int addr) {
	int file;
	char filename[] = "/dev/i2c-1";
	file = open(filename, O_RDWR);
	if (file < 0) {
		exit(1);
	}
	if (ioctl(file, I2C_SLAVE, addr) < 0) {
		exit(1);
	}
	return(file);
}

int i2c_write_byte_data(int file, uint8_t reg, uint8_t data) {
	char buf[2];
	buf[0] = reg;
	buf[1] = data;
	if (write(file, buf, 2) != 2) {
		// ERROR HANDLING; you can check errno to see what went wrong
		exit(1);
	}
	exit(0);
}

int i2c_read_byte_data(int file, uint8_t reg) {
	char buf[1];
	buf[0] = reg;
	if (write(file, buf, 1) != 1) {
		// ERROR HANDLING; you can check errno to see what went wrong
		exit(1);
	}
	if (read(file, buf, 1) != 1) {
		// ERROR HANDLING; you can check errno to see what went wrong
		exit(1);
	}
	//buf[0] now contains the value of the register at 0x20
	return(buf[0]);
}