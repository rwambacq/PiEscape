#ifndef PIESCAPE2_GL_LOCK_H
#define PIESCAPE2_GL_LOCK_H

#include "gl_proctex_object.h"

typedef struct GLLock {
#ifdef __cplusplus
    struct GLProcTexObject parent;
#else
    union {
        struct GLProcTexObject;
        GLProcTexObject gl_pt_object;
    };
#endif
    
    
} GLLock;

void gl_lock_init(GLLock *, Graphics *);
void gl_lock_free(GLLock *);
void  gl_lock_draw(GLLock* obj, uint64_t time, t_ivec2 gridPos, GameColor lockColor, int active);

#endif //PIESCAPE2_GL_LOCK_H
