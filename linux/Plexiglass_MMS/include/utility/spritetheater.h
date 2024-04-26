#ifndef PLEX_SPRITETHEATER_H
#define PLEX_SPRITETHEATER_H

#include "windowing/animation.h"

#include "utility/timer.h"

#include "sound/sound.h"

// This unit contains 'sprite theater' which is like a simple setup for a diorama-like presentation for simple sprite-based cutscenese.

// NOTE: I'm working on this as it's needed to be worked on. It's not quite experimental as it's well tested but requires a lot of leg work to get off the ground.

#define PLEX_createActor(sprite, space, hidden) PLEX_createActorInt(sprite, space, hidden, __LINE__, __FILE__)

#define PLEX_addSpriteTheaterSlide(r, g, b, message, actor, waitTime, pauseForWait) PLEX_addSpriteTheaterSlideInt(r, g, b, message, actor, waitTime, pauseForWait, __LINE__, __FILE__)

#define PLEX_hideActorChange(actor) PLEX_hideActorChangeInt(actor, __LINE__, __FILE__)
#define PLEX_unhideActorChange(actor) PLEX_unhideActorChangeInt(actor, __LINE__, __FILE__)
#define PLEX_animationActorChange(actor, index) PLEX_animationActorChangeInt(actor, index, __LINE__, __FILE__)
#define PLEX_moveActorChange(actor, x, y) PLEX_moveActorChangeInt(actor, x, y, __LINE__, __FILE__)

#define PLEX_spriteTheaterBackgroundChange(backdrop, name) PLEX_spriteTheaterBackgroundChangeInt(backdrop, name, __LINE__, __FILE__)

#define PLEX_actorResize(actor, w, h) PLEX_actorResizeInt(actor, w, h, __LINE__, __FILE__)

#define PLEX_spriteTheaterMusicChange(ID) PLEX_spriteTheaterMusicChangeInt(ID, __LINE__, __FILE__)
#define PLEX_spriteTheaterMusicStop() PLEX_spriteTheaterMusicStopInt(__LINE__, __FILE__)
#define PLEX_spriteTheaterSoundEffect(ID) PLEX_spriteTheaterSoundEffectInt(ID, __LINE__, __FILE__)

#ifdef __cplusplus
extern "C"
{
#endif
	// creates an 'actor' for sprite theater based on sprite and space

	extern int64_t PLEX_createActorInt(PLEX_SPRITE* sprite, const PLEX_RECT space, const bool hidden, const size_t line, const char* const filename);

	// adds a slide to the current sprite theater

	extern void PLEX_addSpriteTheaterSlideInt(const uint8_t r, const uint8_t g, const uint8_t b, const char* const message, const int64_t actor, const uint64_t waitTime, const bool pauseForWait, const size_t line, const char* const filename);

	// sets sprite theater to the next slide

	extern void PLEX_nextSlide();

	// hides or unhides actor

	extern void PLEX_hideActorChangeInt(const size_t actor, const size_t line, const char* const filename);
	extern void PLEX_unhideActorChangeInt(const size_t actor, const size_t line, const char* const filename);

	// changes the animation of an actor

	extern void PLEX_animationActorChangeInt(const size_t actor, const size_t index, const size_t line, const char* const filename);

	// moves an actor

	extern void PLEX_moveActorChangeInt(const size_t actor, const double x, const double y, const size_t line, const char* const filename);

	// marks to change the background during whatever slide

	extern void PLEX_spriteTheaterBackgroundChangeInt(const size_t backdrop, const char* const name, const size_t line, const char* const filename);

	// change the size of actor

	extern void PLEX_actorResizeInt(const size_t actor, const double w, const double h, const size_t line, const char* const filename);

	// allows some control over musical cues and sound effects. However, these only work in a customized musical system to work. If this sounds confusing check out the signals in sound.h and sound.c

	extern void PLEX_spriteTheaterMusicChangeInt(const size_t ID, const size_t line, const char* const filename);
	extern void PLEX_spriteTheaterMusicStopInt(const size_t line, const char* const filename);
	extern void PLEX_spriteTheaterSoundEffectInt(const size_t ID, const size_t line, const char* const filename);

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
