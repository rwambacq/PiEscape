#include "orientation_system.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

OrientationSystem* system_orientation_alloc() {
    OrientationSystem* res = calloc(1, sizeof(OrientationSystem));
    system_orientation_init(res);
    return res;
}

void system_orientation_init(OrientationSystem* system) {
	sensor_imu_init();
}


void system_orientation_free(OrientationSystem* system) {
    //TODO
}

void system_orientation_update(OrientationSystem* system, Engine* engine) {
	float value_x = get_imu_x();
	float value_y = get_imu_y();
	float value_z = get_imu_z();
	printf("imu %f %f %f/n", value_x, value_y, value_z);
}
