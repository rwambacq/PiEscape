#ifdef RPI
#include "real_sensors_system.h"
#include "../../sensor/lps25h.h"
#include "../../sensor/hts221.h"
#include "../../sensor/i2c.h"

#include <stdlib.h>
#include <stdio.h>

#include <assert.h>

RealSensorsSystem* system_real_sensors_alloc() {
    RealSensorsSystem* res = calloc(1, sizeof(RealSensorsSystem));
    system_real_sensors_init(res);
    return res;
}

void system_real_sensors_init(RealSensorsSystem* system) {
	lps25h_init(1);
	hts221_init(1);
	system->rpiTemperature = lps25h_read_temperature();
	system->rpiAirPressure = lps25h_read_pressure();
	system->rpiHumidity = hts221_read_humidity();
}


void system_real_sensors_free(RealSensorsSystem* system) {
    //TODO
}

void system_real_sensors_update(RealSensorsSystem* system, Engine* engine) {
	system->rpiAirPressure = lps25h_read_pressure();
	system->rpiTemperature = lps25h_read_temperature();
	system->rpiHumidity = hts221_read_humidity();
}
#endif