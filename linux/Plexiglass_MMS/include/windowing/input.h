#ifndef PLEX_INPUT_H
#define PLEX_INPUT_H

#include "windowing/window.h"

#include "utility/math.h"
#include "utility/timer.h"

#include "terminate.h"

#ifdef __cplusplus
extern "C"
{
#endif
	// enables or disables player pausing

	extern void PLEX_disablePause();
	extern void PLEX_enablePause();

	// hides the cursor, unhides the cursor, or toggles the cursor hidden or not hidden (experimental/poorly tested)

	extern void PLEX_hideCursor();
	extern void PLEX_unhideCursor();
	extern void PLEX_toggleHideCursor();

	extern bool PLEX_cursorIsHidden();

	// sets up a custom cursor or tinted custom cursor (experimental/poorly tested)
	extern void PLEX_setCustomCursor(const size_t tex, const int64_t w, const int64_t h, const int64_t fx, const int64_t fy, const int64_t fw, const int64_t fh);
	extern void PLEX_setCustomCursorTinted(const size_t tex, const int64_t w, const int64_t h, const int64_t fx, const int64_t fy, const int64_t fw, const int64_t fh, const uint8_t r, const uint8_t g, const uint8_t b);

	// sets different custom cursor textures for click and down (experimental/poorly tested)

	extern void PLEX_setCustomCursorClick(const size_t tex);
	extern void PLEX_setCustomCursorDown(const size_t tex);

	// turns off the custom cursor (experimental/poorly tested)

	extern void PLEX_unsetCustomCursor();

	// starts, updates, and ends the input system. Typically, you never need to call these becaue PLEX_start, PLEX_update, PLEX_end manages all this.

	extern bool PLEX_startInput();
	extern bool PLEX_updateInput();
	extern void PLEX_endInput();

	// returns whether or not pausing is disabled

	extern bool PLEX_pauseDisabled();

	// returns the dimensions of the custom cursor. wh == draw size. fx, fy, fw, fh == crop dimensions

	extern void PLEX_cursorDimens(uint32_t* w, uint32_t* h, uint32_t* fx, uint32_t* fy, uint32_t* fw, uint32_t* fh);

	// returns whether or not you're using the custom cursor. (experimental/poorly tested)

	extern bool PLEX_usingCustomCursor();

	// returns the texture of the custom cursor.  (experimental/poorly tested)

	extern size_t PLEX_customCursor();

	// by ascii value whether a button is down or pressed

	extern bool PLEX_KEY_down(const uint8_t key);
	extern bool PLEX_KEY_press(const uint8_t key);

	extern bool PLEX_KEY_anyPress();

	// returns whether a function key is down or pressed

	extern bool PLEX_KEY_fdown(const size_t number);
	extern bool PLEX_KEY_fpress(const size_t number);

	// sees wehther or not the mouse cusor is within region

	extern bool PLEX_mouseInside(const int64_t x, const int64_t y, const uint32_t w, const uint32_t h, const bool offset);

	extern void PLEX_clearMouseClicks();

	// checks mod keys

	extern bool PLEX_KEY_rshift();
	extern bool PLEX_KEY_lshift();
	extern bool PLEX_KEY_shift();

	extern bool PLEX_KEY_rctrl();
	extern bool PLEX_KEY_lctrl();
	extern bool PLEX_KEY_ctrl();

	// returns whether or not arrow keys are down

	extern bool PLEX_upKey();
	extern bool PLEX_leftKey();
	extern bool PLEX_downKey();
	extern bool PLEX_rightKey();

	// returns whether or not arrow keys are pressed

	extern bool PLEX_upKeyPress();
	extern bool PLEX_leftKeyPress();
	extern bool PLEX_downKeyPress();
	extern bool PLEX_rightKeyPress();

	// returns the mouse x and mouse y

	extern int64_t PLEX_MX();
	extern int64_t PLEX_MY();

	// checks mouse clicks and holds

	extern bool PLEX_left();
	extern bool PLEX_leftClick();

	extern bool PLEX_right();
	extern bool PLEX_rightClick();

	extern bool PLEX_midButton(); // < experimental/untested
	extern bool PLEX_midButtonClick(); // < experimental/untested

	extern double PLEX_mouseScroll(); // < experimental/untested

	extern bool PLEX_mouseMoved();

	// checks whether a button is down or pressed. Controller is the controller ID (usually 0).

	extern bool PLEX_CONTROLLER_down(const size_t controller, const uint8_t button);
	extern bool PLEX_CONTROLLER_press(const size_t controller, const uint8_t button);

	// determines the direction of a controller joystick. Controller is the controller ID (usually 0).

	extern bool PLEX_CONTROLLER_axis(const size_t controller, const size_t axisNum, const double sensitivity);

	extern bool PLEX_CONTROLLER_axisUp(const size_t controller);
	extern bool PLEX_CONTROLLER_axisRight(const size_t controller);
	extern bool PLEX_CONTROLLER_axisDown(const size_t controller);
	extern bool PLEX_CONTROLLER_axisLeft(const size_t controller);

	// returns the number of available controllers

	extern size_t PLEX_CONTROLLER_count();
#ifdef __cplusplus
}
#endif

#endif
