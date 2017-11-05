#include "game.h"

#include <stdlib.h>

void game_init(Game* game, Graphics* graphics) {
    game->graphics = graphics;
    engine_init(&game->engine, graphics);
}

Game* game_alloc(Graphics* graphics) {
    Game* res = malloc(sizeof(Game));
    game_init(res, graphics);
    return res;
}

void game_free(Game* game) {
    engine_free(&game->engine);
    //don't free graphics
}

void game_load_level(Game* g, Level* l) {

	Engine* engine = &g->engine;
	
	engine->level = *l;

	int width = l->breedte;
	int height = l->hoogte;

	int x;
	int y;

	for (x = 0; x < height; x++) {
		for (y = 0; y < width; y++) {

			char current_char = l->level_description[x][y];

			int has_door = (current_char == 'D');
			int has_exit = (current_char == 'E');
			int has_connector = current_char == '-';
			int has_conAnd = current_char == '&';
			int has_conOr = current_char == '|';
			int has_floor = (current_char != 'W' && current_char != 'E' && current_char != 'D');
			int has_ceil = (!has_floor && !has_door && !has_exit && !has_connector);
			int has_key = (current_char == 'a' || current_char == 'b' || current_char == 'c' || current_char == 'o');
			int has_player = (current_char == 'P');
			int has_lock = (current_char == 'A' || current_char == 'B' || current_char == 'C' || current_char == 'O');
	

			EntityId entity_id = get_new_entity_id(engine);

			GridLocationComponent* gridloc = create_component(engine, entity_id, COMP_GRIDLOCATION);
			glmc_ivec2_set(gridloc->pos, x, y);

			ArtComponent* art = create_component(engine, entity_id, COMP_ART);
			art->type = ART_WALL;
			WallArtComponent* wall_info = create_component(engine, entity_id, COMP_WALLART);

			wall_info->has_ceil = has_ceil;
			wall_info->has_floor = has_floor;

			int wall_above = (l->level_description[x][y + 1] == 'W') && (current_char = 'W') || y == width-1;
			int wall_under = (l->level_description[x][y - 1] == 'W') && (current_char = 'W') || y == 0;;
			int wall_right = (l->level_description[x + 1][y] == 'W') && (current_char = 'W') || x == height-1;
			int wall_left = (l->level_description[x - 1][y] == 'W') && (current_char = 'W') || x == 0;

			wall_info->has_wall[N] = wall_above;
			wall_info->has_wall[S] = wall_under;
			wall_info->has_wall[W] = wall_left;
			wall_info->has_wall[E] = wall_right;


			if (has_connector || has_lock) {
				EntityId conn_entity_id = get_new_entity_id(engine);

				GridLocationComponent* gridloc = create_component(engine, conn_entity_id, COMP_GRIDLOCATION);
				glmc_ivec2_set(gridloc->pos, x, y);

			
				ConnectorLogicComponent* conn = create_component(engine, conn_entity_id, COMP_CONNECTORLOGIC);

				ActivatableComponent* activatable = create_component(engine, conn_entity_id, COMP_ACTIVATABLE);
				activatable->active = 0;

				DirectionComponent* directioncomponent = create_component(engine, conn_entity_id, COMP_DIRECTION);
				directioncomponent->dir = N;

				char above = l->level_description[x - 1][y];
				char beneath = l->level_description[x + 1][y];
				char left = l->level_description[x][y - 1];
				char right = l->level_description[x][y + 1];


				if (has_lock) {
					if (above == '-') {
						directioncomponent->dir = W;
					}
					if (beneath == '-') {
						directioncomponent->dir = E;
					}
					if (left == '-') {
						directioncomponent->dir = S;
					}
					if (right == '-') {
						directioncomponent->dir = N;
					}
				}
				else {

					EntityId conn2_entity_id = get_new_entity_id(engine);

					GridLocationComponent* gridloc = create_component(engine, conn2_entity_id, COMP_GRIDLOCATION);
					glmc_ivec2_set(gridloc->pos, x, y);

					ConnectorLogicComponent* conn2 = create_component(engine, conn2_entity_id, COMP_CONNECTORLOGIC);

					ActivatableComponent* activatable2 = create_component(engine, conn2_entity_id, COMP_ACTIVATABLE);
					activatable2->active = 0;

					DirectionComponent* directioncomponent2 = create_component(engine, conn2_entity_id, COMP_DIRECTION);
					directioncomponent2->dir = N;

					ArtComponent* art2 = create_component(engine, conn2_entity_id, COMP_ART);
					art2->type = ART_CONNECTOR;

					if (left == '-' || left == 'A' || left == 'B' || left == 'C' || left == 'O' || left == 'D' || left == '|' || left == '&'){
						directioncomponent->dir = S;
					}
					if (above == '-' || above == 'A' || above == 'B' || above == 'C' || above == 'O' || above == 'D' || above == '|' || above == '&') {
						
						directioncomponent2->dir = W;
					}
					if (right == '-' || right == 'A' || right == 'B' || right == 'C' || right == 'O' || right == 'D' || right == '|' || right == '&') {
						directioncomponent2->dir = N;
					}
					if (beneath == '-' || beneath == 'A' || beneath == 'B' || beneath == 'C' || beneath == 'O' || beneath == 'D' || beneath == '|' || beneath == '&') {
						directioncomponent->dir = E;
					}
				}

				ArtComponent* art = create_component(engine, conn_entity_id, COMP_ART);
				art->type = ART_CONNECTOR;

			}

			if (has_key) {
				EntityId key_entity_id = get_new_entity_id(engine);

				GridLocationComponent* gridloc = create_component(engine, key_entity_id, COMP_GRIDLOCATION);
				glmc_ivec2_set(gridloc->pos, x, y);

				ItemComponent* item = create_component(engine, key_entity_id, COMP_ITEM);
				
				if (current_char == 'a') {
					item->color = A;
				} else if (current_char == 'b') {
					item->color = B;
				} else if (current_char == 'c') {
					item->color = C;
				} else if (current_char == 'o') {
					item->color = O;
				}

				ArtComponent* art = create_component(engine, key_entity_id, COMP_ART);
				art->type = ART_KEY;
			}

			if (has_player) {
				EntityId player_entity_id = get_new_entity_id(engine);

				GridLocationComponent* gridloc = create_component(engine, player_entity_id, COMP_GRIDLOCATION);
				glmc_ivec2_set(gridloc->pos, x, y);

				ContainerComponent* cont = create_component(engine, player_entity_id, COMP_CONTAINER);
				cont->contains_something = 0;

				ArtComponent* art = create_component(engine, player_entity_id, COMP_ART);
				art->type = ART_PLAYER;

				create_component(engine, player_entity_id, COMP_INPUTRECEIVER);

				CameraLookAtComponent* cameralookat = create_component(engine, player_entity_id, COMP_CAMERA_LOOK_AT);
				glmc_vec3_set(cameralookat->pos, x * 1.0f, y * 1.0f, 0.0f);

				MoveActionComponent* moveaction = create_component(engine, player_entity_id, COMP_MOVE_ACTION);

				CameraLookFromComponent* cameralookfrom = create_component(engine, player_entity_id, COMP_CAMERA_LOOK_FROM);
				cameralookfrom->distance = 15.0f;
				cameralookfrom->XYdegees = 0.0f;
				cameralookfrom->Zdegrees = 45.0f;
			}

			if (has_door) {
				EntityId door_entity_id = get_new_entity_id(engine);

				GridLocationComponent* gridloc = create_component(engine, door_entity_id, COMP_GRIDLOCATION);
				glmc_ivec2_set(gridloc->pos, x, y);

				ActivatableComponent* activatable = create_component(engine, door_entity_id, COMP_ACTIVATABLE);
				activatable->active = 0;

				DirectionComponent* directioncomponent = create_component(engine, door_entity_id, COMP_DIRECTION);
				directioncomponent->dir = N;

				ArtComponent* art = create_component(engine, door_entity_id, COMP_ART);
				art->type = ART_DOOR;
			}
			
			if (has_exit) {
			
				EntityId exit_entity_id = get_new_entity_id(engine);

				GridLocationComponent* gridloc = create_component(engine, exit_entity_id, COMP_GRIDLOCATION);
				glmc_ivec2_set(gridloc->pos, x, y);

				ExitComponent* exit = create_component(engine, exit_entity_id, COMP_EXIT);
				exit->done = 0;
				
				ArtComponent* art = create_component(engine, exit_entity_id, COMP_ART);
				art->type = ART_END;
			}

			if (has_lock) {
				EntityId lock_entity_id = get_new_entity_id(engine);

				GridLocationComponent* gridloc = create_component(engine, lock_entity_id, COMP_GRIDLOCATION);
				glmc_ivec2_set(gridloc->pos, x, y);

				ActivatableComponent* activatable = create_component(engine, lock_entity_id, COMP_ACTIVATABLE);
				activatable->active = 0;

				ArtComponent* art = create_component(engine, lock_entity_id, COMP_ART);
				art->type = ART_LOCK;

				LockComponent* lock = create_component(engine, lock_entity_id, COMP_LOCK);

				if (current_char == 'A') {
					lock->requiredKeyColor = A;
				}
				else if (current_char == 'B') {
					lock->requiredKeyColor = B;
				}
				else if (current_char == 'C') {
					lock->requiredKeyColor = C;
				}
				else if (current_char == 'O') {
					lock->requiredKeyColor = O;
				}
			}

			if (has_conAnd) {
				EntityId conn_entity_id = get_new_entity_id(engine);

				GridLocationComponent* gridloc = create_component(engine, conn_entity_id, COMP_GRIDLOCATION);
				glmc_ivec2_set(gridloc->pos, x, y);

				ActivatableComponent* activatable = create_component(engine, conn_entity_id, COMP_ACTIVATABLE);
				activatable->active = 0;

				ArtComponent* art = create_component(engine, conn_entity_id, COMP_ART);
				art->type = ART_CONNECTOR_AND;

				char above = l->level_description[x - 1][y];
				char beneath = l->level_description[x + 1][y];
				char left = l->level_description[x][y - 1];
				char right = l->level_description[x][y + 1];

				if (above == '-') {
					EntityId conn2_entity_id = get_new_entity_id(engine);

					GridLocationComponent* gridloc = create_component(engine, conn2_entity_id, COMP_GRIDLOCATION);
					glmc_ivec2_set(gridloc->pos, x, y);

					ConnectorLogicComponent* conn2 = create_component(engine, conn2_entity_id, COMP_CONNECTORLOGIC);

					ActivatableComponent* activatable2 = create_component(engine, conn2_entity_id, COMP_ACTIVATABLE);
					activatable2->active = 0;

					DirectionComponent* directioncomponent2 = create_component(engine, conn2_entity_id, COMP_DIRECTION);
					directioncomponent2->dir = W;

					ArtComponent* art2 = create_component(engine, conn2_entity_id, COMP_ART);
					art2->type = ART_CONNECTOR;
				}

				if (beneath == '-') {
					EntityId conn2_entity_id = get_new_entity_id(engine);

					GridLocationComponent* gridloc = create_component(engine, conn2_entity_id, COMP_GRIDLOCATION);
					glmc_ivec2_set(gridloc->pos, x, y);

					ConnectorLogicComponent* conn2 = create_component(engine, conn2_entity_id, COMP_CONNECTORLOGIC);

					ActivatableComponent* activatable2 = create_component(engine, conn2_entity_id, COMP_ACTIVATABLE);
					activatable2->active = 0;

					DirectionComponent* directioncomponent2 = create_component(engine, conn2_entity_id, COMP_DIRECTION);
					directioncomponent2->dir = E;

					ArtComponent* art2 = create_component(engine, conn2_entity_id, COMP_ART);
					art2->type = ART_CONNECTOR;
				}

				if (left == '-') {
					EntityId conn2_entity_id = get_new_entity_id(engine);

					GridLocationComponent* gridloc = create_component(engine, conn2_entity_id, COMP_GRIDLOCATION);
					glmc_ivec2_set(gridloc->pos, x, y);

					ConnectorLogicComponent* conn2 = create_component(engine, conn2_entity_id, COMP_CONNECTORLOGIC);

					ActivatableComponent* activatable2 = create_component(engine, conn2_entity_id, COMP_ACTIVATABLE);
					activatable2->active = 0;

					DirectionComponent* directioncomponent2 = create_component(engine, conn2_entity_id, COMP_DIRECTION);
					directioncomponent2->dir = S;

					ArtComponent* art2 = create_component(engine, conn2_entity_id, COMP_ART);
					art2->type = ART_CONNECTOR;
				}

				if (right == '-') {
					EntityId conn2_entity_id = get_new_entity_id(engine);

					GridLocationComponent* gridloc = create_component(engine, conn2_entity_id, COMP_GRIDLOCATION);
					glmc_ivec2_set(gridloc->pos, x, y);

					ConnectorLogicComponent* conn2 = create_component(engine, conn2_entity_id, COMP_CONNECTORLOGIC);

					ActivatableComponent* activatable2 = create_component(engine, conn2_entity_id, COMP_ACTIVATABLE);
					activatable2->active = 0;

					DirectionComponent* directioncomponent2 = create_component(engine, conn2_entity_id, COMP_DIRECTION);
					directioncomponent2->dir = N;

					ArtComponent* art2 = create_component(engine, conn2_entity_id, COMP_ART);
					art2->type = ART_CONNECTOR;
				}
			}

			if (has_conOr) {
				EntityId conn_entity_id = get_new_entity_id(engine);

				GridLocationComponent* gridloc = create_component(engine, conn_entity_id, COMP_GRIDLOCATION);
				glmc_ivec2_set(gridloc->pos, x, y);

				ActivatableComponent* activatable = create_component(engine, conn_entity_id, COMP_ACTIVATABLE);
				activatable->active = 0;

				ArtComponent* art = create_component(engine, conn_entity_id, COMP_ART);
				art->type = ART_CONNECTOR_OR;

				char above = l->level_description[x - 1][y];
				char beneath = l->level_description[x + 1][y];
				char left = l->level_description[x][y - 1];
				char right = l->level_description[x][y + 1];

				if (above == '-') {
					EntityId conn2_entity_id = get_new_entity_id(engine);

					GridLocationComponent* gridloc = create_component(engine, conn2_entity_id, COMP_GRIDLOCATION);
					glmc_ivec2_set(gridloc->pos, x, y);

					ConnectorLogicComponent* conn2 = create_component(engine, conn2_entity_id, COMP_CONNECTORLOGIC);

					ActivatableComponent* activatable2 = create_component(engine, conn2_entity_id, COMP_ACTIVATABLE);
					activatable2->active = 0;

					DirectionComponent* directioncomponent2 = create_component(engine, conn2_entity_id, COMP_DIRECTION);
					directioncomponent2->dir = W;

					ArtComponent* art2 = create_component(engine, conn2_entity_id, COMP_ART);
					art2->type = ART_CONNECTOR;
				}

				if (beneath == '-') {
					EntityId conn2_entity_id = get_new_entity_id(engine);

					GridLocationComponent* gridloc = create_component(engine, conn2_entity_id, COMP_GRIDLOCATION);
					glmc_ivec2_set(gridloc->pos, x, y);

					ConnectorLogicComponent* conn2 = create_component(engine, conn2_entity_id, COMP_CONNECTORLOGIC);

					ActivatableComponent* activatable2 = create_component(engine, conn2_entity_id, COMP_ACTIVATABLE);
					activatable2->active = 0;

					DirectionComponent* directioncomponent2 = create_component(engine, conn2_entity_id, COMP_DIRECTION);
					directioncomponent2->dir = E;

					ArtComponent* art2 = create_component(engine, conn2_entity_id, COMP_ART);
					art2->type = ART_CONNECTOR;
				}

				if (left == '-') {
					EntityId conn2_entity_id = get_new_entity_id(engine);

					GridLocationComponent* gridloc = create_component(engine, conn2_entity_id, COMP_GRIDLOCATION);
					glmc_ivec2_set(gridloc->pos, x, y);

					ConnectorLogicComponent* conn2 = create_component(engine, conn2_entity_id, COMP_CONNECTORLOGIC);

					ActivatableComponent* activatable2 = create_component(engine, conn2_entity_id, COMP_ACTIVATABLE);
					activatable2->active = 0;

					DirectionComponent* directioncomponent2 = create_component(engine, conn2_entity_id, COMP_DIRECTION);
					directioncomponent2->dir = S;

					ArtComponent* art2 = create_component(engine, conn2_entity_id, COMP_ART);
					art2->type = ART_CONNECTOR;
				}

				if (right == '-') {
					EntityId conn2_entity_id = get_new_entity_id(engine);

					GridLocationComponent* gridloc = create_component(engine, conn2_entity_id, COMP_GRIDLOCATION);
					glmc_ivec2_set(gridloc->pos, x, y);

					ConnectorLogicComponent* conn2 = create_component(engine, conn2_entity_id, COMP_CONNECTORLOGIC);

					ActivatableComponent* activatable2 = create_component(engine, conn2_entity_id, COMP_ACTIVATABLE);
					activatable2->active = 0;

					DirectionComponent* directioncomponent2 = create_component(engine, conn2_entity_id, COMP_DIRECTION);
					directioncomponent2->dir = N;

					ArtComponent* art2 = create_component(engine, conn2_entity_id, COMP_ART);
					art2->type = ART_CONNECTOR;
				}
			}
		}
	}
}
