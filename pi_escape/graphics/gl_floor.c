#include <assert.h>
#include "gl_floor.h"

void gl_floor_init(GLFloor *obj, Graphics *graphics) {
    gl_proctex_object_init(&obj->gl_pt_object, graphics, PROGRAM_PROCTEX);
    
    gl_set_color(obj->normalColorA, 1.0f, 0.0f, 0.0f, 1.0f);
    gl_set_color(obj->normalColorB, 0.0f, 1.0f, 0.0f, 1.0f);
    
    gl_set_color(obj->playerTouchColorA, 1.0f, 0.0f, 0.0f, 1.0f);
    gl_set_color(obj->playerTouchColorB, 1.0f, 1.0f, 0.0f, 1.0f);
    
    gl_set_color(obj->activeColorA, 1.0f, 1.0f, 0.0f, 1.0f);
    gl_set_color(obj->activeColorB, 1.0f, 1.0f, 0.0f, 1.0f);
    
    gl_set_color(obj->inactiveColorA, 0.0f, 0.0f, 0.0f, 1.0f);
    gl_set_color(obj->inactiveColorB, 0.0f, 0.0f, 0.0f, 1.0f);
    
    gl_set_color(obj->inactiveExitColorA, 6.0f, 1.0f, -1.0f, 1.0f);
    gl_copy_color_to_from(obj->inactiveExitColorB, obj->normalColorB);
    
    gl_set_color(obj->activeExitColorA, 6.0f, 1.0f, 1.0f, 1.0f);
    gl_copy_color_to_from(obj->activeExitColorB, obj->activeColorB);
    
    obj->normalAmbiantLightMulti = 0.4f;
    obj->playerTouchAmbiantLightMulti = 0.6f;
    obj->activeAmbiantLightMulti = 0.8f;
    obj->inactiveExitAmbiantLightMulti = obj->normalAmbiantLightMulti;
    obj->activeExitAmbiantLightMulti = 0.8f;
    
    obj->defaultProceduralTextureParameterA = 0.1f;
    obj->defaultProceduralTextureID = 1.0f;
    obj->activatedProceduralTextureID = 0.0f;
    
    obj->defaultAmbiantLightMulti = obj->normalAmbiantLightMulti;
    obj->diffuseLightMulti = 0.2f;
    obj->specularLightMulti = 0.05f;
    obj->proceduralTextureID = obj->defaultProceduralTextureID;
    obj->proceduralTextureParameterA = obj->defaultProceduralTextureParameterA;
    obj->proceduralTextureParameterB = 0.1f;
    obj->proceduralTextureParameterC = 0.1f;
    obj->proceduralTextureParameterD = 0.1f;
    
    gl_copy_color_to_from(obj->colorA, obj->normalColorA);
    gl_copy_color_to_from(obj->colorB, obj->normalColorB);
    
    int indicesIndex = 0;
    int quadIndex = 0;
    
    for (int v = 0; v < 4; v++) {
        int i = ((quadIndex * 4) + v) * 3;
        
        obj->vertex_buffer_data[i] = v < 2 ? -1.0 : 1.0; //x
        obj->vertex_buffer_data[i + 1] = -1.0; //y
        obj->vertex_buffer_data[i + 2] = v % 2 == 0 ? 1.0 : -1.0; //z
        
        obj->normal_buffer_data[i] = 0.0; //x
        obj->normal_buffer_data[i + 1] = -1.0; //y
        obj->normal_buffer_data[i + 2] = 0.0; //z
        
        int j = ((quadIndex * 4) + v) * 2;
        obj->texCoord_buffer_data[j] = v < 2 ? 0.0 : 1.0; //s
        obj->texCoord_buffer_data[j+1] = v % 2 == 0 ? 0.0 : 1.0; //t
    }
    
    obj->indices[indicesIndex++] = (quadIndex * 4) + 0;
    obj->indices[indicesIndex++] = (quadIndex * 4) + 3;
    obj->indices[indicesIndex++] = (quadIndex * 4) + 1;
    
    obj->indices[indicesIndex++] = (quadIndex * 4) + 0;
    obj->indices[indicesIndex++] = (quadIndex * 4) + 2;
    obj->indices[indicesIndex++] = (quadIndex * 4) + 3;
    
    quadIndex++;
    
    //normally, this is used for just 1 floor
    assert(quadIndex == 1);
    assert(indicesIndex == 6);
    
    globject_init_indices(&obj->gl_object, obj->graphics, indicesIndex);
    gl_proctex_init_vertexbuffers(&obj->gl_pt_object, quadIndex*4, "GLFloor");
}

void gl_floor_free(GLFloor *obj) {
    gl_proctex_object_free(&obj->gl_pt_object);
}

void gl_floor_draw_color(GLFloor* obj, t_ivec2 gridPos, int highFloor, CellDetail cellDetail, float animationPosition, t_vec4 colorA) {
    switch (cellDetail) {
        case ACTIVE: {
            gl_copy_color_to_from(obj->colorA, obj->activeColorA);
            gl_copy_color_to_from(obj->colorB, obj->activeColorB);
            obj->defaultAmbiantLightMulti = obj->activeAmbiantLightMulti;
            obj->proceduralTextureID = obj->activatedProceduralTextureID;
            obj->proceduralTextureParameterA = obj->defaultProceduralTextureParameterA;
            break;
        }
        case INACTIVE: {
            gl_copy_color_to_from(obj->colorA, obj->inactiveColorA);
            gl_copy_color_to_from(obj->colorB, obj->inactiveColorB);
            obj->defaultAmbiantLightMulti = obj->activeAmbiantLightMulti;
            obj->proceduralTextureID = obj->activatedProceduralTextureID;
            obj->proceduralTextureParameterA = obj->defaultProceduralTextureParameterA;
            break;
        }
        case EXIT_TOUCH :{
            if (animationPosition > 0.0f) {
                gl_copy_color_to_from(obj->colorA, obj->activeExitColorA);
                gl_copy_color_to_from(obj->colorB, obj->activeExitColorB);
                obj->defaultAmbiantLightMulti = obj->activeExitAmbiantLightMulti;// + (animationPosition * 0.5f);
                obj->proceduralTextureID = obj->defaultProceduralTextureID;
                //obj->proceduralTextureParameterA = 0.5f + (animationPosition * -0.5f);
                obj->proceduralTextureParameterA = 0.1f;
            } else {
                gl_copy_color_to_from(obj->colorA, obj->activeExitColorA);
                gl_copy_color_to_from(obj->colorB, obj->inactiveExitColorB);
                obj->defaultAmbiantLightMulti = obj->inactiveExitAmbiantLightMulti;
                obj->proceduralTextureID = obj->defaultProceduralTextureID;
                obj->proceduralTextureParameterA = 0.1f;
            }
            break;
        }
        case PLAYER_TOUCH: {
            gl_copy_color_to_from(obj->colorA, colorA);
            gl_copy_color_to_from(obj->colorB, obj->playerTouchColorB);
            obj->defaultAmbiantLightMulti = obj->playerTouchAmbiantLightMulti;
            obj->proceduralTextureID = obj->defaultProceduralTextureID;
            obj->proceduralTextureParameterA = obj->defaultProceduralTextureParameterA;
            break;
        }
        case NONE: {
            gl_copy_color_to_from(obj->colorA, colorA);
            gl_copy_color_to_from(obj->colorB, obj->normalColorB);
            obj->defaultAmbiantLightMulti = obj->normalAmbiantLightMulti;
            obj->proceduralTextureID = obj->defaultProceduralTextureID;
            obj->proceduralTextureParameterA = obj->defaultProceduralTextureParameterA;
            break;
        }
    }

    t_vec3 worldPos;
    level_pos_to_world_pos2i(gridPos, worldPos);

    if (highFloor) {
        worldPos[1] += 2.0f;
    }

    gl_proctex_draw(&obj->gl_pt_object, worldPos);
}
