#include "plex.h"

// NOTE: function descriptions in header.

static bool p_running = false;

// ---------------------------- //
// PUBLIC FUNCTIONS:
// ---------------------------- //

bool PLEX_startPlexInt(const char* const title)
{
	if(p_running) return true;

	if(PLEX_CORE_OS == PLEX_CORE_NOOS) // must be first
	{
		printf("%s\r\n", PLEX_ERRORMESS_ERROR_SYSTEMFAIL_NOOS);
		return false;
	}

	if(!PLEX_startErrorSystem()) // must be second
	{
		printf("%s\r\n", PLEX_ERRORMESS_ERROR_SYSTEMFAIL_INIT);
		return false;
	}

	if(!PLEX_startMem()) // must be third
	{
		PLEX_error(PLEX_ERROR_CATEGORY_START, PLEX_ERROR_START, PLEX_ERROR_SEVERITY_FATAL, PLEX_ERRORMESS_SUBSYSTEM_MEMORY);
		return false;
	}

	PLEX_startMath(); // < cannot possibly fail

	if(!PLEX_startDefaults())
	{
		PLEX_error(PLEX_ERROR_CATEGORY_START, PLEX_ERROR_START, PLEX_ERROR_SEVERITY_FATAL, PLEX_ERRORMESS_SUBSYSTEM_DEFAULT);
		return false;
	}

	if(!PLEX_startTime())
	{
		PLEX_error(PLEX_ERROR_CATEGORY_START, PLEX_ERROR_START, PLEX_ERROR_SEVERITY_FATAL, PLEX_ERRORMESS_SUBSYSTEM_TIME);
		PLEX_errorSDL(PLEX_ERROR_SEVERITY_FATAL);
		return false;
	}

	if(!PLEX_startWindow(title, PLEX_DEF_width(), PLEX_DEF_height(), 1.3333333333))
	{
		PLEX_error(PLEX_ERROR_CATEGORY_START, PLEX_ERROR_START, PLEX_ERROR_SEVERITY_FATAL, PLEX_ERRORMESS_SUBSYSTEM_WINDOW);
		PLEX_errorSDL(PLEX_ERROR_SEVERITY_FATAL);
		return false;
	}

	if(!PLEX_startTextures())
	{
		PLEX_error(PLEX_ERROR_CATEGORY_START, PLEX_ERROR_START, PLEX_ERROR_SEVERITY_FATAL, PLEX_ERRORMESS_SUBSYSTEM_TEXTURE);
		PLEX_errorSDL(PLEX_ERROR_SEVERITY_FATAL);
		return false;
	}

	if(!PLEX_startScreenEffects(PLEX_getGameTime()))
	{
		PLEX_error(PLEX_ERROR_CATEGORY_START, PLEX_ERROR_START, PLEX_ERROR_SEVERITY_FATAL, PLEX_ERRORMESS_SUBSYSTEM_SCREENEFFECTS);
		return false;
	}

	if(!PLEX_startGraphics())
	{
		PLEX_error(PLEX_ERROR_CATEGORY_START, PLEX_ERROR_START, PLEX_ERROR_SEVERITY_FATAL, PLEX_ERRORMESS_SUBSYSTEM_GRAPHICS);
		PLEX_errorSDL(PLEX_ERROR_SEVERITY_FATAL);
		return false;
	}

	if(!PLEX_startInput())
	{
		PLEX_error(PLEX_ERROR_CATEGORY_START, PLEX_ERROR_START, PLEX_ERROR_SEVERITY_FATAL, PLEX_ERRORMESS_SUBSYSTEM_INPUT);
		PLEX_errorSDL(PLEX_ERROR_SEVERITY_FATAL);
		return false;
	}

	if(!PLEX_startSound())
	{
		PLEX_error(PLEX_ERROR_CATEGORY_START, PLEX_ERROR_START, PLEX_ERROR_SEVERITY_FATAL, PLEX_ERRORMESS_SUBSYSTEM_SOUND);
		return false;
	}


	p_running = true;
	return true;
}

bool PLEX_updatePlex()
{
	if(!PLEX_updateErrorSystem())
	{
		PLEX_error(PLEX_ERROR_CATEGORY_UPDATE, PLEX_ERROR_UPDATE, PLEX_ERROR_SEVERITY_FATAL, PLEX_ERRORMESS_SUBSYSTEM_ERROR);
		return false;
	}

	if(!PLEX_updateMem())
	{
		PLEX_error(PLEX_ERROR_CATEGORY_UPDATE, PLEX_ERROR_UPDATE, PLEX_ERROR_SEVERITY_FATAL, PLEX_ERRORMESS_SUBSYSTEM_MEMORY);
		return false;
	}

	if(!PLEX_updateTime())
	{
		PLEX_error(PLEX_ERROR_CATEGORY_UPDATE, PLEX_ERROR_UPDATE, PLEX_ERROR_SEVERITY_FATAL, PLEX_ERRORMESS_SUBSYSTEM_TIME);
		return false;
	}

	if(!PLEX_updateWindow())
	{
		PLEX_error(PLEX_ERROR_CATEGORY_UPDATE, PLEX_ERROR_UPDATE, PLEX_ERROR_SEVERITY_FATAL, PLEX_ERRORMESS_SUBSYSTEM_WINDOW);
		return false;
	}

	if(!PLEX_updateInput())
	{
		PLEX_error(PLEX_ERROR_CATEGORY_UPDATE, PLEX_ERROR_UPDATE, PLEX_ERROR_SEVERITY_FATAL, PLEX_ERRORMESS_SUBSYSTEM_INPUT);
		return false;
	}

	if(!PLEX_updateScreenEffects(PLEX_getGameTime()))
	{
		PLEX_error(PLEX_ERROR_CATEGORY_UPDATE, PLEX_ERROR_UPDATE, PLEX_ERROR_SEVERITY_FATAL, PLEX_ERRORMESS_SUBSYSTEM_SCREENEFFECTS);
		return false;
	}

	if(!PLEX_updateGraphics(PLEX_getWindowContents(), PLEX_WW(false), PLEX_WH(false)))
	{
		PLEX_error(PLEX_ERROR_CATEGORY_UPDATE, PLEX_ERROR_UPDATE, PLEX_ERROR_SEVERITY_FATAL, PLEX_ERRORMESS_SUBSYSTEM_GRAPHICS);
		return false;
	}

	if(!PLEX_updateSound())
	{
		PLEX_error(PLEX_ERROR_CATEGORY_UPDATE, PLEX_ERROR_UPDATE, PLEX_ERROR_SEVERITY_FATAL, PLEX_ERRORMESS_SUBSYSTEM_SOUND);
		return false;
	}

	return true;
}

void PLEX_endPlex()
{
	if(!PLEX_isSpriteTheaterOver()) PLEX_endSpriteTheater();

	PLEX_endSound();
	PLEX_endScreenEffects();
	PLEX_endGraphics();
	PLEX_endInput();
	PLEX_endWindow();
	PLEX_endTime();
	PLEX_endDefaults();
	PLEX_endMem(); // must be second to last
	PLEX_endErrorSystem();
}
