#include "container_system.h"


#include <stdlib.h>
#include <stdio.h>

#include <assert.h>

ContainerSystem* system_container_alloc() {
    ContainerSystem* res = calloc(1, sizeof(ContainerSystem));
    system_container_init(res);
    return res;
}

void system_container_init(ContainerSystem* system) {
    //TODO
}


void system_container_free(ContainerSystem* system) {
    //TODO
}

void system_container_update(ContainerSystem* system, Engine* engine) {
    //TODO
}
