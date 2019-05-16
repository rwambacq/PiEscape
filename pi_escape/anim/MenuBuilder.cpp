#include "MenuBuilder.h"

using namespace std;

shared_ptr<MenuDefinition> MenuBuilder::build() {
	MenuDefinition toReturn;

	int menuSize = this->entries.size();
	int i=0;

	//middle of the screen + amount_of_entries/2*MENU_ITEM_Y_SPACING above 
	int topY = 1080/2 + menuSize/2*MENU_ITEM_Y_SPACING; 

	int maxTextLength = 0;
	for (i = 0; i < entries.size(); i++) {
		if (entries.at(i).getAction().size() > maxTextLength) {
			maxTextLength = entries.at(i).getAction().size();
		}
	}
	int lTopX = 1920 / 2 - maxTextLength / 2 * MEAN_LETTER_WIDTH;

	for (i = 0; i < entries.size(); i++) {
		EntryBuilder cur = entries.at(i);
		cout << "i: " << i << ", action: " << cur.getAction() << ". Now making GDCs..." << endl;
		MenuItem toAdd = MenuItem(this->manager.makeGlyphDrawCommands(cur.getLongText(), lTopX, topY - MENU_ITEM_Y_SPACING * i), cur.getAction(), cur.getAnimations(), cur.getFont());

		toReturn.addMenuItem(&toAdd);
	}
	return std::make_shared<MenuDefinition>(toReturn);
}

MenuBuilder::MenuBuilder(FontManager mgr) {
	this->manager = mgr;
}