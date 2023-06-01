#ifndef MMS_MMS0_H
#define MMS_MMS0_H

#include "game.h"

#ifdef __cplusplus
extern "C"
{
#endif
	extern void MMS_narration(const char* message);

	extern void MMS_createSpritePamaleana(const size_t pamaleanaSpriteTex);
	extern void MMS_createActorPamaleana(const uint32_t x, const uint32_t y, const bool hidden);
	extern void MMS_addPamaleanaAnimation(const uint64_t duration);
	extern PLEX_ANIMATION* MMS_getPamaleanaAnimation(const size_t index);

	extern void MMS_pamTalkWait(const char* message, const uint64_t wait, const bool stopMusic);
	extern void MMS_pamTalk(const char* message);
	extern void MMS_pamNarration(const char* message);

	extern void MMS_pamChangeAnimation(const size_t index);
	extern void MMS_pamHide();
	extern void MMS_pamUnhide();

	extern void MMS_createSpriteLevine(const size_t levineSpriteTex);
	extern void MMS_createActorLevine(const uint32_t x, const uint32_t y, const bool hidden);
	extern void MMS_addLevineAnimation(const uint64_t duration);
	extern PLEX_ANIMATION* MMS_getLevineAnimation(const size_t index);

	extern void MMS_levineTalkWait(const char* message, const uint64_t wait, const bool stopMusic);
	extern void MMS_levineTalk(const char* message);
	extern void MMS_levineNarration(const char* message);

	extern void MMS_levineChangeAnimation(const size_t index);
	extern void MMS_levineHide();
	extern void MMS_levineUnhide();

	extern void MMS_extrapTalk(const char* message);
	extern void MMS_extrapNarration(const char* message);

	extern void MMS_bossTalk(const char* message);

	extern void MMS_startPamaleanasCubicle(const size_t pamaleanaSpriteTex, const size_t cubicleBackdrop, const size_t venariusSpriteTex, const size_t extraplocanSpriteTex, const size_t fontPlate, const size_t label, const size_t bubble, const size_t arrow);

	extern void MMS0_startChapter();
	extern bool MMS0_updateChapter(const size_t pamaleanaSpriteTex, const size_t levineSpriteTex, const size_t cubicleBackdrop, const size_t venariusSpriteTex, const size_t extraplocanSpriteTex, const size_t fontPlate, const size_t label, const size_t bubble, const size_t arrow);
	extern void MMS0_endChapter();
#ifdef __cplusplus
}
#endif

#endif
