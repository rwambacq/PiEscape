#ifndef ACTIVATION_SYSTEM_H
#define ACTIVATION_SYSTEM_H
#define EMPTY_SYSTEM_STRUCT char c


typedef struct ActivationSystem {
	EMPTY_SYSTEM_STRUCT;
} ActivationSystem;

#include "../engine.h"

void system_activation_init(ActivationSystem*);
ActivationSystem* system_activation_alloc();
void system_activation_update(ActivationSystem*, Engine*);
void system_activation_free(ActivationSystem*);



#endif