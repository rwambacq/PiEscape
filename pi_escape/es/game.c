#include "game.h"
#include <assert.h>
#include <stdlib.h>
#include "../../util/sleep.h"

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
				
				IsConnectorComponent* conne = create_component(engine, conn_entity_id, COMP_ISCONNECTOR);
				conne->x = 0;

				char above = l->level_description[x - 1][y];
				char beneath = l->level_description[x + 1][y];
				char left = l->level_description[x][y - 1];
				char right = l->level_description[x][y + 1];


				if (has_lock) {
					if (above == '-' || above == '|' || above == '&') {
						directioncomponent->dir = W;
					}
					if (beneath == '-' || beneath == '|' || beneath == '&') {
						directioncomponent->dir = E;
					}
					if (left == '-' || left == '|' || left == '&') {
						directioncomponent->dir = S;
					}
					if (right == '-' || right == '|' || right == '&') {
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

					IsConnectorComponent* conne2 = create_component(engine, conn2_entity_id, COMP_ISCONNECTOR);
					conne2->x = 0;

					ArtComponent* art2 = create_component(engine, conn2_entity_id, COMP_ART);
					art2->type = ART_CONNECTOR;



					if (left == '-' || left == 'A' || left == 'B' || left == 'C' || left == 'O' || left == 'D' || left == '|' || left == '&'){
						if (directioncomponent->dir == NULL) {
							directioncomponent->dir = S;
						}
						else {
							directioncomponent2->dir = S;
						}
					}
					if (above == '-' || above == 'A' || above == 'B' || above == 'C' || above == 'O' || above == 'D' || above == '|' || above == '&') {
						if (directioncomponent->dir == NULL) {
							directioncomponent->dir = W;
						}
						else {
							directioncomponent2->dir = W;
						}
					}
					if (right == '-' || right == 'A' || right == 'B' || right == 'C' || right == 'O' || right == 'D' || right == '|' || right == '&') {
						if (directioncomponent->dir == NULL) {
							directioncomponent->dir = N;
						}
						else {
							directioncomponent2->dir = N;
						}
					}
					if (beneath == '-' || beneath == 'A' || beneath == 'B' || beneath == 'C' || beneath == 'O' || beneath == 'D' || beneath == '|' || beneath == '&') {
						if (directioncomponent->dir == NULL) {
							directioncomponent->dir = E;
						}
						else {
							directioncomponent2->dir = E;
						}
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


				char checkmuur = l->level_description[x-1][y];

				if (checkmuur == 'W') {
					directioncomponent->dir = E;
				}

				IsDoorComponent* doorcomponent = create_component(engine, door_entity_id, COMP_ISDOOR);
				doorcomponent->x = 0;

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


				if (above == '-' || above == '|' || above == '&' || above == 'A' || above == 'B' || above == 'C' || above == 'O' || above == 'D') {
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

				if (beneath == '-' || beneath == '|' || beneath == '&' || beneath == 'A' || beneath == 'B' || beneath == 'C' || beneath == 'O' || beneath == 'D') {

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

				if (left == '-' || left == '|' || left == '&' || left == 'A' || left == 'B' || left == 'C' || left == 'O' || left == 'D') {
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

				if (right == '-' || right == '|' || right == '&' || right == 'A' || right == 'B' || right == 'C' || right == 'O' || right == 'D') {

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
				
				IsConnectorComponent* conne = create_component(engine, conn_entity_id, COMP_ISCONNECTOR);
				conne->x = 0;

				ConnectorOr* ok = create_component(engine, conn_entity_id, COMP_CONNOR);
				ok->current = 0;
				ok->needed = 1; 

				ArtComponent* art = create_component(engine, conn_entity_id, COMP_ART);
				art->type = ART_CONNECTOR_OR;

				char above = l->level_description[x - 1][y];
				char beneath = l->level_description[x + 1][y];
				char left = l->level_description[x][y - 1];
				char right = l->level_description[x][y + 1];

				if (above == '-' || above == '|' || above == '&' || above == 'A' || above == 'B' || above == 'C' || above == 'O' || above == 'D') {
					EntityId conn2_entity_id = get_new_entity_id(engine);

					GridLocationComponent* gridloc = create_component(engine, conn2_entity_id, COMP_GRIDLOCATION);
					glmc_ivec2_set(gridloc->pos, x, y);


					ActivatableComponent* activatable2 = create_component(engine, conn2_entity_id, COMP_ACTIVATABLE);
					activatable2->active = 0;

					DirectionComponent* directioncomponent2 = create_component(engine, conn2_entity_id, COMP_DIRECTION);
					directioncomponent2->dir = W;

					IsConnectorComponent* conne = create_component(engine, conn2_entity_id, COMP_ISCONNECTOR);
					conne->x = 0;

					ArtComponent* art2 = create_component(engine, conn2_entity_id, COMP_ART);
					art2->type = ART_CONNECTOR;
				}

				if (beneath == '-' || beneath == '|' || beneath == '&' || beneath == 'A' || beneath == 'B' || beneath == 'C' || beneath == 'O' || beneath == 'D') {
					EntityId conn2_entity_id = get_new_entity_id(engine);

					GridLocationComponent* gridloc = create_component(engine, conn2_entity_id, COMP_GRIDLOCATION);
					glmc_ivec2_set(gridloc->pos, x, y);

					ActivatableComponent* activatable2 = create_component(engine, conn2_entity_id, COMP_ACTIVATABLE);
					activatable2->active = 0;

					IsConnectorComponent* conne = create_component(engine, conn2_entity_id, COMP_ISCONNECTOR);
					conne->x = 0;

					DirectionComponent* directioncomponent2 = create_component(engine, conn2_entity_id, COMP_DIRECTION);
					directioncomponent2->dir = E;

					ArtComponent* art2 = create_component(engine, conn2_entity_id, COMP_ART);
					art2->type = ART_CONNECTOR;
				}

				if (left == '-' || left == '|' || left == '&' || left == 'A' || left == 'B' || left == 'C' || left == 'O' || left == 'D') {

					EntityId conn2_entity_id = get_new_entity_id(engine);

					GridLocationComponent* gridloc = create_component(engine, conn2_entity_id, COMP_GRIDLOCATION);
					glmc_ivec2_set(gridloc->pos, x, y);

					ActivatableComponent* activatable2 = create_component(engine, conn2_entity_id, COMP_ACTIVATABLE);
					activatable2->active = 0;
					
					IsConnectorComponent* conne = create_component(engine, conn2_entity_id, COMP_ISCONNECTOR);
					conne->x = 0;

					DirectionComponent* directioncomponent2 = create_component(engine, conn2_entity_id, COMP_DIRECTION);
					directioncomponent2->dir = S;

					ArtComponent* art2 = create_component(engine, conn2_entity_id, COMP_ART);
					art2->type = ART_CONNECTOR;
				}

				if (right == '-' || right == '|' || right == '&' || right == 'A' || right == 'B' || right == 'C' || right == 'O' || right == 'D') {
					EntityId conn2_entity_id = get_new_entity_id(engine);

					GridLocationComponent* gridloc = create_component(engine, conn2_entity_id, COMP_GRIDLOCATION);
					glmc_ivec2_set(gridloc->pos, x, y);

					IsConnectorComponent* conne = create_component(engine, conn2_entity_id, COMP_ISCONNECTOR);
					conne->x = 0;


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


	EntityIterator itlock;
	search_entity_3(engine, COMP_ACTIVATABLE,COMP_ART,COMP_LOCK, &itlock);

	while (next_entity(&itlock)) {
		EntityId lockje = itlock.entity_id;
		assert(lockje != NO_ENTITY);
		ActivatableComponent* xx = get_component(engine, lockje, COMP_ACTIVATABLE);
		GridLocationComponent* locs = get_component(engine, lockje, COMP_GRIDLOCATION);
		EntityId sleutelgat = NULL;
		int x = locs->pos[0];
		int y = locs->pos[1];
		int voorgaand = 100;
		int laterx = x;
		int latery = y;
		int lockx = x;
		int locky = x;
		char above = l->level_description[x - 1][y];
		char beneath = l->level_description[x + 1][y];
		char left = l->level_description[x][y - 1];
		char right = l->level_description[x][y + 1];
		int plaatsje;
		int curr;
		EntityIterator itdoor;
		EntityId last;
		search_entity_3(engine, COMP_ISCONNECTOR, COMP_ACTIVATABLE, COMP_DIRECTION, &itdoor);
		while (next_entity(&itdoor)) {
			EntityId door = itdoor.entity_id;
			assert(door != NO_ENTITY);
			GridLocationComponent* plaats = get_component(engine, door, COMP_GRIDLOCATION);

			int plaatsx = plaats->pos[0];
			int plaatsy = plaats->pos[1];
			
			if (plaatsx == x && plaatsy == y) {
				ConnectionsComponent* nextje = create_component(engine, lockje, COMP_CONNECTIONS);
				nextje->next = door;
				nextje->prev = NULL;
				ActivatableComponent* xx = get_component(engine, nextje->next, COMP_ACTIVATABLE);
				//xx->active = !xx->active;
				ConnectionsComponent* nextpathje = create_component(engine, door, COMP_CONNECTIONS);
				nextpathje->prev = lockje;
				last = door;
			}
		}

		
		int hasnext = above == '-' | beneath == '-' | left == '-' | right == '-';

		curr = 0;
		while (1) {
				if ((above == '-' || above == '|') && curr != 2 && voorgaand != 2 && voorgaand != 3 && voorgaand != 4) {
					x -= 1;
					curr = 1;

				}
				else if ((beneath == '-' || beneath == '|') && curr != 1 && voorgaand != 1 && voorgaand != 3 && voorgaand != 4) {
					x += 1;
					curr = 2;
				}
				else if ((left == '-' || left == '|') && curr != 4 && voorgaand != 1 && voorgaand != 3 && voorgaand != 2) {
					y -= 1;
					curr = 3;
				}
				else if ((right == '-' || right == '|' ) && curr != 3 && voorgaand != 1 && voorgaand != 4 && voorgaand != 2) {
					y += 1;
					curr = 4;
				}
				else {
					printf("pfffffff");
					int xloc = x;
					int yloc = y;

					if (above == 'D') {
						xloc -= 1;
					}
					else if (beneath == 'D') {
						xloc += 1;
					}
					else if (left == 'D') {
						yloc -= 1;
					}
					else if (right == 'D') {
						yloc += 1;
					}

					EntityIterator itdoor;
					search_entity_3(engine, COMP_ART, COMP_ISDOOR, COMP_DIRECTION, &itdoor);
					while (next_entity(&itdoor)) {
						EntityId door = itdoor.entity_id;
						assert(door != NO_ENTITY);
						GridLocationComponent* positie = get_component(engine, door, COMP_GRIDLOCATION);
						if (positie->pos[0] == xloc && positie->pos[1] == yloc) {
							ConnectionsComponent* nieuwpath = get_component(engine, last, COMP_CONNECTIONS);
							nieuwpath->next = door;
							ConnectionsComponent* oldpath = create_component(engine, door, COMP_CONNECTIONS);
							oldpath->next = NULL;
							oldpath->prev = last;
							ConnectionsComponent* sleuteldeur = get_component(engine, lockje, COMP_CONNECTIONS);
							sleuteldeur->prev = door;

						}
					}

					break;
				}

				voorgaand = 0;
			EntityIterator itdoor;
			EntityId een = NULL;
			EntityId twee = NULL;
			EntityId drie = NULL;
			int gewoonweg = 1;

			search_entity_2(engine, COMP_ISCONNECTOR, COMP_ACTIVATABLE, &itdoor);
			while (next_entity(&itdoor)) {
				EntityId door = itdoor.entity_id;
				assert(door != NO_ENTITY);
				GridLocationComponent* plaats = get_component(engine, door, COMP_GRIDLOCATION);

				int plaatsx = plaats->pos[0];
				int plaatsy = plaats->pos[1];

				if (plaatsx == x && plaatsy == y) {
					ActivatableComponent* xx = get_component(engine, door, COMP_ACTIVATABLE);
					//xx->active = !xx->active;

					DirectionComponent* xdir = get_component(engine, door, COMP_DIRECTION);
					plaatsje = curr;
					if (xdir != NULL) {
						if (plaatsje == 1) {
							if (xdir->dir == E) {
								een = door;
								IsConnectorComponent* aanofuit = get_component(engine, door, COMP_ISCONNECTOR);
								aanofuit->x = 1;
							}
							else {
								if (twee == NULL) {
									twee = door;
								}
							}
						}
						else if (plaatsje == 2) {
							if (xdir->dir == W) {
								een = door;
								IsConnectorComponent* aanofuit = get_component(engine, door, COMP_ISCONNECTOR);
								aanofuit->x = 1;
							}
							else {
								if (twee == NULL) {
									twee = door;
								}
							}
						}
						else if (plaatsje == 3) {
							if (xdir->dir == N) {
								een = door;
								IsConnectorComponent* aanofuit = get_component(engine, door, COMP_ISCONNECTOR);
								aanofuit->x = 1;
							}
							else {
								if (twee == NULL) {
									twee = door;
								}
							}
						}
						else if (plaatsje == 4) {
							if (xdir->dir == S) {
								een = door;
								IsConnectorComponent* aanofuit = get_component(engine, door, COMP_ISCONNECTOR);
								aanofuit->x = 1;
							}
							else {
								if (twee == NULL) {
									twee = door;
								}
							}
						}
					}
					else {
						gewoonweg = 0;
						drie = door;
						ActivatableComponent* opl = get_component(engine, drie, COMP_ACTIVATABLE);
						//opl->active = 1;
					}
					
				}
			}
			EntityId misschien  = NULL;
			int aantalok = 0;
			if (!gewoonweg) {
				EntityIterator erover;
				search_entity_2(engine, COMP_ISCONNECTOR, COMP_ACTIVATABLE, &erover);
				while (next_entity(&erover)) {
					EntityId aantalactief = erover.entity_id;
					assert(aantalactief != NO_ENTITY);
					GridLocationComponent* plaats = get_component(engine, aantalactief, COMP_GRIDLOCATION);
					int plaatsx = plaats->pos[0];
					int plaatsy = plaats->pos[1];

					if (plaatsx == x && plaatsy == y) {
						if (!has_component(engine, aantalactief, COMP_CONNOR)) {
							IsConnectorComponent* aanofuit = get_component(engine, aantalactief, COMP_ISCONNECTOR);
							if (aanofuit->x == 0) {
								aantalok += 1;
								misschien = aantalactief;
							}
						}
					}

				}

			}

			if (aantalok == 1) {
				printf("\n\nhalle\n\n");
				ActivatableComponent* opl = get_component(engine, misschien, COMP_ACTIVATABLE);
				
				//twee = misschien;
				gewoonweg = 1;
			}

			ConnectionsComponent* nieuwpath = get_component(engine, last, COMP_CONNECTIONS);
			nieuwpath->next = een;
			ConnectionsComponent* nieuwpath2;

			if (drie != NULL) {
				ConnectionsComponent* nieuwpath1 = create_component(engine, een, COMP_CONNECTIONS);
				nieuwpath1->prev = last;
				nieuwpath1->next = drie;
				ConnectionsComponent* nieuwpath2;
				nieuwpath2 = create_component(engine, drie, COMP_CONNECTIONS);
				nieuwpath2->prev = een;
				if (gewoonweg) {
					ConnectionsComponent* rar = create_component(engine, misschien, COMP_CONNECTIONS);
					rar->prev = drie;
					rar->next = NULL;
					nieuwpath2->next = misschien;
					DirectionComponent* bijna = create_component(engine, misschien, COMP_DIRECTION);

					if (bijna->dir == N) {
						voorgaand = 3;
					}
					if (bijna->dir == E) {
						voorgaand = 2;
					}
					if (bijna->dir == S) {
						voorgaand = 4;
					}
					if (bijna->dir == W) {
						voorgaand = 1;
					}



					last = misschien;
				}
				else {
					nieuwpath2->next = NULL;
					break;
				}
				

			}

			else {
				ConnectionsComponent* nieuwpath1 = create_component(engine, een, COMP_CONNECTIONS);
				nieuwpath1->prev = last;
				nieuwpath1->next = twee;
				nieuwpath2 = create_component(engine, twee, COMP_CONNECTIONS);
				nieuwpath2->prev = een;
				last = twee;
			}



			
			above = l->level_description[x - 1][y];
			beneath = l->level_description[x + 1][y];
			left = l->level_description[x][y - 1];
			right = l->level_description[x][y + 1];

		}
	
	}
}
