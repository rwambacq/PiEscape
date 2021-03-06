#include "input_system.h"

#include <SDL.h>
#include <SDL_timer.h>
#include <SDL_video.h>

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#include "../game_util.h"
#include "../game.h"


InputSystem* system_input_alloc() {
	InputSystem* res = calloc(1, sizeof(InputSystem));
	system_input_init(res);
	return res;
}

void system_input_init(InputSystem* system) {
	system->sensorField = 3;
	system->intro_can_be_played = 1;
}


void system_input_free(InputSystem* system) {

}

static void handleKeyDown(InputSystem* system, Engine* engine, SDL_keysym *keysym, EntityId input_recv_entity_id)
{
	switch (keysym->sym) {
	case SDLK_ESCAPE:
		//ignore untile key released
		break;
	case SDLK_KP_ENTER: //fall-through
	case SDLK_RETURN:   //fall-through
	case SDLK_SPACE: {
		//engine->context.demo = !engine->context.demo;
		break;
	}
	default:
		break;
	}




}

static void handleKeyUp(InputSystem* system, Engine* engine, SDL_keysym *keysym, EntityId inputReceiverEntity)
{
	EntityIterator player_it;
	EntityIterator itdoor;
	search_entity_2(engine, COMP_GRIDLOCATION, COMP_INPUTRECEIVER, &player_it);
	next_entity(&player_it);
	EntityId player_entity_id = player_it.entity_id;
	assert(player_entity_id != NO_ENTITY);
		switch (keysym->sym) {
		case SDLK_o:
			//key die deuren opent of sluit, handig voor debug
			search_entity_3(engine, COMP_ART, COMP_ISDOOR, COMP_DIRECTION, &itdoor);
			while (next_entity(&itdoor)) {
				EntityId door = itdoor.entity_id;
				assert(door != NO_ENTITY);
				ActivatableComponent* xx = get_component(engine, door, COMP_ACTIVATABLE);
				xx->active = !xx->active;
			}
			break;
		case SDLK_i:
			//hiermee wordt het spelen van een intro aan het begin van elk level getoggeld
			system->intro_can_be_played = !system->intro_can_be_played;
			break;
		case SDLK_ESCAPE:
			engine->context.is_exit_game = 1;
			break;
		case SDLK_UP: {
			//engine->context.demo = !engine->context.demo;
			MoveActionComponent* move = create_component(engine, player_entity_id, COMP_MOVE_ACTION);
			move->x_min_move = 1;
			break;
		}
		case SDLK_DOWN: {
			//engine->context.demo = !engine->context.demo;
			MoveActionComponent* move = create_component(engine, player_entity_id, COMP_MOVE_ACTION);
			move->x_plus_move = 1;
			break;
		}
		case SDLK_LEFT: {
			//engine->context.demo = !engine->context.demo;
			MoveActionComponent* move = create_component(engine, player_entity_id, COMP_MOVE_ACTION);
			move->y_plus_move = 1;
			break;
		}
		case SDLK_RIGHT: {
			//engine->context.demo = !engine->context.demo;
			MoveActionComponent* move = create_component(engine, player_entity_id, COMP_MOVE_ACTION);
			move->y_min_move = 1;
			break;
		}
		case SDLK_KP_ENTER: {
			//engine->context.demo = !engine->context.demo;
			EntityIterator itlock;
			search_entity_3(engine, COMP_ACTIVATABLE, COMP_ART, COMP_ACTIVATION, &itlock);
			int next = 1;
			if (next_entity(&itlock)) {
				EntityId lockje = itlock.entity_id;
				assert(lockje != NO_ENTITY);
				ActivationComponent* aanmaken = get_component(engine, lockje, COMP_ACTIVATION);
				next = 0;
			}
			if (next) {
				ItemActionComponent* action = create_component(engine, player_entity_id, COMP_ITEMACTION);
			}
		}
		case SDLK_RETURN: {
			//engine->context.demo = !engine->context.demo;
			EntityIterator itlock;
			search_entity_3(engine, COMP_ACTIVATABLE, COMP_ART, COMP_ACTIVATION, &itlock);
			int next = 1;
			if (next_entity(&itlock)) {
				EntityId lockje = itlock.entity_id;
				assert(lockje != NO_ENTITY);
				ActivationComponent* aanmaken = get_component(engine, lockje, COMP_ACTIVATION);
				next = 0;
			}
			if (next) {
				ItemActionComponent* action = create_component(engine, player_entity_id, COMP_ITEMACTION);
			}
			break;
		}
#ifndef RPI
		case SDLK_h: {
			system->sensorField = 0;
			break;
		}
		case SDLK_t: {
			system->sensorField = 1;
			break;
		}
		case SDLK_p: {
			system->sensorField = 2;
			break;
		}
		case SDLK_SLASH: {
			if (system->sensorField == 0) {
				if (engine->process_sensor_system->humidity < 100) {
					engine->process_sensor_system->humidity += 1;
				}
			}
			else if (system->sensorField == 1) {
				engine->process_sensor_system->temperature += 1;
			}
			else if (system->sensorField == 2) {
				engine->process_sensor_system->airPressure += 1;
			}
			break;
		}
		case SDLK_EQUALS: {
			if (system->sensorField == 0) {
				if (engine->process_sensor_system->humidity > 0) {
					engine->process_sensor_system->humidity -= 1;
				}
			}
			else if (system->sensorField == 1) {
				if (engine->process_sensor_system->temperature > -273) {
					engine->process_sensor_system->temperature -= 1;
				}
			}
			else if (system->sensorField == 2) {
				if (engine->process_sensor_system->airPressure > 0) {
					engine->process_sensor_system->airPressure -= 1;
				}
			}
			break;
		}
#endif
		default:
			break;
		}
	}

void system_input_update(InputSystem* system, Engine* engine) {
	EntityId input_recv_entity_id = search_first_entity_1(engine, COMP_INPUTRECEIVER);
	CameraLookFromComponent* cameraLookFrom = search_first_component(engine, COMP_CAMERA_LOOK_FROM);

	if (!player_blocked) {
		SDL_Event event;
		memset(&event, 0, sizeof(SDL_Event));
		/* Grab all the events off the queue. */
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_KEYDOWN:
				/* Handle key presses. */
				handleKeyDown(system, engine, &event.key.keysym, input_recv_entity_id);
				break;
			case SDL_KEYUP:
				/* Handle key release. */
				handleKeyUp(system, engine, &event.key.keysym, input_recv_entity_id);
				break;
			case SDL_QUIT:
				/* Handle quit requests (like Ctrl-c). */
				engine->context.is_exit_game = 1;
				break;
			case SDL_MOUSEMOTION: {
				SDL_MouseMotionEvent *mouseMotionEvent = (SDL_MouseMotionEvent *) &event;
				//There are 2 mouse events we want to ignore:
				// - initially, an event is generated that moves the mouse from 0,0 to the current position
				// - then, each time SDL_WarpMouse is called, an event is generated for that call
				if ((mouseMotionEvent->x != mouseMotionEvent->xrel || mouseMotionEvent->y != mouseMotionEvent->yrel)
					//                    &&
					//                    (mouseMotionEvent->x != system->screenMidX || mouseMotionEvent->y != system->screenMidY)
					) {
					//                    SDL_WarpMouse(screenMidX, screenMidY);

					int buttonDown = mouseMotionEvent->state & SDL_BUTTON_LEFT;

					if (buttonDown) {
						float x_move = mouseMotionEvent->xrel * 1.0f;
						float y_move = mouseMotionEvent->yrel * 1.0f;
							cameraLookFrom->XYdegees = fmod((cameraLookFrom->XYdegees - x_move), 360);

							if (cameraLookFrom->Zdegrees >= 0.0f && cameraLookFrom->Zdegrees <= 90.0f) {
								cameraLookFrom->Zdegrees -= y_move;
								if (cameraLookFrom->Zdegrees >= 90.0f) {
									cameraLookFrom->Zdegrees = 89.9f;
								}
								if (cameraLookFrom->Zdegrees <= 00.0f) {
									cameraLookFrom->Zdegrees = 0.1f;
								}
							}
					}
					else {
						//printf("Mouse moved %f %f\n", mouseMotionEvent->xrel * 1.0f, mouseMotionEvent->yrel * 1.0f);
					}
				}
				break;
			}
			case SDL_MOUSEBUTTONUP: {
				//SDL 1.2 handles the wheel in a silly way
				switch (event.button.button) {
				case SDL_BUTTON_WHEELUP: {
					if (cameraLookFrom->distance > 5) {
						cameraLookFrom->distance -= 1;
					}
					break;
				}
				case SDL_BUTTON_WHEELDOWN: {
					if (cameraLookFrom->distance < 25) {
						cameraLookFrom->distance += 1;
					}
					break;
				}
				}
				break;
			}
			}
		}
	}
}