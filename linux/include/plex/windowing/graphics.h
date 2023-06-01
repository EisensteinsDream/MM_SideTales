#ifndef PLEX_GRAPHICS_H
#define PLEX_GRAPHICS_H

#include "windowing/texture.h"
#include "windowing/input.h"

#include "mem/expandarray.h"

// This unit is where all the functions that are used to draw on the screen.

// NOTE: this is a very important unit, of course. There are some libraries like 'animation' and 'sprite theater' that streamline this process. But these functions are the most direct ways to get things drawmn.

#ifdef __cplusplus
extern "C"
{
#endif

// --- SET DRAW ATTRIBUTE FUNCTIONS ---

	extern void PLEX_drawBlendMode(const SDL_BlendMode mode);

// --- CLEAR SCREEN FUNCTIONS ---

	extern void PLEX_clear(const size_t order, const uint8_t r, const uint8_t g, const uint8_t b);

	extern void PLEX_clearAlpha(const size_t order, const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a);

	extern void PLEX_clearOffsetArea(const size_t order, const uint8_t r, const uint8_t g, const uint8_t b);

	extern void PLEX_clearOffsetAreaAlpha(const size_t order, const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a);

	extern void PLEX_fillRect(const size_t order, const int64_t x, const int64_t y, const int64_t w, const int64_t h, const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a, const bool offset);

// --- BASIC DRAW FUNCTIONS ---

	extern void PLEX_drawRect(const size_t order, const int64_t x, const int64_t y, const int64_t w, const int64_t h, const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a, const uint8_t thickness, const bool offset);

	extern void PLEX_drawLine(const size_t order, const int64_t x, const int64_t y, const int64_t x2, const int64_t y2, const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a, const uint8_t thickness, const bool offset);

	extern void PLEX_drawLine2(const size_t order, const int64_t x, const int64_t y, const int64_t length, const double direction, const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a, const uint8_t thickness, const bool offset);

	extern void PLEX_drawLine3(const size_t order, const PLEX_LINE line, const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a, const uint8_t thickness, const bool offset);

// --- TEXTURE BASED DRAW FUNCTIONS ---

	extern void PLEX_printTexture(const size_t order, const size_t texture, const int64_t fx, const int64_t fy, const int64_t fw, const int64_t fh, const int64_t tx, const int64_t ty, const int64_t tw, const int64_t th, const bool offset);

	extern void PLEX_printTextureTinted(const size_t order, const size_t texture, const int64_t fx, const int64_t fy, const int64_t fw, const int64_t fh, const int64_t tx, const int64_t ty, const int64_t tw, const int64_t th, const uint8_t r, const uint8_t g, const uint8_t b, const bool offset);

	extern void PLEX_printFontLetter(const size_t order, const double wPerc, const double hPerc, const int64_t ox, const int64_t oy, const int64_t ow, const int64_t oh, const uint8_t symbol, const char* const alphaString, const size_t texture, const bool offset);

	extern void PLEX_printFontLetterTinted(const size_t order, const double wPerc, const double hPerc, const int64_t ox, const int64_t oy, const int64_t ow, const int64_t oh, const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t symbol, const char* const alphaString, const size_t texture, const bool offset);

	extern void PLEX_printFontString(const size_t order, const double wPerc, const double hPerc, const int64_t ox, const int64_t oy, const int64_t ow, const int64_t oh, const char* const string, const char* const alphaString, const size_t texture, const bool offset);

	extern void PLEX_printFontStringTinted(const size_t order, const double wPerc, const double hPerc, const int64_t ox, const int64_t oy, const int64_t ow, const int64_t oh, const uint8_t r, const uint8_t g, const uint8_t b, const char* const string, const char* const alphaString, const size_t texture, const bool offset);

// --- WIDGET DRWAING FUNCTIONS ---

	extern bool PLEX_drawArrowButton(const size_t order, const int64_t x, const int64_t y, const int64_t w, const int64_t h, const uint8_t fr, const uint8_t fg, const uint8_t fb, const uint8_t fa, const uint8_t br, const uint8_t bg, const uint8_t bb, const uint8_t ba, const uint8_t thickness, const bool flip, const bool offset);

	extern void PLEX_drawIcon(const size_t order, const int64_t x, const int64_t y, const int64_t w, const int64_t h, const uint8_t br, const uint8_t bg, const uint8_t bb, const uint8_t ba, const size_t texture, const int64_t tx, const int64_t ty, const int64_t tw, const int64_t th, const uint8_t thickness, const bool offset);

	extern void PLEX_drawIconTinted(const size_t order, const int64_t x, const int64_t y, const int64_t w, const int64_t h, const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t br, const uint8_t bg, const uint8_t bb, const uint8_t ba, const size_t texture, const int64_t tx, const int64_t ty, const int64_t tw, const int64_t th, const uint8_t thickness, const bool offset);

	extern void PLEX_drawLabel(const size_t order, const char* const message, const double wPerc, const double hPerc, const uint8_t br, const uint8_t bg, const uint8_t bb, const uint8_t ba, const int64_t x, const int64_t y, const int64_t w, const int64_t h, const double ipx, const double ipy, const double ipw, const double iph, const uint8_t thickness, const char* const alphaString, const size_t fontTexture, const size_t texture, const int64_t tx, const int64_t ty, const int64_t tw, const int64_t th, const bool offset);

	extern void PLEX_drawTooltip(const size_t order, const char* const message, const double wPerc, const double hPerc, const uint8_t br, const uint8_t bg, const uint8_t bb, const uint8_t ba, const int64_t x, const int64_t y, const int64_t w, const int64_t h, const int64_t dx, const int64_t dy, const int64_t lw, const int64_t lh, const double ipx, const double ipy, const double ipw, const double iph, const uint8_t thickness, const char* const alphaString, const size_t fontTexture, const size_t texture, const int64_t tx, const int64_t ty, const int64_t tw, const int64_t th, const bool offset);

	extern void PLEX_drawTextBox(const size_t order, const char* const message, const double wPerc, const double hPerc, const uint8_t fr, const uint8_t fg, const uint8_t fb, const uint8_t fa, const int64_t x, const int64_t y, const int64_t w, const int64_t h, const uint32_t fw, const uint32_t fh, const double ipx, const double ipy, const double ipw, const double iph, const uint8_t thickness, const char* const alphaString, const size_t fontTexture, const size_t texture, const int64_t tx, const int64_t ty, const int64_t tw, const int64_t th, const double lineSpacePerc, const bool offset);

	extern void PLEX_drawTextBoxTinted(const size_t order, const char* const message, const double wPerc, const double hPerc, const uint8_t fr, const uint8_t fg, const uint8_t fb, const uint8_t fa, const uint8_t br, const uint8_t bg, const uint8_t bb, const uint8_t ba, const int64_t x, const int64_t y, const int64_t w, const int64_t h, const uint32_t fw, const uint32_t fh, const double ipx, const double ipy, const double ipw, const double iph, const uint8_t thickness, const char* const alphaString, const size_t fontTexture, const size_t texture, const int64_t tx, const int64_t ty, const int64_t tw, const int64_t th, const double lineSpacePerc, const bool offset);

	extern bool PLEX_drawButton(const size_t order, const char* const message, const double wPerc, const double hPerc, const int64_t x, const int64_t y, const int64_t w, const int64_t h, const int64_t tx, const int64_t ty, const int64_t tw, const int64_t th, const uint8_t fr, const uint8_t fg, const uint8_t fb, const uint8_t fa, const uint8_t br, const uint8_t bg, const uint8_t bb, const uint8_t ba, const uint8_t thickness, const char* const alphaString, const size_t font, const size_t texture, const bool offset);

	extern bool PLEX_drawIconButton(const size_t order, const bool selected, const int64_t x, const int64_t y, const int64_t w, const int64_t h, const int64_t tx, const int64_t ty, const int64_t tw, const int64_t th, const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a, const uint8_t sr, const uint8_t sg, const uint8_t sb, const uint8_t sa, const uint8_t ovr, const int64_t ovb, const int64_t ovg, const int64_t ova, const uint8_t sovr, const int64_t sovb, const int64_t sovg, const int64_t sova, const uint8_t thickness, const size_t texture, const bool offset);

	extern bool PLEX_twoImageButton(const size_t order, const bool selected, const int64_t x, const int64_t y, const int64_t w, const int64_t h, const int64_t tx, const int64_t ty, const int64_t tw, const int64_t th, const int64_t tx2, const int64_t ty2, const int64_t tw2, const int64_t th2, const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a, const uint8_t sr, const uint8_t sg, const uint8_t sb, const uint8_t sa, const uint8_t thickness, const size_t texture1, const size_t texture2, const bool offset);

	extern void PLEX_drawChargeBar(const size_t order, const double fullness, const int64_t x, const int64_t y, const int64_t w, const int64_t h, const uint8_t fr, const uint8_t fg, const uint8_t fb, const uint8_t fa, const uint8_t br, const uint8_t bg, const uint8_t bb, const uint8_t ba, const uint8_t thickness, const bool vertical, const bool offset);

	// --- SPECIALIZED DRAW FUNCTIONS ---

	extern void PLEX_drawPanorama(const size_t order, const size_t texture);

	// --- FUNCTIONS TO HIGHLIGHT WIDGETS ---

	extern void PLEX_showAllTooltips();
	extern void PLEX_showTogglesTooltips();
	extern void PLEX_showAllClickables();

	// --- FUNCTIONS TO SET AND UNSET A CURRENT DRAW TARGET --

	extern void PLEX_setTarget(const size_t texture);
	extern void PLEX_unsetTarget();

	// -- EXTRA FUNCTIONS ---

	extern size_t PLEX_takeScreencap(const bool offset);

	// starts, updates, and ends the graphics system. PLEX_start, PLEX_update, and PLEX_end manages these. Typically, you never need to call these.

	extern bool PLEX_startGraphics();
	extern bool PLEX_updateGraphics(SDL_Window* window, const uint32_t w, const uint32_t h);
	extern void PLEX_endGraphics();
#ifdef __cplusplus
}
#endif

#endif
