#include "Animation.h"

using namespace std;

Animation::~Animation() {

}

std::vector<GlyphDrawCommand> FadeInAnimation::applyTransform(const std::vector<GlyphDrawCommand>& draws, float position) const {
    float curAlpha = position;
    
    std::vector<GlyphDrawCommand> res;
    
    for (std::vector<GlyphDrawCommand>::const_iterator it = draws.begin(); it != draws.end(); it++) {
        const GlyphDrawCommand& cur = *it;
        GlyphDrawCommand replacement = cur.changeAlpha(cur.getColor()[3] * curAlpha);
        res.push_back(replacement);
    }
    
    return res;
}

FadeInAnimation::~FadeInAnimation() {

}




ColorAnimation::ColorAnimation(t_vec4 newColor)
        : newColor{newColor[0], newColor[1], newColor[2], newColor[3]} {}

ColorAnimation::ColorAnimation(float r, float g, float b, float a)
        : newColor{r,g,b,a} {}

ColorAnimation::ColorAnimation(float r, float g, float b)
        : newColor{r,g,b,1.0f} {}

ColorAnimation::~ColorAnimation() {

}

std::vector<GlyphDrawCommand> ColorAnimation::applyTransform(const std::vector<GlyphDrawCommand> &draws, float position) const {
    std::vector<GlyphDrawCommand> res;
    
    for (std::vector<GlyphDrawCommand>::const_iterator it = draws.begin(); it != draws.end(); it++) {
        const GlyphDrawCommand& cur = *it;

        t_vec4 curColor;
        curColor[0] = (cur.getColor()[0] * (1.0f - position)) + (newColor[0] * position);
        curColor[1] = (cur.getColor()[1] * (1.0f - position)) + (newColor[1] * position);
        curColor[2] = (cur.getColor()[2] * (1.0f - position)) + (newColor[2] * position);
        curColor[3] = (cur.getColor()[3] * (1.0f - position)) + (newColor[3] * position);

        GlyphDrawCommand replacement = cur.changeColor(curColor[0], curColor[1], curColor[2]);
        res.push_back(replacement);
    }
    
    return res;
}

//TODO implement the rest of the animations
