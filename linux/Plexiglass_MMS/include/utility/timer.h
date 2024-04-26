#ifndef PLEX_TIMER_H
#define PLEX_TIMER_H

#include "utility/fakestrings.h"

// this unit stores realtime timer information. Based on SDL_Ticks() so it has microsecond prescision.

// NOTE: This is an absolutely crucial library within PLEX.

// A struct that essentially holds all of the markers for a timer.
typedef struct
{
	uint64_t time, lastTime, marker;
	bool paused, unpaused, started;
	double multiplier, secondsPass;
} PLEX_TIMER;

#ifdef __cplusplus
extern "C"
{
#endif
	// FUNCTIONS RELATED TO INDIVIDUAL TIMERS

	// generates a new timer, unset.

	extern PLEX_TIMER PLEX_genTimer();

	// starts the timer based off of 'time' in microseconds

	extern void PLEX_startTimer(PLEX_TIMER* timer, const uint64_t baseTime);

	// syncronizes the timer to a 'time' in microseconds

	extern void PLEX_updateTimer(PLEX_TIMER* timer, const uint64_t time);

	// pauses, unpauses timer

	extern void PLEX_pause(PLEX_TIMER* timer);
	extern void PLEX_unpause(PLEX_TIMER* timer);

	// moves the time in the timer forward or back manually

	extern void PLEX_fastForward(PLEX_TIMER* timer, const uint64_t by);
	extern void PLEX_rewind(PLEX_TIMER* timer, const uint64_t by);

	// reset the timer back to 0

	extern void PLEX_resetTimer(PLEX_TIMER* timer);

	// sets a multiplier to the timer. Typically, it is 1 or 100%. If at 3 time begins to calculate 3 times faster. If 0.5 the time begins to calculate at half the speed.

	extern void PLEX_setMultiplierTimer(PLEX_TIMER* timer, const double multiplier);

	// returns whether or not the timer is paused

	extern bool PLEX_getPaused(PLEX_TIMER* timer);

	// returns the time of the timer

	extern uint64_t PLEX_getTime(PLEX_TIMER* timer);

	// get the number of seconds passed since last update

	extern double PLEX_getSecondsPassed(PLEX_TIMER* timer);

	// returns the time of the timer counted down from 'from'

	extern uint64_t PLEX_countdownTime(PLEX_TIMER* timer, const uint64_t from);

	// returns whether or not a timer has been started

	extern bool PLEX_timerStarted(const PLEX_TIMER timer);

	// returns a string of the formatted time. Something like '382' seconds coming off as '6:22'

	extern PLEX_FAKESTRING_10 PLEX_formatStringTime(const uint64_t time);

	// returns a time string formatted from timer, and a countdown timer

	extern PLEX_FAKESTRING_10 PLEX_timeString(PLEX_TIMER* timer);
	extern PLEX_FAKESTRING_10 PLEX_timeCountString(PLEX_TIMER* timer, const uint64_t from);

	// FUNCTIONS FOR GLOBAL TIME

	// starts the global time, updates the global time, ends the global time

	extern bool PLEX_startTime();
	extern bool PLEX_updateTime();
	extern void PLEX_endTime();

	// pauses global time, unpauses global time, returns whether or not the global time is paused

	extern void PLEX_pauseGameTime();
	extern void PLEX_unpauseGameTime();
	extern bool PLEX_getGameTimePaused();

	// returns the global time

	extern uint64_t PLEX_getGameTime();

	extern double PLEX_getGameSecondsPassed();

	// returns global time but ignoring all pausing and unpausing

	extern uint64_t PLEX_getNoPauseTime();

	extern double PLEX_getNoPauseGameSecondsPassed();

	// returns the time string of the global time

	extern const char* PLEX_getGameTimeString();

	// returns the time string of the global time ignoring pause

	extern const char* PLEX_getNoPauseGameTimeString();
#ifdef __cplusplus
}
#endif

#endif
