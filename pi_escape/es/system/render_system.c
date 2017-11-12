#include "render_system.h"
#include "../../../util/util.h"
#include "../../graphics/gl_glyph.h"
#include "../../../util/rgb_triple.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <math.h>

RenderSystem* system_render_alloc(Graphics* graphics) {
    RenderSystem* res = calloc(1, sizeof(RenderSystem));
    system_render_init(res, graphics);
    return res;
}

void system_render_init(RenderSystem* render_system, Graphics* graphics) {
    render_system->graphics = graphics;
    
    gl_key_init(&render_system->key, graphics);
    gl_floor_init(&render_system->floor, graphics);
    gl_wall_init(&render_system->wall[N], graphics, N);
    gl_wall_init(&render_system->wall[S], graphics, S);
    gl_wall_init(&render_system->wall[E], graphics, E);
    gl_wall_init(&render_system->wall[W], graphics, W);
    gl_player_init(&render_system->player, graphics);
    gl_lock_init(&render_system->lock, graphics);
    gl_door_init(&render_system->door[X], graphics, X);
    gl_door_init(&render_system->door[Y], graphics, Y);
    gl_connector_init(&render_system->connector[N], graphics, N);
    gl_connector_init(&render_system->connector[S], graphics, S);
    gl_connector_init(&render_system->connector[E], graphics, E);
    gl_connector_init(&render_system->connector[W], graphics, W);
    gl_connector_center_init(&render_system->connector_center[AND], graphics, AND);
    gl_connector_center_init(&render_system->connector_center[OR], graphics, OR);
}

void system_render_free(RenderSystem* render_system) {
    gl_key_free(&render_system->key);
    gl_floor_free(&render_system->floor);
    gl_wall_free(&render_system->wall[N]);
    gl_wall_free(&render_system->wall[S]);
    gl_wall_free(&render_system->wall[E]);
    gl_wall_free(&render_system->wall[W]);
    gl_player_free(&render_system->player);
    gl_lock_free(&render_system->lock);
    gl_door_free(&render_system->door[X]);
    gl_door_free(&render_system->door[Y]);
    gl_connector_free(&render_system->connector[N]);
    gl_connector_free(&render_system->connector[S]);
    gl_connector_free(&render_system->connector[E]);
    gl_connector_free(&render_system->connector[W]);
}

static void set_camera(RenderSystem* system, Engine* engine) {
    CameraLookFromComponent* cameraLookFrom = search_first_component(engine, COMP_CAMERA_LOOK_FROM);
    CameraLookAtComponent* cameraLookAt = search_first_component(engine, COMP_CAMERA_LOOK_AT);
    if (cameraLookFrom != NULL && cameraLookAt != NULL) {
        t_vec3 upDown = { 0, 1, 0 };
        
        glmc_assign_vec3(system->graphics->cameraLookAt, cameraLookAt->pos);
        glmc_assign_vec3(system->graphics->cameraLookFrom, cameraLookFrom->pos);
        glmc_assign_vec3(system->graphics->cameraUp, upDown);
    } else {
        fprintf(stderr, "Camera not found\n");
        t_vec3 lookFrom = { 0.0f, -10.0f, 30.0f };
        t_vec3 lookAt = { 5.0f, 5.0f, 5.0f };
        t_vec3 upDown = { 0, 1, 0 };
        glmc_assign_vec3(system->graphics->cameraLookAt, lookAt);
        glmc_assign_vec3(system->graphics->cameraLookFrom, lookFrom);
        glmc_assign_vec3(system->graphics->cameraUp, upDown);
    }
}

GameColor convert_key_color(ItemColor itemColor) {
    switch (itemColor) {
        case A: return BLUE;
        case B: return GREEN;
        case C: return RED;
        case O: return KEY_ALL;
        default: fatal("Unknown color");
    }
    //unreachable
    return KEY_ALL;
}

void system_render_update(RenderSystem* system, Engine* engine) {
    set_camera(system, engine);
    
    uint64_t time = engine->context.time;

    system->graphics->background_color[0] = (float)engine->process_sensor_system->temperatureColor->rgbRed/255;
    system->graphics->background_color[1] = (float)engine->process_sensor_system->temperatureColor->rgbGreen/255;
    system->graphics->background_color[2] = (float)engine->process_sensor_system->temperatureColor->rgbBlue/255;

    t_vec4 wall_color;
    wall_color[0] = (float)engine->process_sensor_system->humidityColor->rgbRed / 255;
    wall_color[1] = (float)engine->process_sensor_system->humidityColor->rgbGreen / 255;
    wall_color[2] = (float)engine->process_sensor_system->humidityColor->rgbBlue / 255;
    wall_color[3] = 1.0f;

    t_vec4 floor_color;
    floor_color[0] = (float)engine->process_sensor_system->airPressureColor->rgbRed / 255;
    floor_color[1] = (float)engine->process_sensor_system->airPressureColor->rgbGreen / 255;
    floor_color[2] = (float)engine->process_sensor_system->airPressureColor->rgbBlue/255;
    floor_color[3] = 1.0f;


    EntityIterator player_it;
    search_entity_2(engine, COMP_GRIDLOCATION, COMP_INPUTRECEIVER, &player_it);
    next_entity(&player_it);
    EntityId player_entity_id = player_it.entity_id;
    assert(player_entity_id != NO_ENTITY);
    
    t_ivec2 player_grid_pos;
    t_vec3 player_gl_pos;
    GridLocationComponent* player_grid_comp = get_component(engine, player_entity_id, COMP_GRIDLOCATION);
    glmc_assign_ivec2(player_grid_pos, player_grid_comp->pos);
    
    level_pos_to_world_pos2i(player_grid_pos, player_gl_pos);

    float lightPower = 21.0f + (10.0f * sinf((M_PI_F / 2000.0f) * time));
    t_vec3 light_gl_pos = { 2.0f, 2.0f, 1.0f };
    if (player_grid_comp != NULL) {
        glmc_assign_vec3(light_gl_pos, player_gl_pos);
    }
    
    graphics_begin_draw_withlight(system->graphics, lightPower, light_gl_pos);
    
    EntityIterator draw_it;
    search_entity_2(engine, COMP_GRIDLOCATION, COMP_ART, &draw_it);
    
    while(next_entity(&draw_it)) {
        EntityId drawable_entity_id = draw_it.entity_id;
        assert(drawable_entity_id != NO_ENTITY);
        
        GridLocationComponent* grid = get_component(engine, drawable_entity_id, COMP_GRIDLOCATION);
        ArtComponent* art = get_component(engine, drawable_entity_id, COMP_ART);
        
        int at_player_location = glmc_equal_ivec2(grid->pos, player_grid_pos);
        
        switch (art->type) {
            case ART_WALL: {
                WallArtComponent *wallInfo = get_component(engine, drawable_entity_id, COMP_WALLART);
                assert(wallInfo != NULL);
                if (wallInfo != NULL) {
                    for (Direction dir = DIRECTION_FIRST; dir <= DIRECTION_LAST; dir++) {
                        if (wallInfo->has_wall[dir]) {
                            gl_wall_draw_color(&system->wall[dir], grid->pos, wall_color);
                        }
                    }
                    
                    if (wallInfo->has_floor) {
                        if (at_player_location) {
                            gl_floor_draw_color(&system->floor, grid->pos, 0, PLAYER_TOUCH, 0.0f, floor_color);
                        } else {
                            gl_floor_draw_color(&system->floor, grid->pos, 0, NONE, 0.0f, floor_color);
                        }
                    }
                    if (wallInfo->has_ceil) {
                        gl_floor_draw_color(&system->floor, grid->pos, 1, NONE, 0.0f, floor_color);
                    }
                }
                break;
            }
            case ART_END: {
                float exit_anim_pos = 0.0f; //TODO
    
                gl_floor_draw_color(&system->floor, grid->pos, 0, EXIT_TOUCH, exit_anim_pos, floor_color);
                
                break;
            }
            case ART_PLAYER: {
                t_vec2 pos;
                glmc_assign_vec2_from_ivec2(pos, grid->pos);
                gl_player_draw(&system->player, time, pos);
                break;
            }
            case ART_KEY: {
                ItemComponent* item = get_component(engine, drawable_entity_id, COMP_ITEM);
                InContainerComponent* container = get_component(engine, drawable_entity_id, COMP_INCONTAINER);
                assert(item != NULL);

                t_vec2 pos;
                glmc_assign_vec2_from_ivec2(pos, grid->pos);
                
                gl_key_draw(&system->key, time, pos, container != NULL, convert_key_color(item->color));
                break;
            }
            case ART_DOOR: {
                DirectionComponent* dir = get_component(engine, drawable_entity_id, COMP_DIRECTION);
                ActivatableComponent* act = get_component(engine, drawable_entity_id, COMP_ACTIVATABLE);
                assert(dir != NULL);
                assert(act != NULL);
                
                if (!act->active) {
                    gl_door_draw(&system->door[dir->dir == N || dir->dir == S ? Y : X],
                                 time, grid->pos, BLUE);
                }
                gl_floor_draw_color(&system->floor, grid->pos, 0, act->active ? ACTIVE : INACTIVE, 0.0f, floor_color);
                break;
            }
            case ART_LOCK: {
                LockComponent* lock = get_component(engine, drawable_entity_id, COMP_LOCK);
                ActivatableComponent* act = get_component(engine, drawable_entity_id, COMP_ACTIVATABLE);
                assert(lock != NULL);
                assert(act != NULL);
                
                gl_lock_draw(&system->lock, time, grid->pos,
                             convert_key_color(lock->requiredKeyColor), act->active);
                break;
            }
            case ART_CONNECTOR: {
                DirectionComponent* dir = get_component(engine, drawable_entity_id, COMP_DIRECTION);
                ActivatableComponent* act = get_component(engine, drawable_entity_id, COMP_ACTIVATABLE);
                assert(dir != NULL);
                assert(act != NULL);
                
                gl_connector_draw(&system->connector[dir->dir], time,
                                  grid->pos, act->active);
                break;
            }
            case ART_CONNECTOR_AND: {
                ActivatableComponent* act = get_component(engine, drawable_entity_id, COMP_ACTIVATABLE);
                assert(act != NULL);
                
                gl_connector_center_draw(&system->connector_center[AND], time,
                                  grid->pos, act->active);
                break;
            }
            case ART_CONNECTOR_OR: {
                ActivatableComponent* act = get_component(engine, drawable_entity_id, COMP_ACTIVATABLE);
                assert(act != NULL);
                
                gl_connector_center_draw(&system->connector_center[OR], time,
                                  grid->pos, act->active);
                break;
            }
            default: {
                fatal("Unsupported art: %d\n", art->type);
                //ignore unsupported ART
            }
        }
    }
    
    graphics_end_draw(system->graphics);
}
