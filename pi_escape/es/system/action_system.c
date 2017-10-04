#include "action_system.h"

#include <assert.h>

#include <stdlib.h>

ActionSystem* system_action_alloc() {
    ActionSystem* res = calloc(1, sizeof(ActionSystem));
    system_action_init(res);
    return res;
}

void system_action_init(ActionSystem* system) {
    //TODO
}


void system_action_free(ActionSystem* system) {
    //TODO
}

void system_action_update(ActionSystem* system, Engine* engine) {
    //TODO
}
