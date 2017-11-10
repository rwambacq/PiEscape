#ifndef PIESCAPE2_COMPONENTS_H
#define PIESCAPE2_COMPONENTS_H
#define TO_IMPLEMENT_STRUCT char c


#include "entity.h"
#include "component_enums.h"
#include "es_memory_manager_collections.h"

#include "../level/levelloader.h"
#include "game_util.h"
#include "../../util/util.h"

#include <stdint.h>
#include <glmc.h>

typedef struct CameraLookFromComponent {
    t_vec3 pos;
    
    float distance;
    float XYdegees;
    float Zdegrees;
} CameraLookFromComponent;

typedef struct CameraLookAtComponent {
    t_vec3 pos;
    
    uint64_t last_update;
} CameraLookAtComponent;

typedef struct MoveActionComponent {
	int x_plus_move;
	int x_min_move;
	int y_plus_move;
	int y_min_move;
} MoveActionComponent;

typedef struct IsDoorComponent{
	int x;
} IsDoorComponent;

typedef struct IsConnectorComponent {
	int x;
} IsConnectorComponent;


typedef struct GridLocationComponent {
    t_ivec2 pos;
} GridLocationComponent;

typedef struct OneTimeAnimationComponent {
    //TODO
	float progress;
} OneTimeAnimationComponent;

typedef struct MoveAnimationComponent {
    //TODO
	float progress;
} MoveAnimationComponent;

typedef struct ConnectorOr {
	int needed;
	int current;
}ConnectorOr;

typedef struct WalkComponent {
    //TODO	
	TO_IMPLEMENT_STRUCT;
} WalkComponent;

typedef struct WallArtComponent {
    int has_ceil;
    int has_floor;
    int has_wall[4]; //indexed by Direction
} WallArtComponent;

typedef struct BlockingComponent {
    //TODO
	TO_IMPLEMENT_STRUCT;
} BlockingComponent;

typedef struct ItemComponent {
    ItemColor color;
    //TODO
} ItemComponent;

typedef struct LockDoorComponent {
	EntityId door;
	//TODO
} LockDoorComponent;


typedef struct InContainerComponent {
	int previous_location_x;
	int previous_location_y;
	GridLocationComponent* entity_location;
} InContainerComponent;

typedef struct ContainerComponent {
	int contains_something;
	EntityId id;
} ContainerComponent;

typedef struct ActivationComponent {
	int currenttime;
	int getto;
} ActivationComponent;

typedef struct ActivatableComponent {
    int active;
} ActivatableComponent;

typedef struct ConnectionsComponent {
	EntityId prev;
	EntityId next;
} ConnectionsComponent;

typedef struct LockComponent {
    ItemColor requiredKeyColor;
} LockComponent;

typedef struct AndOrIn {
	int x;
}AndOrIn;

typedef struct ConnectorLogicComponent {
	EntityId andor;
	EntityId deelaanor;
} ConnectorLogicComponent;

typedef struct InputReceiverComponent {
    //TODO
	TO_IMPLEMENT_STRUCT;
} InputReceiverComponent;

typedef struct MoveHistoryComponent {
    //TODO
	TO_IMPLEMENT_STRUCT;
} MoveHistoryComponent;

typedef struct ItemActionComponent {
    //TODO
	TO_IMPLEMENT_STRUCT;
} ItemActionComponent;

typedef struct DirectionComponent {
    Direction dir;
} DirectionComponent;

typedef struct ArtComponent {
    ArtType type;
} ArtComponent;

typedef struct ExitComponent {
	int done;
} ExitComponent;

#endif //PIESCAPE2_COMPONENTS_H
