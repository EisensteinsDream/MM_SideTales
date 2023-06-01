#ifndef PLEX_TERMINATE_H
#define PLEX_TERMINATE_H

// This unit contains the terminate signals for PLEX. If a program using PLEX is set up properly than these functions can set it to terminate at any point. Also, in termination, all of the proper 'end' functions are executed. This is much reccomended to use with plex rather than something like 'exit' or std::exit since it's build into PLEX. Of course, these functions are not in the standard library level and will not escape from inescapable loops.

#include "meta/cinclude.h"

#ifdef __cplusplus
extern"C"
{
#endif
	// functions that 'ask' to quit. Essentially these will not terminate the program but put a signal up that, if intercepted, a custom quit screen can execute from within the program. (I.E. an exit screen.) However, in absence of a formal 'exit' screen these will simply terminate. They are executed when a user hits the close button or escape button and those means of exiting are enabled in config.

	extern void PLEX_askQuit();
	extern void PLEX_unaskQuit(); // < stops asking if executed before the update checks (if set up right, at the end of the update loop)
	extern bool PLEX_askingToQuit();

	// signal to terminate the program. Simply terminates the program without asking

	extern void PLEX_terminate();
	extern void PLEX_unterminate(); // < cancels terminate if executed before the update checks (if set up right, at the end of the update loop)
	extern bool PLEX_isTerminated();
#ifdef __cplusplus
}
#endif

#endif
