#define __STDC_FORMAT_MACROS

#ifdef __cplusplus
extern "C"
{
#endif

#include "pi_escape/graphics/opengl_game_renderer.h"
#include "pi_escape/es/game.h"

#ifdef __cplusplus
}
#endif


#include <SDL.h>
#undef main //Weird bug on windows where SDL overwrite main definition
#include <SDL_timer.h>

#include "pi_escape/anim/FontManager.h"

#include <string>
#include <cassert>

using namespace std;

int main() {
	vector<GlyphDrawCommand> glifjes;
	t_vec4 col = { 1.0f, 0.0f, 0.0f, 1.0f };
	GlyphDrawCommand curr_cmd(0,0,0,0,0,0, col);
	int i=0;
	string punts = ".";
	int leadbounce=0;
    int imgFlags = IMG_INIT_PNG;
    if(!(IMG_Init(imgFlags) & imgFlags)) {
        fatal("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
    }

    Graphics* graphics = graphics_alloc(0, 0);

    GLGlyph glGlyph;

   	FontManager lettertypeToezichthouder(graphics, &glGlyph);
	lettertypeToezichthouder.loadFont("zorque72", "pi_escape/graphics/zorque72.png", "pi_escape/graphics/zorque72.fnt");

	gl_glyph_init(&glGlyph, graphics, (char*)lettertypeToezichthouder.getFontImageFilename().c_str());

	glifjes = lettertypeToezichthouder.makeGlyphDrawCommands("YE_BOI_SUPERHAX0R", 500, 700);

    

    //this is a demo of gl_glyph_draw

    Uint32 start_time_ms = SDL_GetTicks();
    Uint32 diff_time_ms = 0;
    while (diff_time_ms < 5000) {
        graphics_begin_draw(graphics);

        glmc_vec4_set(col, 0.0f, diff_time_ms / 5000.0f, 0.0f, 1.0f);
		for (i = 0; i < glifjes.size(); i++) {
			cout << "Spijslijst vergladden " + punts << endl; // sehr wichtig!
			if (punts == "...") { punts = "."; } else { punts += "."; }
			if (i <= leadbounce) {
				glifjes[i].bounce();
				curr_cmd = glifjes[i];
			}
			else { // this case is only for when the elements further than leadbounce haven't started bouncing yet
				curr_cmd = glifjes[i];
			}
			
			gl_glyph_draw(&glGlyph, curr_cmd.getLTopX(), curr_cmd.getLTopY() + curr_cmd.getBounceDiff() * 6,
				curr_cmd.getGlyphX(), curr_cmd.getGlyphY(),
				curr_cmd.getGlyphWidth(), curr_cmd.getGlyphHeight(),
				col);
		}

        graphics_end_draw(graphics);
		leadbounce++; // cause a further letter to be the start of bounce wave

        Uint32 cur_time_ms = SDL_GetTicks();
        diff_time_ms = cur_time_ms - start_time_ms;
    }

    gl_glyph_free(&glGlyph);
    graphics_free(graphics);
    free(graphics);

    return 0;
}
