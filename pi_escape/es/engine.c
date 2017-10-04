#include "engine.h"

#include <stdlib.h>
#include <assert.h>

#include <SDL_timer.h>


Engine* engine_alloc(Graphics* graphics) {
    Engine* res = calloc(1, sizeof(Engine));
    engine_init(res, graphics);
    return res;
}
void engine_init(Engine* engine, Graphics* graphics) {
    context_init(&engine->context);
    assert(!engine->context.is_exit_game);
    
    es_memory_manager_init(&engine->es_memory);
    
    engine->render_system = system_render_alloc(graphics);
    engine->input_system = system_input_alloc();
    engine->lock_system = system_lock_alloc();
    engine->action_system = system_action_alloc();
    engine->activation_system = system_activation_alloc();
    engine->animation_system = system_animation_alloc();
    engine->camera_system = system_camera_alloc();
    engine->container_system = system_container_alloc();
    engine->end_system = system_endlevel_alloc();
    engine->move_system = system_move_alloc();
    engine->process_sensor_system = system_process_sensor_alloc();
#ifdef RPI
    engine->real_sensors_system = system_real_sensors_alloc();
    engine->orientation_system = system_orientation_alloc();
#endif
    
    assert(engine->render_system != NULL);
    assert(engine->input_system != NULL);
    assert(engine->lock_system != NULL);
    assert(engine->action_system != NULL);
    assert(engine->activation_system != NULL);
    assert(engine->animation_system != NULL);
    assert(engine->camera_system != NULL);
    assert(engine->container_system != NULL);
    assert(engine->end_system != NULL);
    assert(engine->move_system != NULL);
    assert(engine->process_sensor_system != NULL);
#ifdef RPI
    assert(engine->real_sensors_system != NULL);
    assert(engine->orientation_system != NULL);
#endif
}

void engine_free(Engine* e) {
    context_free(&e->context);
    
    //free all systems
    system_render_free(e->render_system);
    system_input_free(e->input_system);
    system_lock_free(e->lock_system);
    system_action_free(e->action_system);
    system_activation_free(e->activation_system);
    system_animation_free(e->animation_system);
    system_camera_free(e->camera_system);
    system_container_free(e->container_system);
    system_endlevel_free(e->end_system);
    system_move_free(e->move_system);
    system_process_sensor_free(e->process_sensor_system);
#ifdef RPI
    system_real_sensors_free(e->real_sensors_system);
    system_orientation_free(e->orientation_system);
    free(e->real_sensors_system);
    free(e->orientation_system);
#endif
    
    es_memory_manager_free(&e->es_memory);

    free(e->render_system);
    free(e->input_system);
    free(e->lock_system);
    free(e->action_system);
    free(e->activation_system);
    free(e->animation_system);
    free(e->camera_system);
    free(e->container_system);
    free(e->end_system);
    free(e->move_system);
    free(e->process_sensor_system);
    
    //note: don't free graphics here
}

void engine_update(Engine* engine) {
    engine->context.time = SDL_GetTicks();
    
    system_input_update(engine->input_system, engine);
    system_lock_update(engine->lock_system, engine);
    system_activation_update(engine->activation_system, engine);
    system_move_update(engine->move_system, engine);
    system_action_update(engine->action_system, engine);
    system_container_update(engine->container_system, engine);
    system_camera_update(engine->camera_system, engine);
    system_endlevel_update(engine->end_system, engine);
    system_animation_update(engine->animation_system, engine);
#ifdef RPI
    system_real_sensors_update(engine->real_sensors_system, engine);
    system_orientation_update(engine->orientation_system, engine);
#endif
    system_process_sensor_update(engine->process_sensor_system, engine);
    system_render_update(engine->render_system, engine);
}
