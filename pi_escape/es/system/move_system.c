#include "move_system.h"

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

#define PLAYER_MOVE_MS 100

MoveSystem* system_move_alloc() {
    MoveSystem* res = calloc(1, sizeof(MoveSystem));
    system_move_init(res);
    return res;
}

void system_move_init(MoveSystem* system) {
    //TODO
}


void system_move_free(MoveSystem* system) {
    //TODO
}

void system_move_update(MoveSystem* system, Engine* engine) {
    //TODO
}
