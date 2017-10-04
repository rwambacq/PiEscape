#include <assert.h>
#include "gl_key.h"

#define GLKEY_H 0.8f
#define GLKEY_W 0.3f

void gl_key_init(GLKey *obj, Graphics *graphics) {
    gl_proctex_object_init(&obj->gl_pt_object, graphics, PROGRAM_PROCTEX);
    
    obj->blend = 1;
    
    obj->defaultAmbiantLightMulti = 0.9f;  //always quite bright
    obj->diffuseLightMulti = 0.0;   //
    obj->specularLightMulti = 0.75; //extra shiny!
    obj->proceduralTextureID = 0.0;
    
    obj->proceduralTextureParameterA = 0.0;
    
    int indicesIndex = 0;
    
    //bottom
    int t = 0;
    obj->vertex_buffer_data[t] = 0.0;
    obj->vertex_buffer_data[t + 1] = -GLKEY_H;
    obj->vertex_buffer_data[t + 2] = 0.0;
    
    //middle 3
    t+=3;
    obj->vertex_buffer_data[t] = 0.0;
    obj->vertex_buffer_data[t + 1] = 0.0;
    obj->vertex_buffer_data[t + 2] = GLKEY_W;
    t+=3;
    obj->vertex_buffer_data[t] = -GLKEY_W;
    obj->vertex_buffer_data[t + 1] = 0.0;
    obj->vertex_buffer_data[t + 2] = -GLKEY_W;
    t+=3;
    obj->vertex_buffer_data[t] = GLKEY_W;
    obj->vertex_buffer_data[t + 1] = 0.0;
    obj->vertex_buffer_data[t + 2] = -GLKEY_W;
    
    //top
    t+=3;
    obj->vertex_buffer_data[t] = 0.0;
    obj->vertex_buffer_data[t + 1] = GLKEY_H;
    obj->vertex_buffer_data[t + 2] = 0.0;
    
    t+=3;
    assert(t == 3*5);
    
    //same for texCoords
    int tc = 0;
    obj->texCoord_buffer_data[tc] = 0.0;
    obj->texCoord_buffer_data[tc+1] = 0.0;
    tc+=2;
    obj->texCoord_buffer_data[tc] = 1.0;
    obj->texCoord_buffer_data[tc+1] = 0.0;
    tc+=2;
    obj->texCoord_buffer_data[tc] = 0.0;
    obj->texCoord_buffer_data[tc+1] = 1.0;
    tc+=2;
    obj->texCoord_buffer_data[tc] = 0.5;
    obj->texCoord_buffer_data[tc+1] = 0.5;
    tc+=2;
    obj->texCoord_buffer_data[tc] = 1.0;
    obj->texCoord_buffer_data[tc+1] = 1.0;
    tc+=2;
    
    for (int i = 0; i < 3*5; i += 3) {
        //for now give everything point up as normal
        obj->normal_buffer_data[i] = 0.0f; //x
        obj->normal_buffer_data[i + 1] = -1.0f; //y
        obj->normal_buffer_data[i + 2] = 0.0f; //z
    }
    
    obj->indices[indicesIndex++] = 0;
    obj->indices[indicesIndex++] = 2;
    obj->indices[indicesIndex++] = 1;
    
    obj->indices[indicesIndex++] = 0;
    obj->indices[indicesIndex++] = 3;
    obj->indices[indicesIndex++] = 2;
    
    obj->indices[indicesIndex++] = 0;
    obj->indices[indicesIndex++] = 1;
    obj->indices[indicesIndex++] = 3;
    
    obj->indices[indicesIndex++] = 4;
    obj->indices[indicesIndex++] = 1;
    obj->indices[indicesIndex++] = 2;
    
    obj->indices[indicesIndex++] = 4;
    obj->indices[indicesIndex++] = 2;
    obj->indices[indicesIndex++] = 3;
    
    obj->indices[indicesIndex++] = 4;
    obj->indices[indicesIndex++] = 3;
    obj->indices[indicesIndex++] = 1;
    
    assert((indicesIndex / (3*2)) == 3);
    
    globject_init_indices(&obj->gl_object, obj->graphics, indicesIndex);
    gl_proctex_init_vertexbuffers(&obj->gl_pt_object, 5, "GLKey");
}

void gl_key_free(GLKey *obj) {
    gl_proctex_object_free(&obj->gl_pt_object);
}

void gl_key_draw(GLKey* obj, uint64_t time, t_vec2 gridPos, int pickedUp, GameColor color) {
    switch(color) {
        case RED: {
            gl_set_color(obj->colorA, 1.0f, 0.0f, 0.0f, 0.75f);
            break;
        }
        case GREEN: {
            gl_set_color(obj->colorA, 0.0f, 1.0f, 0.0f, 0.75f);
            break;
        }
        case BLUE: {
            gl_set_color(obj->colorA, 0.0f, 0.0f, 1.0f, 0.75f);
            break;
        }
        case LOCK_ANY: {
            gl_set_color(obj->colorA, 1.0f, 1.0f, 1.0f, 0.75f);
            break;
        }
        case KEY_ALL: {
            gl_set_color(obj->colorA, 6.0f, 3.0f, 2.0f, 0.75f);
            gl_set_color(obj->colorB, 6.0f, 3.0f, 2.0f, 0.75f);
            break;
        }
    }
    
    t_vec3 worldPos;
    level_pos_to_world_pos2f(gridPos, worldPos);
    
    if (pickedUp) {
        worldPos[1] += 1.5f;
    }
    
    gl_proctex_draw(&obj->gl_pt_object, worldPos);
}
