#include "es_memory_manager_collections.h"
#include "es_memory_manager.h"
#include "engine.h"

#include <stdlib.h>
#include <assert.h>

void search_component(Engine* engine,
                      ComponentId component_id,
                      ComponentIterator* res) {
	if (logging_benchmark) {
		fprintf(benchfile, "search_component(Engine*,%d,ComponentIterator*)\n", component_id);
	}
    res->engine = engine;
    res->component_id = component_id;
    res->entity_id = (EntityId) -1; //bit of a hack
}

// updates given iterator to hold new information about next component found
int next_component(ComponentIterator* res) {
	if (logging_benchmark) {
		fprintf(benchfile, "next_component(ComponentIterator*)\n");
	}
//    for (EntityId entity_id = res->entity_id+1; entity_id < MAX_ENTITIES; entity_id++) {
    for (EntityId entity_id = res->entity_id+1; entity_id < res->engine->es_memory.next_entity_id; entity_id++) {
        if (!res->engine->es_memory.components[res->component_id][entity_id].free) {
            res->entity_id = entity_id;
            res->comp = &res->engine->es_memory.components[res->component_id][entity_id].camera_lookfrom;
            return 1;
        }
    }
    return 0;
}

void* search_first_component(Engine* engine, ComponentId component_id) {
	if(logging_benchmark){
		fprintf(benchfile, "search_first_component(Engine*,%d)\n", component_id);
	}
    ComponentIterator it;
    search_component(engine, component_id, &it);
    if (next_component(&it))
        return it.comp;
    else
        return NULL;
}



static int get_requires_component(uint32_t component_id_filter, ComponentId component_id) {
    uint32_t mask = (uint32_t) (1 << component_id);
    return (mask & component_id_filter) != 0;
}
uint32_t set_requires_component(uint32_t component_id_filter, ComponentId component_id) {
    uint32_t mask = (uint32_t) (1 << component_id);
    return component_id_filter | mask;
}


void search_entity(Engine* engine,
                   uint32_t component_id_filter,
                   EntityIterator* res) {
	if (logging_benchmark) {
		fprintf(benchfile, "search_entity(Engine*,%u,EntityIterator*)\n", component_id_filter);
	}
    res->engine = engine;
    res->component_id_filter = component_id_filter;
    res->entity_id = (EntityId) -1; //bit of a hack
}

void search_entity_1(Engine* engine,
                     ComponentId component_id1,
                     EntityIterator* res) {
	if (logging_benchmark) {
		fprintf(benchfile, "search_entity_1(Engine*,%d,EntityIterator*)\n", component_id1);
	}
    uint32_t component_id_filter = set_requires_component(0, component_id1);
    search_entity(engine, component_id_filter, res);
}
void search_entity_2(Engine* engine,
                     ComponentId component_id1,
                     ComponentId component_id2,
                     EntityIterator* res) {
	if (logging_benchmark) {
		fprintf(benchfile, "search_entity_2(Engine*,%d,%d,EntityIterator*)\n", component_id1, component_id2);
	}
    uint32_t component_id_filter = set_requires_component(0, component_id1);
    component_id_filter = set_requires_component(component_id_filter, component_id2);
    search_entity(engine, component_id_filter, res);
}
void search_entity_3(Engine* engine,
                     ComponentId component_id1,
                     ComponentId component_id2,
                     ComponentId component_id3,
                     EntityIterator* res) {
	if (logging_benchmark) {
		fprintf(benchfile, "search_entity_3(Engine*,%d,%d,%d,EntityIterator*)\n", component_id1, component_id2, component_id3);
	}
    uint32_t component_id_filter = set_requires_component(0, component_id1);
    component_id_filter = set_requires_component(component_id_filter, component_id2);
    component_id_filter = set_requires_component(component_id_filter, component_id3);
    search_entity(engine, component_id_filter, res);
}

EntityId search_first_entity_with_mask(Engine *engine, uint32_t component_id_filter) {
	if (logging_benchmark) {
		fprintf(benchfile, "search_first_entity_with_mask(Engine*,%u)\n", component_id_filter);
	}
    EntityIterator it;
    search_entity(engine, component_id_filter, &it);
    if (next_entity(&it))
        return it.entity_id;
    else
        return NO_ENTITY;
}
EntityId search_first_entity_1(Engine* engine, ComponentId component_id1) {
	if (logging_benchmark) {
		fprintf(benchfile, "search_first_entity_1(Engine*,%d)\n", component_id1);
	}
    uint32_t component_id_filter = set_requires_component(0, component_id1);
    return search_first_entity_with_mask(engine, component_id_filter);
}
EntityId search_first_entity_2(Engine* engine, ComponentId component_id1, ComponentId component_id2) {
	if (logging_benchmark) {
		fprintf(benchfile, "search_first_entity_2(Engine*,%d,%d)\n", component_id1, component_id2);
	}
    uint32_t component_id_filter = set_requires_component(0, component_id1);
    component_id_filter = set_requires_component(component_id_filter, component_id2);
    return search_first_entity_with_mask(engine, component_id_filter);
}
EntityId search_first_entity_3(Engine* engine, ComponentId component_id1, ComponentId component_id2, ComponentId component_id3) {
	if (logging_benchmark) {
		fprintf(benchfile, "search_first_entity_3(Engine*,%d,%d,%d)\n", component_id1, component_id2, component_id3);
	}
    uint32_t component_id_filter = set_requires_component(0, component_id1);
    component_id_filter = set_requires_component(component_id_filter, component_id2);
    component_id_filter = set_requires_component(component_id_filter, component_id3);
    return search_first_entity_with_mask(engine, component_id_filter);
}

int next_entity(EntityIterator* res) {
	if (logging_benchmark) {
		fprintf(benchfile, "next_entity(EntityIterator*)\n");
	}
    for (EntityId entity_id = res->entity_id+1; entity_id < res->engine->es_memory.next_entity_id; entity_id++) {
        if (res->component_id_filter != 0) {
            for (ComponentId component_id = 0; component_id < COMPONENT_ID_SIZE; component_id++) {
                if (get_requires_component(res->component_id_filter, component_id) &&
                    res->engine->es_memory.components[component_id][entity_id].free) {
                    //no match. Try the next entity
                    goto next_entity_loop;
                }
            }
        }
        //all match, return it
        res->entity_id = entity_id;
        return 1;

        next_entity_loop: ;
    }
    return 0;
}

void entitylist_init(int initial_size, EntityList* dest) {
	if (logging_benchmark) {
		fprintf(benchfile, "entitylist_init(%d,EntityList*)\n", initial_size);
	}
    dest->allocated = initial_size < 16 ? 16 : initial_size;
    dest->entity_ids = malloc(dest->allocated * sizeof(EntityId));
    dest->count = 0;
    assert(dest->entity_ids != NULL);
}
void entitylist_free(EntityList* dest) {
	if (logging_benchmark) {
		fprintf(benchfile, "entitylist_free(EntityList*)\n");
	}
    free(dest->entity_ids);
    dest->count = 0;
    dest->allocated = 0;
    dest->entity_ids = NULL;
}

void entitylist_add(EntityList* dest, EntityId entity_id) {
	if (logging_benchmark) {
		fprintf(benchfile, "entitylist_add(EntityList*,%d)\n", entity_id);
	}
    if (dest->count + 1 == dest->allocated) {
        int new_size = dest->allocated > 0 ? dest->allocated * 2 : 16;
        dest->entity_ids = realloc(dest->entity_ids, new_size * sizeof(EntityId));
        dest->allocated = new_size;
    }
    dest->entity_ids[dest->count++] = entity_id;
}



void entityqueue_init(EntityQueue* queue, EntityId first_entity_id) {
	if (logging_benchmark) {
		fprintf(benchfile, "entityqueue_init(EntityQueue*,%d)\n", first_entity_id);
	}
    queue->front = malloc(sizeof(EntityQueueLink));
    queue->front->entity_id = first_entity_id;
    queue->front->next = NULL;
    queue->front->prev = NULL;
    queue->back = queue->front;
    queue->size = 1;
}
void entityqueue_free(EntityQueue* queue) {
	if (logging_benchmark) {
		fprintf(benchfile, "entityqueue_free(EntityQueue*)\n");
	}
    while (!entityqueue_is_empty(queue)) {
        entityqueue_pop_front(queue);
    }
}
int entityqueue_is_empty(EntityQueue* queue) {
	if (logging_benchmark) {
		fprintf(benchfile, "entity_is_empty(EntityQueue*)\n");
	}
    return queue->size == 0;
}
void entityqueue_push_back(EntityQueue* queue, EntityId entity_id) {
	if (logging_benchmark) {
		fprintf(benchfile, "entityqueue_push_back(EntityQueue*,%d)\n", entity_id);
	}
    EntityQueueLink* extra = malloc(sizeof(EntityQueueLink));
    extra->entity_id = entity_id;
    extra->next = NULL;
    extra->prev = queue->back;
    if (queue->back != NULL)
        queue->back->next = extra;
    else
        queue->front = extra;
    queue->back = extra;
    queue->size++;
}
EntityId entityqueue_pop_front(EntityQueue* queue) {
	if (logging_benchmark) {
		fprintf(benchfile, "entity_pop_front(EntityQueue*)\n");
	}
    if (queue->size < 0)
        fatal("Can't pop empty queue");
    EntityQueueLink* orig_front = queue->front;
    queue->front = orig_front->next;
    if (queue->front == NULL)
        queue->back = NULL;
    else
        queue->front->prev = NULL;
    EntityId res = orig_front->entity_id;
    free(orig_front);
    queue->size--;
    return res;
}
