#ifndef PIESCAPE2_MENUBUILDER_H
#define PIESCAPE2_MENUBUILDER_H

#include "Animation.h"
#include "Menu.h"
#include "GameUICreator.h"

class MenuBuilder {
private:
	FontManager manager;
	std::vector<EntryBuilder> entries;
public:
	MenuBuilder(FontManager);
	EntryBuilder& addEntry(std::string, std::string, std::string, std::string);
	std::shared_ptr<MenuDefinition> build();
};


#endif //PIESCAPE2_MENUBUILDER_H
