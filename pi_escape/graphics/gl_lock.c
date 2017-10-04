#include <assert.h>
#include "gl_lock.h"

#define GLLOCK_SIZE 0.4

void gl_lock_init(GLLock *obj, Graphics *graphics) {
    gl_proctex_object_init(&obj->gl_pt_object, graphics, PROGRAM_PROCTEX);
    
    obj->defaultAmbiantLightMulti = 0.9f;   //always quite bright
    obj->diffuseLightMulti = 0.1f;   //
    obj->specularLightMulti = 0.0;  //not shiny
    obj->proceduralTextureID = 0.0;
    
    int indicesIndex = 0;
    int quadIndex = 0;
    
    for (int v = 0; v < 4; v++) {
        int i = ((quadIndex * 4) + v) * 3;
        
        obj->vertex_buffer_data[i] = v < 2 ? -GLLOCK_SIZE : GLLOCK_SIZE;
        obj->vertex_buffer_data[i + 1] = -0.85f; //y
        obj->vertex_buffer_data[i + 2] = v % 2 == 0 ? GLLOCK_SIZE : -GLLOCK_SIZE;
        
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
    
    assert(quadIndex == 1);
    assert(indicesIndex == 6);
    
    globject_init_indices(&obj->gl_object, obj->graphics, indicesIndex);
    gl_proctex_init_vertexbuffers(&obj->gl_pt_object, quadIndex*4, "GLLock");
}

void gl_lock_free(GLLock *obj) {
    gl_proctex_object_free(&obj->gl_pt_object);
}

void gl_lock_draw(GLLock* obj, uint64_t time, t_ivec2 gridPos, GameColor lockColor, int active) {
    switch(lockColor) {
        case RED: {
            gl_set_color(obj->colorA, 1.0f, 0.0f, 0.0f, 1.0f);
            break;
        }
        case GREEN: {
            gl_set_color(obj->colorA, 0.0f, 1.0f, 0.0f, 1.0f);
            break;
        }
        case BLUE: {
            gl_set_color(obj->colorA, 0.0f, 0.0f, 1.0f, 1.0f);
            break;
        }
        case LOCK_ANY: {
            gl_set_color(obj->colorA, 1.0f, 1.0f, 1.0f, 1.0f);
            break;
        }
        case KEY_ALL: {
            gl_set_color(obj->colorA, 5.0f, 3.0f, 2.0f, 1.0f);
            break;
        }
    }
    
    if (active) {
        obj->proceduralTextureID = 1.0;
        gl_set_color(obj->colorB, 1.0f, 1.0f, 0.0f, 1.0f);
        obj->proceduralTextureParameterA = 0.2f;
    } else {
        obj->proceduralTextureID = 0.0;
    }
    
    t_vec3 worldPos;
    level_pos_to_world_pos2i(gridPos, worldPos);
    
    
    gl_proctex_draw(&obj->gl_pt_object, worldPos);
}
