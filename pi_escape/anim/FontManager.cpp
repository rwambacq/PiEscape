#include "FontManager.h"

#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

FontManager::FontManager(Graphics* graphics, GLGlyph* glyph) {
	this->graphics = graphics;
	this->glyph = glyph;
	hpos = TEXT_LEFT;
	vpos = TEXT_MIDDLE;
	glmc_vec4_set(color, 0.0f, 0.0f, 0.0f, 0.0f);
	glmc_vec2_set(scale, 0.0f, 0.0f);
	this->fontName = "empty";
	this->fontMetaFilename = "empty";
	this->fontImageFilename = "empty";
}

void FontManager::loadFont(const std::string& fontName, const std::string& fontImageFilename, const std::string& fontMetaFilename) {
	this->fontName = fontName;
	this->fontMetaFilename = fontMetaFilename;
	this->fontImageFilename = fontImageFilename;
}

FontManager::~FontManager() {

}

void FontManager::setColor(const t_vec4& color) {
	glmc_vec4_set(this->color, color[0], color[1], color[2], color[3]);
}

void FontManager::setScale(const t_vec2& scale) {
	glmc_vec2_set(this->scale, scale[0], scale[1]);
}

string FontManager::getFontImageFilename() {
	return fontImageFilename;
}

void FontManager::setHpos(TextJustification h) {
	this->hpos = h;
}

void FontManager::setVpos(TextVerticalPosition v) {
	this->vpos = v;
}

void FontManager::setColor(float r, float g, float b, float a) {
	glmc_vec4_set(color, r, g, b, a);
}

void FontManager::setScale(float x, float y) {
	glmc_vec2_set(scale, x, y);
}

void FontManager::setFont(const std::string& fontName) {
	this->fontName = fontName;
	this->fontImageFilename = "pi_escape/graphics/" + fontName + ".png";
	this->fontMetaFilename = "pi_escape/graphics/" + fontName + ".fnt";
}

//TODO: implement draw so it isn't shit

//void FontManager::draw(const GlyphDrawCommand& glyphDraw) const {
//	gl_glyph_draw(this->glyph, glyphDraw.getLTopX(), glyphDraw.getLTopY(), glyphDraw.getGlyphX(), glyphDraw.getGlyphY(), glyphDraw.getGlyphWidth(), glyphDraw.getGlyphHeight(), this->color);
//}

std::vector<GlyphDrawCommand> FontManager::makeGlyphDrawCommands(std::string text, int x, int y) const {
	int leftTopX = x;
	int leftTopY = y;

	std::vector<GlyphDrawCommand> toReturn(10);
	for (char& c : text) {
		int charId = (int)c;

		ifstream bestand;

		string line;

		int currentLine = 0;

		string search = "char id=" + charId;

		bestand.open(this->fontMetaFilename.c_str());
		if (bestand.is_open()) {
			while (getline(bestand, line)) {
				currentLine++;
				if (line.find(search, 0) != string::npos) {
					istringstream iss(line);
					string a, b, c, d, e, f;
					int id, x, y, width, height ;

					iss >> a >> id >> b >> x >> c >> y >> d >> width >> e >> height >> f;
					const int cTopX = leftTopX;
					const int cTopY = leftTopY;
					const int cid = id;
					const int cx = x;
					const int cy = y;
					const int cwidth = width;
					const int cheight = height;
					GlyphDrawCommand toInsert(leftTopX, leftTopY, x, y, width, height, this->color);

					leftTopX += (width + 5);
				}
			}
			bestand.close();
		}
	}
}


GlyphDrawCommand GlyphDrawCommand::changeColor(float r, float g, float b) const {
	GlyphDrawCommand toReturn = GlyphDrawCommand(*this);
	glmc_vec4_set(toReturn.color, r, g, b, color[3]);
	return toReturn;
}

GlyphDrawCommand GlyphDrawCommand::changeAlpha(float a) const {
	GlyphDrawCommand toReturn = GlyphDrawCommand(*this);
	glmc_vec4_set(toReturn.color, color[0], color[1], color[2], a);
	return toReturn;
}

GlyphDrawCommand::GlyphDrawCommand(const int pos_ltop_x, const int pos_ltop_y, const int glyph_x, const int glyph_y,
                                   const int glyph_w, const int glyph_h, const t_vec4 &color){
	glmc_vec4_set(this->color, color[0], color[1], color[2], color[3]);
	this->pos_ltop_x = pos_ltop_x;
	this->pos_ltop_y = pos_ltop_y;
	this->glyph_x = glyph_x;
	this->glyph_y = glyph_y;
	this->glyph_w  = glyph_w;
	this->glyph_h = glyph_h;
}

GlyphDrawCommand::GlyphDrawCommand(const GlyphDrawCommand &orig) { // copy constructor
	glmc_vec4_set(this->color, orig.color[0], orig.color[1], orig.color[2], orig.color[3]);
	this->pos_ltop_x = orig.pos_ltop_x;
	this->pos_ltop_y = orig.pos_ltop_y;
	this->glyph_x = orig.glyph_x;
	this->glyph_y = orig.glyph_y;
	this->glyph_w = orig.glyph_w;
	this->glyph_h = orig.glyph_h;
}

const t_vec4 &GlyphDrawCommand::getColor() const {
    return color;
}

int GlyphDrawCommand::getLTopY() {
	return this->pos_ltop_y;
}

int GlyphDrawCommand::getLTopX() {
	return this->pos_ltop_x;
}

int GlyphDrawCommand::getGlyphY() {
	return this->glyph_y;
}

int GlyphDrawCommand::getGlyphX() {
	return this->glyph_x;
}

int GlyphDrawCommand::getGlyphWidth() {
	return this->glyph_w;
}

int GlyphDrawCommand::getGlyphHeight() {
	return this->glyph_h;
}

