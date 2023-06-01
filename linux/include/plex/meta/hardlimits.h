#ifndef PLEX_HARDLIMITS_H
#define PLEX_HARDLIMITS_H

// This header contains a series of macros which contain predetermined limitations for buffers and other things. These are building block elements to all of PLEX. As with all headers in the meta folder, there is no correlating source file.

// NOTE: When PLEX originally started it had no dynamic memory allocation (heap allocation) so nearly everything was a predetermined static buffer. That was, clearly, a mistake. However, some remnants of that system still exist within PLEX so this file remains. It is possible that this file will be renamed, merged with another file, or deleted at some point. However, the structural integrity of PLEX still relies on it for the moment.

// MEMORY

#define PLEX_LIMIT_MEM_EXP 64
#define PLEX_LIMIT_MEMENTRY_EXP 16
#define PLEX_LIMIT_MEM_PRELOAD 128

#define PLEX_LIMIT_MEMREUSE_MAX 1024 // < the maximum size that an allocated memory spot needs to be to be recycled.

// DEFAULTS

#define PLEX_LIMIT_DEFAULTS_EXP 32

#define PLEX_LIMIT_DEFAULT_SBUFFERLEN 32

#define PLEX_LIMIT_DEFAULT_DEFAULTBUFFLEN 128

#define PLEX_DEFAULT_COMMENTBUFFERLIMIT 256

// MATH

#define PLEX_LIMIT_POLYSIDES 32

#define PLEX_LIMIT_SQRTTABLE 1000 // < typically this should be 1000 or less. In most cases, it should be 1000

// INPUT

#define PLEX_LIMIT_CONTROLLERS 16

// GRAPHICS

#define PLEX_LIMIT_DRAWLIST_EXP 64
#define PLEX_LIMIT_TEXLIMIT_EXP 32

#define PLEX_DEFAULT_FRAMEMAX 64
#define PLEX_DEFAULT_ANIMATIONMAX 64

#define PLEX_SCREENEFFECTS_LIMIT 32

#if (PLEX_RENDERER_TYPE == PLEX_RENDERER_TYPE_GL_1_4)
	#define PLEX_LIMIT_GL_INDICES 960 // < shapecount * 4
	#define PLEX_LIMIT_GL_VERTSET 2880 // < shapecount * 12
	#define PLEX_LIMIT_GL_COLVERTSET 3840 // < shapecount * 16
	#define PLEX_LIMIT_GL_TEXVERTSET 1920 // < shapecount * 8
#endif

// SOUND

#define PLEX_LIMIT_SOUNDCHANNELS 32
#define PLEX_LIMIT_PREBUFFERCOUNT 64

#define PLEX_LIMIT_CHANNELBUFFERDEFAULT 1323000

#define PLEX_SOUNDBREAKOUTATTEMPTS 32

// FILE MANAGEMENT

#define PLEX_LIMIT_LINEPARSE 1024
#define PLEX_LIMIT_TOKENLIMIT 32

#define PLEX_LIMIT_STARTINDEXFILECAP 128
#define PLEX_LIMIT_STARTINDEXFILEGROWTH 16

// UTILITY

#define PLEX_LIMIT_GENMAX 1024 // generator

#define PLEX_LIMIT_PROMMAX 64 // prominence machine

#define PLEX_LIMIT_NUMCONVPREC 1000000000 // < should be a number starting with 1 then the number of 0's to indicate precision. Like one million would be six zeroes. This macro is used to convert digits in floating numbers to strings.

// SPRITE THEATER

#define PLEX_SPRITETHEATER_SLIDELIMIT 128
#define PLEX_SPRITETHEATER_ACTORLIMIT 128
#define PLEX_SPRITETHEATER_ACTORCHANGELIMIT 128
#define PLEX_SPRITETHEATER_MUSICCHANGELIMIT 128

#endif
