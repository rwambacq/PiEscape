#include "process_sensor_system.h"
#include "../../../util/rgb_triple.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>

#define PI 3.14159265

ProcessSensorSystem* system_process_sensor_alloc() {
    ProcessSensorSystem* res = calloc(1, sizeof(ProcessSensorSystem));
    system_process_sensor_init(res);
    return res;
}

void system_process_sensor_init(ProcessSensorSystem* system) {
    //standaard condities
	system->humidity = 35;
	system->temperature = 15;
	system->airPressure = 1013;
	system->humidityColor = calloc(1, sizeof(system->humidityColor));
	system->temperatureColor = calloc(1, sizeof(system->temperatureColor));
	system->airPressureColor = calloc(1, sizeof(system->airPressureColor));
}

void system_process_sensor_free(ProcessSensorSystem* system) {
    //TODO
    
}

void system_process_sensor_update(ProcessSensorSystem* system, Engine* engine) {
#ifdef RPI 
	system->humidity = engine->real_sensors_system->rpiHumidity;
	system->temperature = engine->real_sensors_system->rpiTemperature;
	system->airPressure = engine->real_sensors_system->rpiAirPressure;
#endif
	system->humidityColor->rgbRed = (unsigned char)(sin(system->humidity / 30 * 2 * PI) * 127 + 128);
	system->humidityColor->rgbGreen = (unsigned char)(sin(system->humidity / 30 * 2 * PI + PI / 3) * 127 + 128);
	system->humidityColor->rgbBlue = (unsigned char)(sin(system->humidity / 30 * 2 * PI + 2 * PI / 3) * 127 + 128);

	system->temperatureColor->rgbRed = (unsigned char)(sin(system->temperature/30 * 2 * PI) * 127 + 128);
	system->temperatureColor->rgbGreen = (unsigned char)(sin(system->temperature / 30 * 2 * PI + PI / 3) * 127 + 128);
	system->temperatureColor->rgbBlue = (unsigned char)(sin(system->temperature / 30 * 2 * PI + 2 * PI / 3) * 127 + 128);

	system->airPressureColor->rgbRed = (unsigned char)(sin(system->airPressure / 30 * 2 * PI) * 127 + 128);
	system->airPressureColor->rgbGreen = (unsigned char)(sin(system->airPressure / 30 * 2 * PI + PI / 3) * 127 + 128);
	system->airPressureColor->rgbBlue = (unsigned char)(sin(system->airPressure / 30 * 2 * PI + 2 * PI / 3) * 127 + 128);
}