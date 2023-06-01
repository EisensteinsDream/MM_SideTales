#ifndef PLEX_ERROR_H
#define PLEX_ERROR_H

#include "terminate.h"

#ifdef __cplusplus
extern"C"
{
#endif
	#define PLEX_errorNoTerminate(header, body) PLEX_errorSetMeta(__FILE__, __LINE__); PLEX_errorNoTerminateInt(header, body);
	#define PLEX_errorTerminate(header, body) PLEX_errorSetMeta(__FILE__, __LINE__); PLEX_errorTerminateInt(header, body);

	// --- PLEX errors ---

	extern void PLEX_errorSetMeta(const char* const fileNanme, const size_t errorLine);

	// executes what is called in most languages a 'warning'. An error that shows but does not terminate the program. heading being type of error and body being error message.

	extern void PLEX_errorNoTerminateInt(const char* const header, const char* const body);

	// executes an error that does terminate the program. A fatal error as it were. There are no exceptions in regular C or PLEX.

	extern void PLEX_errorTerminateInt(const char* const header, const char* const body);

	// --- Checks for Non-PLEX errors ---

	// checks for an SDL error and shows it if there is one present
	extern bool PLEX_errorSDL();

	// checks for a port audio error and shows it if there is one present
	extern bool PLEX_errorPortAudio(const PaError error);
#ifdef __cplusplus
}
#endif

#endif
