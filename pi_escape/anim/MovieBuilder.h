#ifndef PIESCAPE2_ANIMATIONSEQUENCEBUILDER_H
#define PIESCAPE2_ANIMATIONSEQUENCEBUILDER_H

#include "Animation.h"
#include "Movie.h"
#include "GameUICreator.h"

class MovieBuilder {
private:
	long duration;
	std::vector<MovieEntry> entries;
public:
	MovieBuilder();
	MovieEntry addText(std::string text);
	void setDuration(long tijd);
	std::shared_ptr<MovieDefinition> build();
};

#endif //PIESCAPE2_ANIMATIONSEQUENCEBUILDER_H
