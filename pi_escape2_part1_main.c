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

/*
runs game while also keeping track of how many updates have been done every second, then print fps statistics.
*/
void run_game_print_perf_stat(Game* game) {
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

void run_game(Game* game) {
	while (!game->engine.context.is_exit_game) {
		engine_update(&(game->engine));
	}
}

int main(int argc, char **argv) {
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

    Level* level = levelloader_load_level(level_loader, 0);
    game_load_level(pi_escape_2, level);

	int width = level->breedte;
	int height = level->hoogte;

	int s;
	for (s = 0; s < height; s++) {
		printf("%s\n", level->level_description[s]);
	}

    //TODO: support playing all levels in sequence

	run_game(pi_escape_2);
	//run_game_print_perf_stat(pi_escape_2);

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
