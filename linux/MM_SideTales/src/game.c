#include "game.h"

bool MMS_startGame()
{
	if(!PLEX_startPlex())
	{
		PLEX_error(PLEX_ERROR_CATEGORY_INPROJECT, PLEX_ERROR_START, PLEX_ERROR_SEVERITY_FATAL, MMS_ERRORMESS_PLEXFAIL);
		return false;
	}

	return true;
}

bool MMS_updateGame()
{
	MM_COMMON_update();

	if(!PLEX_updatePlex())
	{
		PLEX_error(PLEX_ERROR_CATEGORY_INPROJECT, PLEX_ERROR_UPDATE, PLEX_ERROR_SEVERITY_FATAL, MMS_ERRORMESS_PLEXFAIL);
		return false;
	}

	if(PLEX_askingToQuit()) PLEX_terminate();

	return !PLEX_isTerminated();
}

void MMS_endGame()
{
	PLEX_endPlex();
}
