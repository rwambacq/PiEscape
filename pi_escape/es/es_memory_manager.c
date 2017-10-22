#define BENCH
#define BENCH_RUN
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


void es_memory_manager_init(ESMemory* mem) {
	if (logging_benchmark) {
		fprintf(benchfile,
			"es_memory_manager_init(ESMemory*)\n");
	}

    mem->next_entity_id = 0;
    
    for (ComponentId component_id = (ComponentId) 0; component_id < COMPONENT_ID_SIZE; component_id++) {
        for (EntityId entity_id = 0; entity_id < MAX_ENTITIES; entity_id++) {
            memset(&mem->components[component_id][entity_id], 0, sizeof(AllComponent));
            mem->components[component_id][entity_id].free = 1;
        }
    }
}

void es_memory_manager_free(ESMemory* mem) {
    //free any component that needs freeing. (most don't)
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
	/*if (logging_benchmark) {
		fprintf(benchfile,
		"get_component(Engine*,%d,%d)\n",
			entity_id, component_id);
	}*/
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
    assert(engine->es_memory.components[component_id][entity_id].free);
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

EntityId get_new_entity_id(Engine* engine) {
	if (logging_benchmark) {
		fprintf(benchfile,
			"get_new_entity_id(Engine*)\n");
	}

    if (engine->es_memory.next_entity_id == MAX_ENTITIES) {
        fatal("Fatal error: Maximum number of entities used: %u", MAX_ENTITIES);
    }
    return engine->es_memory.next_entity_id++;
}
