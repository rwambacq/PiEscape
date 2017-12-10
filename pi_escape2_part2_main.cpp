#define __STDC_FORMAT_MACROS

#ifdef __cplusplus
extern "C"
{
#endif

#include "pi_escape/graphics/opengl_game_renderer.h"
#include "pi_escape/es/game.h"


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

#ifdef __cplusplus
}
#endif


#include <SDL.h>
#undef main //Weird bug on windows where SDL overwrite main definition
#include <SDL_timer.h>
#include "util/sleep.h"
#include "pi_escape/anim/FontManager.h"
#include "pi_escape/anim/GameUICreator.h"
#include <string>
#include <cassert>
#include <string>
#include <cassert>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <thread>
#include <bitset>
#include <iterator>
#include "util/sleep.h"
#include "pi_escape/led/ledcontrol.h"
#include "pi_escape/led/fake_led.h"

using namespace std;

int getal = 1;
int soort_animatie;

void startGameFromLevel(int level, Graphics* graphics);
void fill_level_loader(LevelLoader* level_loader);

int main() {
	t_vec4 col = { 1.0f, 0.0f, 0.0f, 1.0f };

	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags)) {
		fatal("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
	}

	Graphics* graphics = graphics_alloc(0, 0);
	GLGlyph glGlyph;

	FontManager lettertypeToezichthouder(graphics, &glGlyph);

	bool done = false;
	while (!done) {

		// HERE IS WHERE THE INTRO MOVIE SHOULD BE PLAYED

		// START MENU
		lettertypeToezichthouder.setColor(col);
		lettertypeToezichthouder.loadFont("arcade72", "pi_escape/graphics/arcade72.png", "pi_escape/graphics/arcade72.fnt");

		gl_glyph_init(&glGlyph, graphics, (char*)lettertypeToezichthouder.getFontImageFilename().c_str());

		shared_ptr<MenuDefinition> menudef = GameUICreator(lettertypeToezichthouder).createGameMenu();
		vector<MenuItem> items = (*menudef).getMenuItems();

		MenuController controller;
		controller.menuLoop(&items, &lettertypeToezichthouder);

		// CHECK SELECTION AND EXECUTE ACTION ACCORDINGLY

		switch (controller.getMenuSelection()) {
		case 0:
			startGameFromLevel(0, graphics);
			//PLAY OUTRO MOVIE AFTER THIS
			break;
		case 1:
			startGameFromLevel(7, graphics);
			//PLAY OUTRO MOVIE AFTER THIS
			break;
		case 2:
			done = true;
			break;
		}
		glmc_vec3_set(graphics->background_color, 0.0f, 0.0f, 0.0f);

	}
	gl_glyph_free(&glGlyph);
	graphics_free(graphics);
	free(graphics);

	return 0;
}

void startGameFromLevel(int lvl, Graphics* graphics) {
	player_blocked = 1;
	// if you call the main game with more than one argument, assume it is benchmarking.
	/*if (argc > 1) {
	printf("benchmark mode\n\n%d\n", logging_benchmark);
	logging_benchmark = 1;
	benchfile = fopen(BENCHLOG_FILE_PATH, "w");
	if (benchfile == NULL) { printf("Error when opening file!\nCalls to memory mgmt will not be logged!\n"); exit(1); }
	else { printf("benchmark file is: %s\n", BENCHLOG_FILE_PATH); }
	}
	else {
	printf("normal mode\n\n");
	}*/

	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags)) {
		fatal("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
	}

	LevelLoader* level_loader = levelloader_alloc();
	fill_level_loader(level_loader);

	//initialise context, engine and assemblage, and add systems
	Game* pi_escape_2 = game_alloc(graphics);

	int level_nr = lvl;
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

		ExitComponent* exit_comp = static_cast<ExitComponent*>(get_component(&pi_escape_2->engine, exit_id, COMP_EXIT));
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

	levelloader_free(level_loader);
	free(level_loader);

	/*if (logging_benchmark) {
	fclose(benchfile);
	}*/
}

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

void toonLed(int geval) {
	string c;
	SPGM_RGBTRIPLE g = { 250,0,0 };
	SPGM_RGBTRIPLE bl = { 0,0,0 };
	SPGM_RGBTRIPLE zl = { 0,0,250 };
	//getal = 0;

	SPGM_RGBTRIPLE gridzwart[8][8];

	if (geval == 0) {
		c = "Start Tutorial";
		SPGM_RGBTRIPLE grid[8][8] = {
			{ bl,bl,bl,bl,bl,bl,bl,bl },
			{ bl,bl,bl,bl,bl,bl,bl,bl },
			{ bl,bl,bl,bl,bl,bl,bl,bl },
			{ bl,bl,bl,bl,bl,bl,bl,bl },
			{ bl,bl,bl,bl,bl,bl,bl,bl },
			{ bl,bl,bl,bl,bl,bl,bl,bl },
			{ bl,bl,bl,bl,bl,bl,bl,bl },
			{ bl,bl,zl,zl,zl,zl,bl,bl } };
		for (int i = 0; i < 8; i++) {
			for (int x = 0; x < 8; x++) {
				gridzwart[i][x] = grid[i][x];
			}
		}

	}
	else if (geval == 1) {
		c = "Start Game";
		SPGM_RGBTRIPLE grid[8][8] = {
			{ bl,bl,bl,zl,zl,bl,bl,bl },
			{ bl,bl,bl,bl,bl,bl,bl,bl },
			{ bl,bl,bl,bl,bl,bl,bl,bl },
			{ bl,bl,bl,bl,bl,bl,bl,bl },
			{ bl,bl,bl,bl,bl,bl,bl,bl },
			{ bl,bl,bl,bl,bl,bl,bl,bl },
			{ bl,bl,bl,bl,bl,bl,bl,bl },
			{ bl,bl,bl,zl,zl,bl,bl,bl } };
		for (int i = 0; i < 8; i++) {
			for (int x = 0; x < 8; x++) {
				gridzwart[i][x] = grid[i][x];
			}
		}
	}
	else {
		c = "Exit";
		SPGM_RGBTRIPLE grid[8][8] = {
			{ bl,bl,zl,zl,zl,zl,bl,bl },
			{ bl,bl,bl,bl,bl,bl,bl,bl },
			{ bl,bl,bl,bl,bl,bl,bl,bl },
			{ bl,bl,bl,bl,bl,bl,bl,bl },
			{ bl,bl,bl,bl,bl,bl,bl,bl },
			{ bl,bl,bl,bl,bl,bl,bl,bl },
			{ bl,bl,bl,bl,bl,bl,bl,bl },
			{ bl,bl,bl,bl,bl,bl,bl,bl } };
		for (int i = 0; i < 8; i++) {
			for (int x = 0; x < 8; x++) {
				gridzwart[i][x] = grid[i][x];
			}
		}
	}


	unsigned int** arrays = new unsigned int*[79];

	//lezen font

	std::ifstream is("pi_escape/led/TinyFont", std::ifstream::binary);
	if (is) {
		int x = 0;
		unsigned char * buffer = new unsigned char[3];
		while (x < 237) {
			is.read((char*)buffer, 3);
			unsigned int *toevoeg = new unsigned int[2];
			toevoeg[0] = (unsigned int)buffer[1];
			toevoeg[1] = (unsigned int)buffer[2];
			arrays[(unsigned int)buffer[0] - 48] = toevoeg;
			x += 3;
		}
		delete[] buffer;
		is.close();
	}


	//opvullen voor letter

	for (std::string::iterator it = c.begin(); it != c.end(); ++it) {
		SPGM_RGBTRIPLE gridgroen[8][4];

		if (*it == ' ') {

			SPGM_RGBTRIPLE gridgoen[8][4] = {
				{ bl,bl,bl,bl },
				{ bl,bl,bl,bl },
				{ bl,bl,bl,bl },
				{ bl,bl,bl,bl },
				{ bl,bl,bl,bl },
				{ bl,bl,bl,bl },
				{ bl,bl,bl,bl },
				{ bl,bl,bl,bl } };

			for (int i = 0; i < 8; i++) {
				for (int x = 0; x < 4; x++) {
					gridgroen[i][x] = gridgoen[i][x];
				}
			}
		}
		else {
			unsigned int xx = arrays[((int)*it) - 48][1];
			unsigned int cc = arrays[((int)*it) - 48][0];

			std::string binary = std::bitset<8>(xx).to_string(); //to binary
			std::string binary2 = std::bitset<8>(cc).to_string(); //to binary
			string totaal = binary.substr(1, 8) + binary2;



			for (int y = 0; y < totaal.length(); y += 3) {
			}
			int i = 0;

			for (int x = 0; x < 4; x++) {
				gridgroen[i][x] = bl;
			}
			i++;

			if (binary.substr(0, 1) == "1") {
				for (int x = 0; x < 4; x++) {
					gridgroen[i][x] = bl;
				}
				i++;
			}

			for (int y = 0; y < totaal.length(); y += 3) {
				int xy = i;
				int x = 0;
				string str = totaal.substr(y, 3);
				for (std::string::iterator it = str.begin(); it != str.end(); ++it) {

					if (*it == '1') {
						gridgroen[i][x] = g;
					}
					else {
						gridgroen[i][x] = bl;
					}
					x += 1;
				}

				int yx = i;
				for (x; x < 4; x++) {
					gridgroen[i][x] = bl;
				}
				i++;
			}

			int iy = i;
			for (int y = 0; y < 4; y++) {
				gridgroen[i][y] = bl;
			}
			i++;

			if (binary.substr(0, 1) == "0") {
				for (int y = 0; y < 4; y++) {
					gridgroen[i][y] = bl;
				}
			}

		}

		showColor2(gridzwart);
		//aantal keer

		//while komt hierrond

		for (int x = 0; x < 4; x++) {
			//shiften
			for (int y = 1; y < 7; y++) {
				for (int i = 0; i < 8 - 1; i++)
				{
					gridzwart[y][i] = gridzwart[y][i + 1];
				}
				gridzwart[y][7] = gridgroen[y][x];
			}
			showColor2(gridzwart);
			sleep_ms(200);


		}
	}

	//opkuis van led
	for (int x = 0; x < 8; x++) {
		//shiften
		for (int y = 1; y < 7; y++) {
			for (int i = 0; i < 8 - 1; i++)
			{
				gridzwart[y][i] = gridzwart[y][i + 1];
			}
			gridzwart[y][7] = bl;
		}
		showColor2(gridzwart);
		sleep_ms(200);
	}
}