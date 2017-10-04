#include <assert.h>
#include "gl_connector_center.h"

#define GLLOCK_SIZE 0.4

void gl_connector_center_init(GLConnectorCenter *obj, Graphics *graphics, ConnectorCenterType connectorCenterType) {
    gl_proctex_object_init(&obj->gl_pt_object, graphics, PROGRAM_PROCTEX);
    
    obj->defaultAmbiantLightMulti = 1.0;
    obj->diffuseLightMulti = 0.2f;
    obj->specularLightMulti = 0.5;
    obj->proceduralTextureID = 0.0;
    
    int indicesIndex = 0;
    int quadIndex = 0;
    
    //center
    for (int v = 0; v < 4; v++) {
        int i = ((quadIndex * 4) + v) * 3;
        
        if (connectorCenterType == AND) {
            obj->vertex_buffer_data[i] = v == 0 ? -0.7f : (v == 2 ? 0.7f : 0.0f);
            obj->vertex_buffer_data[i + 1] = -0.85f; //y
            obj->vertex_buffer_data[i + 2] = v == 1 ? 0.7f : (v == 3 ? -0.7f : 0.0f);
        } else {
            obj->vertex_buffer_data[i] = v < 2 ? -0.6 : 0.6;
            obj->vertex_buffer_data[i + 1] = -0.85f; //y
            obj->vertex_buffer_data[i + 2] = v % 2 == 0 ? 0.6 : -0.6;
        }
        
        obj->normal_buffer_data[i] = 0.0f; //x
        obj->normal_buffer_data[i + 1] = -1.0f; //y
        obj->normal_buffer_data[i + 2] = 0.0f; //z
        
        int j = ((quadIndex * 4) + v) * 2;
        obj->texCoord_buffer_data[j] = v < 2 ? 0.0 : 1.0; //u
        obj->texCoord_buffer_data[j+1] = v % 2 == 0 ? 0.0 : 1.0; //w
    }
    
    if (connectorCenterType == AND) {
        obj->indices[indicesIndex++] = (quadIndex * 4) + 0;
        obj->indices[indicesIndex++] = (quadIndex * 4) + 1;
        obj->indices[indicesIndex++] = (quadIndex * 4) + 2;
        
        obj->indices[indicesIndex++] = (quadIndex * 4) + 0;
        obj->indices[indicesIndex++] = (quadIndex * 4) + 2;
        obj->indices[indicesIndex++] = (quadIndex * 4) + 3;
    } else {
        obj->indices[indicesIndex++] = (quadIndex * 4) + 0;
        obj->indices[indicesIndex++] = (quadIndex * 4) + 3;
        obj->indices[indicesIndex++] = (quadIndex * 4) + 1;
        
        obj->indices[indicesIndex++] = (quadIndex * 4) + 0;
        obj->indices[indicesIndex++] = (quadIndex * 4) + 2;
        obj->indices[indicesIndex++] = (quadIndex * 4) + 3;
    }
    
    quadIndex++;
    
    assert(quadIndex == 1);
    assert(indicesIndex == 6);
    
    globject_init_indices(&obj->gl_object, obj->graphics, indicesIndex);
    gl_proctex_init_vertexbuffers(&obj->gl_pt_object, quadIndex*4, "GLConnectorCenter");
}

void gl_connector_center_free(GLConnectorCenter *obj) {
    gl_proctex_object_free(&obj->gl_pt_object);
}

void gl_connector_center_draw(GLConnectorCenter* obj, uint64_t time, t_ivec2 gridPos,
                       int active) {
    if (active) {
        gl_set_color(obj->colorA, 1.0f, 1.0f, 0.0f, 1.0f);
    } else {
        gl_set_color(obj->colorA, 0.0f, 0.0f, 0.0f, 1.0f);
    }
    
    t_vec3 worldPos;
    level_pos_to_world_pos2i(gridPos, worldPos);
    
    
    gl_proctex_draw(&obj->gl_pt_object, worldPos);
}
