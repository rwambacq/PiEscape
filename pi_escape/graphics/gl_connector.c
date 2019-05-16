#include <assert.h>
#include "gl_connector.h"

#define GLCONNECTOR_HALF_WIDTH 0.2f
//#define GLCONNECTOR_CENTER_OFFSET 0.3f    // keep small distance from center, leaving between connectors
//#define GLCONNECTOR_CENTER_OFFSET 0.0f    // extend to center of tile
#define GLCONNECTOR_CENTER_OFFSET (-GLCONNECTOR_HALF_WIDTH)   //extend beyond center to cover width

void gl_connector_init(GLConnector *obj, Graphics *graphics, Direction direction) {
    gl_proctex_object_init(&obj->gl_pt_object, graphics, PROGRAM_PROCTEX);
    
    obj->defaultAmbiantLightMulti = 1.0;
    obj->diffuseLightMulti = 0.2f;
    obj->specularLightMulti = 0.5;
    obj->proceduralTextureID = 0.0;
    gl_set_color(obj->colorA, 1.0f, 0.0f, 1.0f, 1.0f);
    gl_set_color(obj->colorB, 0.2f, 0.2f, 1.0f, 1.0f);
    obj->proceduralTextureParameterA = 0.1f;
    
    int indicesIndex = 0;
    int quadIndex = 0;
    
    for (Direction curDir = DIRECTION_FIRST; curDir <= DIRECTION_LAST; curDir++) {
        if (direction == curDir) {
            for (int v = 0; v < 4; v++) {
                int i = ((quadIndex * 4) + v) * 3;
                
                switch (curDir) {
                    case E: { obj->vertex_buffer_data[i] = v < 2 ? GLCONNECTOR_CENTER_OFFSET : 1.0; break; }
                    case W: { obj->vertex_buffer_data[i] = v < 2 ? -1.0 : -GLCONNECTOR_CENTER_OFFSET; break; }
                    case N: //fall-through
                    case S: { obj->vertex_buffer_data[i] = v < 2 ? -GLCONNECTOR_HALF_WIDTH : GLCONNECTOR_HALF_WIDTH; break; }
                }
                obj->vertex_buffer_data[i + 1] = -0.9f; //y
                switch (curDir) {
                    case E: //fall-through
                    case W: { obj->vertex_buffer_data[i + 2] = v % 2 == 0 ? GLCONNECTOR_HALF_WIDTH : -GLCONNECTOR_HALF_WIDTH; break; }
                    case S: { obj->vertex_buffer_data[i + 2] = v % 2 == 0 ? 1.0 : GLCONNECTOR_CENTER_OFFSET; break; }
                    case N: { obj->vertex_buffer_data[i + 2] = v % 2 == 0 ? -GLCONNECTOR_CENTER_OFFSET : -1.0; break; }
                }
                
                obj->normal_buffer_data[i] = 0.0f; //x
                obj->normal_buffer_data[i + 1] = -1.0f; //y
                obj->normal_buffer_data[i + 2] = 0.0f; //z
                
                int j = ((quadIndex * 4) + v) * 2;
                obj->texCoord_buffer_data[j] = v < 2 ? 0.0 : 1.0; //u
                obj->texCoord_buffer_data[j+1] = v % 2 == 0 ? 0.0 : 1.0; //w
            }
            
            obj->indices[indicesIndex++] = (quadIndex * 4) + 0;
            obj->indices[indicesIndex++] = (quadIndex * 4) + 3;
            obj->indices[indicesIndex++] = (quadIndex * 4) + 1;
            
            obj->indices[indicesIndex++] = (quadIndex * 4) + 0;
            obj->indices[indicesIndex++] = (quadIndex * 4) + 2;
            obj->indices[indicesIndex++] = (quadIndex * 4) + 3;
            
            quadIndex++;
        }
    }
    
    
    assert(quadIndex == 1);
    assert(indicesIndex == 6);
    
    globject_init_indices(&obj->gl_object, obj->graphics, indicesIndex);
    gl_proctex_init_vertexbuffers(&obj->gl_pt_object, quadIndex*4, "GLConnector");
}

void gl_connector_free(GLConnector *obj) {
    gl_proctex_object_free(&obj->gl_pt_object);
}

void gl_connector_draw(GLConnector* obj, uint64_t time, t_ivec2 gridPos,
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
