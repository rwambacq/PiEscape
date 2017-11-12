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

void set_int_args_1(char* line, char* a_str) {
	int ai = 0;
	while (*line != '\0' && a_str[ai] != '\0') {
		if (*line != ',' && *line != ')') { // build a
			a_str[ai++] = *line;
			line++;
		}
		else {
			a_str[ai] = '\0';
			line++;
		}
	}
}

void set_int_args_2(char* line, char* a_str, char* b_str) {
	int ai = 0, bi = 0;
	while (*line != '\0' && b_str[bi] != '\0') {
		if (a_str[ai] == '\0') { // build b
			if (*line != ')' && *line != ',') {
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
}

void set_int_args_3(char* line, char* a_str, char* b_str, char* c_str) {
	int ai = 0, bi = 0, ci = 0;
	while (*line != '\0' && c_str[ci] != '\0') {
		if (a_str[ai] == '\0' && b_str[bi] == '\0') { // build c
			if (*line != ')' && *line != ',') {
				c_str[ci++] = *line;
				line++;
			}
			else {
				c_str[ci] = '\0';
				line++;
			}
		}
		else if (a_str[ai] == '\0') { // build b
			if (*line != ')' && *line != ',') {
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
}

Uint32 tick() {
	return SDL_GetTicks();
}

Uint32 tock(Uint32 tic) {
	return SDL_GetTicks() - tic;
}

// do all the dirty work
void crunch_line(Engine* engine, char* line) {
	int len = (int) strlen(line);
	char a_str[10];
	char b_str[10];
	char c_str[10];
	int i = 0;
	for (i = 0; i < 10; i++) { a_str[i] = 'e'; b_str[i] = 'e'; c_str[i] = 'e'; }
	int a = 0; // integer argument placeholder
	int b = 0; // integer argument placeholder
	int c = 0; // integer argument placeholder
	if (!strncmp("get_new_entity_id", line, (int) sizeof("get_new_entity_id") / sizeof(char) - 1)) {
		get_new_entity_id(engine);		
	}
	else if (!strncmp("init", line, (int) sizeof("init") / sizeof(char) - 1)) {
		es_memory_manager_init(&engine->es_memory);
	}
	else if (!strncmp("create_component", line, (int) sizeof("create_component") / sizeof(char) - 1)) {
		line += (int) sizeof("create_component(Engine*") / sizeof(char);		
		set_int_args_2(line, a_str, b_str);
		a = str_to_int(a_str, strlen(a_str));
		b = str_to_int(b_str, strlen(b_str));
		create_component(engine, a, b);
	}
	else if (!strncmp("get_component", line, (int) sizeof("get_component") / sizeof(char) - 1)) {
		line += (int) sizeof("get_component(Engine*") / sizeof(char);
		set_int_args_2(line, a_str, b_str);
		//printf("a_str = %s, b_str = %s\n", a_str, b_str);
		a = str_to_int(a_str, strlen(a_str));
		b = str_to_int(b_str, strlen(b_str));
		get_component(engine, a, b);
	}
	else if (!strncmp("search_first_entity_1", line, (int) sizeof("search_first_entity_1") / sizeof(char) - 1)){
		line += (int) sizeof("search_first_entity_1(Engine*") / sizeof(char);
		set_int_args_1(line, a_str);
		//printf("a_str = %s\n", a_str);
		a = str_to_int(a_str, strlen(a_str));
		search_first_entity_1(engine, a);
	}
	else if (!strncmp("search_first_entity_2", line, (int) sizeof("search_first_entity_2") / sizeof(char) - 1)) {
		line += (int) sizeof("search_first_entity_2(Engine*") / sizeof(char);
		set_int_args_2(line, a_str, b_str);
		//printf("a_str = %s, b_str = %s\n", a_str, b_str);
		a = str_to_int(a_str, strlen(a_str));
		b = str_to_int(b_str, strlen(b_str));
		search_first_entity_2(engine, a, b);
	}
	else if (!strncmp("search_first_entity_3", line, (int) sizeof("search_first_entity_3") / sizeof(char) - 1)) {
		line += (int) sizeof("search_first_entity_3(Engine*") / sizeof(char);
		set_int_args_3(line, a_str, b_str, c_str);
		// printf("a_str = %s, b_str = %s, c_str = %s\n", a_str, b_str, c_str);
		a = str_to_int(a_str, strlen(a_str));
		b = str_to_int(b_str, strlen(b_str));
		c = str_to_int(c_str, strlen(c_str));
		search_first_entity_3(engine, a, b, c);
	}
	else if (!strncmp("search_entity_1", line, (int) sizeof("search_entity_1") / sizeof(char) - 1)) {
		line += (int) sizeof("search_entity_1(Engine*") / sizeof(char);
		set_int_args_1(line, a_str);
		//printf("a_str = %s\n", a_str);
		a = str_to_int(a_str, strlen(a_str));
		search_entity_1(engine, a, bench_ent_it_ptr);
	}
	else if (!strncmp("search_entity_2", line, (int) sizeof("search_entity_2") / sizeof(char) - 1)) {
		line += (int) sizeof("search_entity_2(Engine*") / sizeof(char);
		set_int_args_2(line, a_str, b_str);
		//printf("a_str = %s, b_str = %s\n", a_str, b_str);
		a = str_to_int(a_str, strlen(a_str));
		b = str_to_int(b_str, strlen(b_str));
		search_entity_2(engine, a, b, bench_ent_it_ptr);
	}
	else if (!strncmp("search_entity_3", line, (int) sizeof("search_entity_3") / sizeof(char) - 1)) {
		line += (int) sizeof("search_entity_3(Engine*") / sizeof(char);
		set_int_args_3(line, a_str, b_str, c_str);
		//printf("a_str = %s, b_str = %s, c_str = %s\n", a_str, b_str, c_str);
		a = str_to_int(a_str, strlen(a_str));
		b = str_to_int(b_str, strlen(b_str));
		c = str_to_int(c_str, strlen(c_str));
		search_entity_3(engine, a, b, c, bench_ent_it_ptr);
	}
	else if (!strncmp("next_entity", line, (int) sizeof("next_entity") / sizeof(char) - 1)) {
		next_entity(bench_ent_it_ptr);
	}
	else if (!strncmp("search_entity", line, (int) sizeof("search_entity") / sizeof(char) - 1)) {
		line += (int) sizeof("search_entity(Engine*") / sizeof(char);
		set_int_args_1(line, a_str);
		//printf("a_str = %s\n", a_str);
		a = str_to_int(a_str, strlen(a_str));
		search_entity(engine, a, bench_ent_it_ptr);
	}
	else if (!strncmp("search_first_component", line, (int) sizeof("search_first_component") / sizeof(char) - 1)) {
		line += (int) sizeof("search_first_component(Engine*") / sizeof(char);
		set_int_args_1(line, a_str);
		//printf("a_str = %s\n", a_str);
		a = str_to_int(a_str, strlen(a_str));
		search_first_component(engine, a);
	}
	else if (!strncmp("next_component", line, (int) sizeof("next_component") / sizeof(char) - 1)) {
		next_component(bench_comp_it_ptr);
	}
	else if (!strncmp("search_component", line, (int) sizeof("search_component") / sizeof(char) - 1)) {
		line += (int) sizeof("search_component(Engine*") / sizeof(char);
		set_int_args_1(line, a_str);
		//printf("a_str = %s\n", a_str);
		a = str_to_int(a_str, strlen(a_str));
		search_component(engine, a, bench_comp_it_ptr);
	}
}

int main(int argc, char **argv){
	// variable declarations;
	Uint32 tic;
	Uint32 toc;
	unsigned long line_count = 0;
	unsigned long i = 1;
	FILE* f;
	EntityIterator bench_ent_it;
	bench_ent_it_ptr = &bench_ent_it;
	ComponentIterator bench_comp_it;
	bench_comp_it_ptr = &bench_comp_it;
	char line[50];
	LevelLoader* level_loader;
	Graphics* graphics;
	Game* pi_escape_2;


	printf("Running Benchmarks...\n");
	running_benchmark = 1;

	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags)) {
		fatal("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
	}

	level_loader = levelloader_alloc();

	//init the graphics system
	graphics = graphics_alloc(0, 0);

	//initialise context, engine and assemblage, and add systems
	pi_escape_2 = game_alloc(graphics);

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
		exit(2);
	}
	else {
		printf("File opened successfully\n");
	}

	printf("Counting amount of lines in provided file...\n");
	while (!feof(f)) {
		fscanf(f, "%s", line);
		line_count++;
	}
	fclose(f);
	printf("Done\n");
	
	f = fopen(*(argv + 1), "r");
	printf("Now running all %u calls\n", line_count);
	// run function calls being read from benchlog file sequentially
	tic = tick();
	while (!feof(f)) {
		fscanf(f, "%s", line);
		crunch_line(&pi_escape_2->engine, line);
		i++;
		if (!(i % 100000)) {
			+printf("Running...%.2f%%\n", roundf((float)i / (float)line_count * 10000) / 100);
		}
	}
	toc = tock(tic);
	printf("Program took %f seconds to execute under synthetic load.\n", toc / 1000.0f);
	printf("The application took up %u bytes of space in the system memory.\n", sizeof(AllComponent)*COMPONENT_ID_SIZE*curr_max_entities + sizeof(int));
	printf("sizeof(AllComponent)=%u\n", sizeof(AllComponent));
	printf("matrix size: %d x %d\n", COMPONENT_ID_SIZE, curr_max_entities);

	fclose(f);
	
    game_free(pi_escape_2);
	free(pi_escape_2);

	graphics_free(graphics);
	free(graphics);

	levelloader_free(level_loader);
	free(level_loader);

    return 0;
}
