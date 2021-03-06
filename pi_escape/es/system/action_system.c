#include "action_system.h"

#include <assert.h>
#include "../../../util/sleep.h"
#include <stdlib.h>
#include<stdio.h>



void checkForLock(Engine* engine, EntityId key);

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
					//Switch key
					ItemComponent* oud = get_component(engine, container->id, COMP_ITEM);
					EntityId contained = container->id;
					container->id = item;
					free_component(engine, contained, COMP_INCONTAINER);
					InContainerComponent* item_incontainer = create_component(engine, item, COMP_INCONTAINER);
					item_incontainer->entity_location = ent_loc;
					item_incontainer->previous_location_x = ent_loc->pos[0];
					item_incontainer->previous_location_y = ent_loc->pos[1];
					ItemComponent* ok = get_component(engine, container->id, COMP_ITEM);
					int x = (int)ok->color;
					showColor(x);
					if (oud->color == ok->color || oud->color == O) {
					}
					else {
						checkForLock(engine, item);
					}
					break;
				}
				else if (container->contains_something && container->id == item) {
					//Drop key
					EntityId contained = container->id;
					free_component(engine, contained, COMP_INCONTAINER);
					container->contains_something = 0;
					showColor(6);
					checkForLock(engine, item);
				}
				else if (!container->contains_something) {
					//Pick up key
					container->id = item;
					InContainerComponent* item_incontainer = create_component(engine, item, COMP_INCONTAINER);
					item_incontainer->entity_location = ent_loc;
					item_incontainer->previous_location_x = ent_loc->pos[0];
					item_incontainer->previous_location_y = ent_loc->pos[1];
					container->contains_something = 1;

					ItemComponent* ok = get_component(engine, container->id, COMP_ITEM);
					int x = (int)ok->color;
					showColor(x);
					checkForLock(engine, item);
				}
			}
		}
		free_component(engine, ent, COMP_ITEMACTION);
	}

}

void checkForLock(Engine* engine, EntityId key) {
	GridLocationComponent* key_pos = get_component(engine, key, COMP_GRIDLOCATION);

	EntityIterator it;
	search_entity_1(engine, COMP_LOCK, &it);
	while (next_entity(&it)) {
		EntityId lock = it.entity_id;
		assert(lock != NO_ENTITY);
		GridLocationComponent* lock_pos = get_component(engine, lock, COMP_GRIDLOCATION);
		if (key_pos->pos[0] == lock_pos->pos[0] && key_pos->pos[1] == lock_pos->pos[1]) {
			if (!get_component(engine, lock, COMP_INUSE)) {
				inUse* x = create_component(engine, lock, COMP_INUSE);
			}
		}
	}
}