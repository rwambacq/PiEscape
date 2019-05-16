#ifndef PIESCAPE2_GL_CONNECTOR_CENTER_H
#define PIESCAPE2_GL_CONNECTOR_CENTER_H

#include "gl_proctex_object.h"
#include "../es/game_util.h"

typedef struct GLConnectorCenter {
#ifdef __cplusplus
    struct GLProcTexObject parent;
#else
    union {
        struct GLProcTexObject;
        GLProcTexObject gl_pt_object;
    };
#endif
} GLConnectorCenter;

void gl_connector_center_init(GLConnectorCenter *, Graphics *, ConnectorCenterType connectorCenterType);
void gl_connector_center_free(GLConnectorCenter *);
void  gl_connector_center_draw(GLConnectorCenter* obj, uint64_t time, t_ivec2 gridPos, int active);

#endif //PIESCAPE2_GL_CONNECTOR_CENTER_H
