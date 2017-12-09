#define __STDC_FORMAT_MACROS

#ifdef __cplusplus
extern "C"
{
#endif

#include "pi_escape/graphics/opengl_game_renderer.h"
#include "pi_escape/es/game.h"


#define __STDC_FORMAT_MACROS
#include <inttypes.h>

#ifndef STDIO_INCLUDED
#include <stdio.h>
#define STDIO_INCLUDED
#endif


#include "util/sleep.h"
#include "pi_escape/graphics/opengl_game_renderer.h"
#include "pi_escape/level/levelloader.h"
#include "pi_escape/es/game.h"

#include <assert.h>

#include "pi_escape/es/es_memory_manager.h"
#define BENCHLOG_FILE_PATH "benchmarks/benchlog.txt"

#include <SDL.h>
#undef main //Weird bug on windows where SDL overwrite main definition
#include <SDL_timer.h>

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

void startGameFromLevel(int level);
void fill_level_loader(LevelLoader* level_loader);

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
	
	bool done = false;
	while (!done) {

	// HERE IS WHERE THE INTRO SHOULD BE PLAYED

	// START MENU
		lettertypeToezichthouder.setColor(col);
		lettertypeToezichthouder.loadFont("arcade72", "pi_escape/graphics/arcade72.png", "pi_escape/graphics/arcade72.fnt");

		gl_glyph_init(&glGlyph, graphics, (char*)lettertypeToezichthouder.getFontImageFilename().c_str());

		shared_ptr<MenuDefinition> menudef = GameUICreator(lettertypeToezichthouder).createGameMenu();
		vector<MenuItem> items = (*menudef).getMenuItems();

		MenuController controller;
		controller.menuLoop(&items, &lettertypeToezichthouder);

	// CHECK SELECTION AND EXECUTE ACTION ACCORDINGLY
		
		switch (controller.getMenuSelection()) {
		case 0:
			startGameFromLevel(0);
			//PLAY OUTRO AFTER THIS
			break;
		case 1:
			startGameFromLevel(7);
			//PLAY OUTRO AFTER THIS
			break;
		case 2:
			done = true;
			break;
		}

	}
    gl_glyph_free(&glGlyph);
    graphics_free(graphics);
    free(graphics);

    return 0;
}

void startGameFromLevel(int level) {
	
}

void fill_level_loader(LevelLoader* level_loader) {
	strcpy(level_loader->level_paths[0], "pi_escape/level/level_files/tutorial1.lvl");
	strcpy(level_loader->level_paths[1], "pi_escape/level/level_files/tutorial2.lvl");
	strcpy(level_loader->level_paths[2], "pi_escape/level/level_files/tutorial3.lvl");
	strcpy(level_loader->level_paths[3], "pi_escape/level/level_files/tutorial4.lvl");
	strcpy(level_loader->level_paths[4], "pi_escape/level/level_files/tutorial5.lvl");
	strcpy(level_loader->level_paths[5], "pi_escape/level/level_files/tutorial6.lvl");
	strcpy(level_loader->level_paths[6], "pi_escape/level/level_files/tutorial7.lvl");
	strcpy(level_loader->level_paths[7], "pi_escape/level/level_files/game1.lvl");
	strcpy(level_loader->level_paths[8], "pi_escape/level/level_files/game2.lvl");
	strcpy(level_loader->level_paths[9], "pi_escape/level/level_files/game3.lvl");
}
