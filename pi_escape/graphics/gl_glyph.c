#include <assert.h>
#include "gl_glyph.h"
#include "../../util/util.h"

void gl_glyph_init(GLGlyph *obj, Graphics *graphics, char* font_image_filename) {
    gl_proctex_object_init((GLProcTexObject*) obj, graphics, PROGRAM_HUD);
    obj->no_cull = 1;
    obj->blend = 1;
    
    obj->defaultAmbiantLightMulti = 0.4f;
    obj->diffuseLightMulti = 0.2f;
    obj->specularLightMulti = 0.05f;
    obj->proceduralTextureID = 1.0f;
    gl_set_color(obj->colorA, 0.8f, 0.0f, 0.0f, 1.0f);
    gl_set_color(obj->colorB, 0.0f, 0.8f, 0.0f, 1.0f);
    obj->proceduralTextureParameterA = 0.1f;
    obj->proceduralTextureParameterB = 0.1f;
    obj->proceduralTextureParameterC = 0.1f;
    obj->proceduralTextureParameterD = 0.1f;
    
    int indicesIndex = 0;
    int quadIndex = 0;
    
    t_vec3 trans = { 0.2f, 0.3f, 0.0f };
    t_vec3 scale = { 0.1f, 0.1f, 0.0f };
    
    for (int v = 0; v < 4; v++) {
        int i = ((quadIndex * 4) + v) * 3;
        
        obj->vertex_buffer_data[i] = v < 2 ? -1.0 : 1.0; //x
        obj->vertex_buffer_data[i + 1] = v % 2 == 0 ? 1.0 : -1.0; //y
        obj->vertex_buffer_data[i + 2] = 0.0;
        
        obj->vertex_buffer_data[i] = (scale[0] * obj->vertex_buffer_data[i]) + trans[0]; //x
        obj->vertex_buffer_data[i+1] = (scale[1] * obj->vertex_buffer_data[i+1]) + trans[1]; //y
        obj->vertex_buffer_data[i+2] = (scale[2] * obj->vertex_buffer_data[i+2]) + trans[2]; //z
        
        obj->normal_buffer_data[i] = 0.0; //x
        obj->normal_buffer_data[i + 1] = 0.0; //y
        obj->normal_buffer_data[i + 2] = 1.0;
        
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
    
    //normally, this is used for just 1 wall
    assert(quadIndex == 1);
    assert(indicesIndex == 6);
    
    globject_init_indices((GLObject*) obj, obj->graphics, indicesIndex);
    gl_proctex_init_vertexbuffers((GLProcTexObject*) obj, quadIndex*4, "GLGlyph");
    
    
    
    
    
    SDL_Surface* loadedSurface = IMG_Load(font_image_filename);
    if( loadedSurface == NULL ) {
        fatal( "Unable to load image %s! SDL_image Error: %s\n", font_image_filename, IMG_GetError() );
    }
    
    obj->font_image = SDL_DisplayFormatAlpha(loadedSurface);
    if( obj->font_image == NULL ) {
        fatal( "Unable to optimize image %s! SDL Error: %s\n", font_image_filename, SDL_GetError() );
    }
    //Get rid of old loaded surface
    SDL_FreeSurface( loadedSurface );
    
    obj->w = obj->font_image->w;
    obj->h = obj->font_image->h;
    
    handle_gl_error("PRE gl_glyph init glBindTexture");
    
    glGenTextures(1, &obj->tex);
    glBindTexture(GL_TEXTURE_2D, obj->tex);
    handle_gl_error("POST gl_glyph init glBindTexture");
    
    
    int max_texture_size = 0;
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max_texture_size);
    printf("Loaded image with size %dx%d  (max is %d)\n", obj->w, obj->h, max_texture_size);
    
	printf("ok1\n");
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 GL_RGBA,
                 obj->w,
                 obj->h,
                 0,
                 GL_RGBA,
                 GL_UNSIGNED_BYTE,
                 obj->font_image->pixels);
    handle_gl_error("POST gl_glyph init glTexImage2D");
	printf("ok1\n");

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //GL_NEAREST
	printf("ok1\n");
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //GL_NEAREST
	printf("ok1\n");
    handle_gl_error("POST gl_glyph init");
	printf("ok1\n");
}

void gl_glyph_free(GLGlyph *obj) {
    gl_proctex_object_free((GLProcTexObject*) obj);
    
    SDL_FreeSurface( obj->font_image );
}

void gl_glyph_draw(GLGlyph* obj,
                      int pos_ltop_x, int pos_ltop_y,
                      int glyph_x, int glyph_y,
                      int glyph_w, int glyph_h,
                      const t_vec4 color) {
    glmc_assign_vec4(obj->colorA, color);
    
    t_vec2 crop_start = { (glyph_x*1.0f) / obj->w,
                          (glyph_y*1.0f) / obj->h };
    t_vec2 crop_end = { ((glyph_x + glyph_w)*1.0f) / obj->w,
                        ((glyph_y + glyph_h)*1.0f) / obj->h };
    
    int screen_w = obj->graphics->width;
    int screen_h = obj->graphics->height;
    
    float pos_mid_x = pos_ltop_x + (glyph_w / 2.0f);
    float pos_mid_y = pos_ltop_y - (glyph_h / 2.0f);
    
    t_vec3 trans = { (((pos_mid_x*2.0f)/screen_w)-1.0f),
                     (((pos_mid_y*2.0f)/screen_h)-1.0f),
                     0.0f };
    t_vec3 scale = { (glyph_w*1.0f)/screen_w,
                     (glyph_h*1.0f)/screen_h,
                     0.0f };
    
    for (int v = 0; v < 4; v++) {
        int i = v * 3;
        
        obj->vertex_buffer_data[i] = v < 2 ? -1.0 : 1.0;          //x
        obj->vertex_buffer_data[i + 1] = v % 2 == 0 ? 1.0 : -1.0; //y
        obj->vertex_buffer_data[i + 2] = 0.0;                     //z
        
        obj->vertex_buffer_data[i] = (scale[0] * obj->vertex_buffer_data[i]) + trans[0];     //x
        obj->vertex_buffer_data[i+1] = (scale[1] * obj->vertex_buffer_data[i+1]) + trans[1]; //y
        obj->vertex_buffer_data[i+2] = (scale[2] * obj->vertex_buffer_data[i+2]) + trans[2]; //z
        
        int j = v * 2;
        obj->texCoord_buffer_data[j] = v < 2 ? crop_start[0] : crop_end[0];        //u
        obj->texCoord_buffer_data[j+1] = v % 2 == 0 ? crop_start[1] : crop_end[1]; //w
    }
    
    handle_gl_error("POST gl_glyph update bufferdata");
    glBindBuffer(GL_ARRAY_BUFFER, obj->vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 3 * 4, obj->vertex_buffer_data, GL_STATIC_DRAW);
    handle_gl_error2("GLGlyph", "glGenBuffers() glBindBuffer() glBufferData() for vertex_buffer_data");
    
    glBindBuffer(GL_ARRAY_BUFFER, obj->texcoordbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 2 * 4, obj->texCoord_buffer_data, GL_STATIC_DRAW);
    handle_gl_error2("GLGlyph", "glGenBuffers() glBindBuffer() glBufferData() for VertexTexCoord");
    
    handle_gl_error("PRE gl_glyph tex");
    glBindTexture(GL_TEXTURE_2D, obj->tex);
    handle_gl_error("POST gl_glyph tex");
    
    t_vec3 worldPos = { 0.0f, 0.0f, 0.0f };
    gl_proctex_draw((GLProcTexObject*) obj, worldPos);
}
