#ifndef PIESCAPE2_GL_WALL_H
#define PIESCAPE2_GL_WALL_H

#include "gl_proctex_object.h"

#include "../es/game_util.h"

typedef struct GLWall {
#ifdef __cplusplus
    struct GLProcTexObject parent;
#else
    union {
        struct GLProcTexObject;
        GLProcTexObject gl_pt_object;
    };
#endif
    
    
} GLWall;

void gl_wall_init(GLWall *, Graphics *, Direction direction);
void gl_wall_free(GLWall *);
void  gl_wall_draw_color(GLWall* obj, t_ivec2 gridPos, t_vec4 colorA);

#endif //PIESCAPE2_GL_WALL_H
