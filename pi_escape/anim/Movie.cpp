#include "Movie.h"

using namespace std;

MovieDefinition::MovieDefinition(std::vector<MovieEntry> entries, long l ) {
	this->duration = l;
	this->entries = entries;
}

std::vector<MovieEntry> MovieDefinition::getEntries() {
	return this->entries;
}

MovieEntry MovieEntry::setFont(std::string font) {
	this->font = font;
	return *this;
}
MovieEntry MovieEntry::setColor(t_vec4 color) {
	glmc_vec4_set(this->color, color[0], color[1], color[2], color[3]);
	return *this;
}
MovieEntry MovieEntry::setPos_percent(float x, float y) {
	PosPercent pp{ x, y };
	this->pospercent = pp;
	return *this;
}
MovieEntry MovieEntry::setStartTime(long start) {
	this->startTime = start;
	return *this;
}

MovieEntry MovieEntry::setEndTime(long end) {
	this->endTime = end;
	return *this;
}

MovieEntry MovieEntry::addAnimation(Animation* anim, long start, long end) {
	this->animations.push_back(anim);
	return *this;
}

MovieEntry MovieEntry::setText(std::string text) {
	this->text = text;
	return *this;
}

void MovieEntry::endText(){
	return;
}

//TODO
