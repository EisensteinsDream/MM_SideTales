#include "utility/timer.h"

static bool p_running = false;

static PLEX_TIMER p_gameTime, p_noPauseTime;

static PLEX_FAKESTRING_10 p_gameTimeString, p_gameNoPauseTimeString;

// ---------------------------- //
// PUBLIC FUNCTIONS:
// ---------------------------- //

// FUNCTIONS RELATED TO INDIVIDUAL TIMERS

PLEX_TIMER PLEX_genTimer()
{
	PLEX_TIMER ret;

	memset(&ret, 0, sizeof(ret));

	ret.unpaused = true;

	return ret;
}

void PLEX_startTimer(PLEX_TIMER* timer, const uint64_t baseTime)
{
	if(timer == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_TIMING, PLEX_ERROR_SETNULL, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_ONTIMERSTART, PLEX_ERRORMESS_TIMER);
		return;
	}

	timer->time = 0;
	timer->lastTime = 0;
	timer->secondsPass = 0;
	timer->marker = baseTime;
	timer->paused = false;
	timer->unpaused = true;
	timer->multiplier = 1;
	timer->started = true;
}

void PLEX_updateTimer(PLEX_TIMER* timer, const uint64_t time)
{
	uint64_t newTime = 0;

	if(timer == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_TIMING, PLEX_ERROR_ISNULL, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_ONUPDATINGTIMER, PLEX_ERRORMESS_TIMER);
		return;
	}

	if(timer->paused)
	{
		timer->marker = time;

		if(timer->unpaused) // < this is here to ensure that the marker is set to time when unpaused
		{
			timer->paused = false;
			timer->unpaused = false;
		}

		return;
	}

	newTime = (time - timer->marker) * timer->multiplier;

	timer->lastTime = timer->time;
	timer->time += newTime;
	timer->secondsPass = newTime/1000.0;
	timer->marker = time;
	timer->unpaused = false;
}

void PLEX_pause(PLEX_TIMER* timer)
{
	if(timer == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_TIMING, PLEX_ERROR_ISNULL, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_ONTIMERPAUSE, PLEX_ERRORMESS_TIMER);
		return;
	}

	timer->paused = true;
}

void PLEX_unpause(PLEX_TIMER* timer)
{
	if(timer == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_TIMING, PLEX_ERROR_ISNULL, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_ONTIMERUNPAUSE, PLEX_ERRORMESS_TIMER);
		return;
	}

	timer->unpaused = true;
}

void PLEX_fastForward(PLEX_TIMER* timer, const uint64_t by)
{
	if(timer == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_TIMING, PLEX_ERROR_ISNULL, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_ONTIMERFASTFORWARD, PLEX_ERRORMESS_TIMER);
		return;
	}

	timer->time += by;
}

void PLEX_rewind(PLEX_TIMER* timer, const uint64_t by)
{
	if(timer == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_TIMING, PLEX_ERROR_ISNULL, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_ONTIMEREWIND, PLEX_ERRORMESS_TIMER);
		return;
	}

	if(by < timer->time) timer->time -= by;
	else timer->time = 0;
}

void PLEX_resetTimer(PLEX_TIMER* timer)
{
	if(timer == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_TIMING, PLEX_ERROR_ISNULL, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_ONTIMERESET, PLEX_ERRORMESS_TIMER);
		return;
	}

	timer->time = 0;
}

void PLEX_setMultiplierTimer(PLEX_TIMER* timer, const double multiplier)
{
	if(timer == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_TIMING, PLEX_ERROR_SETNULL, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_ONTIMEMULTIPLIER, PLEX_ERRORMESS_TIMER);
		return;
	}

	if(multiplier > 0) timer->multiplier = multiplier;
	else
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_TIMING, PLEX_ERROR_ZEROVALUE, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONTIMEMULTIPLIER, PLEX_ERRORMESS_TIMERMULTIPLIER);
	}
}

double PLEX_getSecondsPassed(PLEX_TIMER* timer)
{
	if(timer == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_TIMING, PLEX_ERROR_GETNULL, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_ONGETTINGTIMERSECONDS, PLEX_ERRORMESS_TIMER);
		return false;
	}

	return timer->secondsPass;
}

bool PLEX_getPaused(PLEX_TIMER* timer)
{
	if(timer == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_TIMING, PLEX_ERROR_GETNULL, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_ONPAUSINGTIMER, PLEX_ERRORMESS_TIMER);
		return false;
	}

	return timer->paused;
}

uint64_t PLEX_getTime(PLEX_TIMER* timer)
{
	if(timer == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_TIMING, PLEX_ERROR_GETNULL, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_TIME, PLEX_ERRORMESS_TIMER);
		return 0;
	}

	return timer->time;
}

uint64_t PLEX_countdownTime(PLEX_TIMER* timer, const uint64_t from)
{
	if(timer == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_TIMING, PLEX_ERROR_GETNULL, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_COUNTDOWNTIME, PLEX_ERRORMESS_TIMER);
		return 0;
	}

	if(from < timer->time) return 0; // < no error

	return from - timer->time;
}

bool PLEX_timerStarted(const PLEX_TIMER timer){ return timer.started; }

PLEX_FAKESTRING_10 PLEX_formatStringTime(const uint64_t time)
{
	const uint64_t rawSeconds = time/1000;

	const uint8_t seconds = rawSeconds % 60;
	const uint8_t minutes = rawSeconds/60 % 60;
	const uint8_t hours = rawSeconds/3600 % 99;

	const uint8_t sdigit1 = seconds/10;
	const uint8_t sdigit2 = seconds % 10;

	const uint8_t mdigit1 = minutes/10;
	const uint8_t mdigit2 = minutes % 10;

	const uint8_t hdigit1 = hours/10;
	const uint8_t hdigit2 = hours % 10;

	PLEX_FAKESTRING_10 ret;

	size_t it = 0;

	uint8_t* edit = (uint8_t*)&ret;

	if(rawSeconds >= 3600)
	{
		edit[it] = '0' + hdigit1;
		it++;
		edit[it] = '0' + hdigit2;
		it++;
		edit[it] = ':';
		it++;
	}

	if(rawSeconds >= 60)
	{
		edit[it] = '0' + mdigit1;
		it++;
		edit[it] = '0' + mdigit2;
		it++;
		edit[it] = ':';
		it++;
	}

	edit[it] = '0' + sdigit1;
	it++;
	edit[it] = '0' + sdigit2;
	it++;
	edit[it] = '\0';

	return ret;
}

PLEX_FAKESTRING_10 PLEX_timeString(PLEX_TIMER* timer)
{
	if(timer == NULL)
	{
		PLEX_error(PLEX_ERROR_CATEGORY_TIMING, PLEX_ERROR_GETNULL, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_TIMESTRING);
		return PLEX_blankString10();
	}

	return PLEX_formatStringTime(timer->time);
}

PLEX_FAKESTRING_10 PLEX_timeCountString(PLEX_TIMER* timer, const uint64_t from)
{
	if(timer == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_TIMING, PLEX_ERROR_GETNULL, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_COUNTDOWNTIMESTRING, PLEX_ERRORMESS_TIMER);
		return PLEX_blankString10();
	}

	return PLEX_formatStringTime(PLEX_countdownTime(timer, from));
}

// FUNCTIONS FOR GLOBAL TIME

bool PLEX_startTime()
{
	uint64_t ticks = 0;

	if(p_running)
	{
		PLEX_errorNoDesc(PLEX_ERROR_CATEGORY_TIMING, PLEX_ERROR_DOUBLESTART, PLEX_ERROR_SEVERITY_MINOR);
		return true;
	}

	if(SDL_InitSubSystem(SDL_INIT_TIMER)) return false; // remember 0 == success with this method

	ticks = SDL_GetTicks();

	p_gameTime = PLEX_genTimer();
	p_noPauseTime = PLEX_genTimer();

	PLEX_startTimer(&p_gameTime, ticks);
	PLEX_startTimer(&p_noPauseTime, ticks);

	p_running = true;

	return true;
}

bool PLEX_updateTime()
{
	uint64_t ticks = 0;

	if(!p_running)
	{
		PLEX_errorNoDesc(PLEX_ERROR_CATEGORY_TIMING, PLEX_ERROR_UPDATEBEFORESTART, PLEX_ERROR_SEVERITY_MINOR);
		return false;
	}

	ticks = SDL_GetTicks();

	PLEX_updateTimer(&p_gameTime, ticks);
	PLEX_updateTimer(&p_noPauseTime, ticks);

	return true;
}

void PLEX_endTime()
{
	if(!p_running)
	{
		PLEX_errorNoDesc(PLEX_ERROR_CATEGORY_TIMING, PLEX_ERROR_ENDBEFORESTART, PLEX_ERROR_SEVERITY_MINOR);
		return;
	}

	SDL_QuitSubSystem(SDL_INIT_TIMER);

	p_running = false;
}

void PLEX_pauseGameTime()
{
	if(!p_running)
	{
		PLEX_error(PLEX_ERROR_CATEGORY_TIMING, PLEX_ERROR_ACTIONBEFORESTART, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONPAUSEGAMETIME);
		return;
	}

	PLEX_pause(&p_gameTime);
}

void PLEX_unpauseGameTime()
{
	if(!p_running)
	{
		PLEX_error(PLEX_ERROR_CATEGORY_TIMING, PLEX_ERROR_ACTIONBEFORESTART, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONUNPAUSEGAMETIME);
		return;
	}

	PLEX_unpause(&p_gameTime);
}

bool PLEX_getGameTimePaused()
{
	if(!p_running)
	{
		PLEX_error(PLEX_ERROR_CATEGORY_TIMING, PLEX_ERROR_ACTIONBEFORESTART, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONGETTINGUNPAUSEGAMETIME);
		return false;
	}

	return PLEX_getPaused(&p_gameTime);
}

uint64_t PLEX_getGameTime()
{
	if(!p_running)
	{
		PLEX_error(PLEX_ERROR_CATEGORY_TIMING, PLEX_ERROR_ACTIONBEFORESTART, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONGETTINGGAMETIME);
		return 0;
	}

	return PLEX_getTime(&p_gameTime);
}

double PLEX_getGameSecondsPassed()
{
	if(!p_running)
	{
		PLEX_error(PLEX_ERROR_CATEGORY_TIMING, PLEX_ERROR_ACTIONBEFORESTART, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONGETTINGGAMETIMERSECONDSNOPAUSE);
		return 0;
	}

	return PLEX_getSecondsPassed(&p_gameTime);
}

uint64_t PLEX_getNoPauseTime()
{
	if(!p_running)
	{
		PLEX_error(PLEX_ERROR_CATEGORY_TIMING, PLEX_ERROR_ACTIONBEFORESTART, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONGETTINGGAMETIMENOPAUSE);
		return 0;
	}

	return PLEX_getTime(&p_noPauseTime);
}

double PLEX_getNoPauseGameSecondsPassed()
{
	if(!p_running)
	{
		PLEX_error(PLEX_ERROR_CATEGORY_TIMING, PLEX_ERROR_ACTIONBEFORESTART, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONGETTINGGAMETIMERSECONDSNOPAUSE);
		return 0;
	}

	return PLEX_getSecondsPassed(&p_noPauseTime);
}

const char* PLEX_getGameTimeString()
{
	if(!p_running)
	{
		PLEX_error(PLEX_ERROR_CATEGORY_TIMING, PLEX_ERROR_ACTIONBEFORESTART, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONGETTINGGAMETIMESTRING);
		return "";
	}

	p_gameTimeString = PLEX_timeString(&p_gameTime);

	return PLEX_fts(p_gameTimeString);
}

const char* PLEX_getNoPauseGameTimeString()
{
	if(!p_running)
	{
		PLEX_error(PLEX_ERROR_CATEGORY_TIMING, PLEX_ERROR_ACTIONBEFORESTART, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONGETTINGGAMETIMENOPAUSETIMESTRING);
		return "";
	}

	p_gameNoPauseTimeString = PLEX_timeString(&p_noPauseTime);

	return PLEX_fts(p_gameNoPauseTimeString);
}
