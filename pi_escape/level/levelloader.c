#include "levelloader.h"
#include "../../../util/util.h"

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>


void level_init(Level* level, int width, int height, int nr) {
    //TODO
}

Level* level_alloc(int width, int height, int nr) {
    Level* res = malloc(sizeof(Level));
    level_init(res, width, height, nr);
    return res;
}

void levelloader_free_level(Level* l) {
    //TODO
}



LevelLoader* levelloader_alloc() {
    LevelLoader* res = malloc(sizeof(LevelLoader));
    return res;
}

void levelloader_free(LevelLoader* ll) {
    //TODO
}


Level* levelloader_load_level(LevelLoader* ll, int level_nr) {
    Level* level = level_alloc(5, 5, 0);

    //TODO

    return level;
}
