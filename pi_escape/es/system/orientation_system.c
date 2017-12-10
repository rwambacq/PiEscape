#include "orientation_system.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <math.h>

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
	CameraLookFromComponent* cameraLookFrom = search_first_component(engine, COMP_CAMERA_LOOK_FROM);
	float value_x = get_imu_x();
	float value_y = get_imu_y();
	float value_z = get_imu_z();
	if (system->sensor_x && system->sensor_y && system->sensor_z) {
		cameraLookFrom->XYdegees = fmod((cameraLookFrom->XYdegees - (value_z - system->sensor_z) * 180 / M_PI), 360);

		if (cameraLookFrom->Zdegrees >= 0.0f && cameraLookFrom->Zdegrees <= 90.0f) {
			cameraLookFrom->Zdegrees -= (value_y - system->sensor_y) * 180 / M_PI;
		}
	}
	system->sensor_x = value_x;
	system->sensor_y = value_y;
	system->sensor_z = value_z;
}
