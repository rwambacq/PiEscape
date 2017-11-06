#include <hts221.h>
#include <stdint.h>

int file;
int32_t H0T0OUT;
int32_t H1T0OUT;
uint16_t H0rH;
uint16_t H1rH;
int32_t T0OUT;
int32_t T1OUT;
uint16_t T0degC;
uint16_t T1degC;



int hts221_init(int frequentie) {
	uint8_t initVal;
	file = i2c_init_adapter(0x5f);
	if (frequentie == 1) {
		initVal = 0x81;
	}
	else if (frequentie > 1 && frequentie <= 7){
		initVal = 0x82;
	}
	else{
		initVal = 0x83;
	}
	i2c_write_byte_data(file, 0x20, initVal);
	//humidity
	H0rH = i2c_read_byte_data(file, 0x30)/2;
	H1rH = i2c_read_byte_data(file, 0x31)/2;
	uint16_t H0T0OUT1 = i2c_read_byte_data(file, 0x36);
	uint16_t H0T0OUT2 = i2c_read_byte_data(file, 0x37);
	if (H0T0OUT2 / 128 == 1) {
		H0T0OUT = -32768 + (H0T0OUT1 + (H0T0OUT2 % 128) * 256);
	}
	else {
		H0T0OUT = (H0T0OUT1 + (H0T0OUT2 % 128)*256);
	}
	uint16_t H1T0OUT1 = i2c_read_byte_data(file, 0x3a);
	uint16_t H1T0OUT2 = i2c_read_byte_data(file, 0x3b);
	if (H1T0OUT2 / 128 == 1) {
		H1T0OUT = -32768 + (H1T0OUT1 + (H1T0OUT2 % 128) * 256);
	}
	else {
		H1T0OUT = (H1T0OUT1 + (H1T0OUT2 % 128) * 256);
	}
	//temperature
	uint16_t T0degCx8 = i2c_read_byte_data(file, 0x32);
	uint16_t T1degCx8 = i2c_read_byte_data(file, 0x33);
	uint16_t MSBdegC = i2c_read_byte_data(file, 0x35);
	T0degC = (T0degCx8 + (MSBdegC % 4) * 256) / 8;
	T1degC = (T1degCx8 + ((MSBdegC % 16)/4) * 256) / 8;
	uint16_t T0OUT1 = i2c_read_byte_data(file, 0x3C);
	uint16_t T0OUT2 = i2c_read_byte_data(file, 0x3D);
	if (T0OUT2 / 128 == 1) {
		T0OUT = -32768 + (T0OUT1 + (T0OUT2 % 128) * 256);
	}
	else {
		T0OUT = (T0OUT1 + (T0OUT2 % 128) * 256);
	}
	uint16_t T1OUT1 = i2c_read_byte_data(file, 0x3E);
	uint16_t T1OUT2 = i2c_read_byte_data(file, 0x3F);
	if (T1OUT2 / 128 == 1) {
		T1OUT = -32768 + (T1OUT1 + (T1OUT2 % 128) * 256);
	}
	else {
		T1OUT = (T1OUT1 + (T1OUT2 % 128) * 256);
	}
	return(0);
}

int hts221_read_temperature(){
	int32_t TOUT;
	uint16_t TOUT1 = i2c_read_byte_data(file, 0x2A);
	uint16_t TOUT2 = i2c_read_byte_data(file, 0x2B);
	if (TOUT2 / 128 == 1) {
		TOUT = -32768 + (TOUT1 + (TOUT2 % 128) * 256);
	}
	else {
		TOUT = (TOUT1 + (TOUT2 % 128) * 256);
	}
	int32_t temp = ((T1degC - T0degC) * (TOUT - T0OUT)) / (T1OUT - T0OUT) + T0degC;
	return(temp);
}

int hts221_read_humidity() {
	int32_t HOUT;
	uint16_t HOUT1 = i2c_read_byte_data(file, 0x28);
	uint16_t HOUT2 = i2c_read_byte_data(file, 0x29);
	if (HOUT2 / 128 == 1) {
		HOUT = -32768 + (HOUT1 + (HOUT2 % 128) * 256);
	}
	else {
		HOUT = (HOUT1 + (HOUT2 % 128) * 256);
	}
	int32_t humid = ((H1rH - H0rH)* (HOUT - H0T0OUT)) / (H1T0OUT - H0T0OUT) + H0rH;
	if (humid > 100) {
		humid = 100;
	}
	return(humid);
}