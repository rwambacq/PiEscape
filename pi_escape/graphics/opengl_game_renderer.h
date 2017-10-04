#ifndef OPENGL_GAME_RENDERER_H
#define OPENGL_GAME_RENDERER_H

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

#include <glmc.h>
#include <SDL_video.h>

typedef enum GameColor { RED, BLUE, GREEN, KEY_ALL, LOCK_ANY } GameColor;
typedef enum GridAxis { X, Y } GridAxis;
typedef enum ConnectorCenterType { AND, OR } ConnectorCenterType;
typedef enum CellDetail { NONE, PLAYER_TOUCH, EXIT_TOUCH, ACTIVE, INACTIVE } CellDetail;

#define UNIFORM_XMACROS \
X(MVP) \
X(V) \
X(M) \
X(LightPosition_worldspace) \
X(TextureLightSettings) \
X(LightPower) \
X(PerDrawTransformation_worldspace) \
X(TimeTicks) \
X(ProceduralTextureID) \
X(ColorA) \
X(ColorB) \
X(ProceduralTextureParameter) \
X(TextureSampler)

#define ATTRIBUTE_XMACROS \
X(VertexBasePosition_modelspace) \
X(VertexNormal_modelspace) \
X(VertexTexCoord)

//#define UNIFORM_XMACROS \
//X(MVP)
//
//#define ATTRIBUTE_XMACROS \
//X(VertexBasePosition_modelspace)

#define MAX_PROGRAM_COUNT 3

#define PROGRAM_PROCTEX 0
#define PROGRAM_HUD 1
#define PROGRAM_TEX 2

typedef struct Graphics {
    SDL_Surface* vidSurface;
    
    uint32_t width;
    uint32_t height;
    
    uint32_t screenMidX;
    uint32_t screenMidY;
    
    GLuint program_id[MAX_PROGRAM_COUNT];
    
    t_vec3 background_color;
    t_vec3 cameraLookFrom;
    t_vec3 cameraLookAt;
    t_vec3 cameraUp;
    
#define X(ATTRIB) GLuint uniform_##ATTRIB[MAX_PROGRAM_COUNT];
    UNIFORM_XMACROS
#undef X

#define X(ATTRIB) GLuint attribute_##ATTRIB[MAX_PROGRAM_COUNT];
    ATTRIBUTE_XMACROS
#undef X

#ifdef RPI
    EGL_DISPMANX_WINDOW_T nativewindow;
    EGLDisplay display;
    EGLSurface surface;
    EGLContext context;
#endif
} Graphics;

Graphics* graphics_alloc(int width, int height);
void graphics_init(Graphics*, int width, int height);
void graphics_clear(Graphics*);
void graphics_free(Graphics*);

void graphics_begin_draw(Graphics*);
void graphics_begin_draw_withlight(Graphics*, float lightPower, t_vec3 lightPos);
void graphics_end_draw(Graphics*);

int handle_gl_error(const char *location);
int handle_gl_error2(char *locationA, char *locationB);

int get_program_index(Graphics*, GLuint program_id);
GLuint get_program_id(Graphics*, int program_index);

#define HAS_ATTRIBUTE(GRAPHICS, PROGRAM_INDEX, ATRIBUTE_NAME) ((GRAPHICS)->attribute_##ATRIBUTE_NAME[PROGRAM_INDEX] != -1)
#define HAS_UNIFORM(GRAPHICS, PROGRAM_INDEX, UNIFORM_NAME) ((GRAPHICS)->uniform_##UNIFORM_NAME[PROGRAM_INDEX] != -1)

#define GET_ATTRIBUTE_ID(GRAPHICS, PROGRAM_INDEX, ATRIBUTE_NAME) ((GRAPHICS)->attribute_##ATRIBUTE_NAME[PROGRAM_INDEX])
#define GET_UNIFORM_ID(GRAPHICS, PROGRAM_INDEX, UNIFORM_NAME) ((GRAPHICS)->uniform_##UNIFORM_NAME[PROGRAM_INDEX])

void level_pos_to_world_pos2i(t_ivec2 levelPos, t_vec3 dest);
void level_pos_to_world_pos2f(t_vec2 levelPos, t_vec3 dest);
void level_pos_to_world_pos3f(t_vec3 levelPos, t_vec3 dest);

int is_sensible_world_coordinate(t_vec3 worldPos);
int is_sensible_color(t_vec4 color);

typedef GLfloat gl_color[4];
void gl_copy_color_to_from(gl_color dest, gl_color source);
void gl_set_color(gl_color dest, GLfloat r, GLfloat g, GLfloat b, GLfloat a);

#endif //OPENGL_GAME_RENDERER_H
