#include "animation_system.h"
#include "../game.h"

// #updates/animation == #updates/second * #seconds/animation 
// increment/update == 1.0/(#updates/animation)
#define UPDATE_INCREMENT 0.05f

#if defined(RPI)
float intro_increment = 0.05f;
#else
float intro_increment = 0.01f;
#endif

#include <stdlib.h>
#include <assert.h>

AnimationSystem* system_animation_alloc() {
    AnimationSystem* res = calloc(1, sizeof(AnimationSystem));
    system_animation_init(res);
    return res;
}

void system_animation_init(AnimationSystem* system) {
	system->intro_playing = 0;

	system->look_left_1 = 0;
	system->look_left_1_timer = 0.0f;
	system->look_left_2 = 0;
	system->look_left_2_timer = 0.0f;
	system->look_right_1 = 0;
	system->look_right_1_timer = 0.0f;
	system->intro_wait_counter = 0;

	system->outro_playing = 0;
	system->zoom_timer = 0.0f;
}


void system_animation_free(AnimationSystem* system) {
    //TODO
}



void system_animation_update(AnimationSystem* system, Engine* engine) {
	
	if (system->intro_playing) {
		CameraLookFromComponent* cameraLookFrom = search_first_component(engine, COMP_CAMERA_LOOK_FROM);

		if (! system->look_left_1 && ! system-> look_right_1 && ! system->look_left_2 && ! system->waiting) {
			cameraLookFrom->Zdegrees = 90.0f;
			cameraLookFrom->distance = 1.0f;
			system->waiting = 1;
		}
		else if (system->waiting) {
			if (system->waiting_timer < 0.5f) {
				system->waiting_timer += intro_increment;
			}
			else {
				system->waiting = 0;
				system->waiting_timer = 0.0f;
				if (system->intro_wait_counter == 0) {
					system->intro_wait_counter += 1;
					system->look_left_1 = 1;
				}
				else if (system->intro_wait_counter == 1) {
					system->intro_wait_counter += 1;
					system->look_right_1 = 1;
				}
				else if (system->intro_wait_counter == 2) {
					system->intro_wait_counter += 1;
					system->look_left_2 = 1;
				}
				else if (system->intro_wait_counter == 3) {
					system->intro_wait_counter = 0;
					cameraLookFrom->distance = 15.0f;
					cameraLookFrom->XYdegees = 0.0f;
					cameraLookFrom->Zdegrees = 45.0f;
					system->intro_playing = 0;
					player_blocked = 0;
				}
			}
		}
		else if (system->look_left_1) {
			if (system->look_left_1_timer < 1.0f) {
				system->look_left_1_timer += intro_increment;
				cameraLookFrom->XYdegees += intro_increment * 90;
			}
			else {
				system->look_left_1 = 0;
				system->look_left_1_timer = 0.0f;
				system->waiting = 1;
			}
		}
		else if (system->look_right_1) {
			if (system->look_right_1_timer < 1.0f) {
				system->look_right_1_timer += intro_increment;
				cameraLookFrom->XYdegees -= intro_increment * 180;
			}
			else {
				system->look_right_1 = 0;
				system->look_right_1_timer = 0.0f;
				system->waiting = 1;
			}
		}
		else if (system->look_left_2) {
			if (system->look_left_2_timer < 1.0f) {
				system->look_left_2_timer += intro_increment;
				cameraLookFrom->XYdegees += intro_increment * 90;
			}
			else {
				system->look_left_2 = 0;
				system->look_left_2_timer = 0.0f;
				system->waiting = 1;
			}
		}
	}
}
