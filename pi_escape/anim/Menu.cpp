#include "Menu.h"

using namespace std;

MenuModel::MenuModel() : UIModel(){}

void MenuDefinition::addMenuItem(MenuItem* item) {
	this->menuItems.push_back(*item);
}

MenuItem::MenuItem(std::vector<GlyphDrawCommand> tekst, std::string action, std::vector<Animation> animations) {
	this->tekst = tekst;
	this->action = action;
	this->selectedAnimations = animations;
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

std::vector<Animation> MenuItem::getSelectedAnimations() {
	return this->selectedAnimations;
}

void MenuController::menuLoop(std::vector<MenuItem> menuItems, FontManager manager) {
	MenuModel model;
	MenuGLView view;

	view.setFontManager(&manager);

	this->model = &model;
	this->view = &view;
	
	SDL_Event event;
	memset(&event, 0, sizeof(SDL_Event));

	while (!(*this->model).isDone()) {
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
	}
}

void MenuController::onKey(SDLKey key) {
	switch (key) {
	case SDLK_UP: {
		(*this->model).menuUp();
		break;
	}
	case SDLK_DOWN: {
		(*this->model).menuDown();
		break;
	}
	case SDLK_ESCAPE: {
		onExitKey();
		break;
	}
	}
}


void MenuController::onExitKey() {
	this->model->setDone(true);
}

void MenuModel::menuUp() {
	this->selection.at(this->selected).selected = false;
	this->selected = (this->selected - 1) % 3;
	if (this->selected < 0) {
		this->selected += 3;
	}
	this->selection.at(this->selected).selected = true;

	cout << this->selected << endl;
}

void MenuModel::menuDown() {
	this->selection.at(this->selected).selected = false;
	this->selected = (this->selected + 1) % 3;
	this->selection.at(this->selected).selected = true;

	cout << this->selected << endl;
}

int MenuModel::isDone() const {
	return this->isGedaan;
}

void MenuGLView::setFontManager(FontManager* mgr) {
	this->manager = mgr;
}

MenuModel::~MenuModel() {}


MenuGLView::MenuGLView() {}
MenuGLView::~MenuGLView() {}
void MenuGLView::draw() {}

MenuController::MenuController() {
	this->model = NULL;
	this->view = NULL;
}
MenuController::~MenuController() {}