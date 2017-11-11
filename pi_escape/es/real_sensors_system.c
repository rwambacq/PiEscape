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
    //TODO
}


void system_real_sensors_free(RealSensorsSystem* system) {
    //TODO
}

void system_real_sensors_update(RealSensorsSystem* system, Engine* engine) {
    //TODO
}
