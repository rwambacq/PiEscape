#ifndef PIESCAPE2_MENU_H
#define PIESCAPE2_MENU_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "../graphics/opengl_game_renderer.h"
#include "../graphics/gl_glyph.h"

#ifdef __cplusplus
}
#endif

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
	int selected = 0;
	bool isGedaan = false;
public:
	void menuUp();
	void menuDown();
	void setDone(bool done);

	std::vector<MenuItem> getMenu();
	int getSelected();

	MenuModel(std::vector<MenuItem>* baseMenu);
	virtual ~MenuModel();
	void setTime(uint64_t time) override;
	uint64_t getTime() const;
	int isDone() const override;
};

class MenuGLView : public UIView {
private:
	FontManager* manager;
	MenuModel* model;
public:
	void setFontManager(FontManager* mgr);
	void setModel(MenuModel*);

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
	void menuLoop(std::vector<MenuItem>*, FontManager*);
	virtual void onKey(SDLKey key);
	virtual void onExitKey();

	MenuController();
	virtual ~MenuController();
};

#endif //PIESCAPE2_MENU_H
