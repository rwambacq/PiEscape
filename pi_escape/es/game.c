#include "game.h"

#include <stdlib.h>

void game_init(Game* game, Graphics* graphics, int* running_benchmark) {
    game->graphics = graphics;
    engine_init(&game->engine, graphics, running_benchmark);
}

Game* game_alloc(Graphics* graphics, int* running_benchmark) {
    Game* res = malloc(sizeof(Game));
    game_init(res, graphics, running_benchmark);
    return res;
}

void game_free(Game* game) {
    engine_free(&game->engine);
    //don't free graphics
}

void game_load_level(Game* g, Level* l) {
    //TODO: use assembly to create all entities needed for the level
}
