#include "container_system.h"


#include <stdlib.h>
#include <stdio.h>

#include <assert.h>

ContainerSystem* system_container_alloc() {
    ContainerSystem* res = calloc(1, sizeof(ContainerSystem));
    system_container_init(res);
    return res;
}

void system_container_init(ContainerSystem* system) {
}


void system_container_free(ContainerSystem* system) {
}

void system_container_update(ContainerSystem* system, Engine* engine) {
	EntityIterator it;
	search_entity_1(engine, COMP_INCONTAINER, &it);
	while (next_entity(&it)) {
		EntityId id = it.entity_id;
		InContainerComponent* cont = get_component(engine, id, COMP_INCONTAINER);
		if (cont->entity_location->pos[0] != cont->previous_location_x || cont->entity_location->pos[1] != cont->previous_location_y) {
			GridLocationComponent* loc = get_component(engine, id, COMP_GRIDLOCATION);
			loc->pos[0] = cont->entity_location->pos[0];
			loc->pos[1] = cont->entity_location->pos[1];
			cont->previous_location_x = cont->entity_location->pos[0];
			cont->previous_location_y = cont->entity_location->pos[1];
		}
	}

}
