#ifndef PLEX_SCREENEFFECTS_H
#define PLEX_SCREENEFFECTS_H

#include "windowing/graphics.h"

// enum that indicates different availble screen effects

// NOTE: this is fairly safe but fairly limited. As needed, a lot more screen effects will be added.


#define PLEX_doScreenEffect(type, r, g, b, a, loop, screenOffset, duration, minPerc, time) PLEX_doScreenEffectInt(type, r, g, b, a, loop, screenOffset, duration, minPerc, time, __LINE__, __FILE__)

#define PLEX_doScreenEffectRanged(type, r, g, b, a, loop, screenOffset, duration, minPerc, range, time) PLEX_doScreenEffectRangedInt(type, r, g, b, a, loop, screenOffset, duration, minPerc, range, time, __LINE__, __FILE__)

typedef enum
{
	PLEX_SCREENEFFECT_TINTFADE = 0,
	PLEX_SCREENEFFECT_SPLATTER = 1
} PLEX_SCREENEFFECT_TYPE;

#ifdef __cplusplus
extern "C"
{
#endif
	// does a screen effect over the entire screen

	extern int64_t PLEX_doScreenEffectInt(const PLEX_SCREENEFFECT_TYPE type, const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a, const bool loop, const bool screenOffset, const uint64_t duration, const double minPerc, const uint64_t time, const size_t linenum, const char* const filename);

	// does a screen effect within a ranged area

	extern int64_t PLEX_doScreenEffectRangedInt(const PLEX_SCREENEFFECT_TYPE type, const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a, const bool loop, const bool screenOffset, const uint64_t duration, const double minPerc, const PLEX_RECT range, const uint64_t time, const size_t linenum, const char* const filename);

	// stops screen effects

	extern void PLEX_ceaseScreenEffect(const int64_t index);

	// disables or enables screen effects

	extern void PLEX_disableScreenEffects();
	extern void PLEX_enableScreenEffects();

	// starts, updates and ends screen effects. Typically, you don't need to call these since the PLEX_start, PLEX_update, and PLEX_end already manage this.

	extern bool PLEX_startScreenEffects(const uint64_t time);
	extern bool PLEX_updateScreenEffects(const uint64_t time);
	extern void PLEX_endScreenEffects();

	// returns whether or not a screen effect is over

	extern bool PLEX_screenEffectOver(const int64_t index, const uint64_t time);

	// returns whether or not the screen effect is disabled

	extern bool PLEX_screenEffectsDisabled();
#ifdef __cplusplus
}
#endif

#endif
