#include "camera_system.h"


#include <stdlib.h>
#include <assert.h>

#include <stdio.h>

CameraSystem* system_camera_alloc() {
    CameraSystem* res = calloc(1, sizeof(CameraSystem));
    system_camera_init(res);
    return res;
}

void system_camera_init(CameraSystem* system) {
    //TODO
}


void system_camera_free(CameraSystem* system) {
    //TODO
}

void system_camera_update(CameraSystem* system, Engine* engine) {
    //TODO
}
