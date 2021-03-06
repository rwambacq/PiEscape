#define __STDC_FORMAT_MACROS
#include <inttypes.h>

#ifndef STDIO_INCLUDED
#include <stdio.h>
#define STDIO_INCLUDED
#endif


#include "util/sleep.h"
#include "pi_escape/graphics/opengl_game_renderer.h"
#include "pi_escape/level/levelloader.h"
#include "pi_escape/es/game.h"

#include <assert.h>

#include "pi_escape/es/es_memory_manager.h"
#define BENCHLOG_FILE_PATH "benchmarks/benchlog.txt"

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
int main(int argc, char **argv) {
	player_blocked = 1;
	// if you call the main game with more than one argument, assume it is benchmarking.
	if (argc > 1) {
		printf("benchmark mode\n\n%d\n", logging_benchmark);
		logging_benchmark = 1;
		benchfile = fopen(BENCHLOG_FILE_PATH, "w");
		if (benchfile == NULL) { printf("Error when opening file!\nCalls to memory mgmt will not be logged!\n"); exit(1); }
		else { printf("benchmark file is: %s\n", BENCHLOG_FILE_PATH); }
	}
	else {
		printf("normal mode\n\n");
	}

    int imgFlags = IMG_INIT_PNG;
    if(!(IMG_Init(imgFlags) & imgFlags)) {
        fatal("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
    }

    LevelLoader* level_loader = levelloader_alloc();
	  fill_level_loader(level_loader);

    //init the graphics system
    Graphics* graphics = graphics_alloc(0, 0);

    //initialise context, engine and assemblage, and add systems
    Game* pi_escape_2 = game_alloc(graphics);

	int level_nr = 0;															//THIS NUMBER DECIDES WHICH LEVEL IS LOADED, FOR TESTING, USE THIS!!
    Level* level = levelloader_load_level(level_loader, level_nr);
    game_load_level(pi_escape_2, level);

    Uint32 start_time_ms = SDL_GetTicks();
    Uint32 last_print_time_ms = start_time_ms;
    long update_count = 0;

	//PLAY THE LEVEL INTRO SCENE
	if (pi_escape_2->engine.input_system->intro_can_be_played) {
		pi_escape_2->engine.animation_system->intro_playing = 1;
	}
	else {
		player_blocked = 0;
	}

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
			player_blocked = 1;
			if (logging_benchmark) {
				fprintf(benchfile, "init\n");
			}
			sleep_ms(500);
			es_memory_manager_init(&(pi_escape_2->engine.es_memory));
			level_nr++;
			levelloader_free_level(level);
			level = levelloader_load_level(level_loader, level_nr);
			game_load_level(pi_escape_2, level);

			int width = level->breedte;
			int height = level->hoogte;

			//PLAY THE LEVEL INTRO SCENE
			if (pi_escape_2->engine.input_system->intro_can_be_played) {
				pi_escape_2->engine.animation_system->intro_playing = 1;
			}
			else {
				player_blocked = 0;
			}
			
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

	if (logging_benchmark) {
		fclose(benchfile);
	}

    return 0;
}
