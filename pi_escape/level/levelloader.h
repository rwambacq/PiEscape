#ifndef LEVELLOADER_H
#define LEVELLOADER_H
#define TO_IMPLEMENT_STRUCT char c

#include <glmc.h>
#include "../es/game_util.h"

typedef enum ItemType { KEY } ItemType;
typedef enum ItemColor { O, A, B, C } ItemColor;

typedef struct Level {
	char level_description[64][64]; /* Since there is no way to dynamically allocate a size to the 2D array, I gave it a fixed size of 64x64 */
	int hoogte;
	int breedte;
	int nr;
} Level;

typedef struct LevelLoader {
	char level_paths[10][50];
} LevelLoader;


LevelLoader* levelloader_alloc();
void levelloader_free(LevelLoader*);

Level* levelloader_load_level(LevelLoader*, int level_nr);
void levelloader_free_level(Level*);

#endif //LEVELLOADER_H
