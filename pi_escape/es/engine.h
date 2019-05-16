#ifndef ENGINE_H
#define ENGINE_H

typedef struct Engine Engine;

#include "entity.h"
#include "context.h"
#include "system/render_system.h"
#include "system/input_system.h"
#include "system/move_system.h"
#include "system/lock_system.h"
#include "system/endlevel_system.h"
#include "system/container_system.h"
#include "system/camera_system.h"
#include "system/animation_system.h"
#include "system/activation_system.h"
#include "system/action_system.h"
#include "system/process_sensor_system.h"
#ifdef RPI
#include "system/real_sensors_system.h"
#include "system/orientation_system.h"
#endif
#include <stdint.h>
#include <glmc.h>

#include "es_memory_manager_collections.h"

#include "es_memory_manager.h"

typedef struct Engine {
	Level level;
    Context context;
    
    //systems
    RenderSystem* render_system;
    InputSystem* input_system;
    LockSystem* lock_system;
    ActionSystem* action_system;
    ActivationSystem* activation_system;
    AnimationSystem* animation_system;
    CameraSystem* camera_system;
    ContainerSystem* container_system;
    EndLevelSystem* end_system;
    MoveSystem* move_system;
    ProcessSensorSystem* process_sensor_system;
#ifdef RPI
    RealSensorsSystem* real_sensors_system;
    OrientationSystem* orientation_system;
#endif
    ESMemory es_memory;
} Engine;

Engine* engine_alloc(Graphics*);
void engine_init(Engine*, Graphics*);
void engine_free(Engine*);

void engine_update(Engine*);

#endif //ENGINE_H
