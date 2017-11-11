#include "move_system.h"

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

MoveSystem* system_move_alloc() {
    MoveSystem* res = calloc(1, sizeof(MoveSystem));
    system_move_init(res);
    return res;
}

void system_move_init(MoveSystem* system) {
    //TODO
}


void system_move_free(MoveSystem* system) {
    //TODO
}

void system_move_update(MoveSystem* system, Engine* engine) {
	EntityIterator player_it;
	search_entity_2(engine, COMP_GRIDLOCATION, COMP_INPUTRECEIVER, &player_it);
	next_entity(&player_it);
	EntityId player_entity_id = player_it.entity_id;
	assert(player_entity_id != NO_ENTITY);
	Level level = engine->level;

		if (has_component(engine, player_entity_id, COMP_MOVE_ACTION)) {
			MoveActionComponent* move = get_component(engine, player_entity_id, COMP_MOVE_ACTION);
			GridLocationComponent* loc = get_component(engine, player_entity_id, COMP_GRIDLOCATION);
			int player_pos_x = loc->pos[0];
			int player_pos_y = loc->pos[1];
			EntityIterator itdoor;
			search_entity_3(engine, COMP_ART, COMP_ISDOOR, COMP_DIRECTION, &itdoor);

			if ( move->x_min_move ) {
				EntityId doorused = NULL;
				while (next_entity(&itdoor)) {
					EntityId door = itdoor.entity_id;
					assert(door != NO_ENTITY);
					GridLocationComponent* door_pos = get_component(engine, door, COMP_GRIDLOCATION);
					if (door_pos->pos[1] == player_pos_y && door_pos->pos[0] == player_pos_x - 1) {
						doorused = door;
					}
				}
				char object_left = level.level_description[player_pos_x - 1][player_pos_y];
				if (object_left != 'W' && (!player_pos_x - 1 < 0)) {
					if (doorused != NULL) {
						ActivatableComponent* door_act = get_component(engine, doorused, COMP_ACTIVATABLE);
						int x = door_act->active;
						if (x == 1) {
							loc->pos[0] -= 1;
						}
					}
					else {
						loc->pos[0] -= 1;
					}
				}
				move->x_min_move = 0;
			}





			else if (move->x_plus_move) {
				EntityId doorused = NULL;
				while (next_entity(&itdoor)) {
					EntityId door = itdoor.entity_id;
					assert(door != NO_ENTITY);
					GridLocationComponent* door_pos = get_component(engine, door, COMP_GRIDLOCATION);
					if (door_pos->pos[1] == player_pos_y && door_pos->pos[0] == player_pos_x + 1) {
						doorused = door;
					}
				}
				char object_right = level.level_description[player_pos_x + 1][player_pos_y];
				if (object_right != 'W' &&  player_pos_x + 1 < level.hoogte) {
					if (doorused != NULL) {
						ActivatableComponent* door_act = get_component(engine, doorused, COMP_ACTIVATABLE);
						int x = door_act->active;
						if (x == 1) {
							loc->pos[0] += 1;
						}
					}
					else {
						loc->pos[0] += 1;
					}
				}
				move->x_plus_move = 0;
			}
			else if (move->y_min_move) {
				EntityId doorused = NULL;
				while (next_entity(&itdoor)) {
					EntityId door = itdoor.entity_id;
					assert(door != NO_ENTITY);
					GridLocationComponent* door_pos = get_component(engine, door, COMP_GRIDLOCATION);
					if (door_pos->pos[1] == player_pos_y + 1 && door_pos->pos[0] == player_pos_x) {
						doorused = door;
					}
				}
				char object_under = level.level_description[player_pos_x][player_pos_y + 1];
				if (object_under != 'W' && player_pos_y + 1 < level.breedte) {
					if (doorused != NULL) {
						ActivatableComponent* door_act = get_component(engine, doorused, COMP_ACTIVATABLE);
						int x = door_act->active;
						if (x == 1) {
							loc->pos[1] += 1;
						}
					}
					else {
						loc->pos[1] += 1;
					}
				}
				move->y_min_move = 0;
			}



			else if ( move->y_plus_move ) {
				EntityId doorused = NULL;
				while (next_entity(&itdoor)) {
					EntityId door = itdoor.entity_id;
					assert(door != NO_ENTITY);
					GridLocationComponent* door_pos = get_component(engine, door, COMP_GRIDLOCATION);
					if (door_pos->pos[1] == player_pos_y - 1 && door_pos->pos[0] == player_pos_x) {
						doorused = door;
					}
				}
				char object_above = level.level_description[player_pos_x][player_pos_y - 1];
				if (object_above != 'W'  && player_pos_y - 1 >= 0) {
					if (doorused != NULL) {
						ActivatableComponent* door_act = get_component(engine, doorused, COMP_ACTIVATABLE);
						int x = door_act->active;
						if (x == 1) {
							loc->pos[1] -= 1;
						}
					}
					else {
						loc->pos[1] -= 1;
					}
				}
				move->y_plus_move = 0;
			}


			create_component(engine, player_entity_id, COMP_MOVE_ANIMATION);
			free_component(engine, player_entity_id, COMP_MOVE_ACTION);
		}
	if (has_component(engine, player_entity_id, COMP_MOVE_ACTION)) {
		free_component(engine, player_entity_id, COMP_MOVE_ACTION);
	}

	//UPDATE CAMERA AT PLAYER MOVEMENT

	CameraLookFromComponent* cameraLookFrom = search_first_component(engine, COMP_CAMERA_LOOK_FROM);
	CameraLookAtComponent* cameraLookAt = search_first_component(engine, COMP_CAMERA_LOOK_AT);

	GridLocationComponent* player_grid_comp = get_component(engine, player_entity_id, COMP_GRIDLOCATION);

	float xy_degrees = cameraLookFrom->XYdegees;
	float z_degrees = cameraLookFrom->Zdegrees;

	float xy_radians = xy_degrees*(M_PI / 180);
	float z_radians = z_degrees*(M_PI / 180);

	float dist = cameraLookFrom->distance;

	float player_x = player_grid_comp->pos[0];
	float player_y = player_grid_comp->pos[1];
	float player_z = 0.0;

	float camera_x;
	float camera_y;
	float camera_z;

	camera_x = player_x + (dist * cos(xy_radians) * sin(z_radians));
	camera_y = player_y + (dist * sin(xy_radians) * sin(z_radians));
	camera_z = player_z + (dist * cos(z_radians));

	//CAMERALOOKFROM

	if (cameraLookFrom->pos[0] < camera_x - 0.005) {
		cameraLookFrom->pos[0] += 0.005;
	}
	else if (cameraLookFrom->pos[0] > camera_x + 0.005) {
		cameraLookFrom->pos[0] -= 0.005;
	}

	if (cameraLookFrom->pos[1] < camera_y - 0.005) {
		cameraLookFrom->pos[1] += 0.005;
	}
	else if (cameraLookFrom->pos[1] > camera_y + 0.005) {
		cameraLookFrom->pos[1] -= 0.005;
	}

	if (cameraLookFrom->pos[2] < camera_z - 0.005) {
		cameraLookFrom->pos[2] += 0.005;
	}
	else if (cameraLookFrom->pos[2] > camera_z + 0.005) {
		cameraLookFrom->pos[2] -= 0.005;
	}

	//CAMERALOOKAT

	if (cameraLookAt->pos[0] < player_x - 0.005) {
		cameraLookAt->pos[0] += 0.005;
	}
	else if (cameraLookAt->pos[0] > player_x + 0.005) {
		cameraLookAt->pos[0] -= 0.005;
	}

	if (cameraLookAt->pos[1] < player_y - 0.005) {
		cameraLookAt->pos[1] += 0.005;
	}
	else if (cameraLookAt->pos[1] > player_y + 0.005) {
		cameraLookAt->pos[1] -= 0.005;
	}

	if (cameraLookAt->pos[2] < player_z - 0.005) {
		cameraLookAt->pos[2] += 0.005;
	}
	else if (cameraLookAt->pos[2] > player_z + 0.005) {
		cameraLookAt->pos[2] -= 0.005;
	}
}
