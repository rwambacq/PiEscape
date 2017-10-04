#ifndef ACTION_SYSTEM_H
#define ACTION_SYSTEM_H
#define EMPTY_SYSTEM_STRUCT char c

typedef struct ActionSystem {
	EMPTY_SYSTEM_STRUCT;
} ActionSystem;

#include "../engine.h"

void system_action_init(ActionSystem*);
ActionSystem* system_action_alloc();
void system_action_update(ActionSystem*, Engine*);
void system_action_free(ActionSystem*);



#endif
