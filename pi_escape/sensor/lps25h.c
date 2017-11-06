#include <lps25h.h>
#include <stdint.h>

int file;

int lps25h_init(int frequentie) {
	uint8_t initVal;
	file = i2c_init_adapter(0x5c);
	if (frequentie == 1) {
		initVal = 0x90;
	}
	else if (frequentie > 1 && frequentie <= 7) {
		initVal = 0xA0;
	}
	else if (frequentie > 7 && frequentie <= 13) {
		initVal = 0xB0;
	}
	else
	{
		initVal = 0xC0;
	}
	i2c_write_byte_data(file, 0x20, initVal);
	return(0);
}

int lps25h_read_pressure(){
	int32_t PressOut;
	uint8_t PressOutXl = i2c_read_byte_data(file, 0x28);
	uint8_t PressOutL = i2c_read_byte_data(file, 0x29);
	uint8_t PressOutH = i2c_read_byte_data(file, 0x2A);
	if (PressOutH / 128 == 1) {
		PressOut = (-8388608 + (PressOutXl + PressOutL * 256 + (PressOutH % 128) * 65536)) / 4096;
	}
	else {
		PressOut = (PressOutXl + PressOutL * 256 + (PressOutH % 128) * 65536) / 4096;
	}
	return(PressOut);
}

int lps25h_read_temperature() {
	int16_t TempOut;
	uint8_t TempOutL = i2c_read_byte_data(file, 0x2B);
	uint8_t TempOutH = i2c_read_byte_data(file, 0x2C);
	if (TempOutH / 128 == 1) {
		TempOut = 42 + (-32768 + (TempOutL + (TempOutH % 128) * 256)) / 480;
	}
	else {
		TempOut = 42 + (TempOutL + (TempOutH % 128) * 256);
	}
	return(TempOut);
}
	