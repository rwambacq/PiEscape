#ifndef PIESCAPE2_GL_KEY_H
#define PIESCAPE2_GL_KEY_H

#include "gl_proctex_object.h"

typedef struct GLKey {
#ifdef __cplusplus
    struct GLProcTexObject parent;
#else
    union {
        struct GLProcTexObject;
        GLProcTexObject gl_pt_object;
    };
#endif
} GLKey;

void gl_key_init(GLKey *, Graphics *);
void gl_key_free(GLKey *);
void  gl_key_draw(GLKey* obj, uint64_t time, t_vec2 gridPos, int pickedUp, GameColor color);

#endif //PIESCAPE2_GL_KEY_H
