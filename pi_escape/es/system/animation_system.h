#ifndef ANIMATION_SYSTEM_H
#define ANIMATION_SYSTEM_H
#define EMPTY_SYSTEM_STRUCT char c


typedef struct AnimationSystem {
	EMPTY_SYSTEM_STRUCT;
} AnimationSystem;

#include "../engine.h"

void system_animation_init(AnimationSystem*);
AnimationSystem* system_animation_alloc();
void system_animation_update(AnimationSystem*, Engine*);
void system_animation_free(AnimationSystem*);



#endif