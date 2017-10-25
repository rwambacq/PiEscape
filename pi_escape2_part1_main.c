#define __STDC_FORMAT_MACROS
#include <inttypes.h>
#include <stdio.h>
#include "util/sleep.h"
#include "pi_escape/graphics/opengl_game_renderer.h"
#include "pi_escape/level/levelloader.h"
#include "pi_escape/es/game.h"

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

    Level* level = levelloader_load_level(level_loader, 0);
    game_load_level(pi_escape_2, level);

	int width = level->breedte;
	int height = level->hoogte;

	int s;
	for (s = 0; s < height; s++) {
		printf("%s\n", level->level_description[s]);
	}

    //TODO: support playing all levels in sequence

    Uint32 start_time_ms = SDL_GetTicks();
    Uint32 last_print_time_ms = start_time_ms;
    long update_count = 0;

    while (!pi_escape_2->engine.context.is_exit_game) {
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
