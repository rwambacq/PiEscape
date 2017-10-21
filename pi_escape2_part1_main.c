#define __STDC_FORMAT_MACROS
#include <inttypes.h>
#include <stdio.h>
#include "util/sleep.h"
#include "pi_escape/graphics/opengl_game_renderer.h"
#include "pi_escape/level/levelloader.h"
#include "pi_escape/es/game.h"

#include "pi_escape/es/es_memory_manager.h"

#include <SDL.h>
#undef main //Weird bug on windows where SDL overwrite main definition
#include <SDL_timer.h>

void create_demo_entities(Engine* engine) {
    //As a demo, this creates a few example entities. This should be completely replaced by the level_loader and assemblage in the real game
    for (int x = 0; x < 5; x++) {
        for (int y = 0; y < 5; y++) {
            int has_door = x == 2 && y == 2;
            int has_floor = x != 2 && !has_door;
            int has_ceil = !has_floor && !has_door;
            int has_key = x == 1 && y == 1;
            int has_player = x == 3 && y == 3;
            int has_lock = x == 0 && y == 0;

            EntityId entity_id = get_new_entity_id(engine);

            GridLocationComponent* gridloc = create_component(engine, entity_id, COMP_GRIDLOCATION);
            glmc_ivec2_set(gridloc->pos, x, y);

            ArtComponent* art = create_component(engine, entity_id, COMP_ART);
            art->type = ART_WALL;

            WallArtComponent* wall_info = create_component(engine, entity_id, COMP_WALLART);
            wall_info->has_ceil = has_ceil;
            wall_info->has_floor = has_floor;
            wall_info->has_wall[N] = has_door || y == 4 ;
            wall_info->has_wall[S] = has_door || y == 0 ;
            wall_info->has_wall[W] = x == 0 || (x == 3 && y != 2);
            wall_info->has_wall[E] = x == 4 || (x == 1 && y != 2);

            if (has_key) {
                EntityId key_entity_id = get_new_entity_id(engine);

                GridLocationComponent* gridloc = create_component(engine, key_entity_id, COMP_GRIDLOCATION);
                glmc_ivec2_set(gridloc->pos, x, y);

                ItemComponent* item = create_component(engine, key_entity_id, COMP_ITEM);
                item->color = A;

                ArtComponent* art = create_component(engine, key_entity_id, COMP_ART);
                art->type = ART_KEY;
            }

            if (has_player) {
                EntityId player_entity_id = get_new_entity_id(engine);

                GridLocationComponent* gridloc = create_component(engine, player_entity_id, COMP_GRIDLOCATION);
                glmc_ivec2_set(gridloc->pos, x, y);

                ArtComponent* art = create_component(engine, player_entity_id, COMP_ART);
                art->type = ART_PLAYER;

                create_component(engine, player_entity_id, COMP_INPUTRECEIVER);

                CameraLookAtComponent* cameralookat = create_component(engine, player_entity_id, COMP_CAMERA_LOOK_AT);
               glmc_vec3_set(cameralookat->pos, x * 1.0f, y * 1.0f, 0.0f);

                CameraLookFromComponent* cameralookfrom = create_component(engine, player_entity_id, COMP_CAMERA_LOOK_FROM);
                cameralookfrom->distance = 15.0f;
                cameralookfrom->XYdegees = 0.0f;
                cameralookfrom->Zdegrees = 25.0f;
               glmc_vec3_set(cameralookfrom->pos, 4.0f, -4.0f, 4.0f); //this normally gets overridden by camera system
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

            if (has_lock) {
                EntityId lock_entity_id = get_new_entity_id(engine);

                GridLocationComponent* gridloc = create_component(engine, lock_entity_id, COMP_GRIDLOCATION);
                glmc_ivec2_set(gridloc->pos, x, y);

                ActivatableComponent* activatable = create_component(engine, lock_entity_id, COMP_ACTIVATABLE);
                activatable->active = 0;

                ArtComponent* art = create_component(engine, lock_entity_id, COMP_ART);
                art->type = ART_LOCK;

                LockComponent* lock = create_component(engine, lock_entity_id, COMP_LOCK);
                lock->requiredKeyColor = B;
            }
        }
    }
}


void runGameAndPrintPerfomanceStatistics(Game* game) {
	Uint32 start_time_ms = SDL_GetTicks();
	Uint32 last_print_time_ms = start_time_ms;
	long update_count = 0;

	while (!game->engine.context.is_exit_game) {
		Uint32 cur_time_ms = SDL_GetTicks();
		Uint32 diff_time_ms = cur_time_ms - last_print_time_ms;

		engine_update(&game->engine);
		update_count++;

		//print performance statistics each second
		if (diff_time_ms > 1000) {
			float time_ms_per_update = (float)diff_time_ms / (float)update_count;
			float fps = 1.0f / time_ms_per_update * 1000.0f;
			printf("This second: %f updates. Average time per update: %f ms.\n", fps, time_ms_per_update);

			last_print_time_ms = cur_time_ms;
			update_count = 0;
		}
	}
}

void runGame(Game* game) {
	while (!game->engine.context.is_exit_game) {
		engine_update(&(game->engine));
	}
}

int main(int argc, char **argv) {
	if (argc > 1) {
		printf("benchmark mode\n\n%d\n", logging_benchmark);
		logging_benchmark = 1;
	}
	else {
		printf("normal mode\n\n");
	}

    int imgFlags = IMG_INIT_PNG;
    if(!(IMG_Init(imgFlags) & imgFlags)) {
        fatal("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
    }

    LevelLoader* level_loader = levelloader_alloc();

    //init the graphics system
    Graphics* graphics = graphics_alloc(0, 0);

    //initialise context, engine and assemblage, and add systems
    Game* pi_escape_2 = game_alloc(graphics);

    //TODO: don't use this
    create_demo_entities(&(pi_escape_2->engine));

    //TODO: use the 2 lines below instead of using create_demo_entities
    // Level* level = levelloader_load_level(level_loader, 0);
    // game_load_level(pi_escape_2, level);

    //TODO: support playing all levels in sequence

	runGame(pi_escape_2);
	//runGameAndPrintPerfomanceStatistics(pi_escape_2);

    game_free(pi_escape_2);
	free(pi_escape_2);

    graphics_free(graphics);
    free(graphics);

    levelloader_free(level_loader);
    free(level_loader);

    return 0;
}
