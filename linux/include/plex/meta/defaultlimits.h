#ifndef PLEX_DEFAULTLIMITS_H
#define PLEX_DEFAULTLIMITS_H

// This folder contains macros for what I consider the 'default defaults' or the values that defaults regularly go to if the config file's entry is corrupt or non-existent. As with all headers in the meta folder, there is no correlating source file.

// NOTE: These might not forever be defined as macros. Then again, they might be. I'm not sure how I want to go about that yet. They are for now, though, and they're here.

// Window

// Any time you see width and height without any descriptors it's safe to assume it's the window width and height. Not all graphics of all projects are guaranteed to look good in 4096x2160 but I don't have a 4k means to test them. Still, those are the bounds ftm.

#define PLEX_DEFMIN_WIDTH 800
#define PLEX_DEFMAX_WIDTH 4096
#define PLEX_DEFAULT_WIDTH 800

#define PLEX_DEFMIN_HEIGHT 600
#define PLEX_DEFMAX_HEIGHT 2160
#define PLEX_DEFAULT_HEIGHT 600

// Input

// Custom cursor colors

#define PLEX_DEFAULT_CURSORR 255
#define PLEX_DEFAULT_CURSORG 255
#define PLEX_DEFAULT_CURSORB 255

#endif
