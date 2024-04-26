#ifndef PLEX_HARDLIMITS_H
#define PLEX_HARDLIMITS_H

// This header contains a series of macros which contain predetermined limitations for buffers and other things. These are building block elements to all of PLEX. As with all headers in the meta folder, there is no correlating source file.

// NOTE: When PLEX originally started it had no dynamic memory allocation (heap allocation) so nearly everything was a predetermined static buffer. That was, clearly, a mistake. However, some remnants of that system still exist within PLEX so this file remains. It is possible that this file will be renamed, merged with another file, or deleted at some point. However, the structural integrity of PLEX still relies on it for the moment.

// ERROR

#define PLEX_LIMIT_ERROR_MESSLEN 1024
#define PLEX_LIMIT_ERROR_SHOWLEN 16

// DEFAULTS

#define PLEX_LIMIT_DEFAULT_SBUFFERLEN 32
#define PLEX_LIMIT_DEFAULT_DEFAULTBUFFLEN 128
#define PLEX_DEFAULT_COMMENTBUFFERLIMIT 256

// MATH

#define PLEX_LIMIT_POLYSIDES 32

// INPUT

#define PLEX_LIMIT_CONTROLLERS 16

// SOUND

#define PLEX_LIMIT_SOUNDCHANNELS 16
#define PLEX_LIMIT_SOUNDEFFECTS 5
#define PLEX_LIMIT_PREBUFFERCOUNT 64

#endif
