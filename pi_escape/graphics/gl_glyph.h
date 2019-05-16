#ifndef PIESCAPE2_GL_GLYPH_H
#define PIESCAPE2_GL_GLYPH_H

#include "gl_proctex_object.h"

#include "../es/game_util.h"

#include <SDL_image.h>

typedef struct GLGlyph {
#ifdef __cplusplus
    struct GLProcTexObject parent;
#else
    union {
        struct GLProcTexObject;
        GLProcTexObject gl_pt_object;
    };
#endif
    
    int w,h;
    SDL_Surface* font_image;
    GLuint tex;
} GLGlyph;

void gl_glyph_init(GLGlyph *, Graphics *, char* font_image_filename);
void gl_glyph_free(GLGlyph *);
void  gl_glyph_draw(GLGlyph* obj,
                    int pos_ltop_x, int pos_ltop_y,
                    int glyph_x, int glyph_y,
                    int glyph_w, int glyph_h,
                    const t_vec4 color);

#endif //PIESCAPE2_GL_GLYPH_H
