#ifndef ORIENTATION_SYSTEM_H
#define ORIENTATION_SYSTEM_H

#include <stdint.h>

typedef struct OrientationSystem {
    //TODO
} OrientationSystem;

#include "../engine.h"

void system_orientation_init(OrientationSystem*);
OrientationSystem* system_orientation_alloc();
void system_orientation_update(OrientationSystem*, Engine*);
void system_orientation_free(OrientationSystem*);

#endif //ORIENTATION_SYSTEM_H
