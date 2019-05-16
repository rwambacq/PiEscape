#ifndef PIESCAPE2_GL_FLOOR_H
#define PIESCAPE2_GL_FLOOR_H

#include "gl_proctex_object.h"

typedef struct GLFloor {
#ifdef __cplusplus
    struct GLProcTexObject parent;
#else
    union {
        struct GLProcTexObject;
        GLProcTexObject gl_pt_object;
    };
#endif
    
    GLfloat normalAmbiantLightMulti;
    GLfloat playerTouchAmbiantLightMulti;
    GLfloat activeAmbiantLightMulti;
    GLfloat inactiveExitAmbiantLightMulti;
    GLfloat activeExitAmbiantLightMulti;
    
    GLfloat defaultProceduralTextureParameterA;
    GLfloat defaultProceduralTextureID;
    GLfloat activatedProceduralTextureID;
    
    gl_color normalColorA;
    gl_color normalColorB;
    gl_color playerTouchColorA;
    gl_color playerTouchColorB;
    gl_color activeColorA;
    gl_color activeColorB;
    gl_color inactiveColorA;
    gl_color inactiveColorB;
    gl_color inactiveExitColorA;
    gl_color inactiveExitColorB;
    gl_color activeExitColorA;
    gl_color activeExitColorB;
} GLFloor;

void gl_floor_init(GLFloor *, Graphics *);
void gl_floor_free(GLFloor *);
void  gl_floor_draw_color(GLFloor* obj, t_ivec2 gridPos, int highFloor, CellDetail cellDetail, float animationPosition, t_vec4 colorA);

#endif //PIESCAPE2_GL_FLOOR_H
