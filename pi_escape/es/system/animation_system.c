#include "animation_system.h"

// #updates/animation == #updates/second * #seconds/animation 
// increment/update == 1.0/(#updates/animation)
#define UPDATE_INCREMENT 0.05

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
	// search for all entities with a OneTimeAnimationComponent (COMP_ONE_TIME_ANIMATION)
	ComponentIterator it;
	OneTimeAnimationComponent* curr_ot_comp;
	MoveAnimationComponent* curr_mov_comp;

	search_component(engine, COMP_ONE_TIME_ANIMATION, &it);
	while (next_component(&it)) {
		curr_ot_comp = (OneTimeAnimationComponent*) it.comp;
		curr_ot_comp->progress += UPDATE_INCREMENT;
		if (curr_ot_comp->progress == 1) {
			// remove the animation component
			free_component(engine, it.entity_id, it.component_id);
		}
	}

	search_component(engine, COMP_MOVE_ANIMATION, &it);
	while (next_component(&it)) {
		curr_mov_comp = (MoveAnimationComponent*)it.comp;
		curr_mov_comp->progress += UPDATE_INCREMENT;
		if (curr_mov_comp->progress == 1) {
			// remove the animation component
			free_component(engine, it.entity_id, it.component_id);
		}
	}
}
