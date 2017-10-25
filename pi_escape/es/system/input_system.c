#include "input_system.h"

#include <SDL.h>
#include <SDL_timer.h>
#include <SDL_video.h>

#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "../game_util.h"

InputSystem* system_input_alloc() {
    InputSystem* res = calloc(1, sizeof(InputSystem));
    system_input_init(res);
    return res;
}

void system_input_init(InputSystem* system) {
}


void system_input_free(InputSystem* system) {
    
}

static void handleKeyDown(InputSystem* system, Engine* engine, SDL_keysym *keysym, EntityId input_recv_entity_id)
{
    switch( keysym->sym ) {
        case SDLK_ESCAPE:
            //ignore untile key released
            break;
        case SDLK_KP_ENTER: //fall-through
        case SDLK_RETURN:   //fall-through
        case SDLK_SPACE: {
            engine->context.demo = !engine->context.demo;
            break;
        }
        default:
            break;
    }
    
    
    
    
}

static void handleKeyUp(InputSystem* system, Engine* engine, SDL_keysym *keysym, EntityId inputReceiverEntity)
{
    switch( keysym->sym ) {
        case SDLK_ESCAPE:
            engine->context.is_exit_game = 1;
            break;
        case SDLK_UP:{
            engine->context.demo = !engine->context.demo;
            break;
        }
        case SDLK_DOWN:{
            engine->context.demo = !engine->context.demo;
            break;
        }
        case SDLK_LEFT:{
            engine->context.demo = !engine->context.demo;
            break;
        }
        case SDLK_RIGHT:{
            engine->context.demo = !engine->context.demo;
            break;
        }
        default:
            break;
    }
    
    
}

void system_input_update(InputSystem* system, Engine* engine) {
    EntityId input_recv_entity_id = search_first_entity_1(engine, COMP_INPUTRECEIVER);

    SDL_Event event;
    memset(&event, 0, sizeof(SDL_Event));
    /* Grab all the events off the queue. */
    while( SDL_PollEvent( &event ) ) {
        switch( event.type ) {
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
                        printf("Mouse dragged %f %f\n", x_move, y_move);
						CameraLookFromComponent* cameraLookFrom = search_first_component(engine, COMP_CAMERA_LOOK_FROM);
						cameraLookFrom->XYdegees -= x_move;
						cameraLookFrom->Zdegrees += y_move;
                    } else {
                        //printf("Mouse moved %f %f\n", mouseMotionEvent->xrel * 1.0f, mouseMotionEvent->yrel * 1.0f);
                    }
                }
                break;
            }
            case SDL_MOUSEBUTTONUP: {
                //SDL 1.2 handles the wheel in a silly way
                switch (event.button.button) {
                    case SDL_BUTTON_WHEELUP: {
                        printf("Wheel up\n");
                        break;
                    }
                    case SDL_BUTTON_WHEELDOWN:{
                        printf("Wheel down\n");
                        break;
                    }
                }
                break;
            }
        }



        //demo only, no use in real game
        EntityIterator it;
        search_entity_1(engine, COMP_ACTIVATABLE, &it);
        while(next_entity(&it)) {
            EntityId drawable_entity_id = it.entity_id;
            assert(drawable_entity_id != NO_ENTITY);
            ActivatableComponent* activatable = get_component(engine, drawable_entity_id, COMP_ACTIVATABLE);
            activatable->active = engine->context.demo;
        }
    }
}
