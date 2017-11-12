#include "levelloader.h"
#include "../../../util/util.h"

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>


void level_init(Level* level) {
    //TODO
}

Level* level_alloc() {
    Level* res = malloc(sizeof(Level));
    level_init(res);
    return res;
}

void levelloader_free_level(Level* l) {
	free(l);
}



LevelLoader* levelloader_alloc() {
    LevelLoader* res = malloc(sizeof(LevelLoader));
    return res;
}

void levelloader_free(LevelLoader* ll) {
    //TODO
}


Level* levelloader_load_level(LevelLoader* ll, int level_nr) {
    Level* level = level_alloc();
	/*****************************
	 Level Tile Legend:
	 /: Empty Tile
	 -: Connection Tile
	 W: Wall
	 P: Player
	 E: Exit
	 D: Door
	 &: AND-Connection
	 |: OR-Connection
	 A: Lock (Color A)
	 B: Lock (Color B)
	 C: Lock (Color C)
	 O: Lock (No Color, any key fits)
	 a: Key (Color A)
	 b: Key (Color B)
	 c: Key (Color C)
	 o: Key (Multicolor, fits any lock)
	******************************/

	int p, q;

	for (p = 0; p < 32; p++) {
		for (q = 0; q < 32; q++) {
			level->level_description[p][q] = NULL; /* Alle elementen van de 2D array op NULL zetten om garbage values te vermijden*/
		}
	}

	FILE *level_file;

	level_file = fopen(ll->level_paths[level_nr], "r");

	if (level_file == NULL) { printf("Error when opening file!\n"); exit(1); }
	int counter = 0;
	int longest = 0;
	char buffer[64];

	int length;
	int l;
	char current_char;

	while (fgets(buffer, sizeof(buffer), level_file)) {
		
		length = strlen(buffer);
		if (buffer[length - 1] == '\n') {
			buffer[length - 1] = NULL;
		}

		for (l = 0; l < length; l++) {
			current_char = buffer[l];

			if (current_char == ' ') {
				level->level_description[counter][l] = '/';
			} else if (current_char == '=' || current_char == '-' || current_char == '.') {
				level->level_description[counter][l] = '-';
			} else if (current_char == '*' || current_char == 'X' || current_char == 'x') {
				level->level_description[counter][l] = 'W';
			} else if (current_char == 'S' || current_char == 's') {
				level->level_description[counter][l] = 'P';
			} else if (current_char == 'E' || current_char == 'e') {
				level->level_description[counter][l] = 'E';
			} else if (current_char == 'D' || current_char == 'd' || current_char == '#') {
				level->level_description[counter][l] = 'D';
			} else {
				level->level_description[counter][l] = current_char;
			}
		}

		if (length > longest) {
			longest = length;
		}

		counter++;
	}

	level->breedte = longest-1;
	level->hoogte = counter;
	level->nr = level_nr;

	fclose(level_file);

	int i;
	int j;
	for (i = 0; i < level->hoogte; i++) { // Als er een rij te weinig komt te staan onderaan, ligt het hieraan
		for ( j = 0; j < level->breedte; j++ ) { // Als er een kolom te veel komt te staan rechts, ligt het hieraan
			if (level->level_description[i][j] == NULL) {
				level->level_description[i][j] = '/';
			}
		}
	}

    return level;
}
