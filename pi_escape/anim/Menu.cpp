#include "Menu.h"
#include "../led/fake_led.h"
#include "../../util/sleep.h"
#include <thread>

using namespace std;

int getal = 1;
int soort_animatie;



MenuModel::MenuModel(std::vector<MenuItem> *m) : UIModel() {
	this->baseMenu = *m;
}

MenuModel::MenuModel() {

}

void MenuDefinition::addMenuItem(MenuItem* item) {
	this->menuItems.push_back(*item);
}

MenuItem::MenuItem(std::vector<GlyphDrawCommand> tekst, std::string action, std::vector<Animation*> animations, std::string font) {
	this->tekst = tekst;
	this->action = action;
	this->selectedAnimations = animations;
	this->font = font;
}

vector<MenuItem> MenuDefinition::getMenuItems() {
	return this->menuItems;
}

vector<GlyphDrawCommand> MenuItem::getTekst() {
	return this->tekst;
}

string MenuItem::getAction() {
	return this->action;
}

std::vector<Animation*> MenuItem::getSelectedAnimations() {
	return this->selectedAnimations;
}


// controller

std::string MenuItem::getFont() {
	return this->font;
}

void MenuController::menuLoop(std::vector<MenuItem>* menuItems, FontManager* manager) {
	MenuModel model(menuItems);
	MenuGLView view;

	MRef m{ model };
	this->mRef = &m;

	view.setFontManager(manager);
	view.setModel(&m);

	int firstSelected = m.model.getSelected();


	// GEBRUIK HIER BOVENSTAANDE INT OM DE LED FUNCTIE AAN TE ROEPEN (2 ANDERE PRINTS ZITTEN IN MENUUP en MENUDOWN)
	std::thread first(aanroeper, firstSelected);
	first.detach();
	SDL_Event event;
	memset(&event, 0, sizeof(SDL_Event));

	while (!this->mRef->model.isDone()) {
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_KEYDOWN:
				this->onKey(event.key.keysym.sym);
				break;
			case SDL_QUIT:
				this->onExitKey();
				break;
			}
		}
		// draw menu (read: alert view that model has changed)
		view.draw();
	}
}

void MenuController::onKey(SDLKey key) {
	switch (key) {
	case SDLK_UP: {
		this->mRef->model.menuUp();
		break;
	}
	case SDLK_DOWN: {
		this->mRef->model.menuDown();
		break;
	}
	case SDLK_ESCAPE: {
		onExitKey();
		break;
	}
	case SDLK_RETURN: {
		onEnterKey();
		break;
	}
	case SDLK_KP_ENTER: {
		onEnterKey();
		break;
	}
	}
}

void MenuController::onEnterKey() {
	this->menuSelection = this->mRef->model.getSelected();
	this->mRef->model.setDone(true);
}

void MenuController::onExitKey() {
	this->menuSelection = 2;
	this->mRef->model.setDone(true);
}

int MenuController::getMenuSelection() {
	return this->menuSelection;
}


//model
void MenuModel::menuUp() {
	this->selected = (this->selected - 1) % 3;
	if (this->selected < 0) {
		this->selected += 3;
	}

	cout << this->selected << endl;
	changegetal(this->selected);
	sleep_ms(750);
}

void MenuModel::menuDown() {
	this->selected = (this->selected + 1) % 3;

	cout << this->selected << endl;
	//std::thread first(aanroeper, 0);
	//first.detach();
	changegetal(this->selected);
	sleep_ms(750);
}

void MenuModel::setDone(bool done) {
	this->isGedaan = done;
}

int MenuModel::isDone() const {
	return this->isGedaan;
}

MenuModel::~MenuModel() {

}

vector<MenuItem> MenuModel::getMenu() {
	return this->baseMenu;
}

int MenuModel::getSelected() {
	return this->selected;
}

void MenuModel::setTime(uint64_t time) {
	this->time = time;
}
uint64_t MenuModel::getTime() const {
	return this->time;
}

// view
MenuGLView::MenuGLView() {}
MenuGLView::~MenuGLView() {}

void MenuGLView::setFontManager(FontManager* mgr) {
	this->manager = mgr;
}

void MenuGLView::setModel(MRef* m) {
	this->model = m;
}

void MenuGLView::draw() {
	vector<MenuItem> items;
	int i = 0;
	int j = 0;
	vector<GlyphDrawCommand> GDCs;
	GlyphDrawCommand curr_cmd;

	graphics_begin_draw(this->manager->getGraphics());
	items = this->model->model.getMenu();
	for (i = 0; i < items.size(); i++) {
		MenuItem curr_item = items.at(i);
		GDCs = curr_item.getTekst();
		for (j = 0; j < GDCs.size(); j++) {
			curr_cmd = GDCs.at(j);
			if (i == this->model->model.getSelected()) {
				curr_cmd = curr_cmd.changeColor(1, 1, 1);
			}
			//cout << "in ons model bevindt het glyphje zich op: " << this->manager->getGlyphPtr() << endl;
			gl_glyph_draw(this->manager->getGlyphPtr(), curr_cmd.getLTopX(), curr_cmd.getLTopY(),
				curr_cmd.getGlyphX(), curr_cmd.getGlyphY(),
				curr_cmd.getGlyphWidth(), curr_cmd.getGlyphHeight(),
				curr_cmd.getColor());
		}
	}
	graphics_end_draw(this->manager->getGraphics());
}







MenuController::MenuController() {

}
MenuController::~MenuController() {}