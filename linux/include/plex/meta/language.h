#ifndef PLEX_LANGUAGE_H
#define PLEX_LANGUAGE_H

// this header is a branching header for different language compiles of PLEX. Currently the only supported language is English and I'm hoping that won't remain that way forever. To see the string macros contained within just go to the "meta/language/language_en.h" header. As with all headers in the meta folder, there is no correlating source file. There are also similiar language files for error messages and default names (ie. config variable names)

// NOTE: in future versions language might not be determined by a precompiled macro. If changed, this will also change the way that errormess.h and defaultnames.h work.

// language codes for plex

#define PLEX_LANGUAGE_ENGLISH 0

// if no language is defined defaults to English

#ifndef PLEX_LANGUAGE
#define PLEX_LANGUAGE PLEX_LANGUAGE_ENGLISH
#endif

// branches to relevant language header

#if (PLEX_LANGUAGE == PLEX_LANGUAGE_ENGLISH)
#include "language/language_en.h"
#endif

#endif
