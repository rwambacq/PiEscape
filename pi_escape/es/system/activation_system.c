#include "activation_system.h"


#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <sys\timeb.h> 

ActivationSystem* system_activation_alloc() {
    ActivationSystem* res = calloc(1, sizeof(ActivationSystem));
    system_activation_init(res);
    return res;
}

void system_activation_init(ActivationSystem* system) {
    //TODO
}


void system_activation_free(ActivationSystem* system) {
    //TODO
}


void system_activation_update(ActivationSystem* system, Engine* engine) {

	EntityIterator itlock;
	search_entity_3(engine, COMP_ACTIVATABLE, COMP_ART, COMP_ACTIVATION, &itlock);

	if (next_entity(&itlock)) {
		EntityId lockje = itlock.entity_id;
		assert(lockje != NO_ENTITY);
		ActivationComponent* aanmaken = get_component(engine, lockje, COMP_ACTIVATION);
		//naar deur toe stream
		if (aanmaken->currenttime == 100) {
			int uit = 0;
			//teller die volgt tot aan een bepaald getal, bepaald onze snelheid
			if (aanmaken->getto == aanmaken->currenttime) {
				uit = 1;
				//rekening houden met een and or
				if (has_component(engine, lockje, COMP_CONNOR)) {
					ConnectorOr* x = get_component(engine, lockje, COMP_CONNOR);
					if (x->current >= x->needed) {
						ActivatableComponent* licht = get_component(engine, lockje, COMP_ACTIVATABLE);
						licht->active = 1;
						ConnectionsComponent* nieuwpath = get_component(engine, lockje, COMP_CONNECTIONS);
						EntityId volgende = nieuwpath->next;
						ActivationComponent* activatie = create_component(engine, volgende, COMP_ACTIVATION);
						activatie->getto = 0;
						activatie->currenttime = aanmaken->currenttime;
					}
					else {
					}
				}
				else {
					ActivatableComponent* licht = get_component(engine, lockje, COMP_ACTIVATABLE);
					licht->active = 1;
					ConnectionsComponent* nieuwpath = get_component(engine, lockje, COMP_CONNECTIONS);
					EntityId volgende = nieuwpath->next;
					ActivationComponent* activatie = create_component(engine, volgende, COMP_ACTIVATION);
					activatie->getto = 0;
					activatie->currenttime = aanmaken->currenttime;
				}
			}
			else {
				aanmaken->getto += 1;
			}

			if (uit == 1) {
				free_component(engine, lockje, COMP_ACTIVATION);
			}
		}
		else if (aanmaken->currenttime == 50) {
			int uit = 0;
			if (aanmaken->getto == aanmaken->currenttime) {
				uit = 1;
				ActivatableComponent* licht = get_component(engine, lockje, COMP_ACTIVATABLE);
				ConnectionsComponent* nieuwpath = get_component(engine, lockje, COMP_CONNECTIONS);
				licht->active = 0;
				ConnectionsComponent* nieuwpath = get_component(engine, lockje, COMP_CONNECTIONS);
				EntityId volgende = nieuwpath->prev;
				ActivationComponent* activatie = create_component(engine, volgende, COMP_ACTIVATION);
				activatie->getto = 0;
				activatie->currenttime = aanmaken->currenttime;
				free_component(engine, lockje, COMP_ACTIVATION);
			}
			else {
				aanmaken->getto += 1;
			}
		}
		else {
			//stel er loopt iets mis, kuist dit op
			EntityIterator itje;
			search_entity_2(engine, COMP_ACTIVATABLE, COMP_ACTIVATION, &itje);
			while (next_entity(&itje)) {
				EntityId lockje = itje.entity_id;
				assert(lockje != NO_ENTITY);
				ActivationComponent* aanmaken = get_component(engine, lockje, COMP_ACTIVATION);
				free_component(engine, lockje, COMP_ACTIVATION);

			}
		}
	}
}
