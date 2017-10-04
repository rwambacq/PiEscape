#include <assert.h>
#include "gl_door.h"

void gl_door_init(GLDoor *obj, Graphics *graphics, GridAxis parallelGridAxis) {
    gl_proctex_object_init(&obj->gl_pt_object, graphics, PROGRAM_PROCTEX);
    
    obj->blend = 1;
    obj->no_cull = 1;
    
    obj->defaultAmbiantLightMulti = 1.0;
    obj->diffuseLightMulti = 0.2f;
    obj->specularLightMulti = 0.5;
    obj->proceduralTextureID = 0.0;
    gl_set_color(obj->colorA, 0.0f, 0.0f, 1.0f, 0.5f);
    gl_set_color(obj->colorB, 0.2f, 0.2f, 1.0f, 0.5f);
    obj->proceduralTextureParameterA = 0.025f;
    
    int indicesIndex = 0;
    int quadIndex = 0;
    
    if (parallelGridAxis == X) {
        for (int v = 0; v < 4; v++) {
            int i = ((quadIndex * 4) + v) * 3;
            
            obj->vertex_buffer_data[i] = v < 2 ? -1.0 : 1.0; //x
            obj->vertex_buffer_data[i + 1] = v % 2 == 0 ? 1.0 : -1.0; //y
            obj->vertex_buffer_data[i + 2] = 0.0; //z
            
            obj->normal_buffer_data[i] = 0.0; //x
            obj->normal_buffer_data[i + 1] = 0.0; //y
            obj->normal_buffer_data[i + 2] = -1.0; //z
            
            int j = ((quadIndex * 4) + v) * 2;
            obj->texCoord_buffer_data[j] = v < 2 ? 0.0 : 1.0; //u
            obj->texCoord_buffer_data[j+1] = v % 2 == 0 ? 0.0 : 1.0; //w
        }
        
        obj->indices[indicesIndex++] = (quadIndex * 4) + 0;
        obj->indices[indicesIndex++] = (quadIndex * 4) + 1;
        obj->indices[indicesIndex++] = (quadIndex * 4) + 3;
        
        obj->indices[indicesIndex++] = (quadIndex * 4) + 0;
        obj->indices[indicesIndex++] = (quadIndex * 4) + 3;
        obj->indices[indicesIndex++] = (quadIndex * 4) + 2;
        
        quadIndex++;
    }
    
    if (parallelGridAxis == Y) {
        for (int v = 0; v < 4; v++) {
            int i = ((quadIndex * 4) + v) * 3;
            
            obj->vertex_buffer_data[i] = 0.0; //x
            obj->vertex_buffer_data[i + 1] = v < 2 ? -1.0 : 1.0; //y
            obj->vertex_buffer_data[i + 2] = v % 2 == 0 ? 1.0 : -1.0; //z
            
            obj->normal_buffer_data[i] = -1.0; //x
            obj->normal_buffer_data[i + 1] = 0.0; //y
            obj->normal_buffer_data[i + 2] = 0.0; //z
            
            int j = ((quadIndex * 4) + v) * 2;
            obj->texCoord_buffer_data[j] = v < 2 ? 0.0 : 1.0; //u
            obj->texCoord_buffer_data[j+1] = v % 2 == 0 ? 0.0 : 1.0; //w
        }
        
        obj->indices[indicesIndex++] = (quadIndex * 4) + 0;
        obj->indices[indicesIndex++] = (quadIndex * 4) + 1;
        obj->indices[indicesIndex++] = (quadIndex * 4) + 3;
        
        obj->indices[indicesIndex++] = (quadIndex * 4) + 0;
        obj->indices[indicesIndex++] = (quadIndex * 4) + 3;
        obj->indices[indicesIndex++] = (quadIndex * 4) + 2;
        
        quadIndex++;
    }
    
    globject_init_indices(&obj->gl_object, obj->graphics, indicesIndex);
    gl_proctex_init_vertexbuffers(&obj->gl_pt_object, quadIndex*4, "GLDoor");
}

void gl_door_free(GLDoor *obj) {
    gl_proctex_object_free(&obj->gl_pt_object);
}

void gl_door_draw(GLDoor* obj, uint64_t time, t_ivec2 gridPos, GameColor color) {
    switch(color) {
        case RED: {
            gl_set_color(obj->colorA, 1.0f, 0.0f, 0.0f, 0.5f);
            break;
        }
        case GREEN: {
            gl_set_color(obj->colorA, 0.0f, 1.0f, 0.0f, 0.5f);
            break;
        }
        case BLUE: {
            gl_set_color(obj->colorA, 0.0f, 0.0f, 1.0f, 0.5f);
            break;
        }
        case LOCK_ANY: {
            gl_set_color(obj->colorA, 1.0f, 1.0f, 1.0f, 0.5f);
            break;
        }
        case KEY_ALL: {
            gl_set_color(obj->colorA, 5.0f, 3.0f, 2.0f, 0.5f);
            gl_set_color(obj->colorB, 5.0f, 3.0f, 2.0f, 0.5f);
            break;
        }
    }
    
    t_vec3 worldPos;
    level_pos_to_world_pos2i(gridPos, worldPos);
    
    
    gl_proctex_draw(&obj->gl_pt_object, worldPos);
}
