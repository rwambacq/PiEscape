#ifndef PIESCAPE2_GL_CONNECTOR_H
#define PIESCAPE2_GL_CONNECTOR_H

#include "gl_proctex_object.h"
#include "../es/game_util.h"

typedef struct GLConnector {
#ifdef __cplusplus
    struct GLProcTexObject parent;
#else
    union {
        struct GLProcTexObject;
        GLProcTexObject gl_pt_object;
    };
#endif
    
    
} GLConnector;

void gl_connector_init(GLConnector *, Graphics *, Direction direction);
void gl_connector_free(GLConnector *);
void  gl_connector_draw(GLConnector* obj, uint64_t time, t_ivec2 gridPos, int active);

#endif //PIESCAPE2_GL_CONNECTOR_H
