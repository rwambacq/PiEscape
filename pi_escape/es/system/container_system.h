#ifndef CONTAINER_SYSTEM_H
#define CONTAINER_SYSTEM_H
#define EMPTY_SYSTEM_STRUCT char c


typedef struct ContainerSystem {
	EMPTY_SYSTEM_STRUCT;
} ContainerSystem;

#include "../engine.h"

void system_container_init(ContainerSystem*);
ContainerSystem* system_container_alloc();
void system_container_update(ContainerSystem*, Engine*);
void system_container_free(ContainerSystem*);


#endif //CONTAINER_SYSTEM_H