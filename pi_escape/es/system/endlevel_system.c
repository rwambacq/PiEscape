#include "endlevel_system.h"


#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

EndLevelSystem* system_endlevel_alloc() {
    EndLevelSystem* res = calloc(1, sizeof(EndLevelSystem));
    system_endlevel_init(res);
    return res;
}

void system_endlevel_init(EndLevelSystem* system) {
    //TODO
}


void system_endlevel_free(EndLevelSystem* system) {
    //TODO
}

void system_endlevel_update(EndLevelSystem* system, Engine* engine) {
	EntityIterator exit_it;
	search_entity_1(engine, COMP_EXIT, &exit_it);
	next_entity(&exit_it);
	EntityId exit = exit_it.entity_id;
	assert(exit != NO_ENTITY);
	GridLocationComponent* exit_pos = get_component(engine, exit, COMP_GRIDLOCATION);
	ExitComponent* exit_comp = get_component(engine, exit, COMP_EXIT);

	EntityIterator player_it;
	search_entity_2(engine, COMP_GRIDLOCATION, COMP_INPUTRECEIVER, &player_it);
	next_entity(&player_it);
	EntityId player = player_it.entity_id;
	assert(player != NO_ENTITY);
	GridLocationComponent* player_pos = get_component(engine, player, COMP_GRIDLOCATION);

	if (exit_pos->pos[0] == player_pos->pos[0] && exit_pos->pos[1] == player_pos->pos[1]) {
		exit_comp->done = 1;
		showColor(6);
	}
}
