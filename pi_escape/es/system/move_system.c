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
	if (!has_component(engine, player_entity_id, COMP_MOVE_ANIMATION)) {
		if (has_component(engine, player_entity_id, COMP_MOVE_ACTION)) {
			MoveActionComponent* move = get_component(engine, player_entity_id, COMP_MOVE_ACTION);
			GridLocationComponent* loc = get_component(engine, player_entity_id, COMP_GRIDLOCATION);

			int player_pos_x = loc->pos[0];
			int player_pos_y = loc->pos[1];

			if ( move->x_min_move ) {
				char object_left = level.level_description[player_pos_x - 1][player_pos_y];
				if (object_left != 'W' && object_left != 'D' && (!player_pos_x - 1 < 0)) {
					loc->pos[0] -= 1;
				}
				move->x_min_move = 0;
			}
			else if ( move->x_plus_move ) {
				char object_right = level.level_description[player_pos_x + 1][player_pos_y];
				printf("%d",player_pos_x);
				if (object_right != 'W' && object_right != 'D' && player_pos_x + 1 < level.hoogte) {
					loc->pos[0] += 1;
				}

				move->x_plus_move = 0;
			}
			else if ( move->y_min_move ) {
				printf("right");
				char object_under = level.level_description[player_pos_x][player_pos_y + 1];
				if (object_under != 'W' && object_under != 'D' && player_pos_y + 1 < level.breedte) {
					loc->pos[1] += 1;
				}
				move->y_min_move = 0;
			}
			else if ( move->y_plus_move ) {
				printf("left");
				char object_above = level.level_description[player_pos_x][player_pos_y - 1];
				if (object_above != 'W' && object_above != 'D' && player_pos_y - 1 >= 0) {
					loc->pos[1] -= 1;
				}
				move->x_plus_move = 0;
			}
			free_component(engine, player_entity_id, COMP_MOVE_ACTION);
		}
	} else if (has_component(engine, player_entity_id, COMP_MOVE_ACTION)) {
		free_component(engine, player_entity_id, COMP_MOVE_ACTION);
	}
}
