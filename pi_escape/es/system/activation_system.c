#include "activation_system.h"


#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

ActivationSystem* system_activation_alloc() {
    ActivationSystem* res = calloc(1, sizeof(ActivationSystem));
    system_activation_init(res);
    return res;
}

void system_activation_init(ActivationSystem* system) {
    //TODO
}


void system_activation_free(ActivationSystem* system) {
    //TODO
}


void system_activation_update(ActivationSystem* system, Engine* engine) {
    //TODO
}
