#include "endlevel_system.h"


#include <stdlib.h>

#include <stdio.h>
#include <assert.h>

EndLevelSystem* system_endlevel_alloc() {
    EndLevelSystem* res = calloc(1, sizeof(EndLevelSystem));
    system_endlevel_init(res);
    return res;
}

void system_endlevel_init(EndLevelSystem* system) {
    //TODO
}


void system_endlevel_free(EndLevelSystem* system) {
    //TODO
}

void system_endlevel_update(EndLevelSystem* system, Engine* engine) {
    //TODO
}
