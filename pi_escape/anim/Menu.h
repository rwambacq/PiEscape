#ifndef PIESCAPE2_MENU_H
#define PIESCAPE2_MENU_H

#include "UI.h"
#include "Animation.h"

//TODO

typedef struct MenuItem {
	std::vector<GlyphDrawCommand> tekst;
	std::string action;
	bool selected;
} MenuItem;

class MenuDefinition {
private:
	std::vector<MenuItem> menuItems;
public:
	void addMenuItem(MenuItem* item);
	std::vector<MenuItem> getMenuItems();
};


class MenuModel : public UIModel {
	//TODO
};

class MenuGLView : public UIView {
	//TODO
};

class MenuController : public UIController {
	//TODO

};

#endif //PIESCAPE2_MENU_H
