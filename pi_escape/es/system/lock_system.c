#include "lock_system.h"


#include <stdlib.h>
#include <stdio.h>

#include <assert.h>

LockSystem* system_lock_alloc() {
    LockSystem* res = calloc(1, sizeof(LockSystem));
    system_lock_init(res);
    return res;
}

void system_lock_init(LockSystem* system) {
    //TODO
}


void system_lock_free(LockSystem* system) {
    //TODO
}


void system_lock_update(LockSystem* system, Engine* engine) {
	EntityIterator it;
	search_entity_2(engine, COMP_ACTIVATION, COMP_LOCK, &it);
	while (next_entity(&it)) {
		EntityId lock = it.entity_id;
		assert(lock != NO_ENTITY);
		GridLocationComponent* lock_pos = get_component(engine, lock, COMP_GRIDLOCATION);
		LockComponent* lock_color = get_component(engine, lock, COMP_LOCK);
		ActivatableComponent* lock_act = get_component(engine, lock, COMP_ACTIVATABLE);

		EntityIterator key_it;
		search_entity_1(engine, COMP_ITEM, &key_it);
		while (next_entity(&key_it)) {
			EntityId key = key_it.entity_id;
			assert(key != NO_ENTITY);
			GridLocationComponent* key_pos = get_component(engine, key, COMP_GRIDLOCATION);
			ItemComponent* key_color = get_component(engine, key, COMP_ITEM);
			if (key_pos->pos[0] == lock_pos->pos[0] && key_pos->pos[1] == lock_pos->pos[1] && (!has_component(engine, key, COMP_INCONTAINER))) {
				if (( ! has_component(engine, key, COMP_INCONTAINER) ) && (lock_color->requiredKeyColor == key_color->color || lock_color->requiredKeyColor == O || key_color->color == O)) {
					lock_act->active = 1;
				}
				else {
					lock_act->active = 0;
				}
				break;
			}
		}
		free_component(engine, lock, COMP_ACTIVATION);
	}
}
