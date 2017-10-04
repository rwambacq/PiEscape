#ifndef LOCK_SYSTEM_H
#define LOCK_SYSTEM_H
#define EMPTY_SYSTEM_STRUCT char c


typedef struct LockSystem {
	EMPTY_SYSTEM_STRUCT;
} LockSystem;

#include "../engine.h"

#define DOWNSTREAM_ACTIVATION_DELAY_MS 100l
#define DOWNSTREAM_DEACTIVATION_DELAY_MS 0l

void system_lock_init(LockSystem*);
LockSystem* system_lock_alloc();
void system_lock_update(LockSystem*, Engine*);
void system_lock_free(LockSystem*);



#endif //LOCK_SYSTEM_H