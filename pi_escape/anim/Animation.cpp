#include "Animation.h"

using namespace std;

Animation::~Animation() {

}

vector<GlyphDrawCommand> Animation::applyTransform(const vector<GlyphDrawCommand>& draws,float position) const {
	return draws;
}

vector<GlyphDrawCommand> FadeInAnimation::applyTransform(const std::vector<GlyphDrawCommand>& draws, float position) const {
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

vector<GlyphDrawCommand> ColorAnimation::applyTransform(const vector<GlyphDrawCommand> &draws, float position) const {
    vector<GlyphDrawCommand> res;
    
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

std::vector<GlyphDrawCommand> RainbowColorAnimation::applyTransform( const std::vector<GlyphDrawCommand>& draws, float position) const {
	std::vector<GlyphDrawCommand> res;

	for (std::vector<GlyphDrawCommand>::const_iterator it = draws.begin(); it != draws.end(); it++) {
		const GlyphDrawCommand& cur = *it;

		rgb this_rgb;
		this_rgb.r = cur.getColor()[0];
		this_rgb.g = cur.getColor()[1];
		this_rgb.b = cur.getColor()[2];
		hsv hsv = RgbToHsv(this_rgb);

		hsv.h = fmod(hsv.h + (359.0f * position), 359.0f); // causes colours to cycle through all hues once, stops at original colour

		rgb newColor = HsvToRgb(hsv);
		GlyphDrawCommand toReturn = cur.changeColor(newColor.r, newColor.g, newColor.b);

		res.push_back(toReturn);
	}

	return res;
}

RainbowColorAnimation::~RainbowColorAnimation() {
}

// see https://en.wikipedia.org/wiki/HSL_and_HSV#Converting_to_RGB for an algorithm to convert hsv values to rgb
rgb HsvToRgb(hsv hsv) {

	float h = hsv.h;
	float s = hsv.s;
	float v = hsv.v;

	rgb rgb;
	float c, hAcc, x;
	float r1, g1, b1, m;

	if (s == 0.0f) {
		rgb.r = v;
		rgb.g = v;
		rgb.b = v;
		return rgb;
	}

	c = v * s;
	m = v - c;

	hAcc = h / 60;
	x = c * (1 - abs(fmod(hAcc, 2) - 1));

	if (hAcc >= 0 && hAcc <= 1) {
		r1 = c;
		g1 = x;
		b1 = 0;
	}
	else if (hAcc > 1 && hAcc <= 2) {
		r1 = x;
		g1 = c;
		b1 = 0;
	}
	else if (hAcc > 2 && hAcc <= 3) {
		r1 = 0;
		g1 = c;
		b1 = x;
	}
	else if (hAcc > 3 && hAcc <= 4) {
		r1 = 0;
		g1 = x;
		b1 = c;
	}
	else if (hAcc > 4 && hAcc <= 5) {
		r1 = x;
		g1 = 0;
		b1 = c;
	}
	else if (hAcc > 5 && hAcc < 6) {
		r1 = c;
		g1 = 0;
		b1 = x;
	}
	else {
		r1 = 0;
		g1 = 0;
		b1 = 0;
	}

	rgb.r = r1 + m;
	rgb.g = g1 + m;
	rgb.b = b1 + m;

	return rgb;
}
// see https://www.rapidtables.com/convert/color/rgb-to-hsv.html for the algorithm used to convert rgb to hsv
hsv RgbToHsv(rgb rgb) {
	hsv hsv;
	float r = rgb.r, g = rgb.g, b = rgb.b;
	float rgbMin, rgbMax, delta;
	float h, s, v;

	rgbMin = fmin_min(rgb.r, fmin_min(rgb.g, rgb.b));
	rgbMax = fmax_max(rgb.r, fmax_max(rgb.g, rgb.b));
	v = rgbMax;
	delta = rgbMax - rgbMin;

	// calculate hue
	if (delta == 0.0f) {
		h = 0.0f;
	}
	else if (rgbMax == r) {
		h = fmod((g - b) / delta, 6.0f);
		if (h < 0) {
			h += 6.0f;
		}
		h *= 60.0f;
	}
	else if (rgbMax == g) {
		h = 60.0f * (((b - r) / delta) + 2.0f);
	}
	else if (rgbMax == b) {
		h = 60.0f * (((r - g) / delta) + 4.0f);
	}

	// calculate 
	//       S
	//     A   T
	//   U   R   A
	// T   I   O   N

	if (rgbMax == 0.0f) {
		s = 0.0f;
	}
	else {
		s = delta / rgbMax;
	}

	hsv.h = h;
	hsv.s = s;
	hsv.v = v;

	return hsv;
}

float fmin_min(float f1, float f2) {
	if (f1 <= f2) {
		return f1;
	}
	return f2;
}

float fmax_max(float f1, float f2) {
	if (f1 >= f2) {
		return f1;
	}
	return f2;
}

std::vector<GlyphDrawCommand> MoveAnimation::applyTransform( const std::vector<GlyphDrawCommand>& draws, float position) const {
	std::vector<GlyphDrawCommand> res;

	for (std::vector<GlyphDrawCommand>::const_iterator it = draws.begin(); it != draws.end(); it++) {
		const GlyphDrawCommand& cur = *it;

		GlyphDrawCommand toReturn = cur.move(position * (newPos[0] - cur.getLTopX()), position * (newPos[1] - cur.getLTopY()));
		res.push_back(toReturn);
	}

	return res;
}

MoveAnimation::MoveAnimation(t_ivec2 relPos)
	: newPos{ relPos[0], relPos[1] } {}

MoveAnimation::MoveAnimation(int x, int y)
	: newPos{ x, y } {}

MoveAnimation::~MoveAnimation() {

}

GlyphIteratingAnimation::GlyphIteratingAnimation(Animation* animation, float overlap) : animation(animation), overlap(overlap) {}

GlyphIteratingAnimation::~GlyphIteratingAnimation() {

}

vector<GlyphDrawCommand> GlyphIteratingAnimation::applyTransform(const vector<GlyphDrawCommand>& draws, float position) const{
	return draws;
}
// TODO implement  
// RepeatAnimation, 
// SineAnimation, 
// ReverseAnimation, 
// InOutAnimation
