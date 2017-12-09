#ifndef PIESCAPE2_MENUBUILDER_H
#define PIESCAPE2_MENUBUILDER_H

#define MENU_ITEM_Y_SPACING 95
#define MEAN_LETTER_WIDTH 42

#include "Animation.h"
#include "Menu.h"
#include "GameUICreator.h"

class MenuBuilder {
private:
	FontManager manager;
	std::vector<EntryBuilder> entries;
public:
	MenuBuilder(FontManager);
	EntryBuilder& addEntry(std::string, std::string, std::string, std::string, char);
	std::shared_ptr<MenuDefinition> build();
};


#endif //PIESCAPE2_MENUBUILDER_H
