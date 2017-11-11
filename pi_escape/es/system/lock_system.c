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
	search_entity_2(engine, COMP_INUSE, COMP_LOCK, &it);
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
			if(0){
			printf("oei jammer");
			}
			else {
				if (key_pos->pos[0] == lock_pos->pos[0] && key_pos->pos[1] == lock_pos->pos[1] && (!has_component(engine, key, COMP_INCONTAINER))) {
					if ((!has_component(engine, key, COMP_INCONTAINER)) && (lock_color->requiredKeyColor == key_color->color || lock_color->requiredKeyColor == O || key_color->color == O)) {
						lock_act->active = 1;
						printf("nee wtf");
						checkForActivation(engine, lock, 0);
						break;
					}
				}
				else if (has_component(engine, key, COMP_INCONTAINER) && (lock_color->requiredKeyColor == key_color->color || lock_color->requiredKeyColor == O || key_color->color == O)) {
					

					if (lock_act->active != 0) {
						lock_act->active = 0;
					}
					checkForActivation(engine, lock, 1);
				}
			}
		}
		free_component(engine, lock, COMP_INUSE);
	}
}


void checkForActivation(Engine* engine, EntityId lock, int x) {

	ActivatableComponent* aan = get_component(engine, lock, COMP_ACTIVATABLE);
	ConnectionsComponent* naast = get_component(engine, lock, COMP_CONNECTIONS);
	ActivatableComponent* aans = get_component(engine, naast->prev, COMP_ACTIVATABLE);


	if (x==0) {
		ConnectionsComponent* nieuwpath = get_component(engine, lock, COMP_CONNECTIONS);
		if (has_component(engine, lock, COMP_CONNECTORLOGIC)) {
			ConnectorLogicComponent *xje = get_component(engine, lock, COMP_CONNECTORLOGIC);
			ConnectorOr* x = get_component(engine, xje->andor, COMP_CONNOR);
			ActivatableComponent* lamptest = get_component(engine, nieuwpath->next, COMP_ACTIVATABLE);
			if (lamptest->active != 1) {
				x->current += 1;
			}
		}
			EntityId volgende = nieuwpath->next;
			ActivationComponent* activatie = create_component(engine, volgende, COMP_ACTIVATION);
			activatie->currenttime = 25;
			activatie->getto = 0;
	}
	else if (x) {

		if (has_component(engine, lock, COMP_CONNECTORLOGIC)) {
			ConnectorLogicComponent *xje = get_component(engine, lock, COMP_CONNECTORLOGIC);
			ConnectorOr* xi = get_component(engine, xje->andor, COMP_CONNOR);
			xi->current -= 1;
			printf("\n nodig : %d  : %d", xi->needed, xi->current);

			if (xi->current >= xi->needed) {
				printf("a");
				ConnectorLogicComponent* xje = get_component(engine, lock, COMP_CONNECTORLOGIC);
				lock = xje->deelaanor;
				EntityId volgende = lock;
				ActivationComponent* activatie = create_component(engine, volgende, COMP_ACTIVATION);
				activatie->currenttime = 10;
				activatie->getto = 0;
			}
			else {
				printf("oe");

				ConnectorLogicComponent *x = get_component(engine, lock, COMP_CONNECTORLOGIC);
				LockDoorComponent* deurie = get_component(engine, x->andor, COMP_LOCKDOOR);
				
				ConnectionsComponent* aanpassen = get_component(engine, xje->andor, COMP_CONNECTIONS);
				aanpassen->prev = xje->deelaanor;
				
				//ConnectionsComponent* nieuwpath = get_component(engine, deurie->door, COMP_CONNECTIONS);
				//EntityId volgende = nieuwpath->prev;
				ActivationComponent* activatie;
				ActivatableComponent* pfkzientbeu = get_component(engine, deurie->door, COMP_ACTIVATABLE);

				if (pfkzientbeu->active == 1) {
					if (has_component(engine, deurie->door, COMP_DOUBLEDOOR)) {

						printf("zou niet mogen");
						DoubleDoor* deutj = get_component(engine, deurie->door, COMP_DOUBLEDOOR);
						ActivatableComponent* eentje = get_component(engine, deutj->een, COMP_ACTIVATABLE);
						ActivatableComponent* tweetje = get_component(engine, deutj->twee, COMP_ACTIVATABLE);
						printf("\neen : %d \n twee: %d\n", eentje->active, tweetje->active);

						if (eentje->active == 1 && tweetje->active == 1) {
							WalkComponent* nieuwpath = get_component(engine, x->andor, COMP_WALKABLE);
							ActivationComponent* activatie = create_component(engine, nieuwpath->lastconn, COMP_ACTIVATION);
							activatie->currenttime = 10;
							activatie->getto = 0;
						}
						else {
							printf("test1");
							WalkComponent* nieuwpath = get_component(engine, x->andor, COMP_WALKABLE);
							if (eentje->active || tweetje->active) {
								ActivatableComponent* lamp = get_component(engine, nieuwpath->lastconn, COMP_ACTIVATABLE);
								if (lamp->active == 1) {
									ActivatableComponent* act = get_component(engine, deurie->door, COMP_ACTIVATABLE);
									act->active = 0;
								}
							}
							ActivatableComponent* orand = get_component(engine, x->andor, COMP_ACTIVATABLE);
							if (orand->active == 1) {
								ActivationComponent* activatie = create_component(engine, nieuwpath->lastconn, COMP_ACTIVATION);
								activatie->currenttime = 10;
								activatie->getto = 0;
							}
							else {
								ActivationComponent* activatie = create_component(engine, x->deelaanor, COMP_ACTIVATION);
								activatie->currenttime = 10;
								activatie->getto = 0;
							}
						}
					}
					else {
						printf("\nhahahhah\n");
						printf("doe da");
						activatie = create_component(engine, deurie->door, COMP_ACTIVATION);
						activatie->currenttime = 10;
						activatie->getto = 0;
					}


				}
				else {
				
					printf("\nhahahhah\n");
					printf("doe dit");
					activatie = create_component(engine, xje->deelaanor, COMP_ACTIVATION);
					activatie->currenttime = 10;
					activatie->getto = 0;
				}
				

				
			}
		}
		else {
			
			if (has_component(engine, lock, COMP_CONNECTORLOGIC)) {
				ConnectorLogicComponent *x = get_component(engine, lock, COMP_CONNECTORLOGIC);
				LockDoorComponent* deurie = get_component(engine, x->andor, COMP_LOCKDOOR);
				ActivatableComponent* pffff = get_component(engine, deurie->door, COMP_ACTIVATABLE);
				pffff->active = 0;
			}
			else {
				LockDoorComponent* deurie = get_component(engine, lock, COMP_LOCKDOOR);
				EntityId volgende = deurie->door;
				if (has_component(engine, volgende, COMP_DOUBLEDOOR)) {
					DoubleDoor* deutj = get_component(engine, volgende, COMP_DOUBLEDOOR);
					ActivatableComponent* eentje = get_component(engine, deutj->een, COMP_ACTIVATABLE);
					ActivatableComponent* tweetje = get_component(engine, deutj->twee, COMP_ACTIVATABLE);
					printf("\neen : %d \n twee: %d\n", eentje->active, tweetje->active);
					if (eentje->active && tweetje->active) {
						WalkComponent* nieuwpath = get_component(engine, lock, COMP_WALKABLE);
						ActivationComponent* activatie = create_component(engine, nieuwpath->lastconn, COMP_ACTIVATION);
						activatie->currenttime = 10;
						activatie->getto = 0;
					}
					else {

						ActivatableComponent* act = get_component(engine, volgende, COMP_ACTIVATABLE);
						act->active = 0;
						WalkComponent* nieuwpath = get_component(engine, lock, COMP_WALKABLE);
						ActivationComponent* activatie = create_component(engine, nieuwpath->lastconn, COMP_ACTIVATION);
						activatie->currenttime = 10;
						activatie->getto = 0;
					}
				}
				else {
					printf("\n oei neen");
					ActivationComponent* activatie = create_component(engine, volgende, COMP_ACTIVATION);
					activatie->currenttime = 10;
					activatie->getto = 0;
				}
			}
		}
	}


}