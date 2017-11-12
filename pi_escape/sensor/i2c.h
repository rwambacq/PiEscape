#ifdef RPI
#ifndef I2C
#define I2C

#include <stdlib.h>
#ifdef RPI
#include <sys/ioctl.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#endif
#include <fcntl.h>
#include <stdint.h>


//TODO

int i2c_init_adapter(int addr);
int i2c_write_byte_data(int file, uint8_t reg, uint8_t data);
int i2c_read_byte_data(int file, uint8_t reg);

#endif /* I2C */
#endif