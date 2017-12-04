#include "FontManager.h"

using namespace std;

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
	this->fontName = fontName;
	this->fontImageFilename = "pi_escape/graphics/" + fontName + ".png";
	this->fontMetaFilename = "pi_escape/graphics/" + fontName + ".fnt";
}

//TODO: implement draw so it isn't shit

std::vector<GlyphDrawCommand> FontManager::makeGlyphDrawCommands(std::string text, int x, int y) const {
	int leftTopX = x;
	int leftTopY = y;
	int i = 0;

	int maxHeight = 0;

	std::vector<GlyphDrawCommand> toReturn(text.size());
	cout << "text: " << text << endl;
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
					/*cout << "gevonden letter id: " << id << endl;
					cout << "x en y in png file: " << x << " " << y << endl;
					cout << "width en height: " << width << " " << height << endl;
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
	glmc_vec4_set(this->color, 0, 0 , 0, 1);
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

void GlyphDrawCommand::bounce() {
		if (this->up) { // bounce up
			this->bounceDiff++;
			if (this->bounceDiff == 10) {
				this->up = false;
			}
		}
		else { // bounce down
			this->bounceDiff--;
			if (this->bounceDiff == 0) {
				this->up = true;
			}
		}
}

const t_vec4 &GlyphDrawCommand::getColor() const {
    return color;
}

int GlyphDrawCommand::getBounceDiff() {
	return this->bounceDiff;
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

