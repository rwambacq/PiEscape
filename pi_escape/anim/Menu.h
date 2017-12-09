#ifndef PIESCAPE2_MENU_H
#define PIESCAPE2_MENU_H

#include "UI.h"
#include "Animation.h"

typedef struct MenuSelection {
	MenuItem item;
	bool selected;
} MenuSelection;

class MenuItem {
private:
	std::vector<GlyphDrawCommand> tekst;
	std::vector<Animation> selectedAnimations;
	std::string action;
public:
	MenuItem(std::vector<GlyphDrawCommand>, std::string, std::vector<Animation>);
	std::vector<GlyphDrawCommand> getTekst();
	std::vector<Animation> getSelectedAnimations();
	std::string getAction();

};

class MenuDefinition {
private:
	std::vector<MenuItem> menuItems;
public:
	void addMenuItem(MenuItem* item);
	std::vector<MenuItem> getMenuItems();
};

class MenuController : public UIController {
private:
	class::MenuModel* model;
	class::MenuGLView* view;
public:
	void menuLoop(std::vector<MenuItem> menuItems, FontManager manager);
	void onKey(SDLKey key) override;
	void onExitKey() override;

	MenuController();
	virtual ~MenuController();
};

class MenuModel : public UIModel {
protected:
	uint64_t time;
private:
	std::vector<MenuItem> baseMenu;
	std::vector<MenuSelection> selection;
	int selected;
	bool isDone;
public:
	void menuUp();
	void menuDown();
	void updateSelection(std::vector<MenuSelection> selection, int selected);
	int getSelection();
	MenuItem getSelectedItem();
	void setDone(bool done);

	MenuModel();
	~MenuModel();
	void setTime(uint64_t time) override;
	uint64_t getTime() const;
	int isDone() const override;
};

class MenuGLView : public UIView {
private:
	FontManager* manager;
public:
	void setFontManager(FontManager* mgr);
	void drawMenu(std::vector<MenuSelection> menu);

	MenuGLView();
	virtual ~MenuGLView();

	void draw() override;
};

#endif //PIESCAPE2_MENU_H
