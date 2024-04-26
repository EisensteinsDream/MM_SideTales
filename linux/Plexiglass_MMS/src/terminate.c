#include "terminate.h"

// static variables

static bool p_terminated = false;
static bool p_askQuit = false;

// ---------------------------- //
// PUBLIC FUNCTIONS:
// ---------------------------- //

// functions that ask PLEX to quit

void PLEX_askQuit(){ p_askQuit = true; }
void PLEX_unaskQuit(){ p_askQuit = false; }
bool PLEX_askingToQuit(){ return p_askQuit; }

// functions that tell PLEX to quit

void PLEX_terminate(){ p_terminated = true; }
void PLEX_unterminate(){ p_terminated = false; }
bool PLEX_isTerminated(){ return p_terminated; }
