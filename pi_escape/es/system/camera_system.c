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

	if (cameraLookFrom->prev_XY != cameraLookFrom->XYdegees || cameraLookFrom->prev_Z != cameraLookFrom->Zdegrees || cameraLookFrom->prev_dist != cameraLookFrom->distance) {
		EntityIterator player_it;
		search_entity_2(engine, COMP_GRIDLOCATION, COMP_INPUTRECEIVER, &player_it);
		next_entity(&player_it);
		EntityId player_entity_id = player_it.entity_id;
		assert(player_entity_id != NO_ENTITY);

		GridLocationComponent* player_grid_comp = get_component(engine, player_entity_id, COMP_GRIDLOCATION);

		float xy_degrees = cameraLookFrom->XYdegees;
		float z_degrees = cameraLookFrom->Zdegrees;

		float xy_radians = xy_degrees*(M_PI / 180);
		float z_radians = z_degrees*(M_PI / 180);

		float dist = cameraLookFrom->distance;

		float player_x = player_grid_comp->pos[0];
		float player_y = player_grid_comp->pos[1];
		float player_z = 0.0;

		glmc_vec3_set(cameraLookAt->pos, player_x, player_y, player_z);

		float camera_x;
		float camera_y;
		float camera_z;

		camera_x = player_x + (dist * cos(xy_radians) * sin(z_radians));
		camera_y = player_y + (dist * sin(xy_radians) * sin(z_radians));
		camera_z = player_z + (dist * cos(z_radians));

		glmc_vec3_set(cameraLookFrom->pos, camera_x, camera_y, camera_z);

		cameraLookFrom->prev_dist = cameraLookFrom->distance;
		cameraLookFrom->prev_XY = cameraLookFrom->XYdegees;
		cameraLookFrom->prev_Z = cameraLookFrom->Zdegrees;
	}
}