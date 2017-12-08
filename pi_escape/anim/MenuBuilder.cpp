#include "MenuBuilder.h"

using namespace std;

shared_ptr<MenuDefinition> MenuBuilder::build() {
	MenuDefinition toReturn;

	int menuSize = this->entries.size();

	for (int i = 0; i < entries.size(); i++) {
		EntryBuilder* cur = entries.at(i);
		MenuItem toAdd;

		toAdd.action = cur->getAction();
		toAdd.tekst = mgr->makeGlyphDrawCommands(cur->getLongText(), 0, 234 * i);
		toAdd.selected = (i == 0);

		toReturn.addMenuItem(&toAdd);
	}
	return std::make_shared<MenuDefinition>(toReturn);
}

MenuBuilder::MenuBuilder(FontManager *mgr) {
	this->mgr = mgr;
}