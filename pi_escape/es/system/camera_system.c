#include "camera_system.h"


#include <stdlib.h>
#include <assert.h>

#include <stdio.h>
#include <math.h>

CameraSystem* system_camera_alloc() {
    CameraSystem* res = calloc(1, sizeof(CameraSystem));
    system_camera_init(res);
    return res;
}

void system_camera_init(CameraSystem* system) {
    //TODO
}


void system_camera_free(CameraSystem* system) {
    //TODO
}

void system_camera_update(CameraSystem* system, Engine* engine) {

	CameraLookFromComponent* cameraLookFrom = search_first_component(engine, COMP_CAMERA_LOOK_FROM);
	CameraLookAtComponent* cameraLookAt = search_first_component(engine, COMP_CAMERA_LOOK_AT);
	MoveActionComponent* movement = search_first_component(engine, COMP_MOVE_ACTION);

	float xy_degrees = cameraLookFrom->XYdegees;
	float z_degrees = cameraLookFrom->Zdegrees;

	float xy_radians = xy_degrees*(M_PI / 180);
	float z_radians = z_degrees*(M_PI / 180);

	float dist = cameraLookFrom->distance;

	float player_x = movement->player_x;
	float player_y = movement->player_y;
	float player_z = movement->player_z;

	float camera_x;
	float camera_y;
	float camera_z;

	printf("xy rad: %f\n", xy_radians);
	printf("z rad: %f\n", z_radians);

	camera_x = player_x + (dist * cos(xy_radians) * sin(z_radians));
	camera_y = player_y + (dist * sin(xy_radians) * sin(z_radians));
	camera_z = player_z + (dist * cos(z_radians));

	printf("X: %f\n", camera_x);
	printf("Y: %f\n", camera_y);
	printf("Z: %f\n", camera_z);

	glmc_vec3_set(cameraLookFrom->pos, camera_x, camera_y, camera_z);
}
