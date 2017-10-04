#include "orientation_system.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

OrientationSystem* system_orientation_alloc() {
    OrientationSystem* res = calloc(1, sizeof(OrientationSystem));
    system_orientation_init(res);
    return res;
}

void system_orientation_init(OrientationSystem* system) {
    //TODO
}


void system_orientation_free(OrientationSystem* system) {
    //TODO
}

void system_orientation_update(OrientationSystem* system, Engine* engine) {
    //TODO
}
