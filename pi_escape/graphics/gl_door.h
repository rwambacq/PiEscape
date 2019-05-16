#ifndef PIESCAPE2_GL_DOOR_H
#define PIESCAPE2_GL_DOOR_H

#include "gl_proctex_object.h"

typedef struct GLDoor {
#ifdef __cplusplus
    struct GLProcTexObject parent;
#else
    union {
        struct GLProcTexObject;
        GLProcTexObject gl_pt_object;
    };
#endif
    
    
} GLDoor;

void gl_door_init(GLDoor *, Graphics *, GridAxis parallelGridAxis);
void gl_door_free(GLDoor *);
void  gl_door_draw(GLDoor* obj, uint64_t time, t_ivec2 gridPos, GameColor color);

#endif //PIESCAPE2_GL_DOOR_H
