#ifndef PLEX_SPRITETHEATER_H
#define PLEX_SPRITETHEATER_H

#include "windowing/animation.h"

#include "utility/timer.h"

#include "sound/sound.h"

// This unit contains 'sprite theater' which is like a simple setup for a diorama-like presentation for simple sprite-based cutscenese.

// NOTE: I'm working on this as it's needed to be worked on. It's not quite experimental as it's well tested but requires a lot of leg work to get off the ground.

#ifdef __cplusplus
extern "C"
{
#endif
	// creates an 'actor' for sprite theater based on sprite and space

	extern int64_t PLEX_createActor(const PLEX_SPRITE sprite, const PLEX_RECT space, const bool hidden);

	// adds a slide to the current sprite theater

	extern void PLEX_addSpriteTheaterSlide(const uint8_t r, const uint8_t g, const uint8_t b, const char* const message, const int64_t actor, const uint64_t waitTime, const bool pauseForWait);

	// sets sprite theater to the next slide

	extern void PLEX_nextSlide(const uint64_t time);

	// hides or unhides actor at index

	extern void PLEX_hideActorChange(const size_t actor);
	extern void PLEX_unhideActorChange(const size_t actor);

	// changes the animation of an actor at index

	extern void PLEX_animationActorChange(const size_t actor, const size_t index);

	// moves an actor at index

	extern void PLEX_animationActorMove(const size_t actor, const double x, const double y);

	// marks to change the background during whatever slide

	extern void PLEX_spriteTheaterBackgroundChange(const size_t backdrop, const char* const name);

	// allows some control over musical cues and sound effects. However, these only work in a customized musical system to work. If this sounds confusing check out the signals in sound.h and sound.c

	extern void PLEX_spriteTheaterMusicChange(const size_t ID);
	extern void PLEX_spriteTheaterMusicStop();
	extern void PLEX_spriteTheaterSoundEffect(const size_t ID);

	// begins, updates, and ends sprite theater. These are not the typical begin, update, and end functions. They are NOT managed by PLEX_start, PLEX_update, or PLEX_end because it start, updates, and ends as these are called.

	extern bool PLEX_beginSpriteTheater(const char* const stageName, const size_t backdrop, const uint32_t labelPlateX, const uint32_t labelPlateY, const uint32_t labelPlateW, const uint32_t labelPlateH, const uint8_t labelR, const uint8_t labelG, const uint8_t labelB, const size_t font, const size_t labelPlate, const size_t bubble, const size_t arrow, const uint64_t time);
	extern bool PLEX_updateSpriteTheater(const uint64_t time);
	extern void PLEX_endSpriteTheater();

	// returns whether or not it's in sprite theater

	extern bool PLEX_inSpriteTheater();

	// returns whether or not a sprite theater is over

	extern bool PLEX_isSpriteTheaterOver();
#ifdef __cplusplus
}
#endif

#endif
