#include "Menu.h"

using namespace std;

void MenuDefinition::addMenuItem(MenuItem* item) {
	this->menuItems.push_back(*item);
}

MenuItem::MenuItem(std::vector<GlyphDrawCommand> tekst, std::string action) {
	this->tekst = tekst;
	this->action = action;
}

vector<MenuItem> MenuDefinition::getMenuItems() {
	return this->menuItems;
}