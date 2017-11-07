#include "lock_system.h"


#include <stdlib.h>
#include <stdio.h>

#include <assert.h>


void checkForActivation(Engine* engine, EntityId lock,int x);

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
					checkForActivation(engine, lock, 0);
				}
				else {
					lock_act->active = 0;
					checkForActivation(engine, lock, 1);
				}
				break;
			}
			else if (has_component(engine, key, COMP_INCONTAINER) && (lock_color->requiredKeyColor == key_color->color || lock_color->requiredKeyColor == O || key_color->color == O)) {
				lock_act->active = 0;
				checkForActivation(engine, lock, 1);
				break;
			}
		}
		free_component(engine, lock, COMP_ACTIVATION);
	}
}


void checkForActivation(Engine* engine, EntityId lock, int x) {

	ActivatableComponent* aan = get_component(engine, lock, COMP_ACTIVATABLE);
	ConnectionsComponent* naast = get_component(engine, lock, COMP_CONNECTIONS);
	ActivatableComponent* aans = get_component(engine, naast->prev, COMP_ACTIVATABLE);

	printf("%d", aan->active);

	if (x==0) {
		ConnectionsComponent* nieuwpath = get_component(engine, lock, COMP_CONNECTIONS);
		EntityId volgende = nieuwpath->next;
		ActivationComponent* activatie = create_component(engine, volgende, COMP_ACTIVATION);
		activatie->currenttime = 100;
		activatie->getto = 0;
	}
	else if (x) {
		ConnectionsComponent* nieuwpath = get_component(engine, lock, COMP_CONNECTIONS);
		EntityId volgende = nieuwpath->prev;
		ActivationComponent* activatie = create_component(engine, volgende, COMP_ACTIVATION);
		activatie->currenttime = 50;
		activatie->getto = 0;
	}


}

