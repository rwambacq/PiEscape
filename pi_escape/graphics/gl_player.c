#include <assert.h>
#include "gl_player.h"
#include <math.h>

#define GLPLAYER_SIZE 0.05

void gl_player_init(GLPlayer *obj, Graphics *graphics) {
    gl_proctex_object_init(&obj->gl_pt_object, graphics, PROGRAM_PROCTEX);
    
    
    obj->defaultAmbiantLightMulti = 0.9f;
    obj->diffuseLightMulti = 0.0f;
    obj->specularLightMulti = 0.0f;
    
    obj->proceduralTextureID = 0.0f;
    
    gl_set_color(obj->colorA, 0.8f, 0.8f, 0.0f, 1.0f);
    gl_set_color(obj->colorB, 0.8f, 0.0f, 0.8f, 1.0f);
    obj->proceduralTextureParameterA = 0.1f;

//    power = 2.0f;
    
    int indicesIndex = 0;
    
    for (int side = 0; side < 6; side++) {
        int normal_x = side == 0 ? 1 : ( side == 1 ? -1 : 0);
        int normal_y = side == 2 ? 1 : ( side == 3 ? -1 : 0);
        int normal_z = side == 4 ? 1 : ( side == 5 ? -1 : 0);
        
        for (int v = 0; v < 4; v++) {
            int i = ((side * 4) + v) * 3;
            
            obj->vertex_buffer_data[i] = normal_x == 0 ?
                                    (v < 2 ? -GLPLAYER_SIZE : GLPLAYER_SIZE) :
                                    normal_x * GLPLAYER_SIZE; //x
            obj->vertex_buffer_data[i + 1] = normal_y == 0 ?
                                        (normal_x != 0 ? (v < 2 ? -GLPLAYER_SIZE : GLPLAYER_SIZE) : (v % 2 == 0 ? GLPLAYER_SIZE : -GLPLAYER_SIZE)) :
                                        normal_y * GLPLAYER_SIZE; //y
            obj->vertex_buffer_data[i + 2] = normal_z == 0 ?
                                        (v % 2 == 0 ? GLPLAYER_SIZE : -GLPLAYER_SIZE) :
                                        normal_z * GLPLAYER_SIZE; //z
            
            obj->normal_buffer_data[i] = normal_x * 1.0; //x
            obj->normal_buffer_data[i + 1] = normal_y * 1.0; //y
            obj->normal_buffer_data[i + 2] = normal_z * 1.0; //z
            
            int j = ((side * 4) + v) * 2;
            obj->texCoord_buffer_data[j] = v < 2 ? 0.0 : 1.0; //u
            obj->texCoord_buffer_data[j+1] = v % 2 == 0 ? 0.0 : 1.0; //w
        }
        
        if (side != 1 && side != 2 && side != 5) {
            obj->indices[indicesIndex++] = (side * 4) + 0;
            obj->indices[indicesIndex++] = (side * 4) + 1;
            obj->indices[indicesIndex++] = (side * 4) + 3;
            
            obj->indices[indicesIndex++] = (side * 4) + 0;
            obj->indices[indicesIndex++] = (side * 4) + 3;
            obj->indices[indicesIndex++] = (side * 4) + 2;
        } else {
            obj->indices[indicesIndex++] = (side * 4) + 0;
            obj->indices[indicesIndex++] = (side * 4) + 3;
            obj->indices[indicesIndex++] = (side * 4) + 1;
            
            obj->indices[indicesIndex++] = (side * 4) + 0;
            obj->indices[indicesIndex++] = (side * 4) + 2;
            obj->indices[indicesIndex++] = (side * 4) + 3;
        }
    }
    
    globject_init_indices(&obj->gl_object, obj->graphics, indicesIndex);
    gl_proctex_init_vertexbuffers(&obj->gl_pt_object, 6 * 4, "GLPlayer");
}

void gl_player_free(GLPlayer *obj) {
    gl_proctex_object_free(&obj->gl_pt_object);
}

static void get_world_pos(uint64_t time, t_vec2 gridPos, t_vec3 dest) {
    level_pos_to_world_pos2f(gridPos, dest);
    
    float bound = 0.3f;
    float base = time / 1000.0f;
    
    dest[0] = dest[0] + sinf(base) * bound;
    dest[1] = dest[1] + cosf(base) * bound;
    dest[2] = dest[2] + sinf(base*2.5f) * bound;
}

void gl_player_draw(GLPlayer* obj, uint64_t time, t_vec2 gridPos) {
//    const float minPower = 10.0f;
//    const float maxPower = 20.0f;
//    float base = time / 1000.0f;
//    power = (sinf(base) * (minPower/2.0f)) + (maxPower - (minPower/2.0f));
    
    t_vec3 worldPos;
    get_world_pos(time, gridPos, worldPos);
    
    
    gl_proctex_draw(&obj->gl_pt_object, worldPos);
}
