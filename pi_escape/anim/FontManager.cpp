#include "FontManager.h"

using namespace std;

FontManager::FontManager(){}

FontManager::FontManager(Graphics* graphics, GLGlyph* glyph) {
	this->graphics = graphics;
	this->glyph = glyph;
	hpos = TEXT_LEFT;
	vpos = TEXT_MIDDLE;
	glmc_vec4_set(color, 0.0f, 0.0f, 0.0f, 1.0f);
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
	this->fontName = fontName + "72";
	this->fontImageFilename = "pi_escape/graphics/" + fontName + "72.png";
	this->fontMetaFilename = "pi_escape/graphics/" + fontName + "72.fnt";
}

std::vector<GlyphDrawCommand> FontManager::makeGlyphDrawCommands(std::string text, int x, int y) const {
	int leftTopX = x;
	int leftTopY = y;
	int i = 0;

	int maxHeight = 0;

	std::vector<GlyphDrawCommand> toReturn(text.size());
	//cout << "text: " << text << endl;
	for (char& c : text) {
		/*cout << "################################" << endl;
		cout << "letter " << i << ": " << c << "(id should be " << (int) c << ")" << endl;*/
		int charId = (int)c;

		ifstream bestand;

		string line;

		ostringstream oss;
		oss << "char id=" << charId;

		string search = oss.str();

		bestand.open(this->fontMetaFilename.c_str());
		if (bestand.is_open()) {
			while (getline(bestand, line)) {
				if (line.find(search, 0) != string::npos) {
					//cout << "current line: " << line << endl;
					istringstream iss(line);
					int id, xx, yy, width, height, xadvance ;

					iss.ignore(INT_MAX, ' ');
					iss.ignore(INT_MAX, '=');
					iss >> id;
					iss.ignore(INT_MAX, '=');
					iss >> xx;
					iss.ignore(INT_MAX, '=');
					iss >> yy;
					iss.ignore(INT_MAX, '=');
					iss >> width;
					iss.ignore(INT_MAX, '=');
					iss >> height;
					iss.ignore(INT_MAX, '=');
					iss.ignore(INT_MAX, '=');
					iss.ignore(INT_MAX, '=');
					iss >> xadvance;
					/*cout << "leftTopX, leftTopY of this letter: " << leftTopX << ", " << leftTopY << endl;
					cout << "gevonden letter id: " << id << endl;
					cout << "x en y in png file: " << xx << " " << yy << endl;
					cout << "width en height: " << width << " " << height << endl;
					cout << "xadvance (afstand tot volgende letter): " << xadvance << endl;
					cout << "##############################" << endl;*/

					if (height > maxHeight){
						maxHeight = height;
					}
					
					GlyphDrawCommand toInsert(leftTopX, leftTopY, xx, yy, width, height, this->color);

					leftTopX += xadvance;
					toReturn[i] = toInsert;


				}
			}
			bestand.close();
		}
		i++;
	}

	for (GlyphDrawCommand& g : toReturn) {
		int heightDiff = maxHeight - g.getGlyphHeight();
		g.setLtopY(g.getLTopY() - heightDiff);
	}

	return toReturn;
}

void GlyphDrawCommand::setLtopY(float y) {
	this->pos_ltop_y = y;
}

Graphics* FontManager::getGraphics() {
	return this->graphics;
}

GLGlyph* FontManager::getGlyphPtr() {
	return this->glyph;
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

GlyphDrawCommand GlyphDrawCommand::move(int x_offset, int y_offset) const {
	GlyphDrawCommand toReturn = GlyphDrawCommand(*this);
	toReturn.pos_ltop_x += x_offset;
	toReturn.pos_ltop_y += y_offset;
	return toReturn;
}

GlyphDrawCommand::GlyphDrawCommand() {
	glmc_vec4_set(this->color, 1, 1 , 1, 1);
	this->up = true;
	this->bounceDiff = 0;
	this->pos_ltop_x = 0;
	this->pos_ltop_y = 0;
	this->glyph_x = 0;
	this->glyph_y = 0;
	this->glyph_w = 0;
	this->glyph_h = 0;
}

GlyphDrawCommand::GlyphDrawCommand(const int pos_ltop_x, const int pos_ltop_y, const int glyph_x, const int glyph_y,
                                   const int glyph_w, const int glyph_h, const t_vec4 &color){
	glmc_vec4_set(this->color, color[0], color[1], color[2], color[3]);
	this->up = true;
	this->bounceDiff = 0;
	this->pos_ltop_x = pos_ltop_x;
	this->pos_ltop_y = pos_ltop_y;
	this->glyph_x = glyph_x;
	this->glyph_y = glyph_y;
	this->glyph_w  = glyph_w;
	this->glyph_h = glyph_h;
}

GlyphDrawCommand::GlyphDrawCommand(const GlyphDrawCommand &orig) { // copy constructor
	glmc_vec4_set(this->color, orig.color[0], orig.color[1], orig.color[2], orig.color[3]);
	this->up = orig.up;
	this->bounceDiff = orig.bounceDiff;
	this->pos_ltop_x = orig.pos_ltop_x;
	this->pos_ltop_y = orig.pos_ltop_y;
	this->glyph_x = orig.glyph_x;
	this->glyph_y = orig.glyph_y;
	this->glyph_w = orig.glyph_w;
	this->glyph_h = orig.glyph_h;
}

bool GlyphDrawCommand::operator==(const GlyphDrawCommand& a) const {
	return pos_ltop_x == a.pos_ltop_x
		&& pos_ltop_y == a.pos_ltop_y
		&& glyph_x == a.glyph_x
		&& glyph_y == a.glyph_y
		&& glyph_w == a.glyph_w
		&& glyph_h == a.glyph_h
		&& color == a.color;
}

GlyphDrawCommand GlyphDrawCommand::changeColor(const t_vec4& newColor) const {
	GlyphDrawCommand toReturn(*this);
	glmc_vec4_set(toReturn.color, newColor[0], newColor[1], newColor[2], newColor[3]);
	return toReturn;
}

GlyphDrawCommand GlyphDrawCommand::changeColor(float r, float g, float b, float a) const {
	GlyphDrawCommand toReturn(*this);
	glmc_vec4_set(toReturn.color, r, g, b, a);
	return toReturn;
}

const t_vec4 &GlyphDrawCommand::getColor() const {
    return color;
}

const int GlyphDrawCommand::getBounceDiff() const {
	return this->bounceDiff;
}

const int GlyphDrawCommand::getLTopY() const {
	return this->pos_ltop_y;
}

const int GlyphDrawCommand::getLTopX() const {
	return this->pos_ltop_x;
}

const int GlyphDrawCommand::getGlyphY() const {
	return this->glyph_y;
}

const int GlyphDrawCommand::getGlyphX() const {
	return this->glyph_x;
}

const int GlyphDrawCommand::getGlyphWidth() const {
	return this->glyph_w;
}

const int GlyphDrawCommand::getGlyphHeight() const {
	return this->glyph_h;
}
