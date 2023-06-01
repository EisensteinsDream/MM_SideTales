#include "game.h"

bool MMS_startGame()
{
	if(!PLEX_startPlex(true, PLEX_LANGUAGE_TITLE))
	{
	   // error
	   return false;
	}

	return true;
}

bool MMS_updateGame()
{
	MM_COMMON_updateTheme();

	if(!PLEX_updatePlex())
	{
	   // error
	   return false;
	}

	if(PLEX_askingToQuit()) PLEX_terminate();

	return !PLEX_isTerminated();
}

void MMS_endGame()
{
	PLEX_endPlex();
}
