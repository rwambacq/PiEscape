#ifndef ORIENTATION_SYSTEM_H
#define ORIENTATION_SYSTEM_H

#include "../../imu/RTIMU_c_wrapper.h"
#include <stdint.h>

typedef struct OrientationSystem {
	float sensor_x;
	float sensor_y;
	float sensor_z;
} OrientationSystem;

#include "../engine.h"

void system_orientation_init(OrientationSystem*);
OrientationSystem* system_orientation_alloc();
void system_orientation_update(OrientationSystem*, Engine*);
void system_orientation_free(OrientationSystem*);

#endif //ORIENTATION_SYSTEM_H
