#ifndef PLEX_CINCLUDE_H
#define PLEX_CINCLUDE_H

// This is basically the main 'meta' header and should always be included before all of the non-'meta' headers. It also containes pretty much all standard library includes that are used. That way, they're in one place. If PLEX was a tree, this would be where the roots meet the stump. All external headers are included here as well. As with all headers in the meta folder, there is no correlating source file.

// NOTE: there will not be many notes in this section. Most of which that are within the PLEX code have definitions within their particular files.

#include "meta/osadapt.h" // < always keep first

// external libraries

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_gamecontroller.h>

#include <portaudio.h>

#include <sndfile.h>

// C standard

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <errno.h>
#include <limits.h>

#include <stdbool.h>
#include <stdint.h>
#include <inttypes.h>

// OS specific

#if PLEX_CORE_IS_UNIX
	#include <unistd.h>
	#include <dirent.h>
	#include <sys/time.h>
	#include <sys/types.h>
	#include <sys/stat.h>
	#include <pthread.h>
	#include <sched.h>

	#include <sys/utsname.h>
#endif

#if (PLEX_CORE_OS == PLEX_CORE_WINDOWS)
	#include <windows.h>
	#include <conio.h>
	#include <process.h>
#endif

#include "meta/standardsizes.h"
#include "meta/hardlimits.h"
#include "meta/errormess.h"

#endif
