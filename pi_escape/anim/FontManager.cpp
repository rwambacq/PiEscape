#include "FontManager.h"

using namespace std;

//TODO: the code below consists only of placeholders to make it compile. You need to replace them with the correct implementation

GlyphDrawCommand GlyphDrawCommand::changeColor(float r, float g, float b) const {
    //TODO implement correctly
    t_vec4 col;
    return GlyphDrawCommand(0, 0, 0, 0, 0, 0, col);
}

GlyphDrawCommand GlyphDrawCommand::changeAlpha(float a) const {
    //TODO implement correctly
    t_vec4 col;
    return GlyphDrawCommand(0, 0, 0, 0, 0, 0, col);
}

GlyphDrawCommand::GlyphDrawCommand(const int pos_ltop_x, const int pos_ltop_y, const int glyph_x, const int glyph_y,
                                   const int glyph_w, const int glyph_h, const t_vec4 &color) : color(color) {
    //TODO: implement
}

GlyphDrawCommand::GlyphDrawCommand(const GlyphDrawCommand &orig) : color(orig.color) {
    //TODO: implement this copy constructor
}

const t_vec4 &GlyphDrawCommand::getColor() const {
    return color;
}
