#ifndef MMS_GAME_H
#define MMS_GAME_H

#include "plex.h"

#include "mmcommon.h"

#include "meta/mms_language.h"
#include "meta/mms_filepaths.h"

#include "minigame/minigame0.h"
#include "minigame/minigame1.h"

#ifdef __cplusplus
extern "C"
{
#endif
	extern bool MMS_startGame();
	extern bool MMS_updateGame();
	extern void MMS_endGame();
#ifdef __cplusplus
}
#endif

#endif
