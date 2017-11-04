#include "action_system.h"

#include <assert.h>

#include <stdlib.h>

ActionSystem* system_action_alloc() {
    ActionSystem* res = calloc(1, sizeof(ActionSystem));
    system_action_init(res);
    return res;
}

void system_action_init(ActionSystem* system) {
    //TODO
}


void system_action_free(ActionSystem* system) {
    //TODO
}

void system_action_update(ActionSystem* system, Engine* engine) {
	EntityIterator it;
	search_entity_2(engine, COMP_CONTAINER, COMP_ITEMACTION, &it);
	while (next_entity(&it)) {
		EntityId ent = it.entity_id;
		assert(ent != NO_ENTITY);
		GridLocationComponent* ent_loc = get_component(engine, ent, COMP_GRIDLOCATION);
		ContainerComponent* container = get_component(engine, ent, COMP_CONTAINER);

		EntityIterator item_it;
		search_entity_1(engine, COMP_ITEM, &item_it);
		while (next_entity(&item_it)) {
			EntityId item = item_it.entity_id;
			assert(item != NO_ENTITY);

			GridLocationComponent* item_loc = get_component(engine, item, COMP_GRIDLOCATION);
			if (item_loc->pos[0] == ent_loc->pos[0] && item_loc->pos[1] == ent_loc->pos[1]) {
				if (container->contains_something && container->id != item) {
					EntityId contained = container->id;
					container->id = item;
					free_component(engine, contained, COMP_INCONTAINER);
					InContainerComponent* item_incontainer = create_component(engine, item, COMP_INCONTAINER);
					item_incontainer->entity_location = ent_loc;
					item_incontainer->previous_location_x = ent_loc->pos[0];
					item_incontainer->previous_location_y = ent_loc->pos[1];
					break;
				}
				else if (container->contains_something && container->id == item ) {
					EntityId contained = container->id;
					free_component(engine, contained, COMP_INCONTAINER);
					container->contains_something = 0;
				}
				else {
					container->id = item;
					InContainerComponent* item_incontainer = create_component(engine, item, COMP_INCONTAINER);
					item_incontainer->entity_location = ent_loc;
					item_incontainer->previous_location_x = ent_loc->pos[0];
					item_incontainer->previous_location_y = ent_loc->pos[1];
					container->contains_something = 1;
				}
			}
		}
		free_component(engine, ent, COMP_ITEMACTION);
	}
}
