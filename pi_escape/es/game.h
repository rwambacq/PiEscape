#ifndef GAME_H
#define GAME_H

#include "engine.h"

#include "../level/levelloader.h"

#include "context.h"
#include "assemblage.h"
#include "system/render_system.h"
#include "system/input_system.h"
#include "system/move_system.h"
#include "system/camera_system.h"
#include "system/lock_system.h"
#include "system/action_system.h"
#include "system/activation_system.h"
#include "system/container_system.h"
#include "system/endlevel_system.h"
#include "system/animation_system.h"
#include "system/process_sensor_system.h"
#ifdef RPI
#include "system/real_sensors_system.h"
#include "system/orientation_system.h"
#endif
#include "../graphics/opengl_game_renderer.h"

int player_blocked;

typedef struct Game {
    Graphics* graphics;
    Engine engine;
} Game;

void game_init(Game*, Graphics*);
Game* game_alloc(Graphics*);
void game_free(Game*);

void game_load_level(Game*, Level*);

#endif //GAME_H
