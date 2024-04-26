#ifndef PLEX_STANDARDNUMBERS_H
#define PLEX_STANDARDNUMBERS_H

// This is a header that contains a set of standard numbers that aren't limits nor effected by memory size profiles. As with all headers in the meta folder, there is no correlating source file.

// MATH

#define PLEX_STANDNUM_SQRTTABLE 1000 // < typically this should be 1000 or less. In most cases, it should be 1000

// FONTS

// Standard dimensions for bitmap fonts essentially 1/16th width by 1/8th height. Optional but useful for printing from bitmap fonts:

#define PLEX_STANDNUM_FW 0.0625
#define PLEX_STANDNUM_FH 0.125

#define PLEX_STANDNUM_LETTERBUFF 0.8

// SOUND

#define PLEX_STANDNUM_CHANNELBUFFERDEFAULT 1323000

#define PLEX_STANDNUM_SOUNDBREAKOUTATTEMPTS 32
#define PLEX_STANDNUM_SOUNDLOCKFAIL 5

// STRINGS

#define PLEX_STANDNUM_NUMCONVPREC 1000000000 // < should be a number starting with 1 then the number of 0's to indicate precision. Like one million would be six zeroes. This macro is used to convert digits in floating numbers to strings.

#endif
