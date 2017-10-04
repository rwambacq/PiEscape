#include "context.h"
#include "../../util/rgb_triple.h"

#include <stdlib.h>

void context_init(Context* c) {
    c->is_exit_game = 0;
    c->demo = 0;
    c->time = 0;
}

Context* context_alloc() {
    Context* res = malloc(sizeof(Context));
    context_init(res);
    return res;
}
void context_free(Context* c) {

}