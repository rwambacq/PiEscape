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

	cout << "het glyphje bevindt zich op: " << &glGlyph << endl;

   	FontManager lettertypeToezichthouder(graphics, &glGlyph);

	lettertypeToezichthouder.setColor(col);
	lettertypeToezichthouder.loadFont("arcade72", "pi_escape/graphics/arcade72.png", "pi_escape/graphics/arcade72.fnt");

	gl_glyph_init(&glGlyph, graphics, (char*)lettertypeToezichthouder.getFontImageFilename().c_str());

	shared_ptr<MenuDefinition> menudef = GameUICreator(lettertypeToezichthouder).createGameMenu();
	vector<MenuItem> items = (*menudef).getMenuItems();

	MenuController controller;
	controller.menuLoop(&items, &lettertypeToezichthouder);

    gl_glyph_free(&glGlyph);
    graphics_free(graphics);
    free(graphics);

    return 0;
}
