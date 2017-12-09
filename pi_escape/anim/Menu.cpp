#include "Menu.h"

using namespace std;

MenuModel::MenuModel(std::vector<MenuItem> *m) : UIModel(){
	this->baseMenu = *m;
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
MenuModel* MenuController::getModel() { 
	return this->model; 
}

void MenuController::setModel(MenuModel mod) {
	this->model = &mod;
}

std::string MenuItem::getFont() {
	return this->font;
}

void MenuController::menuLoop(std::vector<MenuItem>* menuItems, FontManager* manager) {
	MenuModel model(menuItems);
	MenuGLView view;

	this->model = &model;
	this->view = &view;

	view.setFontManager(manager);
	view.setModel(&model); // hier zit de fout: bij model heeft de juist lijst met MenuItems
	// maar na deze call verwijst de MenuModel* in view naar een model met een baseMenu met size ???
	// gezien while debugging...
	
	SDL_Event event;
	memset(&event, 0, sizeof(SDL_Event));

	while (!this->model->isDone()) {
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
		this->model->menuUp();
		break;
	}
	case SDLK_DOWN: {
		this->model->menuDown();
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


 //model
void MenuModel::menuUp() {
	this->selected = (this->selected - 1) % 3;
	if (this->selected < 0) {
		this->selected += 3;
	}

	cout << this->selected << endl;
}

void MenuModel::menuDown() {
	this->selected = (this->selected + 1) % 3;

	cout << this->selected << endl;
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

void MenuGLView::setModel(MenuModel* mod) {
	this->model = model;
}

void MenuGLView::draw() {
	vector<MenuItem> items;
	int i = 0;
	int j = 0;
	vector<GlyphDrawCommand> GDCs;
	GlyphDrawCommand curr_cmd;


	graphics_begin_draw(this->manager->getGraphics());
	items = this->model->getMenu();
	//for (i = 0; i < items.size(); i++) {
	//	MenuItem curr_item = items.at(i);
	//	GDCs = curr_item.getTekst();
	//	/*for (j = 0; j < GDCs.size(); j++) {
	//		curr_cmd = GDCs.at(j);
	//		if (i == this->model->getSelected()) {
	//			curr_cmd = curr_cmd.changeColor(1,1,1);
	//		}
	//		cout << "in ons model bevindt het glyphje zich op: " << this->manager->getGlyphPtr() << endl;
	//		gl_glyph_draw(this->manager->getGlyphPtr(), curr_cmd.getLTopX(), curr_cmd.getLTopY(),
	//			curr_cmd.getGlyphX(), curr_cmd.getGlyphY(),
	//			curr_cmd.getGlyphWidth(), curr_cmd.getGlyphHeight(),
	//			curr_cmd.getColor());
	//	}*/
	//}
	graphics_end_draw(this->manager->getGraphics());
}

MenuController::MenuController() {
	
}
MenuController::~MenuController() {}