#ifndef GLOBJECT_H
#define GLOBJECT_H

// Include GLEW
#ifndef RPI
#include <GL/glew.h>
#else
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES/gl.h>

#include <bcm_host.h>
#endif

#include "opengl_game_renderer.h"

#define GLOBJECT_MAX_VERTICES (4*6)                         //enough for a cube with each side seperate vertices
#define GLOBJECT_MAX_TRIANGLES (4*6*2)                      //enough for a cube
#define GLOBJECT_MAX_INDICES (GLOBJECT_MAX_TRIANGLES * 3)   //3 points per triangle...

typedef struct GLObject {
    int program_index; //index in graphics->program_id  (not program ID in OpenGL itself, that is graphics->program_id[program_index])
    
    GLuint elementbuffer;
    int indicesCount;
    unsigned short int indices[GLOBJECT_MAX_INDICES];
} GLObject;

void globject_init(GLObject*, Graphics*, GLuint program_id);
void globject_free(GLObject *);

void globject_init_indices(GLObject*, Graphics*, int indicesCount);

#endif //GLOBJECT_H
