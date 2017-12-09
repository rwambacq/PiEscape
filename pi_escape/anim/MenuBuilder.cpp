#include "MenuBuilder.h"

using namespace std;

shared_ptr<MenuDefinition> MenuBuilder::build() {
	MenuDefinition toReturn;

	int menuSize = this->entries.size();

	for (int i = 0; i < entries.size(); i++) {
		EntryBuilder cur = entries.at(i);
		MenuItem toAdd = MenuItem(this->manager.makeGlyphDrawCommands(cur.getLongText(), 300, 234 * i), cur.getAction());

		toReturn.addMenuItem(&toAdd);
	}
	return std::make_shared<MenuDefinition>(toReturn);
}

MenuBuilder::MenuBuilder(FontManager mgr) {
	this->manager = mgr;
}