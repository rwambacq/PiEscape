#ifndef PIESCAPE2_COMPONENT_ENUMS_H
#define PIESCAPE2_COMPONENT_ENUMS_H

typedef enum ComponentId {
	COMP_CONNOR,
	COMP_ANDORIN,
	COMP_ISCONNECTOR,
	COMP_ISDOOR,
	COMP_GRIDLOCATION,
	COMP_MOVE_ANIMATION,
	COMP_ONE_TIME_ANIMATION,
	COMP_WALLART,
	COMP_WALKABLE,
	COMP_BLOCKING, //5
	COMP_ITEM,
	COMP_ACTIVATION,
	COMP_ACTIVATABLE,
	COMP_CONNECTIONS, //9
	COMP_LOCK,
	COMP_CONNECTORLOGIC,
	COMP_INPUTRECEIVER,
	COMP_MOVE_ACTION,
	COMP_MOVE_HISTORY,
	COMP_ITEMACTION, //15
	COMP_CAMERA_LOOK_AT,
	COMP_CAMERA_LOOK_FROM,
	COMP_ART,
	COMP_DIRECTION, //19
	COMP_CONTAINER,
	COMP_INCONTAINER,
	COMP_EXIT, //22
	COMP_LOCKDOOR,
    
    COMPONENT_ID_SIZE
} ComponentId;


typedef enum OneTimeAnimationType { ANIM_EXIT, ANIM_ERROR, ANIM_MOVE } OneTimeAnimationType;
typedef enum ArtType { ART_DOOR, ART_PLAYER, ART_END, ART_WALL, ART_LOCK, ART_KEY,
    ART_CONNECTOR, ART_CONNECTOR_AND, ART_CONNECTOR_OR } ArtType;
typedef enum ItemActionType { ACTION_PICKUP, ACTION_SWAP, ACTION_DROP } ItemActionType;

#endif //PIESCAPE2_COMPONENT_ENUMS_H
