#ifndef PLEX_LANGUAGE_EN_H
#define PLEX_LANGUAGE_EN_H

// this header contains macro strings which are relevant to the English Language output of PLEX. More information on how that works check out the "meta/language.h" header. As with all headers in the meta folder, there is no correlating source file.

// these two strings are basically strings which determine the order of letters in which bitmap fonts read. Entirely optional but saves some time. The first is for most messages, the second is only for strings that show a timer.

#define PLEX_LANGUAGE_ALPHASTRING "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!*.,;:?\"%'-() /$#"
#define PLEX_LANGUAGE_TIMESTRING "0123456789: "

// The default title. It's commonly overwritten by whatever project is referencing the library.

#define PLEX_LANGUAGE_TITLE "Yet Another"

#endif
