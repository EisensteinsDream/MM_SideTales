#ifndef PLEX_WINDOW_H
#define PLEX_WINDOW_H

#include "windowing/graphics_low.h"

#include "utility/error.h"

// This unit contains all functions related to manipulating the window. PLEX's windowing system has kind of a proportion concept that allows the resizing of the screen and the graphics scalling up and down with it. It also automatically generates a borderspace.

// NOTE: At the moment PLEX simply manages a singular game window but it's very possible that will change.

// these are simply for the math needed to change the ratio. It's based off of 800x600 but that is irrelevant to the actual screen size.

#define PLEX_HEIGHTUNITMAXBASE 24
#define PLEX_SCREENUNITBASE 25
#define PLEX_SCREENRATIOBASE 600.0

#ifdef __cplusplus
extern "C"
{
#endif
	// marks whether or not the window is resized. Typically, you will never need to call these manually either.

	extern void PLEX_markSizing();
	extern void PLEX_unmarkSizing();

	// updates window size and restores window. Also, Typically, you will never need to call these manually.

	extern void PLEX_updateWindowResize();
	extern void PLEX_restoreWindow();

	// changes the aspect ratio of the screen. Relatively untested but it should NOT effect the unit size in a serious way and should scale normally. Of course, may need to be tested more.

	extern void PLEX_changeScreenRatio(const double ratio);

	// starts the window, updates the window, ends the window. These also you never need to call manually because they are called in PLEX_start, PLEX_update, and PLEX_end.

	extern bool PLEX_startWindow(const char* const title, const uint32_t width, const uint32_t height, const double ratio);
	extern bool PLEX_updateWindow();
	extern void PLEX_endWindow();

	// title stuff

	extern void PLEX_setWindowTitle(const char* const title);

	// returns whether or not the window is resizing

	extern bool PLEX_isSizing();

	// converts to a scaleable plex unit. This is CRUCIAL if you want things to adjust to the window's size and resolution. This takes in a double so it allows a fraction. Essentially, if you're running in a 800x600 resolution this 25 pixels width or height.

	extern double PLEX_UNIT(const double count);

	// returns pixel value of percentage of window width or window height

	extern uint32_t PLEX_TRANS_W(const double perc);
	extern uint32_t PLEX_TRANS_H(const double perc);

	// returns the width and height of the window. If the 'offset' is true in either of these then instead of returning the entire window width or height it returns the window width or height beyond the offset (ie. border area)

	extern uint32_t PLEX_WW(const bool offset);
	extern uint32_t PLEX_WH(const bool offset);

	// returns the pixel width or height of the offset (bordered area)

	extern uint32_t PLEX_WOFFSET();
	extern uint32_t PLEX_HOFFSET();

	// returns the percentage of the screen that's the offset.

	extern double PLEX_WOFFSETPERC();
	extern double PLEX_HOFFSETPERC();

	// returns the percentage of the screen that's not the offset.

	extern double PLEX_WCROPPERC();
	extern double PLEX_HCROPPERC();

	// returns unit maxes based on the screen border size

	extern uint32_t PLEX_WIDTHUNITMAX();
	extern uint32_t PLEX_HEIGHTUNITMAX();

	// returns the SDL_Window contents. Use very sparringly.

	extern SDL_Window* PLEX_getWindowContents();

	// returns a string which contains the windows title

	extern const char* PLEX_getWindowTitle();
#ifdef __cplusplus
}
#endif

#endif
