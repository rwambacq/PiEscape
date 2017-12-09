#ifndef PIESCAPE2_MENU_H
#define PIESCAPE2_MENU_H

#include "UI.h"
#include "Animation.h"

class MenuItem {
private:
	std::vector<GlyphDrawCommand> tekst;
	std::vector<Animation*> selectedAnimations;
	std::string action;
	std::string font;
public:
	MenuItem(std::vector<GlyphDrawCommand>, std::string, std::vector<Animation*>, std::string font);
	std::vector<GlyphDrawCommand> getTekst();
	std::vector<Animation*> getSelectedAnimations();
	std::string getAction();
	std::string getFont();
};

typedef struct MenuSelection {
	MenuItem item;
	bool selected;
} MenuSelection;

class MenuDefinition {
private:
	std::vector<MenuItem> menuItems;
public:
	void addMenuItem(MenuItem* item);
	std::vector<MenuItem> getMenuItems();
};

class MenuModel : public UIModel {
protected:
	uint64_t time;
private:
	std::vector<MenuItem> baseMenu;
	std::vector<MenuSelection> selection;
	int selected = 0;
	bool isGedaan = false;
public:
	void menuUp();
	void menuDown();
	void updateSelection(std::vector<MenuSelection> selection, int selected);
	int getSelection();
	MenuItem getSelectedItem();
	void setDone(bool done);

	MenuModel(std::vector<MenuItem> *baseMenu);
	virtual ~MenuModel();
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

	virtual void draw();
};

class MenuController : public UIController {
private:
	MenuModel* model;
	MenuGLView* view;
public:
	void setModel(MenuModel);
	MenuModel* getModel();
	void menuLoop(std::vector<MenuItem>* menuItems, FontManager* manager);
	virtual void onKey(SDLKey key);
	virtual void onExitKey();

	MenuController();
	virtual ~MenuController();
};

#endif //PIESCAPE2_MENU_H
