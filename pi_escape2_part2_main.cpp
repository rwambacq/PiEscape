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

#include "util/sleep.h"

#include "pi_escape/anim/FontManager.h"
#include "pi_escape/anim/GameUICreator.h"

#include <string>
#include <cassert>

using namespace std;

int main() {
	vector<GlyphDrawCommand> glifjes;
	t_vec4 col = { 1.0f, 0.0f, 0.0f, 1.0f };
	GlyphDrawCommand curr_cmd(0,0,0,0,0,0, col);
	int i=0;
	//string punts = ".";
	int bounceInt=0;
    int imgFlags = IMG_INIT_PNG;
    if(!(IMG_Init(imgFlags) & imgFlags)) {
        fatal("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
    }


    Graphics* graphics = graphics_alloc(0, 0);
    GLGlyph glGlyph;

   	FontManager lettertypeToezichthouder(graphics, &glGlyph);
	lettertypeToezichthouder.setColor(col);
	lettertypeToezichthouder.loadFont("zorque72", "pi_escape/graphics/zorque72.png", "pi_escape/graphics/zorque72.fnt");

	gl_glyph_init(&glGlyph, graphics, (char*)lettertypeToezichthouder.getFontImageFilename().c_str());

	shared_ptr<MenuDefinition> menudef = GameUICreator(lettertypeToezichthouder).createGameMenu();
	vector<MenuItem> items = (*menudef).getMenuItems();

	MenuController controller;
	controller.menuLoop(&items, &lettertypeToezichthouder);

	cout << "items size: " << items.size() << endl;
	Uint32 start_time_ms = SDL_GetTicks();
	Uint32 diff_time_ms = 0;
	while (diff_time_ms < 5000) {
		graphics_begin_draw(graphics);
		for (i = 0; i < items.size(); i++) {
			MenuItem curr_item = items.at(i);
			for (int j = 0; j < curr_item.getTekst().size(); j++) {
				curr_cmd = curr_item.getTekst().at(j);
				/*cout << "##############################" << endl;
				cout << "letter: " << curr_item.getAction()[j] << endl;
				cout << "leftTopX, leftTopY of this letter: " << curr_cmd.getLTopX() << ", " << curr_cmd.getLTopY() << endl;
				cout << "x en y in png file: " << curr_cmd.getGlyphX() << " " << curr_cmd.getGlyphY() << endl;
				cout << "width en height: " << curr_cmd.getGlyphWidth() << " " << curr_cmd.getGlyphHeight() << endl;
				cout << "##############################" << endl;*/
				gl_glyph_draw(&glGlyph, curr_cmd.getLTopX(), curr_cmd.getLTopY(),
					curr_cmd.getGlyphX(), curr_cmd.getGlyphY(),
					curr_cmd.getGlyphWidth(), curr_cmd.getGlyphHeight(),
					curr_cmd.getColor());
			}
		}
		graphics_end_draw(graphics);
		diff_time_ms = SDL_GetTicks() - start_time_ms;
	}

	//glifjes = lettertypeToezichthouder.makeGlyphDrawCommands("TAART_ONTSNAP 2", 900, 500);
	//for (i = 0; i < glifjes.size(); i++) {
	//	glifjes[i] = glifjes[i].changeColor(col);
	//}


 //   Uint32 start_time_ms = SDL_GetTicks();
 //   Uint32 diff_time_ms = 0;
 //   while (diff_time_ms < 15000) {
 //       graphics_begin_draw(graphics);

	//	for (i = 0; i < glifjes.size(); i++) {
	//		cout << "you wish" + punts << endl; // Do not delete this print, without it, the animation goes way too fast
	//		if (punts == "...") { punts = "."; } else { punts += "."; }
	//		if (i <= bounceInt) {
	//			curr_cmd = glifjes[i];
	//		}
	//		else { // this case is only for when the elements further than bounceInt haven't started bouncing yet
	//			curr_cmd = glifjes[i];
	//		}

	//		gl_glyph_draw(&glGlyph, curr_cmd.getLTopX(), curr_cmd.getLTopY() + curr_cmd.getBounceDiff(),
	//			curr_cmd.getGlyphX(), curr_cmd.getGlyphY(),
	//			curr_cmd.getGlyphWidth(), curr_cmd.getGlyphHeight(),
	//			curr_cmd.getColor());
	//	}

 //       graphics_end_draw(graphics);
	//	if (bounceInt < (glifjes.size()-1)) {
	//		if (glifjes.at(bounceInt).getBounceDiff() >= 5) {
	//			bounceInt++; // cause a further letter to be the start of bounce wave
	//		}
	//	}

 //       Uint32 cur_time_ms = SDL_GetTicks();
 //       diff_time_ms = cur_time_ms - start_time_ms;
 //   }

    gl_glyph_free(&glGlyph);
    graphics_free(graphics);
    free(graphics);

    return 0;
}
