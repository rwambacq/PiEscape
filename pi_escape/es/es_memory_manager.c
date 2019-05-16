#define BENCH
#include "es_memory_manager.h"

#ifndef TIME_INCLUDED
#include <time.h>
#define TIME_INCLUDED
#endif // !TIME_INCLUDED

#include <assert.h>

/*
When logging benchmarks, don't log actual pointer arguments in function calls, as they will be either Engine's or ESmemory's,
which will be assigned in a random block of memory time and time again when re-executing these benchmarks.
So, in short, only log actual arguments like Id's, in order to be able to replicate benchmarking situations.
*/

////OLD
//void es_memory_manager_init(ESMemory* mem) {
//	// will be executed once for sure, don't write call to this function away to benchlog file.
//
//    mem->next_entity_id = 0;
//    
//    for (ComponentId component_id = (ComponentId) 0; component_id < COMPONENT_ID_SIZE; component_id++) {
//        for (EntityId entity_id = 0; entity_id < MAX_ENTITIES; entity_id++) {
//            memset(&mem->components[component_id][entity_id], 0, sizeof(AllComponent));
//            mem->components[component_id][entity_id].free = 1;
//        }
//    }
//}

// NEW
void es_memory_manager_init(ESMemory* mem) {
	mem->next_entity_id = 0;
	mem->components = (AllComponent**)malloc(COMPONENT_ID_SIZE * sizeof(AllComponent*)); // allocate space for COMPONENT_ID_SIZE (23) AllComponent pointers
	curr_max_entities = START_AMOUNT_ENTITIES;
	for (ComponentId comp_id = (ComponentId)0; comp_id < COMPONENT_ID_SIZE; comp_id++) {
		mem->components[comp_id] = (AllComponent*)malloc(curr_max_entities * sizeof(AllComponent)); // allocate space for curr_max_entities (50-100-200-...) AllComponents
		for (EntityId ent_id = 0; ent_id < curr_max_entities; ent_id++) {
			memset(&mem->components[comp_id][ent_id], 0, sizeof(AllComponent));
			mem->components[comp_id][ent_id].free = 1;
		}
	}
}

void es_memory_manager_reinit(ESMemory* mem) {
	mem->next_entity_id = 0;
	mem->components = (AllComponent**)malloc(COMPONENT_ID_SIZE * sizeof(AllComponent*)); // allocate space for COMPONENT_ID_SIZE (23) AllComponent pointers
	for (ComponentId comp_id = (ComponentId)0; comp_id < COMPONENT_ID_SIZE; comp_id++) {
		mem->components[comp_id] = (AllComponent*)malloc(curr_max_entities * sizeof(AllComponent)); // allocate space for curr_max_entities (50-100-200-...) AllComponents
		for (EntityId ent_id = 0; ent_id < curr_max_entities; ent_id++) {
			memset(&mem->components[comp_id][ent_id], 0, sizeof(AllComponent));
			mem->components[comp_id][ent_id].free = 1;
		}
	}
}

void es_memory_manager_free(ESMemory* mem) {
	//free any component that needs freeing. (most don't)
	int i;
	for (i = 0; i < COMPONENT_ID_SIZE; i++) {
		free(mem->components[i]);
	}
	free(mem->components);
}

int has_component(Engine* engine, EntityId entity_id, ComponentId component_id) {
	if (logging_benchmark) {
		fprintf(benchfile,
			"has_component(Engine*,%d,%d)\n",
			entity_id, component_id);
	}
	fatal_if(entity_id == NO_ENTITY, "has_component(engine, entity_id==NO_ENTITY, component_id=%d)", component_id);
	assert(component_id < COMPONENT_ID_SIZE);
	assert(component_id >= 0);
	assert(entity_id < MAX_ENTITIES);
	assert(entity_id >= 0);
	return !engine->es_memory.components[component_id][entity_id].free;
}

void* get_component(Engine* engine, EntityId entity_id, ComponentId component_id) {
	if (logging_benchmark) {
		fprintf(benchfile,
			"get_component(Engine*,%d,%d)\n",
			entity_id, component_id);
	}
	fatal_if(entity_id == NO_ENTITY, "get_component(engine, entity_id==NO_ENTITY, component_id=%d)", component_id);
	assert(component_id < COMPONENT_ID_SIZE);
	assert(component_id >= 0);
	assert(entity_id < MAX_ENTITIES);
	assert(entity_id >= 0);
	if (engine->es_memory.components[component_id][entity_id].free)
		return NULL;
	assert(!engine->es_memory.components[component_id][entity_id].free);
	return &engine->es_memory.components[component_id][entity_id].camera_lookfrom;
}

void* create_component(Engine* engine, EntityId entity_id, ComponentId component_id) {
	if (logging_benchmark) {
		fprintf(benchfile,
			"create_component(Engine*,%d,%d)\n",
			entity_id, component_id);
	}

	fatal_if(entity_id == NO_ENTITY, "create_component(engine, entity_id==NO_ENTITY, component_id=%d)", component_id);
	assert(entity_id >= 0);
	assert(entity_id < MAX_ENTITIES);
	assert(component_id >= 0);
	assert(component_id < COMPONENT_ID_SIZE);
	engine->es_memory.components[component_id][entity_id].free = 0;
	return &engine->es_memory.components[component_id][entity_id].camera_lookfrom;
}

void free_component(Engine* engine, EntityId entity_id, ComponentId component_id) {
	if (logging_benchmark) {
		fprintf(benchfile,
			"free_component(Engine*,%d,%d)\n",
			entity_id, component_id);
	}

	fatal_if(entity_id == NO_ENTITY, "free_component(engine, entity_id==NO_ENTITY, component_id=%d)", component_id);
	assert(entity_id >= 0);
	assert(entity_id < MAX_ENTITIES);
	assert(component_id >= 0);
	assert(component_id < COMPONENT_ID_SIZE);

	assert(!engine->es_memory.components[component_id][entity_id].free);
	engine->es_memory.components[component_id][entity_id].free = 1;
}

void free_all_components(Engine* engine, ComponentId component_id) {
	assert(component_id >= 0);
	assert(component_id < COMPONENT_ID_SIZE);
	int i;
	for (i = 0; i < engine->es_memory.next_entity_id; i++) {
		if (!engine->es_memory.components[component_id][i].free) {
			engine->es_memory.components[component_id][i].free = 1;
		}
	}
}

////OLD
//EntityId get_new_entity_id(Engine* engine) {
//	if (logging_benchmark) {
//		fprintf(benchfile,
//			"get_new_entity_id(Engine*)\n");
//	}
//
//    if (engine->es_memory.next_entity_id == MAX_ENTITIES) {
//        fatal("Fatal error: Maximum number of entities used: %u", MAX_ENTITIES);
//    }
//    return engine->es_memory.next_entity_id++;
//}

//NEW
EntityId get_new_entity_id(Engine* engine) {
	if (logging_benchmark) {
		fprintf(benchfile,
			"get_new_entity_id(Engine*)\n");
	}

	if (engine->es_memory.next_entity_id == curr_max_entities) {
		printf("changing max_entities from %d to %d\n", curr_max_entities, curr_max_entities * 2);
		change_es_memory_size(engine, engine->es_memory.components);
	}
	return engine->es_memory.next_entity_id++;
}

void change_es_memory_size(Engine* engine, AllComponent** old_components_ptr) {
	curr_max_entities *= 2;
	int nex = engine->es_memory.next_entity_id;
	es_memory_manager_reinit(&engine->es_memory); // allocate new memory (in heap memory) for new bigger matrix (curr_max_entities has doubled)
	engine->es_memory.next_entity_id = nex; // reset to correct value, because a call to es_memory_manager_init makes next_entity_id equal 0 again.
											// copy all old AllComponents into new memory
	for (ComponentId comp_id = (ComponentId)0; comp_id < COMPONENT_ID_SIZE; comp_id++) {
		for (EntityId ent_id = 0; ent_id < curr_max_entities / 2; ent_id++) {
			engine->es_memory.components[comp_id][ent_id] = old_components_ptr[comp_id][ent_id];
		}
		free(old_components_ptr[comp_id]); // free the old row (in heap memory)
	}
	free(old_components_ptr); // free the array containing pointers to the rows (in heap memory)
}
