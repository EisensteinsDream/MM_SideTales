#ifndef MMS_MINIGAME0_H
#define MMS_MINIGAME0_H

#include "plex.h"

#include "mmcommon.h"

#ifdef __cplusplus
extern "C"
{
#endif
	extern bool MMS_beginMinigame0(const size_t chapter0spriteTex, const size_t pamaleanaSpriteTex, const uint64_t time);
	extern bool MMS_updateMinigame0(bool* victory, const uint64_t time);
	extern void MMS_endMinigame0();

	extern bool MMS_isMinigame0Running();

	extern void MMS_runMinigame0(const size_t chapter0spriteTex, const size_t pamaleanaSpriteTex);

	extern bool MMS_MMS1Unlocked();
#ifdef __cplusplus
}
#endif

#endif
