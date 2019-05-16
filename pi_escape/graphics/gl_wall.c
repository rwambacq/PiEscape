#include <assert.h>
#include "gl_wall.h"

void gl_wall_init(GLWall *obj, Graphics *graphics, Direction direction) {
    gl_proctex_object_init(&obj->gl_pt_object, graphics, PROGRAM_PROCTEX);
    obj->defaultAmbiantLightMulti = 0.4f;
    obj->diffuseLightMulti = 0.2f;
    obj->specularLightMulti = 0.05f;
    obj->proceduralTextureID = 1.0f;
    gl_set_color(obj->colorA, 1.0f, 0.0f, 0.0f, 1.0f);
    gl_set_color(obj->colorB, 0.0f, 1.0f, 0.0f, 1.0f);
    obj->proceduralTextureParameterA = 0.1f;
    obj->proceduralTextureParameterB = 0.1f;
    obj->proceduralTextureParameterC = 0.1f;
    obj->proceduralTextureParameterD = 0.1f;
    
    int indicesIndex = 0;
    int quadIndex = 0;
    
    for (int doN = 1; doN >= 0; doN--) {
        if ((doN && direction == N) || (!doN && direction == S)) {
            for (int v = 0; v < 4; v++) {
                int i = ((quadIndex * 4) + v) * 3;
                
                obj->vertex_buffer_data[i] = v < 2 ? -1.0 : 1.0; //x
                obj->vertex_buffer_data[i + 1] = v % 2 == 0 ? 1.0 : -1.0; //y
                obj->vertex_buffer_data[i + 2] = doN ? -1.0 : 1.0; //z
                
                obj->normal_buffer_data[i] = 0.0; //x
                obj->normal_buffer_data[i + 1] = 0.0; //y
                obj->normal_buffer_data[i + 2] = doN ? 1.0 : -1.0; //z
                
                int j = ((quadIndex * 4) + v) * 2;
                obj->texCoord_buffer_data[j] = v < 2 ? 0.0 : 1.0; //u
                obj->texCoord_buffer_data[j+1] = v % 2 == 0 ? 0.0 : 1.0; //w
            }
            
            obj->indices[indicesIndex++] = (quadIndex * 4) + 0;
            obj->indices[indicesIndex++] = (quadIndex * 4) + (doN ? 1 : 3);
            obj->indices[indicesIndex++] = (quadIndex * 4) + (doN ? 3 : 1);
            
            obj->indices[indicesIndex++] = (quadIndex * 4) + 0;
            obj->indices[indicesIndex++] = (quadIndex * 4) + (doN ? 3 : 2);
            obj->indices[indicesIndex++] = (quadIndex * 4) + (doN ? 2 : 3);
            
            quadIndex++;
        }
    }
    
    for (int doW = 1; doW >= 0; doW--) {
        if ((doW && direction == W) || (!doW && direction == E)) {
            for (int v = 0; v < 4; v++) {
                int i = ((quadIndex * 4) + v) * 3;
                
                obj->vertex_buffer_data[i] = doW ? -1.0 : 1.0; //x
                obj->vertex_buffer_data[i + 1] = v < 2 ? -1.0 : 1.0; //y
                obj->vertex_buffer_data[i + 2] = v % 2 == 0 ? 1.0 : -1.0; //z
                
                obj->normal_buffer_data[i] = doW ? 1.0 : -1.0; //x
                obj->normal_buffer_data[i + 1] = 0.0; //y
                obj->normal_buffer_data[i + 2] = 0.0; //z
                
                int j = ((quadIndex * 4) + v) * 2;
                obj->texCoord_buffer_data[j] = v < 2 ? 0.0 : 1.0; //u
                obj->texCoord_buffer_data[j+1] = v % 2 == 0 ? 0.0 : 1.0; //w
            }
            
            obj->indices[indicesIndex++] = (quadIndex * 4) + 0;
            obj->indices[indicesIndex++] = (quadIndex * 4) + (doW ? 1 : 3);
            obj->indices[indicesIndex++] = (quadIndex * 4) + (doW ? 3 : 1);
            
            obj->indices[indicesIndex++] = (quadIndex * 4) + 0;
            obj->indices[indicesIndex++] = (quadIndex * 4) + (doW ? 3 : 2);
            obj->indices[indicesIndex++] = (quadIndex * 4) + (doW ? 2 : 3);
            
            quadIndex++;
        }
    }
    
    //normally, this is used for just 1 wall
    assert(quadIndex == 1);
    assert(indicesIndex == 6);
    
    globject_init_indices(&obj->gl_object, obj->graphics, indicesIndex);
    gl_proctex_init_vertexbuffers(&obj->gl_pt_object, quadIndex*4, "GLWall");
}

void gl_wall_free(GLWall *obj) {
    gl_proctex_object_free(&obj->gl_pt_object);
}

void gl_wall_draw_color(GLWall* obj, t_ivec2 gridPos, t_vec4 colorA) {
    t_vec3 worldPos;
    level_pos_to_world_pos2i(gridPos, worldPos);

    gl_set_color(obj->colorA, colorA[0], colorA[1], colorA[2], colorA[3]);
    gl_proctex_draw(&obj->gl_pt_object, worldPos);
}
