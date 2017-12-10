#ifndef PIESCAPE2_ANIMATION_H
#define PIESCAPE2_ANIMATION_H

#include "FontManager.h"

#include <glmc.h>

typedef struct hsv {
	float h;
	float s;
	float v;
} hsv;

typedef struct rgb {
	float r;
	float g;
	float b;
} rgb;

rgb HsvToRgb(hsv);
hsv RgbToHsv(rgb);

class Animation {
public:
    virtual ~Animation();

    /** position is between 0.0f and 1.0f (inclusive),
     * 0.0 being no change, 1.0 being full effect of animation */
    virtual std::vector<GlyphDrawCommand> applyTransform(
            const std::vector<GlyphDrawCommand>& draws,
            float position) const;
};

/** From alpha 0.0f to 1.0f (= from invisible to visible) */
class FadeInAnimation : public Animation {
public:
    virtual ~FadeInAnimation();

    virtual std::vector<GlyphDrawCommand> applyTransform(
            const std::vector<GlyphDrawCommand>& draws,
            float position)
    const override;
};

/** From original color to requested color */
class ColorAnimation : public Animation {
private:
    t_vec4 newColor;
public:
    ColorAnimation(t_vec4 newColor);
    ColorAnimation(float r, float g, float b, float a);
    ColorAnimation(float r, float g, float b);
    virtual ~ColorAnimation();
    
    virtual std::vector<GlyphDrawCommand> applyTransform(
            const std::vector<GlyphDrawCommand>& draws,
            float position)
    const override;
};

/** Cycle through all colors */
class RainbowColorAnimation : public Animation {
private:
public:
    virtual ~RainbowColorAnimation();
    
    virtual std::vector<GlyphDrawCommand> applyTransform(
            const std::vector<GlyphDrawCommand>& draws,
            float position)
    const override;
};

/** Move text position relatively */
class MoveAnimation : public Animation {
private:
	t_ivec2 newPos;
public:
    MoveAnimation(t_ivec2 relPos);
    MoveAnimation(int x, int y);
    virtual ~MoveAnimation();
    
    std::vector<GlyphDrawCommand> applyTransform(
            const std::vector<GlyphDrawCommand>& draws,
            float position)
    const override;
};

/** Apply an animation to each glyph in turn */
class GlyphIteratingAnimation : public Animation {
private:
	Animation* animation;
	float overlap;
	int animIndex = 0;
public:
    /**
     * @param animation
     * @param overlap start the next glyphs animation when the prev glyphs animation is at this position
     */
    GlyphIteratingAnimation(Animation* animation, float overlap);
    virtual ~GlyphIteratingAnimation();
    
    virtual std::vector<GlyphDrawCommand> applyTransform(
            const std::vector<GlyphDrawCommand>& draws,
            float position)
    const override;
};

/** Apply an animation multiple times */
class RepeatAnimation : public Animation {
private:
	Animation* animation;
	int repeats;
	bool inOut;
public:
    RepeatAnimation(Animation* animation, int repeats, bool startIn, bool endIn, bool cycleInOut);
    RepeatAnimation(Animation* animation, int repeats);
    virtual ~RepeatAnimation();
    
    virtual std::vector<GlyphDrawCommand> applyTransform(
            const std::vector<GlyphDrawCommand>& draws,
            float position)
    const override;
};

/** Apply a sine function to an animation. This smooths animations. */
class SineAnimation : public Animation {
private:
public:
    SineAnimation(Animation* animation);
    virtual ~SineAnimation();
    
    virtual std::vector<GlyphDrawCommand> applyTransform(
            const std::vector<GlyphDrawCommand>& draws,
            float position)
    const override;
};

/** Reverse an animation. */
class ReverseAnimation : public Animation {
private:
	Animation* animation;
public:
    ReverseAnimation(Animation* animation);
    virtual ~ReverseAnimation();
    
    virtual std::vector<GlyphDrawCommand> applyTransform(
            const std::vector<GlyphDrawCommand>& draws,
            float position)
    const override;
};

/** First run the animation, then run it in reverse. */
class InOutAnimation : public Animation {
private:
	Animation* animation;
public:
    InOutAnimation(Animation* animation);
    virtual ~InOutAnimation();
    
    virtual std::vector<GlyphDrawCommand> applyTransform(
            const std::vector<GlyphDrawCommand>& draws,
            float position)
    const override;
};

#endif //PIESCAPE2_ANIMATION_H
