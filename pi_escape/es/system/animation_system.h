#ifndef ANIMATION_SYSTEM_H
#define ANIMATION_SYSTEM_H
#define EMPTY_SYSTEM_STRUCT char c


typedef struct AnimationSystem {
	int intro_playing;

	int waiting;
	float waiting_timer;

	int intro_wait_counter;

	int look_left_1;
	float look_left_1_timer;
	int look_right_1;
	float look_right_1_timer;
	int look_left_2;
	float look_left_2_timer;

} AnimationSystem;

#include "../engine.h"

void system_animation_init(AnimationSystem*);
AnimationSystem* system_animation_alloc();
void system_animation_update(AnimationSystem*, Engine*);
void system_animation_free(AnimationSystem*);



#endif