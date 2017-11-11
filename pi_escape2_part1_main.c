#define __STDC_FORMAT_MACROS
#include <inttypes.h>
#include <stdio.h>
#include "util/sleep.h"
#include "pi_escape/graphics/opengl_game_renderer.h"
#include "pi_escape/level/levelloader.h"
#include "pi_escape/es/game.h"

#include <assert.h>
#include <SDL.h>
#undef main //Weird bug on windows where SDL overwrite main definition
#include <SDL_timer.h>

void fill_level_loader(LevelLoader* level_loader) {
	strcpy(level_loader->level_paths[0], "pi_escape/level/level_files/tutorial1.lvl");
	strcpy(level_loader->level_paths[1], "pi_escape/level/level_files/tutorial2.lvl");
	strcpy(level_loader->level_paths[2], "pi_escape/level/level_files/tutorial3.lvl");
	strcpy(level_loader->level_paths[3], "pi_escape/level/level_files/tutorial4.lvl");
	strcpy(level_loader->level_paths[4], "pi_escape/level/level_files/tutorial5.lvl");
	strcpy(level_loader->level_paths[5], "pi_escape/level/level_files/tutorial6.lvl");
	strcpy(level_loader->level_paths[6], "pi_escape/level/level_files/tutorial7.lvl");
	strcpy(level_loader->level_paths[7], "pi_escape/level/level_files/game1.lvl");
	strcpy(level_loader->level_paths[8], "pi_escape/level/level_files/game2.lvl");
	strcpy(level_loader->level_paths[9], "pi_escape/level/level_files/game3.lvl");
}

int main() {
    int imgFlags = IMG_INIT_PNG;
    if(!(IMG_Init(imgFlags) & imgFlags)) {
        fatal("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
    }

    struct LevelLoader* level_loader = levelloader_alloc();
	fill_level_loader(level_loader);

    //init the graphics system
    Graphics* graphics = graphics_alloc(0, 0);

    //initialise context, engine and assemblage, and add systems
    Game* pi_escape_2 = game_alloc(graphics);

	int level_nr = 5;															//THIS NUMBER DECIDES WHICH LEVEL IS LOADED, FOR TESTING, USE THIS!!
    Level* level = levelloader_load_level(level_loader, level_nr);
    game_load_level(pi_escape_2, level);

	int width = level->breedte;
	int height = level->hoogte;
	int s;


    //TODO: support playing all levels in sequence

    Uint32 start_time_ms = SDL_GetTicks();
    Uint32 last_print_time_ms = start_time_ms;
    long update_count = 0;

    while (!pi_escape_2->engine.context.is_exit_game) {
		
		EntityIterator level_exit;
		search_entity_1(&pi_escape_2->engine, COMP_EXIT, &level_exit);
		next_entity(&level_exit);
		EntityId exit_id = level_exit.entity_id;
		assert(exit_id != NO_ENTITY);

		EntityIterator player_it;
		search_entity_2(&pi_escape_2->engine, COMP_GRIDLOCATION, COMP_INPUTRECEIVER, &player_it);
		next_entity(&player_it);
		EntityId player_entity_id = player_it.entity_id;
		assert(player_entity_id != NO_ENTITY);
		Uint32 timer_exit = 0;

		ExitComponent* exit_comp = get_component(&pi_escape_2->engine, exit_id, COMP_EXIT);
		if (exit_comp->done && level_nr < 9) {
			create_component(&pi_escape_2->engine, player_entity_id, COMP_BLOCKING);
			sleep_ms(500);
			free_component(&pi_escape_2->engine, player_entity_id, COMP_BLOCKING);
			es_memory_manager_init(&(pi_escape_2->engine.es_memory));
			level_nr++;
			level = levelloader_load_level(level_loader, level_nr);
			game_load_level(pi_escape_2, level);

			width = level->breedte;
			height = level->hoogte;

			int s;
		}
		else if (exit_comp->done && level_nr == 9) {
			pi_escape_2->engine.context.is_exit_game = 1;
		}

        Uint32 cur_time_ms = SDL_GetTicks();
        Uint32 diff_time_ms = cur_time_ms - last_print_time_ms;

        engine_update(&pi_escape_2->engine);
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

    game_free(pi_escape_2);
    free(pi_escape_2);

    graphics_free(graphics);
    free(graphics);

    levelloader_free(level_loader);
    free(level_loader);

    return 0;
}
