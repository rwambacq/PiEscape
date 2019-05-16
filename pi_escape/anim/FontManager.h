#ifndef PIESCAPE2_FONTMANAGER_H
#define PIESCAPE2_FONTMANAGER_H

#include "../graphics/opengl_game_renderer.h"
#include "../graphics/gl_glyph.h"

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

//for format of .fnt file, see http://www.angelcode.com/products/bmfont/doc/file_format.html

float fmin_min(float, float);
float fmax_max(float, float);

class GlyphDrawCommand {
private:
	bool up;
	int wait;
	int bounceDiff;
    t_vec4 color;
	int pos_ltop_x;
	int pos_ltop_y;
	int glyph_x;
	int glyph_y;
	int glyph_w;
	int glyph_h;
public:
    /**
     * Create a draw command
     * @param pos_ltop_x The X coordinate of the position on the screen of the left top of the glyph
     * @param pos_ltop_y The Y coordinate of the position on the screen of the left top of the glyph
     * @param glyph_x The X coordinate of the glyph in the font image (coordinate x in png)
     * @param glyph_y The Y coordinate of the glyph in the font image (coordinate y in png)
     * @param glyph_w The width of the glyph (both on screen and in the font image)
     * @param glyph_h The height of the glyph (both on screen and in the font image)
     * @param color The color the glyph should have. Note that this includes the alpha (1.0f = opaque, 0.0f = transparent)
     */
    GlyphDrawCommand(const int pos_ltop_x, const int pos_ltop_y,
                     const int glyph_x, const int glyph_y,
                     const int glyph_w, const int glyph_h,
                     const t_vec4& color);
    GlyphDrawCommand(const GlyphDrawCommand& orig);

	GlyphDrawCommand();

    //these method create a NEW GlyphDrawCommand based on a transformation of this one
    GlyphDrawCommand move(int x_offset, int y_offset) const;
    GlyphDrawCommand changeColor(const t_vec4& newColor) const;
    GlyphDrawCommand changeColor(float r, float g, float b, float a) const;
    GlyphDrawCommand changeColor(float r, float g, float b) const;
    GlyphDrawCommand changeAlpha(float a) const;
	void setLtopY(float a);

	bool operator==(const GlyphDrawCommand& a) const;

	const int getBounceDiff()const;
    const t_vec4& getColor() const;
	const int getLTopX() const;
	const int getLTopY()const;
	const int getGlyphX()const;
	const int getGlyphY()const;
	const int getGlyphWidth()const;
	const int getGlyphHeight()const;
};

enum TextJustification { TEXT_LEFT, TEXT_CENTER, TEXT_RIGHT };
enum TextVerticalPosition { TEXT_TOP, TEXT_MIDDLE, TEXT_BOTTOM };

class FontManager {
private:
	Graphics* graphics;
	GLGlyph* glyph;
	TextJustification hpos;
	TextVerticalPosition vpos;
	t_vec4 color;
	t_vec2 scale;
	std::string fontName;
	std::string fontMetaFilename;
	std::string fontImageFilename;
public:
	FontManager();
    FontManager(Graphics* graphics, GLGlyph* glyph);
    ~FontManager();
    
    void loadFont(const std::string& fontName,
                  const std::string& fontImageFilename,
                  const std::string& fontMetaFilename);

    //these methods set attributes for the next  makeGlyphDrawCommands call
    void setHpos(TextJustification hpos);
    void setVpos(TextVerticalPosition vpos);
    void setColor(const t_vec4& color);
    void setScale(const t_vec2& scale);
    void setColor(float colorR, float colorG, float colorB, float colorA);
    void setScale(float xScale, float yScale);
    void setFont(const std::string& fontName);

	Graphics* getGraphics();
	GLGlyph* getGlyphPtr();

	std::string getFontImageFilename();

    std::vector<GlyphDrawCommand> makeGlyphDrawCommands(std::string text, int x, int y) const;
};



#endif //PIESCAPE2_FONTMANAGER_H
