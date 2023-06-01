#ifndef MMS_MINIGAME1_H
#define MMS_MINIGAME1_H

#include "plex.h"

#include "mmcommon.h"

#include "chapters/mms0.h"
#include "chapters/mms1.h"

#ifdef __cplusplus
extern "C"
{
#endif
	extern bool MMS_beginMinigame1(const size_t chapter1SpriteTex, const size_t pamaleanaSpriteTex, const size_t venariusSpriteTex, const size_t extraplocanSpriteTex, const uint64_t time);
	extern bool MMS_updateMinigame1(const size_t fontPlate, const uint64_t time);
	extern void MMS_endMinigame1();

	extern bool MMS_isMinigame1Running();

	extern void MMS_runMinigame1(const size_t chapter1SpriteTex, const size_t pamaleanaSpriteTex, const size_t cubicleBackdrop, const size_t venariusSpriteTex, const size_t extraplocanSpriteTex, const size_t fontPlate, const size_t label, const size_t bubble, const size_t arrow);
#ifdef __cplusplus
}
#endif

#endif
