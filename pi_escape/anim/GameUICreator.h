#ifndef PIESCAPE2_GAMEUICREATOR_H
#define PIESCAPE2_GAMEUICREATOR_H

#include "Menu.h"
#include "Movie.h"

#include <string>
#include <utility>
#include <memory>

enum State {ACTIVATE, OTHER_ACTIVATED, DEFAULT, HOVER};
enum MenuState {MENUSTATECHANGE_START, MENUSTATECHANGE_END};

class EntryBuilder {
private:
	std::vector<Animation*> animations;
	bool enabledOnPc;
	bool enabledOnPi;
	std::string longText;
	std::string shortText;
	char mnemonic;
	std::string font;
	std::string action;
public:
	EntryBuilder();
	EntryBuilder& addAnimation(Animation& anim, State state, MenuState menuState, bool selected, long duration);
	EntryBuilder& setEnabledOnPc(bool e);
	EntryBuilder& setEnabledOnPi(bool e);
	EntryBuilder& setLongText(std::string text);
	EntryBuilder& setShortText(std::string text);
	EntryBuilder& setMnemonic(char mnem);
	EntryBuilder& setFontName(std::string font);
	EntryBuilder& setAction(std::string action);

	std::string getAction();
	std::string getLongText();
};

class GameUICreator {
protected:
	FontManager manager;
public:
    GameUICreator(FontManager);
    virtual ~GameUICreator();
    
    std::shared_ptr<MenuDefinition> createGameMenu();
    std::shared_ptr<MovieDefinition> createIntro();
    std::shared_ptr<MovieDefinition> createCredits();
    std::shared_ptr<MovieDefinition> createOutro();
};

EntryBuilder& addMainMenuAnimation(EntryBuilder& entryBuilder);
#endif //PIESCAPE2_GAMEUICREATOR_H
