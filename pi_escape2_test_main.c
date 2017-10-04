#define __STDC_FORMAT_MACROS
#include <inttypes.h>
#include <stdio.h>

#include "util/sleep.h"
#include "pi_escape/graphics/opengl_game_renderer.h"
#include "pi_escape/level/levelloader.h"
#include "pi_escape/es/game.h"
#include "util/min_unit.h"

#include <SDL.h>
#undef main //Weird bug on windows where SDL overwrite main definition
#include <SDL_timer.h>

static char * test_memory_manager_create_component() {
    Graphics* g = graphics_alloc(0,0);
    Game* pi_escape_2 = game_alloc(g);
    EntityId entity = get_new_entity_id(&pi_escape_2->engine);
    create_component(&pi_escape_2->engine, entity, COMP_ITEM);
    mu_assert(has_component(&pi_escape_2->engine, entity, COMP_ITEM));
    game_free(pi_escape_2);
    free(pi_escape_2);
    graphics_free(g);
    free(g);
    return 0;
}

static char * all_tests() {
    //Basic memory manager test
    mu_run_test(test_memory_manager_create_component);

    return 0;
}


int main() {
    printf("Running tests:\n\n");
    char *result = all_tests();
    if (result != 0) {
        printf("%s\n", result);
        printf("\nAT LEAST ONE TEST FAILED\n");
    }
    else {
        printf("\nALL TESTS PASSED\n");
    }
    printf("Tests run: %d\n", mu_tests_run);

#ifdef _WIN32
    system("pause");
#endif

    return result != 0;
}
