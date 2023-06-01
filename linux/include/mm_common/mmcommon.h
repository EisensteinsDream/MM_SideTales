#ifndef MM_COMMON_GAME_H
#define MM_COMMON_GAME_H

#include <plex.h>

#include "meta/mmcommon_language.h"

#include "mmthemes.h"

#ifdef __cplusplus
extern "C"
{
#endif
	extern bool MM_COMMON_start();
	extern bool MM_COMMON_update();
	extern void MM_COMMON_end();
#ifdef __cplusplus
}
#endif

#endif
