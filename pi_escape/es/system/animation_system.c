#include "animation_system.h"


#include <stdlib.h>
#include <assert.h>

AnimationSystem* system_animation_alloc() {
    AnimationSystem* res = calloc(1, sizeof(AnimationSystem));
    system_animation_init(res);
    return res;
}

void system_animation_init(AnimationSystem* system) {
    //TODO
}


void system_animation_free(AnimationSystem* system) {
    //TODO
}



void system_animation_update(AnimationSystem* system, Engine* engine) {
    //TODO
}
