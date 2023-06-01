#ifndef PLEX_H
#define PLEX_H

#include "windowing/animation.h"

#include "windowing/screeneffects.h"

#include "utility/error.h"

#include "sound/sound.h"

#include "utility/generator.h"

#include "utility/prominencemachine.h"

#include "utility/fileparse.h"

#include "utility/spritetheater.h"

#include "mem/mem.h"

#ifdef __cplusplus
extern "C"
{
#endif
	// starts, updates, and ends the plex system and all of it's subsystems. If set up properly, the start and update functions will kill the program if they return false.

	extern bool PLEX_startPlex(const bool ignoreRunCheck, const char* const title);
	extern bool PLEX_updatePlex();
	extern void PLEX_endPlex();
#ifdef __cplusplus
}
#endif

#endif
