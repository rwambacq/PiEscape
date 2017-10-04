#ifndef PROCESS_SENSOR_SYSTEM_H
#define PROCESS_SENSOR_SYSTEM_H
#define TO_IMPLEMENT_STRUCT char c


#include <stdint.h>

#include "../../../util/rgb_triple.h"

typedef struct ProcessSensorSystem {
	TO_IMPLEMENT_STRUCT;
    //TODO
} ProcessSensorSystem;

#include "../engine.h"

void system_process_sensor_init(ProcessSensorSystem*);
ProcessSensorSystem* system_process_sensor_alloc();
void system_process_sensor_update(ProcessSensorSystem*, Engine*);
void system_process_sensor_free(ProcessSensorSystem*);


#endif //PROCESS_SENSOR_SYSTEM_H