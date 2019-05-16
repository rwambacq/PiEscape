#include "MovieBuilder.h"

MovieBuilder::MovieBuilder() {

}

MovieEntry MovieBuilder::addText(std::string text) {
	MovieEntry entry;
	entry.setText(text);
	this->entries.push_back(entry);
	return entry;
}

void MovieBuilder::setDuration(long tijd) {
	this->duration = tijd;
}

std::shared_ptr<MovieDefinition> MovieBuilder::build() {
	MovieDefinition def(this->entries, this->duration);
	std::shared_ptr<MovieDefinition> returntje = std::make_shared<MovieDefinition>(def);
	return returntje;
}