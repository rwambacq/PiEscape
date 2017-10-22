#ifndef STDIO_INCLUDED
#include <stdio.h>
#define STDIO_INCLUDED
#endif

#include "util/sleep.h"
#include "pi_escape/graphics/opengl_game_renderer.h"
#include "pi_escape/level/levelloader.h"
#include "pi_escape/es/game.h"

#include "pi_escape/es/es_memory_manager.h"

#include <SDL.h>
#undef main //Weird bug on windows where SDL overwrite main definition
#include <SDL_timer.h>

int my_pow(int base, int power) {
	int res = 1;
	int i;
	for (i = 0; i < power; i++) { res *= base; }
	return res;
}

int str_to_int(char* s, int size) {
	int res = 0;
	for (--size; size >= 0; size--) {
		res += ((int)(*s) - 48) * my_pow(10, size);
		s++;
	}
	return res;
}

Uint32 tick() {
	return SDL_GetTicks();
}

Uint32 tock(Uint32 tic) {
	return SDL_GetTicks() - tic;
}

void crunch_line(Engine* engine, char* line) {
	int len = (int) strlen(line);
	char a_str[10];
	char b_str[10];
	int i = 0;
	for (i = 0; i < 10; i++) { a_str[i] = 'e'; b_str[i] = 'e'; }
	int ai = 0;
	int bi = 0;
	int a=0; // first arg for create_component
	int b=0; // second arg for create_component
	Uint32 tic;
	Uint32 delta;
	/*if (!strncmp("es_memory_manager_init", line, (int) sizeof("es_memory_manager_init") / sizeof(char) - 1)) {
		es_memory_manager_init(engine);
	}
	else */if (!strncmp("get_new_entity_id", line, (int) sizeof("get_new_entity_id") / sizeof(char) - 1)) {
		tic = tick(); // start time measuring
		get_new_entity_id(engine);		
		delta = tock(tic); // stop time measuring
		printf("get_new_entity_id(%p) took %d milliseconds to execute\n", engine, delta);
	}
	else if (!strncmp("create_component", line, (int) sizeof("create_component") / sizeof(char) - 1)) {
		line += (int) sizeof("create_component(Engine*") / sizeof(char);
		while (*line != '\0') {
			if (a_str[ai] == '\0') { // build b
				if (*line != ')') {
					b_str[bi++] = *line;
					line++;
				}
				else {
					b_str[bi] = '\0';
					line++;
				}
			}
			else { // build a
				if (*line != ',') {
					a_str[ai++] = *line;
					line++;
				}
				else {
					a_str[ai] = '\0';
					line++;
				}
			}
		}

		a = str_to_int(a_str, strlen(a_str));
		b = str_to_int(b_str, strlen(b_str));
		tic = tick(); // start time measuring
		create_component(engine, a, b);
		delta = tock(tic); // stop time measuring
		printf("create_component(%p %d, %d) took %d milliseconds to execute\n", engine, a, b, delta);
	}
}

int main(int argc, char **argv){
	// variable declarations;
	FILE* f;
	char line[50];
	LevelLoader* level_loader;
	Graphics* graphics;
	Game* pi_escape_2;


	printf("Running Benchmarks...\n");
	running_benchmark = 1; // ! don't allow a call from game_alloc to utilize es_memory_manager_init when benchmarking

	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags)) {
		fatal("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
	}

	level_loader = levelloader_alloc();

	//init the graphics system
	graphics = graphics_alloc(0, 0);

	//initialise context, engine and assemblage, and add systems
	pi_escape_2 = game_alloc(graphics, &running_benchmark);

	// see whether benchlog file has been provided
	if (argc == 1) { // no arguments have been provided (count == 1; counts function name itself as arg) 
		printf("\nNo argument provided as benchlog file!\nQuitting benchmark...\n\n");
		exit(1);
	}
	else {
		printf("Benchlog file provided\nOpening <<%s>>\n", argv[1]);
	}
	
	// open provided file
	f = fopen(*(argv + 1), "r");
	if (f == NULL) {
		printf("Error when reading provided file!\nQuitting benchmark...\n");
	}
	else {
		printf("File opened successfully\nNow running all function calls\n");
	}

	// run function calls being read from benchlog file sequentially
	// TODO clean up spaghetti code!

	while (!feof(f)) {
		fscanf(f, "%s", line);
		printf("Executing: ");
		printf(line);
		printf("\n");
		crunch_line(&pi_escape_2->engine, line);
	}

	fclose(f);
	
    game_free(pi_escape_2);
	free(pi_escape_2);

	graphics_free(graphics);
	free(graphics);

	levelloader_free(level_loader);
	free(level_loader);

    return 0;
}
