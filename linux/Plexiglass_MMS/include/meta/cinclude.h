#ifndef PLEX_CINCLUDE_H
#define PLEX_CINCLUDE_H

// This is basically the main 'meta' header. It should always be included before everything EXCEPT osadapt. It also includes all standard headers and the headers of dependecies that are used by the library. This way they don't need to be individually included in files. As with all headers in the meta folder, there is no correlating source file.

#include "meta/osadapt.h" // < Always keep first. Certain specific headers included below require knowledge of the OS.

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
	#include <dirent.h>

	#include <sys/utsname.h>
#endif

#if (PLEX_CORE_OS == PLEX_CORE_WINDOWS)
	#include <windows.h>
	#include <conio.h>
	#include <process.h>
#endif

#include "meta/standardnumbers.h"
#include "meta/hardlimits.h"
#include "meta/errormess.h"

// This creates a standard path to the resource directory of whatever project it is in. It can be used in a path string thusly to get the "subfolder/file.fle" in the recfolder: PLEX_RECDIRECTORY "subfolder/file.fle". It also can be defined in compilation if a different directory path is needed. As well as undefined and redefined for the same purpose.

#ifndef PLEX_RECDIRECTORY
#define PLEX_RECDIRECTORY "resources/"
#endif

// This macro operates in a similar fasion except for the headerlesstex directory instead of the resource directory. This is useful for generating headerless 'raw' textures which take up more space but load more quickly. Headerless textures explained further in the graphics/graphics and graphics/texture units.

#ifndef PLEX_HEADERLESSDIRECTORY
#define PLEX_HEADERLESSDIRECTORY "headerlesstex/"
#endif

#endif
