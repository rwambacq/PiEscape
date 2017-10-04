#ifndef PIESCAPE2_GAMEUICREATOR_H
#define PIESCAPE2_GAMEUICREATOR_H

#include "Menu.h"
#include "Movie.h"

#include <string>
#include <utility>
#include <memory>

class GameUICreator {
protected:
public:
    GameUICreator();
    virtual ~GameUICreator();
    
    std::shared_ptr<MenuDefinition> createGameMenu();
    std::shared_ptr<MovieDefinition> createIntro();
    std::shared_ptr<MovieDefinition> createCredits();
    std::shared_ptr<MovieDefinition> createOutro();
};

#endif //PIESCAPE2_GAMEUICREATOR_H
