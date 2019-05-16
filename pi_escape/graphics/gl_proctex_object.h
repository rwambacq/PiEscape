#ifndef GLPROCTEXOBJECT_H
#define GLPROCTEXOBJECT_H

#include "globject.h"

#define GLLEVELPART_MAX_QUADS 6
#define GLLEVELPART_MAX_VERTICES (GLLEVELPART_MAX_QUADS*4)



typedef struct GLProcTexObject {
#ifdef __cplusplus
    struct GLObject gl_object;
#else
    union {
        struct GLObject;
        GLObject gl_object;
    };
#endif
    
    Graphics* graphics;
    
    GLuint program_id;  //program ID in OpenGL   (not index in graphics->program_id, note that: program_id = graphics->program_id[program_index])
    
    int blend;
    int no_cull;
    
    GLuint vertexbuffer;
    GLuint normalbuffer;
    GLuint texcoordbuffer;
    
    int vertexCount;
    GLfloat vertex_buffer_data[GLLEVELPART_MAX_VERTICES*3];
    GLfloat normal_buffer_data[GLLEVELPART_MAX_VERTICES*3];
    GLfloat texCoord_buffer_data[GLLEVELPART_MAX_VERTICES*2];
    
    GLfloat minimumAmbiantLightMulti;
    GLfloat defaultAmbiantLightMulti;
    GLfloat diffuseLightMulti;
    GLfloat specularLightMulti;
    GLfloat proceduralTextureID;
    gl_color colorA;
    gl_color colorB;
    GLfloat proceduralTextureParameterA;
    GLfloat proceduralTextureParameterB;
    GLfloat proceduralTextureParameterC;
    GLfloat proceduralTextureParameterD;
} GLProcTexObject;

void gl_proctex_object_init(GLProcTexObject *, Graphics *, int program_index);
void gl_proctex_object_free(GLProcTexObject *);

void gl_proctex_init_vertexbuffers(GLProcTexObject *, int vertexCount, char *objName);

void gl_proctex_draw(GLProcTexObject *, t_vec3 translation);
void gl_proctex_draw_call(GLProcTexObject *);


#endif //GLPROCTEXOBJECT_H
