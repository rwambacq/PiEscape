#include "Menu.h"

using namespace std;

void MenuDefinition::addMenuItem(MenuItem* item) {
	this->menuItems.push_back(*item);
}

vector<MenuItem> MenuDefinition::getMenuItems() {
	return this->menuItems;
}