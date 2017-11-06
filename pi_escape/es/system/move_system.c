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

	printf("Has blocking: %d\n", has_component(engine, player_entity_id, COMP_BLOCKING));
	if (!has_component(engine, player_entity_id, COMP_BLOCKING)) {
		Level level = engine->level;
		if (!has_component(engine, player_entity_id, COMP_MOVE_ANIMATION)) {
			if (has_component(engine, player_entity_id, COMP_MOVE_ACTION)) {
				MoveActionComponent* move = get_component(engine, player_entity_id, COMP_MOVE_ACTION);
				GridLocationComponent* loc = get_component(engine, player_entity_id, COMP_GRIDLOCATION);
				int player_pos_x = loc->pos[0];
				int player_pos_y = loc->pos[1];
				EntityIterator itdoor;
				search_entity_3(engine, COMP_ART, COMP_ISDOOR, COMP_DIRECTION, &itdoor);



				if (move->x_min_move) {
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



				else if (move->y_plus_move) {
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



				free_component(engine, player_entity_id, COMP_MOVE_ACTION);
			}
		}
		else if (has_component(engine, player_entity_id, COMP_MOVE_ACTION)) {
			free_component(engine, player_entity_id, COMP_MOVE_ACTION);
		}
	}
	else {
		if (has_component(engine, player_entity_id, COMP_MOVE_ACTION)) {
			free_component(engine, player_entity_id, COMP_MOVE_ACTION);
		}
	}
}
