#ifndef PIESCAPE2_ANIMATIONSEQUENCE_H
#define PIESCAPE2_ANIMATIONSEQUENCE_H

#include "UI.h"
#include "Animation.h"

typedef struct PosPercent;
class MovieEntry;

/**
 * A sequence of text animations, such as an intro, credits, ...
 */
class MovieDefinition {
private:
	std::vector<MovieEntry> entries;
	long duration;
public:
	MovieDefinition(std::vector<MovieEntry> entries, long duration);
	std::vector<MovieEntry> getEntries();
};


class MovieModel : public UIModel {
    //TODO
};

class MovieGLView : public UIView {
    //TODO
};

class MovieController : public UIController {
    //TODO
    
};

typedef struct PosPercent {
	float x;
	float y;
};

class MovieEntry {
private:
	std::string text;
	std::string font;
	t_vec4 color;
	long startTime;
	long endTime;
	std::vector<Animation*> animations;
	PosPercent pospercent;
public:
	MovieEntry setText(std::string text);
	MovieEntry setFont(std::string font);
	MovieEntry setColor(t_vec4 color);
	MovieEntry setPos_percent(float x, float y);
	MovieEntry setStartTime(long start);
	MovieEntry setEndTime(long end);
	MovieEntry addAnimation(Animation* anim, long start, long end);
	void endText();
};

#endif //PIESCAPE2_ANIMATIONSEQUENCE_H
