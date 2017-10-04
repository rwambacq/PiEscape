#ifndef PIESCAPE2_GL_PLAYER_H
#define PIESCAPE2_GL_PLAYER_H

#include "gl_proctex_object.h"

typedef struct GLPlayer {
#ifdef __cplusplus
    struct GLProcTexObject parent;
#else
    union {
        struct GLProcTexObject;
        GLProcTexObject gl_pt_object;
    };
#endif
    
    
} GLPlayer;

void gl_player_init(GLPlayer *, Graphics *);
void gl_player_free(GLPlayer *);
void  gl_player_draw(GLPlayer* obj, uint64_t time, t_vec2 gridPos);

#endif //PIESCAPE2_GL_PLAYER_H
