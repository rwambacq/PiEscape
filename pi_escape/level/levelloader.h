#ifndef LEVELLOADER_H
#define LEVELLOADER_H
#define TO_IMPLEMENT_STRUCT char c

#include <glmc.h>
#include "../es/game_util.h"

typedef enum ItemType { KEY } ItemType;
typedef enum ItemColor { O, A, B, C } ItemColor;

typedef struct Level {
    //TODO
	TO_IMPLEMENT_STRUCT;
} Level;

typedef struct LevelLoader {
    //TODO
	TO_IMPLEMENT_STRUCT;
} LevelLoader;


LevelLoader* levelloader_alloc();
void levelloader_free(LevelLoader*);

Level* levelloader_load_level(LevelLoader*, int level_nr);
void levelloader_free_level(Level*);

#endif //LEVELLOADER_H
