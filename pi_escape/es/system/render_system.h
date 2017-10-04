#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

typedef struct RenderSystem RenderSystem;

#include "../../graphics/opengl_game_renderer.h"
#include "../engine.h"
#include "../../graphics/gl_key.h"
#include "../../graphics/gl_floor.h"
#include "../../graphics/gl_wall.h"
#include "../../graphics/gl_player.h"
#include "../../graphics/gl_lock.h"
#include "../../graphics/gl_door.h"
#include "../../graphics/gl_connector.h"
#include "../../graphics/gl_connector_center.h"
#include "../../graphics/gl_glyph.h"

#define GLOBJECT_MAX_VERTICES (4*6)                         //enough for a cube with each side seperate vertices

#define GLOBJECT_MAX_TRIANGLES (4*6*2)                      //enough for a cube
#define GLOBJECT_MAX_INDICES (GLOBJECT_MAX_TRIANGLES * 3)   //3 points per triangle...

#define GLLEVELPART_MAX_QUADS 6
#define GLLEVELPART_MAX_VERTICES (GLLEVELPART_MAX_QUADS*4)

typedef struct RenderSystem {
    Graphics* graphics;
    
    GLKey key;
    GLFloor floor;
    GLWall wall[4];
    GLPlayer player;
    GLLock lock;
    GLConnector connector[4];
    GLConnectorCenter connector_center[2];
    GLDoor door[2];
} RenderSystem;

void system_render_init(RenderSystem*, Graphics*);
RenderSystem* system_render_alloc(Graphics*);
void system_render_update(RenderSystem*, Engine*);
void system_render_free(RenderSystem*);

#endif //RENDER_SYSTEM_H