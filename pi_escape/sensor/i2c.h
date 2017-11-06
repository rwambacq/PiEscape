#ifndef I2C
#define I2C

//TODO

int i2c_init_adapter(int addr);
int i2c_write_byte_data(int file, uint8_t reg, uint8_t data);
int i2c_read_byte_data(int file, uint8_t reg);

#endif /* I2C */