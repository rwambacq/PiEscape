#include "lock_system.h"


#include <stdlib.h>
#include <stdio.h>

#include <assert.h>

LockSystem* system_lock_alloc() {
    LockSystem* res = calloc(1, sizeof(LockSystem));
    system_lock_init(res);
    return res;
}

void system_lock_init(LockSystem* system) {
    //TODO
}


void system_lock_free(LockSystem* system) {
    //TODO
}


void system_lock_update(LockSystem* system, Engine* engine) {
    //TODO
}
