#ifndef MMS2_STRUCT_H
#define MMS2_STRUCT_H

#include "minigame/minigame2/meta.h"

// PLAYER

typedef enum
{
	MMS_MINIGAME2DODGEPHASE_STAND = 0,
	MMS_MINIGAME2DODGEPHASE_CROUCH = 1,
	MMS_MINIGAME2DODGEPHASE_LEANLEFT = 2,
	MMS_MINIGAME2DODGEPHASE_LEANRIGHT = 3,
	MMS_MINIGAME2DODGEPHASE_CROUCHLEFT = 4,
	MMS_MINIGAME2DODGEPHASE_CROUCHRIGHT = 5
} MMS_MINIGAME2DODGEPHASE;

// ENEMY

typedef enum
{
	MMS_MINIGAME2ENEMYPHASE_NONE = 0,
	MMS_MINIGAME2ENEMYPHASE_STANDING = 1,
	MMS_MINIGAME2ENEMYPHASE_CROUCHING = 2,
	MMS_MINIGAME2ENEMYPHASE_BEINGDAMAGED = 3,
	MMS_MINIGAME2ENEMYPHASE_MOVERIGHT = 4,
	MMS_MINIGAME2ENEMYPHASE_MOVELEFT = 5,
	MMS_MINIGAME2ENEMYPHASE_MOVEUP = 6,
	MMS_MINIGAME2ENEMYPHASE_MOVEDOWN = 7,
	MMS_MINIGAME2ENEMYPHASE_ROLLRIGHT = 8,
	MMS_MINIGAME2ENEMYPHASE_ROLLLEFT = 9,
	MMS_MINIGAME2ENEMYPHASE_DIVERIGHT = 10,
	MMS_MINIGAME2ENEMYPHASE_DIVELEFT = 11,
	MMS_MINIGAME2ENEMYPHASE_DEAD = 12,
	MMS_MINIGAME2ENEMYPHASE_NODROPIPNGTREASURE = 13,
	MMS_MINIGAME2ENEMYPHASE_DROPPINGTREASURE = 14,
	MMS_MINIGAME2ENEMYPHASE_RELOADING = 15
} MMS_MINIGAME2ENEMYPHASE;

typedef enum
{
	MMS_MINIGAME2MOVEPATTERN_NOMOVE = 0,
	MMS_MINIGAME2MOVEPATTERN_DRIFTLEFT = 1,
	MMS_MINIGAME2MOVEPATTERN_DRIFTRIGHT = 2,
	MMS_MINIGAME2MOVEPATTERN_BIRDFLIGHTLEFT = 3,
	MMS_MINIGAME2MOVEPATTERN_BIRDFLIGHTRIGHT = 4,
	MMS_MINIGAME2MOVEPATTERN_SNAKESLITHER = 5,
	MMS_MINIGAME2MOVEPATTERN_ASTROIDFALL = 6
} MMS_MINIGAME2MOVEPATTERN;

// CALM AREA

struct MMS_MINIGAME2NPC;
struct MMS_MINIGAME2NPCSET;

// PLAYER

struct MMS_MINIGAME2GUN;

// ENEMY

struct MMS_MINIGAME2ENEMY;
struct MMS_MINIGAME2ENEMYGENERATOR;

// COVER

struct MMS_MINIGAME2COVER;
struct MMS_MINIGAME2COVERGENERATOR;

// TREASURE

struct MMS_MINIGAME2TREASUREBOX;

// ROOM

struct MMS_MINIGAME2ROOMCONTENTS;
struct MMS_MINIGAME2PATH;
struct MMS_MINIGAME2ROOM;

#endif
