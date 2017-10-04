#include "opengl_game_renderer.h"
#include "../../util/util.h"

#include <stdlib.h>

#include <SDL.h>

#undef main //Weird bug on windows where SDL overwrite main definition

#include <SDL_timer.h>
#include <assert.h>

#include <glmc.h>

static char* findGlErrorDescription(GLenum err) {
    switch(err) {
        case GL_INVALID_OPERATION: return "INVALID_OPERATION";
        case GL_INVALID_ENUM: return "INVALID_ENUM";
        case GL_INVALID_VALUE: return "INVALID_VALUE";
        case GL_OUT_OF_MEMORY: return "OUT_OF_MEMORY";
        case GL_INVALID_FRAMEBUFFER_OPERATION: return "INVALID_FRAMEBUFFER_OPERATION";
            // case GL_INVALID_INDEX: return "GL_INVALID_INDEX";
    }
    return "Unknown GL ERROR";
}

int handle_gl_error2(char *locationA, char *locationB) {
    char both[256];
    assert((strlen(locationA) + strlen(locationB) + 3) < 256);
    
    sprintf(both, "%s %s", locationA, locationB);
    both[255] = 0;
    
    return handle_gl_error(both);
}
int handle_gl_error(const char *location) {
    int hadError = 0;
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        const char *description = findGlErrorDescription(err);
        if (description == NULL) {
            fprintf(stderr, "OpenGL error: %d", err);
        } else {
            fprintf(stderr, "OpenGL error: %d (%s)", err, description);
        }
        
        if (location != NULL)
            fprintf(stderr, " at \"%s\"\n", location);
        else
            fprintf(stderr, "\n");
        
        hadError = 1;
    }
    
    static int allowedErrors = 0;
    //make it fatal for debugging
    assert(!hadError || allowedErrors-- < 1);
    
    return hadError;
}

static GLchar* read_shader_code(char* filename) {
    FILE* fp = fopen (filename, "rb");
    if(!fp)
        fatal("Could not open '%s'. Are you in the right directory?\n", filename);
    
    fseek(fp, 0L, SEEK_END);
    long file_size = ftell(fp);
    if(file_size <= 0) {
        fclose(fp);
        fatal("file '%s' has size %d\n", filename, file_size);
    }
    rewind(fp);
    
    assert(sizeof(GLchar) == 1);
    GLchar* res = calloc(1, file_size+1);
    //note: the +1 assures that res is always null terminated
    if(!res) {
        fclose(fp);
        fatal("memory alloc failed allocating %d bytes for '%s'\n", file_size+1, filename);
    }
    
    if(fread(res, file_size, 1, fp) != 1) {
        free(res);
        fclose(fp);
        fatal("failed to read all %d bytes of '%s'\n", file_size, filename);
    }
    
    fclose(fp);
    
    return res;
}

static GLuint graphics_load_shaders(char* vertex_file_path, char* fragment_file_path){
    // Create the shaders
    GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
    
    GLchar* vertexShaderCode = read_shader_code(vertex_file_path);
    GLchar* fragmentShaderCode = read_shader_code(fragment_file_path);
    
    GLint result = GL_FALSE;
    int infoLogLength;
    
    // Compile Vertex Shader
    glShaderSource(vertexShaderID, 1, (const GLchar **) &vertexShaderCode, NULL);
    free(vertexShaderCode);
    glCompileShader(vertexShaderID);
    
    handle_gl_error("glCompileShader()");
    
    // Check Vertex Shader
    glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &result);
    glGetShaderiv(vertexShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
    if ( infoLogLength > 0 ){
        //GLchar vertexShaderErrorMessage[infoLogLength+1];
		GLchar* vertexShaderErrorMessage = (GLchar *)calloc(infoLogLength + 1, sizeof(GLchar));
        glGetShaderInfoLog(vertexShaderID, infoLogLength, NULL, vertexShaderErrorMessage);
        fprintf(stdout, "Vertex shader compile message: '%s'\n", vertexShaderErrorMessage);
		free(vertexShaderErrorMessage);
    }
    if ( !result ){
        fatal("Failed to compile vertex shader. \n");
    }
    
    handle_gl_error("glGetShaderiv()");
    
    // Compile Fragment Shader
    glShaderSource(fragmentShaderID, 1, (const GLchar **) &fragmentShaderCode, NULL);
    free(fragmentShaderCode);
    glCompileShader(fragmentShaderID);
    
    handle_gl_error("glCompileShader()");
    
    // Check Fragment Shader
    glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &result);
    glGetShaderiv(fragmentShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
    if ( infoLogLength > 0 ) {
        //GLchar fragmentShaderErrorMessage[infoLogLength+1];
		GLchar * fragmentShaderErrorMessage = (GLchar *)calloc(infoLogLength + 1, sizeof(GLchar));
        glGetShaderInfoLog(fragmentShaderID, infoLogLength, NULL, fragmentShaderErrorMessage);
        fprintf(stdout, "Fragment shader compile message: '%s'\n", fragmentShaderErrorMessage);
		free(fragmentShaderErrorMessage);
    }
    if ( !result ){
        fatal("Failed to compile fragment shader. \n");
    }
    
    if (handle_gl_error("glGetShaderiv()")) {
        fatal("Error occured during GL Shader setup.");
    }
    
    // Link the program
    
    GLuint programID = glCreateProgram();
    
    if (handle_gl_error("glCreateProgram()") || programID == 0) {
        fatal("Error occured during GL Program Linking.");
    }
    
    glAttachShader(programID, vertexShaderID);
    glAttachShader(programID, fragmentShaderID);
    
    int sawError = handle_gl_error("glAttachShader()");
    
    if (sawError) {
        fatal("Error in LoadShaders");
    }
    
    glLinkProgram(programID);
    
    sawError |= handle_gl_error("glLinkProgram()");
    if (sawError) {
        fprintf(stderr, "Error when linking shader programs.\n");
        //the caller might try to fall back to another simpler shader
        //remove allocated memory but don't fail hard
        if (programID != 0) {
            glDeleteProgram(programID);
            handle_gl_error("glDeleteProgram");
        }
        return 0;
    }
    
    // Check the program
    glGetProgramiv(programID, GL_LINK_STATUS, &result);
    glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
    if ( infoLogLength > 0 ){
        fprintf(stderr, "Debug info: OpenGL Shading Language version: '%s'\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
        //GLchar programErrorMessage[infoLogLength+1];
		GLchar * programErrorMessage = (GLchar *)calloc(infoLogLength + 1, sizeof(GLchar));
        glGetProgramInfoLog(programID, infoLogLength, NULL, programErrorMessage);
        fprintf(stdout, "Message when linking shader program. '%s'\n", programErrorMessage);
		free(programErrorMessage);
    }
    if (!result) {
        fprintf(stderr, "Failed to link GLSL program. \n");
        //the caller might try to fall back to another simpler shader
        //remove allocated memory but don't fail hard
        if (programID != 0) {
            glDeleteProgram(programID);
            handle_gl_error("glDeleteProgram");
        }
        return 0;
    }
    
    sawError |= handle_gl_error("glGetProgramiv()");
    
    glDetachShader(programID, vertexShaderID);
    glDetachShader(programID, fragmentShaderID);
    
    sawError |= handle_gl_error("glDetachShader()");
    
    glDeleteShader(vertexShaderID);
    glDeleteShader(fragmentShaderID);
    
    sawError |= handle_gl_error("glDeleteShader()");
    
    if (sawError) {
        fatal("Error in LoadShaders");
    }
    
    return programID;
}


int get_program_index(Graphics* g, GLuint program_id) {
    for (int i = 0; i < MAX_PROGRAM_COUNT; i++) {
        if (g->program_id[i] == program_id)
            return i;
    }
    return 0;
}
GLuint get_program_id(Graphics* g, int program_index) {
    return g->program_id[program_index];
}


static GLuint init_attrib_location(GLuint program, char* attrib_name) {
    GLuint attribute_id = glGetAttribLocation(program, attrib_name);
    if (handle_gl_error("glGetAttribLocation()")) {
        fatal("Error in glGetAttribLocation for program %d attribute %d (\"%s\")", program, attribute_id, attrib_name);
    }
    return attribute_id;
}

static GLuint init_uniform_location(GLuint program_id, char* uniform_name) {
    GLuint uniform_id = glGetUniformLocation(program_id, uniform_name);
    if (handle_gl_error("glGetUniformLocation()")) {
        fatal("Error in glGetUniformLocation for program %d uniform %d (\"%s\")", program_id, uniform_id, uniform_name);
    }
//    printf("Program %d Uniform %s ID = %d\n", program_id, uniform_name, uniform_id);
    return uniform_id;
}



static void init_attributes_and_uniform_ids(Graphics *graphics, int program_index, GLuint program_id) {
    assert(graphics != NULL);
    assert(program_id != 0);
    assert(graphics->program_id[program_index] == program_id);

#define X(ATTRIB) graphics->attribute_##ATTRIB[program_index] = init_attrib_location(program_id, #ATTRIB);
    ATTRIBUTE_XMACROS
#undef X

#define X(UNIFORM) graphics->uniform_##UNIFORM[program_index] = init_uniform_location(program_id, #UNIFORM);
    UNIFORM_XMACROS
#undef X
}


//t_vec3 levelPosToWorldPos2i(t_ivec2 levelPos) {
//return glm::vec3(
//(2.0f * levelPos.x),
//(0.0f),
//(-2.0f * levelPos.y)
//);
//}
//t_vec3 levelPosToWorldPos3i(t_ivec3 levelPos) {
//return glm::vec3(
//(2.0f * levelPos.x),
//(2.0f * levelPos.z),
//(-2.0f * levelPos.y)
//);
//}
void level_pos_to_world_pos2i(t_ivec2 levelPos, t_vec3 dest) {
    assert((void*)levelPos != (void*)dest);
    dest[0] = 2.0f * levelPos[0];
    dest[1] = 0.0f;
    dest[2] = -2.0f * levelPos[1];
}
void level_pos_to_world_pos2f(t_vec2 levelPos, t_vec3 dest) {
    assert((void*)levelPos != (void*)dest);
    dest[0] = 2.0f * levelPos[0];
    dest[1] = 0.0f;
    dest[2] = -2.0f * levelPos[1];
}
void level_pos_to_world_pos3f(t_vec3 levelPos, t_vec3 dest) {
    assert(levelPos != dest);
    dest[0] = 2.0f * levelPos[0];
    dest[1] = 2.0f * levelPos[2];
    dest[2] = -2.0f * levelPos[1];
}

int is_sensible_world_coordinate(t_vec3 worldPos) {
    if (worldPos[0] < -50.0) return 0;
    if (worldPos[1] < -50.0) return 0;
    if (worldPos[2] < -50.0) return 0;
    if (worldPos[0] > 50.0) return 0;
    if (worldPos[1] > 50.0) return 0;
    if (worldPos[2] > 50.0) return 0;
    return 1;
}


int is_sensible_color(t_vec4 color) {
    //special colors interpreted by fragment shader
    if (color[0] == 2.0f) return 1;
    if (color[0] == 3.0f) return 1;
    if (color[0] == 4.0f) return 1;
    if (color[0] == 5.0f) return 1;
    if (color[0] == 6.0f) return 1;
    
    for (int i = 0; i < 4; i++) {
        if (color[i] < 0.0f) return 0;
        if (color[i] > 1.0f) return 0;
    }
    return 1;
}


void gl_copy_color_to_from(gl_color dest, gl_color source) {
    dest[0] = source[0];
    dest[1] = source[1];
    dest[2] = source[2];
    dest[3] = source[3];
}

void gl_set_color(gl_color dest, GLfloat r, GLfloat g, GLfloat b, GLfloat a) {
    dest[0] = r;
    dest[1] = g;
    dest[2] = b;
    dest[3] = a;
}



Graphics* graphics_alloc(int width, int height) {
    Graphics* res = malloc(sizeof(Graphics));
    graphics_init(res, width, height);
    return res;
}

void graphics_clear(Graphics* g) {

}

void graphics_free(Graphics* g) {
    glDeleteProgram(g->program_id[PROGRAM_PROCTEX]);
    handle_gl_error("glDeleteProgram");
    g->program_id[PROGRAM_PROCTEX] = -1;

#ifdef RPI
    bcm_host_deinit();
#endif
    
    SDL_Quit( );
}

void graphics_init(Graphics* graphics, int width, int height) {
    memset(graphics, 0, sizeof(Graphics));
    
    for (int i = 0; i < MAX_PROGRAM_COUNT; i++)
        graphics->program_id[i] = 0;
    
    graphics->background_color[0] = 0.0f;
    graphics->background_color[1] = 0.0f;
    graphics->background_color[2] = 0.3f;
    
    graphics->width = width;
    graphics->height = width;

#ifdef RPI
    bcm_host_init();
#endif
    
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
        fatal( "Video initialization failed: %s\n", SDL_GetError());
    }
    
    /* Information about the current video settings. */
    const SDL_VideoInfo* info = SDL_GetVideoInfo( );
    if( !info ) {
        fatal( "Video query failed: %s\n", SDL_GetError());
    }
    
    if (graphics->width <= 0 || graphics->height <= 0) {
        int w = info->current_w > 1920 ? 1920 : info->current_w;
        int h = info->current_h > 1080 ? 1080 : info->current_h;
        graphics->width = w;
        graphics->height = h;
        graphics->width = w;
        graphics->height = h;
        printf("Using automatic resolution: %dx%d\n", graphics->width, graphics->height);
    }
    
    graphics->screenMidX = graphics->width/2;
    graphics->screenMidY = graphics->height/2;
#ifdef RPI
    graphics->vidSurface = SDL_SetVideoMode(0,0, 32,  SDL_SWSURFACE);
#else
    
    /* Color depth in bits of our window. */
    int bpp = info->vfmt->BitsPerPixel;
    
    SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 5 );
    SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 6 );
    SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 5 );
    SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 16 );
    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );

//    flags = SDL_OPENGL  | SDL_FULLSCREEN;
    int flags = SDL_OPENGL;
    graphics->vidSurface = SDL_SetVideoMode(graphics->width, graphics->height, bpp, flags);
#endif
    if (graphics->vidSurface == NULL) {
        fatal("Video mode set failed: %s\n", SDL_GetError());
    }

#ifndef RPI
    // Initialize GLEW
    glewExperimental = 1; // Needed for core profile
    GLenum glewInitErr = glewInit();
    if (glewInitErr != GLEW_OK) {
        fatal("Failed to initialize GLEW: \"%s\"\n", glewGetErrorString(glewInitErr));
    }
    //fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
    
    int r, g, b, d;
    SDL_GL_GetAttribute(SDL_GL_RED_SIZE, &r);
    SDL_GL_GetAttribute(SDL_GL_GREEN_SIZE, &g);
    SDL_GL_GetAttribute(SDL_GL_BLUE_SIZE, &b);
    SDL_GL_GetAttribute(SDL_GL_DEPTH_SIZE, &d);
    //printf("Window has OpenGL context with sizes: Red=%d, Green=%d, Blue=%d Depth=%d\n", r, g, b, d);
#else
    
    int32_t success = 0;
    EGLBoolean result;
    EGLint num_config;

    bcm_host_init();

    DISPMANX_ELEMENT_HANDLE_T dispman_element;
    DISPMANX_DISPLAY_HANDLE_T dispman_display;
    DISPMANX_UPDATE_HANDLE_T dispman_update;
    VC_RECT_T dst_rect;
    VC_RECT_T src_rect;

    static const EGLint attribute_list[] =
    {
        EGL_RED_SIZE, 4,
        EGL_GREEN_SIZE, 4,
        EGL_BLUE_SIZE, 4,
        EGL_ALPHA_SIZE, 4,
        EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
        EGL_DEPTH_SIZE, 4,
        EGL_NONE
    };

    static const EGLint context_attributes[] =
    {
        EGL_CONTEXT_CLIENT_VERSION, 2,
        EGL_NONE
    };

    EGLConfig config;
//    memset( g->context, 0, sizeof( EGLContext) );
//    memset( nativewindow, 0, sizeof( EGL_DISPMANX_WINDOW_T) );
//    memset( display, 0, sizeof( EGLDisplay) );
//    memset( g->surface, 0, sizeof( EGLSurface) );

    // get an EGL display connection
    graphics->display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

    // initialize the EGL display connection
    result = eglInitialize(graphics->display, NULL, NULL);

    // get an appropriate EGL frame buffer configuration
    result = eglSaneChooseConfigBRCM(graphics->display, attribute_list, &config, 1, &num_config);
    assert(EGL_FALSE != result);

   {
      EGLint r, g, b, a, depth, stencil, samples, sample_buffers;
      eglGetConfigAttrib(graphics->display, config, EGL_RED_SIZE, &r);
      eglGetConfigAttrib(graphics->display, config, EGL_GREEN_SIZE, &g);
      eglGetConfigAttrib(graphics->display, config, EGL_BLUE_SIZE, &b);
      eglGetConfigAttrib(graphics->display, config, EGL_ALPHA_SIZE, &a);
      eglGetConfigAttrib(graphics->display, config, EGL_DEPTH_SIZE, &depth);
      eglGetConfigAttrib(graphics->display, config, EGL_STENCIL_SIZE, &stencil);
      eglGetConfigAttrib(graphics->display, config, EGL_SAMPLES, &samples);
      eglGetConfigAttrib(graphics->display, config, EGL_SAMPLE_BUFFERS, &sample_buffers);
      //fprintf(stdout, "Chose EGL config %d/%d: r=%d,g=%d,b=%d,a=%d, "
      //   "depth=%d,stencil=%d, samples=%d,sample_buffers=%d\n",
      //   (int)config, num_config, r, g, b, a, depth, stencil, samples, sample_buffers);
   }

    // get an appropriate EGL frame buffer configuration
    result = eglBindAPI(EGL_OPENGL_ES_API);
    assert(EGL_FALSE != result);


    // create an EGL rendering context
    graphics->context = eglCreateContext(graphics->display, config, EGL_NO_CONTEXT, context_attributes);
    assert(graphics->context!=EGL_NO_CONTEXT);

    // create an EGL window surface
    success = graphics_get_display_size(0 /* LCD */, (unsigned int*)&width, (unsigned int*)&height);
    assert( success >= 0 );

    dst_rect.x = 0;
    dst_rect.y = 0;
    dst_rect.width = width;
    dst_rect.height = height;

    src_rect.x = 0;
    src_rect.y = 0;
    src_rect.width = width << 16;
    src_rect.height = height << 16;

    dispman_display = vc_dispmanx_display_open( 0 /* LCD */);
    dispman_update = vc_dispmanx_update_start( 0 );

  VC_DISPMANX_ALPHA_T alpha = { DISPMANX_FLAGS_ALPHA_FIXED_ALL_PIXELS,255,0 };
  
   DISPMANX_CLAMP_T clamp;
    memset(&clamp, 0x0, sizeof(DISPMANX_CLAMP_T));

    dispman_element =
    vc_dispmanx_element_add(dispman_update, dispman_display,
                1/*layer*/, &dst_rect, 0/*src*/,
                &src_rect, DISPMANX_PROTECTION_NONE,
                &alpha /*alpha*/, &clamp/*clamp*/, (DISPMANX_TRANSFORM_T)0/*transform*/);

    graphics->nativewindow.element = dispman_element;
    graphics->nativewindow.width = width;
    graphics->nativewindow.height = height;
    vc_dispmanx_update_submit_sync( dispman_update );

    graphics->surface = eglCreateWindowSurface( graphics->display, config, &(graphics->nativewindow), NULL );
    assert(graphics->surface != EGL_NO_SURFACE);

    // connect the context to the surface
    result = eglMakeCurrent(graphics->display, graphics->surface, graphics->surface, graphics->context);
    assert(EGL_FALSE != result);
    
   if (eglSwapInterval(graphics->display, 1) == EGL_FALSE) {
	  fatal("Could not set swap interval\n");
    }
#endif
    
    
    SDL_ShowCursor(1);
    
    
    //window is ready, now setup OpenGL
    
    // Create and compile our GLSL program from the shaders
    
    //We use a simpler shader on the raspberry PI.

#undef FORCE_SIMPLE_SHADER

#ifdef RPI
#define FORCE_SIMPLE_SHADER
#endif

#ifndef FORCE_SIMPLE_SHADER
    graphics->program_id[PROGRAM_PROCTEX] = graphics_load_shaders("pi_escape/graphics/PuzzleMVP.vertexshader",
                                                                  "pi_escape/graphics/PuzzleProcTex.fragmentshader");
//    graphics->program_id[PROGRAM_PROCTEX] = graphics_load_shaders("pi_escape/graphics/simple.vertexshader",
//                                               "pi_escape/graphics/simple.fragmentshader");
    
    if (graphics->program_id[PROGRAM_PROCTEX] == 0) {
        fprintf(stderr, "Something went wrong with normal shader. Will try a simpler shader, without light source.\n");
#endif
        graphics->program_id[PROGRAM_PROCTEX] = graphics_load_shaders("pi_escape/graphics/PuzzleMVP_nolight.vertexshader",
                                                                      "pi_escape/graphics/PuzzleProcTex_nolight.fragmentshader");
//        graphics->program_id[PROGRAM_PROCTEX] = graphics_load_shaders("pi_escape/graphics/simple.vertexshader",
//                                                   "pi_escape/graphics/simple.fragmentshader");
        
        if (graphics->program_id[PROGRAM_PROCTEX] == 0) {
#ifndef FORCE_SIMPLE_SHADER
            fatal("The fallback shader failed to link as well. Cannot continue.\n");
#else
            fprintf(stderr, "Something went wrong with shader. Cannot continue.\n");
#endif
        }
#ifndef FORCE_SIMPLE_SHADER
    }
#endif
    
    printf("Program %d ProcTex ID=%d\n", PROGRAM_PROCTEX, graphics->program_id[PROGRAM_PROCTEX]);
    init_attributes_and_uniform_ids(graphics, PROGRAM_PROCTEX, graphics->program_id[PROGRAM_PROCTEX]);
    
    
    
    graphics->program_id[PROGRAM_HUD] = graphics_load_shaders("pi_escape/graphics/hud.vertexshader", "pi_escape/graphics/hud.fragmentshader");
    if (graphics->program_id[PROGRAM_HUD] == 0) {
        fprintf(stderr, "Something went wrong with hud shader. Cannot continue.\n");
    }
    printf("Program %d PROGRAM_HUD ID=%d\n", PROGRAM_HUD, graphics->program_id[PROGRAM_HUD]);
    init_attributes_and_uniform_ids(graphics, PROGRAM_HUD, graphics->program_id[PROGRAM_HUD]);
}

void mat4_multiply(t_mat4 a, t_mat4 b, t_mat4 dest) {
    dest[0] = a[0] * b[0] + a[4] * b[1] + a[8] * b[2] + a[12] * b[3];
    dest[1] = a[1] * b[0] + a[5] * b[1] + a[9] * b[2] + a[13] * b[3];
    dest[2] = a[2] * b[0] + a[6] * b[1] + a[10] * b[2] + a[14] * b[3];
    dest[3] = a[3] * b[0] + a[7] * b[1] + a[11] * b[2] + a[15] * b[3];
    
    dest[4] = a[0] * b[4] + a[4] * b[5] + a[8] * b[6] + a[12] * b[7];
    dest[5] = a[1] * b[4] + a[5] * b[5] + a[9] * b[6] + a[13] * b[7];
    dest[6] = a[2] * b[4] + a[6] * b[5] + a[10] * b[6] + a[14] * b[7];
    dest[7] = a[3] * b[4] + a[7] * b[5] + a[11] * b[6] + a[15] * b[7];
    
    dest[8] = a[0] * b[8] + a[4] * b[9] + a[8] * b[10] + a[12] * b[11];
    dest[9] = a[1] * b[8] + a[5] * b[9] + a[9] * b[10] + a[13] * b[11];
    dest[10] = a[2] * b[8] + a[6] * b[9] + a[10] * b[10] + a[14] * b[11];
    dest[11] = a[3] * b[8] + a[7] * b[9] + a[11] * b[10] + a[15] * b[11];
    
    dest[12] = a[0] * b[12] + a[4] * b[13] + a[8] * b[14] + a[12] * b[15];
    dest[13] = a[1] * b[12] + a[5] * b[13] + a[9] * b[14] + a[13] * b[15];
    dest[14] = a[2] * b[12] + a[6] * b[13] + a[10] * b[14] + a[14] * b[15];
    dest[15] = a[3] * b[12] + a[7] * b[13] + a[11] * b[14] + a[15] * b[15];
}

void graphics_begin_draw(Graphics* graphics) {
    //draw without light

    t_vec3 dummy = { 0.0f };
    graphics_begin_draw_withlight(graphics, 0.0f, dummy);
}

void graphics_begin_draw_withlight(Graphics* graphics, float lightPower, t_vec3 lightPos) {
    handle_gl_error("PRE graphics_begin_draw_withlight");

#ifdef RPI
    //glViewport(0, 0, width, height);
#endif
    
    //TODO: implement glm calculations in C
    //a good start is GLM C -> https://github.com/prdbrg/glmc
    
//    t_vec4 frustrum = { 45.0f, 4.0f / 3.0f, 0.1f, 100.0f };
    //reduced far and near plane to reduce z-fighting
    t_vec4 frustrum = { 45.0f, 4.0f / 3.0f, 2.0f, 60.0f };
    t_mat4 projection;// = { 0 };
    glmc_perspective(frustrum, projection);
    
    t_vec3 lookFrom, lookAt;
    level_pos_to_world_pos3f(graphics->cameraLookFrom, lookFrom);
    level_pos_to_world_pos3f(graphics->cameraLookAt, lookAt);
    
    t_mat4 view;// = { 0 };
    glmc_look_at(lookFrom, lookAt, graphics->cameraUp, view);
    
    t_mat4 model;// = { 0 };
    glmc_identity(model);
    
    t_mat4 vp;// = { 0 };
    mat4_multiply(projection, view, vp);
    t_mat4 mvp;// = { 0 };
    mat4_multiply(vp, model, mvp);
    
    
    // Clear the screen
    glUseProgram(graphics->program_id[PROGRAM_PROCTEX]);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    handle_gl_error("graphics_begin_draw_withlight glClear");
    
    glClearColor(graphics->background_color[0], graphics->background_color[1], graphics->background_color[2], 1.0f);

    
    
    handle_gl_error("graphics_begin_draw_withlight pre glUniform handling");
//    for (int i = 0 ; i < PROGRAM_COUNT; i++) {
//        ProgramSpecificInfo* program = programs[i];
    
    glUseProgram(graphics->program_id[PROGRAM_PROCTEX]);
    
    //***** Uniforms the same for entire scene **********
    if (HAS_UNIFORM(graphics, PROGRAM_PROCTEX, MVP)) {
        glUniformMatrix4fv(graphics->uniform_MVP[PROGRAM_PROCTEX], 1, GL_FALSE, mvp);
        if (handle_gl_error("graphics_begin_draw_withlight glUniformMatrix4fv() MVP")) {
            fprintf(stderr, "        FAILED glUniformMatrix4fv() MVP for programId=%d  MVPid=%u\n",
                    graphics->program_id[PROGRAM_PROCTEX],
                    graphics->uniform_MVP[PROGRAM_PROCTEX]);
        }
    }
    if (HAS_UNIFORM(graphics, PROGRAM_PROCTEX, M)) {
        glUniformMatrix4fv(graphics->uniform_M[PROGRAM_PROCTEX], 1, GL_FALSE, model);
        handle_gl_error("graphics_begin_draw_withlight glUniformMatrix4fv() M");
    }
    if (HAS_UNIFORM(graphics, PROGRAM_PROCTEX, V)) {
        glUniformMatrix4fv(graphics->uniform_V[PROGRAM_PROCTEX], 1, GL_FALSE, view);
        handle_gl_error("graphics_begin_draw_withlight glUniformMatrix4fv() V");
    }
    
    if (HAS_UNIFORM(graphics, PROGRAM_PROCTEX, LightPosition_worldspace)) {
        assert(is_sensible_world_coordinate(lightPos));
        glUniform3f(graphics->uniform_LightPosition_worldspace[PROGRAM_PROCTEX], lightPos[0], lightPos[1], lightPos[2]);
        handle_gl_error("graphics_begin_draw_withlight glUniform3f() LightPosition_worldspace");
    }
    
    if (HAS_UNIFORM(graphics, PROGRAM_PROCTEX, LightPower)) {
        assert(lightPower >= 0.0f);
        assert(lightPower <= 1000.0f);
        glUniform1f(graphics->uniform_LightPower[PROGRAM_PROCTEX], lightPower);
        handle_gl_error("graphics_begin_draw_withlight glUniform1f() LightPower");
    }
    
    if (HAS_UNIFORM(graphics, PROGRAM_PROCTEX, TimeTicks)) {
        glUniform1f(graphics->uniform_TimeTicks[PROGRAM_PROCTEX], SDL_GetTicks() / 10.0f);
        handle_gl_error("graphics_begin_draw_withlight glUniform1f() TimeTicks");
    }
    
    //defaults in case any object forgets to set them
//    glDisable(GL_DEPTH_TEST);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    
    glEnable(GL_CULL_FACE);
//    glDisable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    
    glDisable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//    }
}

void graphics_end_draw(Graphics* g) {
/*
 * Swap the buffers. This this tells the driver to
 * render the next frame from the contents of the
 * back-buffer, and to set all rendering operations
 * to occur on what was the front-buffer.
 *
 * Double buffering prevents nasty visual tearing
 * from the application drawing on areas of the
 * screen that are being updated at the same time.
 */
#ifdef RPI
    eglSwapBuffers(g->display, g->surface);
#else
    SDL_GL_SwapBuffers( );
#endif
    
    handle_gl_error("POST graphics_end_draw()");
}
