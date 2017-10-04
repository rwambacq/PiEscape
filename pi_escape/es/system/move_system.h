#ifndef MOVE_SYSTEM_H
#define MOVE_SYSTEM_H
#define EMPTY_SYSTEM_STRUCT char c


typedef struct MoveSystem {
	EMPTY_SYSTEM_STRUCT;
} MoveSystem;

#include "../engine.h"

void system_move_init(MoveSystem*);
MoveSystem* system_move_alloc();
void system_move_update(MoveSystem*, Engine*);
void system_move_free(MoveSystem*);

#endif //MOVE_SYSTEM_H
