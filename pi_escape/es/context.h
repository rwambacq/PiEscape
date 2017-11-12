#ifndef CONTEXT_H
#define CONTEXT_H

#include <stdint.h>
#include "../../util/rgb_triple.h"

typedef struct Context {
    uint64_t time;
    int is_exit_game;
    int demo;
} Context;

void context_init(Context*);
Context* context_alloc();
void context_free(Context*);


#endif //CONTEXT_H