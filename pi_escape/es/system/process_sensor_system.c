#include "process_sensor_system.h"
#include "../../../util/rgb_triple.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>

ProcessSensorSystem* system_process_sensor_alloc() {
    ProcessSensorSystem* res = calloc(1, sizeof(ProcessSensorSystem));
    system_process_sensor_init(res);
    return res;
}

void system_process_sensor_init(ProcessSensorSystem* system) {
    //TODO
}

void system_process_sensor_free(ProcessSensorSystem* system) {
    //TODO
    
}

void system_process_sensor_update(ProcessSensorSystem* system, Engine* engine) {
    //TODO
}