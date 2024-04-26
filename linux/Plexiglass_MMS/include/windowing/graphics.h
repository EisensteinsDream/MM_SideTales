#ifndef PLEX_GRAPHICS_H
#define PLEX_GRAPHICS_H

#include "windowing/texture.h"
#include "windowing/input.h"

#include "mem/expandarray.h"

// This unit is where all the functions that are used to draw on the screen.

// NOTE: this is a very important unit, of course. There are some libraries like 'animation' and 'sprite theater' that streamline this process. But these functions are the most direct ways to get things drawn.

	#define PLEX_clear(order, r, g, b) PLEX_clearInt(order, r, g, b, __LINE__, __FILE__)
	#define PLEX_clearAlpha(order, r, g, b, a) PLEX_clearAlphaInt(order, r, g, b, a, __LINE__, __FILE__)
	#define PLEX_clearOffsetArea(order, r, g, b) PLEX_clearOffsetAreaInt(order, r, g, b, __LINE__, __FILE__)
	#define PLEX_clearOffsetAreaAlpha(order, r, g, b, a) PLEX_clearOffsetAreaAlphaInt(order, r, g, b, a, __LINE__, __FILE__)
	#define PLEX_fillRect(order, x, y, w, h, r, g, b, a, offset) PLEX_fillRectInt(order, x, y, w, h, r, g, b, a, offset, __LINE__, __FILE__)
	#define PLEX_drawRect(order, x, y, w, h, r, g, b, a, thickness, offset) PLEX_drawRectInt(order, x, y, w, h, r, g, b, a, thickness, offset, __LINE__, __FILE__)
	#define PLEX_drawLine(order, x, y, x2, y2, r, g, b, a, thickness, offset) PLEX_drawLineInt(order, x, y, x2, y2, r, g, b, a, thickness, offset, __LINE__, __FILE__)
	#define PLEX_drawLine2(order, x, y, length, direction, r, g, b, a, thickness, offset) PLEX_drawLine2Int(order, x, y, length, direction, r, g, b, a, thickness, offset, __LINE__, __FILE__)
	#define PLEX_drawLine3(order, line, r, g, b, a, thickness, offset) PLEX_drawLine3Int(order, line, r, g, b, a, thickness, offset, __LINE__, __FILE__)
	#define PLEX_printTexture(order, texture, fx, fy, fw, fh, tx, ty, tw, th, offset) PLEX_printTextureInt(order, texture, fx, fy, fw, fh, tx, ty, tw, th, offset, __LINE__, __FILE__)
	#define PLEX_printTextureTinted(order, texture, fx, fy, fw, fh, tx, ty, tw, th, r, g, b, offset) PLEX_printTextureTintedInt(order, texture, fx, fy, fw, fh, tx, ty, tw, th, r, g, b, offset, __LINE__, __FILE__)
	#define PLEX_printTextureRotated(order, texture, fx, fy, fw, fh, tx, ty, tw, th, angle, offset) PLEX_printTextureRotatedInt(order, texture, fx, fy, fw, fh, tx, ty, tw, th, angle, offset, __LINE__, __FILE__)
	#define PLEX_printFontLetter(order, wPerc, hPerc, ox, oy, ow, oh, symbol, alphaString, texture, offset) PLEX_printFontLetterInt(order, wPerc, hPerc, ox, oy, ow, oh, symbol, alphaString, texture, offset, __LINE__, __FILE__)
	#define PLEX_printFontLetterTinted(order, wPerc, hPerc, ox, oy, ow, oh, r, g, b, symbol, alphaString, texture, offset) PLEX_printFontLetterTintedInt(order, wPerc, hPerc, ox, oy, ow, oh, r, g, b, symbol, alphaString, texture, offset, __LINE__, __FILE__)
	#define PLEX_printFontString(order, wPerc, hPerc, ox, oy, ow, oh, string, alphaString, texture, offset) PLEX_printFontStringInt(order, wPerc, hPerc, ox, oy, ow, oh, string, alphaString, texture, offset, __LINE__, __FILE__)
	#define PLEX_printFontStringTinted(order, wPerc, hPerc, ox, oy, ow, oh, r, g, b, string, alphaString, texture, offset) PLEX_printFontStringTintedInt(order, wPerc, hPerc, ox, oy, ow, oh, r, g, b, string, alphaString, texture, offset, __LINE__, __FILE__)
	#define PLEX_drawArrowButton(order, x, y, w, h, fr, fg, fb, fa, br, bg, bb, ba, thickness, flip, offset) PLEX_drawArrowButtonInt(order, x, y, w, h, fr, fg, fb, fa, br, bg, bb, ba, thickness, flip, offset, __LINE__, __FILE__)
	#define PLEX_drawIcon(order, x, y, w, h, br, bg, bb, ba, texture, tx, ty, tw, th, thickness, offset) PLEX_drawIconInt(order, x, y, w, h, br, bg, bb, ba, texture, tx, ty, tw, th, thickness, offset, __LINE__, __FILE__)
	#define PLEX_drawIconTinted(order, x, y, w, h, r, g, b, br, bg, bb, ba, texture, tx, ty, tw, th, thickness, offset) PLEX_drawIconTintedInt(order, x, y, w, h, r, g, b, br, bg, bb, ba, texture, tx, ty, tw, th, thickness, offset, __LINE__, __FILE__)
	#define PLEX_drawLabel(order, message, wPerc, hPerc, fr, fg, fb, fa, x, y, w, h, ipx, ipy, ipw, iph, thickness, alphaString, fontTexture, texture, tx, ty, tw, th, offset) PLEX_drawLabelInt(order, message, wPerc, hPerc, fr, fg, fb, fa, x, y, w, h, ipx, ipy, ipw, iph, thickness, alphaString, fontTexture, texture, tx, ty, tw, th, offset, __LINE__, __FILE__)
	#define PLEX_drawTooltip(order, message, wPerc, hPerc, br, bg, bb, ba, hx, hy, hw, hh, dx, dy, lw, lh, ipx, ipy, ipw, iph, thickness, alphaString, fontTexture, texture, tx, ty, tw, th, offset) PLEX_drawTooltipInt(order, message, wPerc, hPerc, br, bg, bb, ba, hx, hy, hw, hh, dx, dy, lw, lh, ipx, ipy, ipw, iph, thickness, alphaString, fontTexture, texture, tx, ty, tw, th, offset, __LINE__, __FILE__)
	#define PLEX_drawTextBox(order, message, wPerc, hPerc, fr, fg, fb, fa, x, y, w, h, fw, fh, ipx, ipy, ipw, iph, thickness, alphaString, fontTexture, texture, tx, ty, tw, th, lineSpacePerc, offset) PLEX_drawTextBoxInt(order, message, wPerc, hPerc, fr, fg, fb, fa, x, y, w, h, fw, fh, ipx, ipy, ipw, iph, thickness, alphaString, fontTexture, texture, tx, ty, tw, th, lineSpacePerc, offset, __LINE__, __FILE__)
	#define PLEX_drawTextBoxTinted(order, message, wPerc, hPerc, fr, fg, fb, fa, br, bg, bb, ba, x, y, w, h, fw, fh, ipx, ipy, ipw, iph, thickness, alphaString, fontTexture, texture, tx, ty, tw, th, lineSpacePerc, offset) PLEX_drawTextBoxTintedInt(order, message, wPerc, hPerc, fr, fg, fb, fa, br, bg, bb, ba, x, y, w, h, fw, fh, ipx, ipy, ipw, iph, thickness, alphaString, fontTexture, texture, tx, ty, tw, th, lineSpacePerc, offset, __LINE__, __FILE__)
	#define PLEX_drawTextBoxScrolling(order, message, wPerc, hPerc, fr, fg, fb, fa, x, y, w, h, fw,  fh, ipx, ipy, ipw, iph, thickness, alphaString, fontTexture, texture, tx, ty, tw, th, lineSpacePerc, pageScroll, offset) PLEX_drawTextBoxScrollingInt(order, message, wPerc, hPerc, fr, fg, fb, fa, x, y, w, h, fw, fh, ipx, ipy, ipw, iph, thickness, alphaString, fontTexture, texture, tx, ty, tw, th, lineSpacePerc, pageScroll, offset, __LINE__, __FILE__)
	#define PLEX_drawTextBoxScrollingTinted(order, message, wPerc, hPerc, fr, fg, fb, fa, br, bg, bb, ba, x, y, w, h, fw, fh, ipx, ipy, ipw, iph, thickness, alphaString, fontTexture, texture, tx, ty, tw, th, lineSpacePerc, pageScroll, offset) PLEX_drawTextBoxScrollingTintedInt(order, message, wPerc, hPerc, fr, fg, fb, fa, br, bg, bb, ba, x, y, w, h, fw, fh, ipx, ipy, ipw, iph, thickness, alphaString, fontTexture, texture, tx, ty, tw, th, lineSpacePerc, pageScroll, offset, __LINE__, __FILE__)
	#define PLEX_drawButton(order, message, wPerc, hPerc, x, y, w, h, tx, ty, tw, th, fr, fg, fb, fa, br, bg, bb, ba, thickness, alphaString, font, texture, offset) PLEX_drawButtonInt(order, message, wPerc, hPerc, x, y, w, h, tx, ty, tw, th, fr, fg, fb, fa, br, bg, bb, ba, thickness, alphaString, font, texture, offset, __LINE__, __FILE__)
	#define PLEX_drawIconButton(order, selected, x, y, w, h, tx, ty, tw, th, r, g, b, a, sr, sg, sb, sa, ovr, ovb, ovg, ova, sovr, sovb, sovg, sova, thickness, texture, offset) PLEX_drawIconButtonInt(order, selected, x, y, w, h, tx, ty, tw, th, r, g, b, a, sr, sg, sb, sa, ovr, ovb, ovg, ova, sovr, sovb, sovg, sova, thickness, texture, offset, __LINE__, __FILE__)
	#define PLEX_twoImageButton(order, selected, x, y, w, h, tx, ty, tw, th, tx2, ty2, tw2, th2, r, g, b, a, sr, sg, sb, sa, thickness, texture1, texture2, offset) PLEX_twoImageButtonInt(order, selected, x, y, w, h, tx, ty, tw, th, tx2, ty2, tw2, th2, r, g, b, a, sr, sg, sb, sa, thickness, texture1, texture2, offset, __LINE__, __FILE__)
	#define PLEX_drawChargeBar(order, fullness, x, y, w, h, fr, fg, fb, fa, br, bg, bb, ba, thickness, vertical, offset) PLEX_drawChargeBarInt(order, fullness, x, y, w, h, fr, fg, fb, fa, br, bg, bb, ba, thickness, vertical, offset, __LINE__, __FILE__)
	#define PLEX_drawPanorama(order, texture) PLEX_drawPanoramaInt(order, texture, __LINE__, __FILE__)
	#define PLEX_takeScreencap(offset) PLEX_takeScreencapInt(offset, __LINE__, __FILE__)

#ifdef __cplusplus
extern "C"
{
#endif

// --- SET DRAW ATTRIBUTE FUNCTIONS ---

	extern void PLEX_drawBlendMode(const SDL_BlendMode mode);

// --- CLEAR SCREEN FUNCTIONS ---

	extern void PLEX_setRefreshColor(const uint8_t r, const uint8_t g, const uint8_t b);

	extern void PLEX_clearInt(const size_t order, const uint8_t r, const uint8_t g, const uint8_t b, const size_t line, const char* const errorfile);

	extern void PLEX_clearAlphaInt(const size_t order, const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a, const size_t line, const char* const errorfile);

	extern void PLEX_clearOffsetAreaInt(const size_t order, const uint8_t r, const uint8_t g, const uint8_t b, const size_t line, const char* const errorfile);

	extern void PLEX_clearOffsetAreaAlphaInt(const size_t order, const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a, const size_t line, const char* const errorfile);

	extern void PLEX_fillRectInt(const size_t order, const int64_t x, const int64_t y, const int64_t w, const int64_t h, const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a, const bool offset, const size_t line, const char* const errorfile);

// --- BASIC DRAW FUNCTIONS ---

	extern void PLEX_drawRectInt(const size_t order, const int64_t x, const int64_t y, const int64_t w, const int64_t h, const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a, const uint8_t thickness, const bool offset, const size_t line, const char* const errorfile);

	extern void PLEX_drawLineInt(const size_t order, const int64_t x, const int64_t y, const int64_t x2, const int64_t y2, const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a, const uint8_t thickness, const bool offset, const size_t line, const char* const errorfile);

	extern void PLEX_drawLine2Int(const size_t order, const int64_t x, const int64_t y, const int64_t length, const double direction, const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a, const uint8_t thickness, const bool offset, const size_t line, const char* const errorfile);

	extern void PLEX_drawLine3Int(const size_t order, const PLEX_LINE line, const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a, const uint8_t thickness, const bool offset, const size_t errorline, const char* const errorfile);

// --- TEXTURE BASED DRAW FUNCTIONS ---

	extern void PLEX_printTextureInt(const size_t order, const size_t texture, const int64_t fx, const int64_t fy, const int64_t fw, const int64_t fh, const int64_t tx, const int64_t ty, const int64_t tw, const int64_t th, const bool offset, const size_t line, const char* const errorfile);

	extern void PLEX_printTextureTintedInt(const size_t order, const size_t texture, const int64_t fx, const int64_t fy, const int64_t fw, const int64_t fh, const int64_t tx, const int64_t ty, const int64_t tw, const int64_t th, const uint8_t r, const uint8_t g, const uint8_t b, const bool offset, const size_t line, const char* const errorfile);

	extern void PLEX_printTextureRotatedInt(const size_t order, const size_t texture, const int64_t fx, const int64_t fy, const int64_t fw, const int64_t fh, const int64_t tx, const int64_t ty, const int64_t tw, const int64_t th, const double angle, const bool offset, const size_t line, const char* const errorfile);

	extern void PLEX_printFontLetterInt(const size_t order, const double wPerc, const double hPerc, const int64_t ox, const int64_t oy, const int64_t ow, const int64_t oh, const uint8_t symbol, const char* const alphaString, const size_t texture, const bool offset, const size_t line, const char* const errorfile);

	extern void PLEX_printFontLetterTintedInt(const size_t order, const double wPerc, const double hPerc, const int64_t ox, const int64_t oy, const int64_t ow, const int64_t oh, const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t symbol, const char* const alphaString, const size_t texture, const bool offset, const size_t line, const char* const errorfile);

	extern void PLEX_printFontStringInt(const size_t order, const double wPerc, const double hPerc, const int64_t ox, const int64_t oy, const int64_t ow, const int64_t oh, const char* const string, const char* const alphaString, const size_t texture, const bool offset, const size_t line, const char* const errorfile);

	extern void PLEX_printFontStringTintedInt(const size_t order, const double wPerc, const double hPerc, const int64_t ox, const int64_t oy, const int64_t ow, const int64_t oh, const uint8_t r, const uint8_t g, const uint8_t b, const char* const string, const char* const alphaString, const size_t texture, const bool offset, const size_t line, const char* const errorfile);

// --- WIDGET DRWAING FUNCTIONS ---

	extern bool PLEX_drawArrowButtonInt(const size_t order, const int64_t x, const int64_t y, const int64_t w, const int64_t h, const uint8_t fr, const uint8_t fg, const uint8_t fb, const uint8_t fa, const uint8_t br, const uint8_t bg, const uint8_t bb, const uint8_t ba, const uint8_t thickness, const bool flip, const bool offset, const size_t line, const char* const errorfile);

	extern void PLEX_drawIconInt(const size_t order, const int64_t x, const int64_t y, const int64_t w, const int64_t h, const uint8_t br, const uint8_t bg, const uint8_t bb, const uint8_t ba, const size_t texture, const int64_t tx, const int64_t ty, const int64_t tw, const int64_t th, const uint8_t thickness, const bool offset, const size_t line, const char* const errorfile);

	extern void PLEX_drawIconTintedInt(const size_t order, const int64_t x, const int64_t y, const int64_t w, const int64_t h, const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t br, const uint8_t bg, const uint8_t bb, const uint8_t ba, const size_t texture, const int64_t tx, const int64_t ty, const int64_t tw, const int64_t th, const uint8_t thickness, const bool offset, const size_t line, const char* const errorfile);

	extern void PLEX_drawLabelInt(const size_t order, const char* const message, const double wPerc, const double hPerc, const uint8_t fr, const uint8_t fg, const uint8_t fb, const uint8_t fa, const int64_t x, const int64_t y, const int64_t w, const int64_t h, const double ipx, const double ipy, const double ipw, const double iph, const uint8_t thickness, const char* const alphaString, const size_t fontTexture, const size_t texture, const int64_t tx, const int64_t ty, const int64_t tw, const int64_t th, const bool offset, const size_t line, const char* const errorfile);

	extern void PLEX_drawTooltipInt(const size_t order, const char* const message, const double wPerc, const double hPerc, const uint8_t br, const uint8_t bg, const uint8_t bb, const uint8_t ba, const int64_t hx, const int64_t hy, const int64_t hw, const int64_t hh, const int64_t dx, const int64_t dy, const int64_t lw, const int64_t lh, const double ipx, const double ipy, const double ipw, const double iph, const uint8_t thickness, const char* const alphaString, const size_t fontTexture, const size_t texture, const int64_t tx, const int64_t ty, const int64_t tw, const int64_t th, const bool offset, const size_t line, const char* const errorfile);

	extern void PLEX_drawTextBoxInt(const size_t order, const char* const message, const double wPerc, const double hPerc, const uint8_t fr, const uint8_t fg, const uint8_t fb, const uint8_t fa, const int64_t x, const int64_t y, const int64_t w, const int64_t h, const uint32_t fw, const uint32_t fh, const double ipx, const double ipy, const double ipw, const double iph, const uint8_t thickness, const char* const alphaString, const size_t fontTexture, const size_t texture, const int64_t tx, const int64_t ty, const int64_t tw, const int64_t th, const double lineSpacePerc, const bool offset, const size_t line, const char* const errorfile);

	extern void PLEX_drawTextBoxTintedInt(const size_t order, const char* const message, const double wPerc, const double hPerc, const uint8_t fr, const uint8_t fg, const uint8_t fb, const uint8_t fa, const uint8_t br, const uint8_t bg, const uint8_t bb, const uint8_t ba, const int64_t x, const int64_t y, const int64_t w, const int64_t h, const uint32_t fw, const uint32_t fh, const double ipx, const double ipy, const double ipw, const double iph, const uint8_t thickness, const char* const alphaString, const size_t fontTexture, const size_t texture, const int64_t tx, const int64_t ty, const int64_t tw, const int64_t th, const double lineSpacePerc, const bool offset, const size_t line, const char* const errorfile);

	extern size_t PLEX_drawTextBoxScrollingInt(const size_t order, const char* const message, const double wPerc, const double hPerc, const uint8_t fr, const uint8_t fg, const uint8_t fb, const uint8_t fa, const int64_t x, const int64_t y, const int64_t w, const int64_t h, const uint32_t fw, const uint32_t fh, const double ipx, const double ipy, const double ipw, const double iph, const uint8_t thickness, const char* const alphaString, const size_t fontTexture, const size_t texture, const int64_t tx, const int64_t ty, const int64_t tw, const int64_t th, const double lineSpacePerc, const size_t pageScroll, const bool offset, const size_t line, const char* const errorfile);

	extern size_t PLEX_drawTextBoxScrollingTintedInt(const size_t order, const char* const message, const double wPerc, const double hPerc, const uint8_t fr, const uint8_t fg, const uint8_t fb, const uint8_t fa, const uint8_t br, const uint8_t bg, const uint8_t bb, const uint8_t ba, const int64_t x, const int64_t y, const int64_t w, const int64_t h, const uint32_t fw, const uint32_t fh, const double ipx, const double ipy, const double ipw, const double iph, const uint8_t thickness, const char* const alphaString, const size_t fontTexture, const size_t texture, const int64_t tx, const int64_t ty, const int64_t tw, const int64_t th, const double lineSpacePerc, const size_t pageScroll, const bool offset, const size_t line, const char* const errorfile);

	extern bool PLEX_drawButtonInt(const size_t order, const char* const message, const double wPerc, const double hPerc, const int64_t x, const int64_t y, const int64_t w, const int64_t h, const int64_t tx, const int64_t ty, const int64_t tw, const int64_t th, const uint8_t fr, const uint8_t fg, const uint8_t fb, const uint8_t fa, const uint8_t br, const uint8_t bg, const uint8_t bb, const uint8_t ba, const uint8_t thickness, const char* const alphaString, const size_t font, const size_t texture, const bool offset, const size_t line, const char* const errorfile);

	extern bool PLEX_drawIconButtonInt(const size_t order, const bool selected, const int64_t x, const int64_t y, const int64_t w, const int64_t h, const int64_t tx, const int64_t ty, const int64_t tw, const int64_t th, const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a, const uint8_t sr, const uint8_t sg, const uint8_t sb, const uint8_t sa, const uint8_t ovr, const int64_t ovb, const int64_t ovg, const int64_t ova, const uint8_t sovr, const int64_t sovb, const int64_t sovg, const int64_t sova, const uint8_t thickness, const size_t texture, const bool offset, const size_t line, const char* const errorfile);

	extern bool PLEX_twoImageButtonInt(const size_t order, const bool selected, const int64_t x, const int64_t y, const int64_t w, const int64_t h, const int64_t tx, const int64_t ty, const int64_t tw, const int64_t th, const int64_t tx2, const int64_t ty2, const int64_t tw2, const int64_t th2, const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a, const uint8_t sr, const uint8_t sg, const uint8_t sb, const uint8_t sa, const uint8_t thickness, const size_t texture1, const size_t texture2, const bool offset, const size_t line, const char* const errorfile);

	extern void PLEX_drawChargeBarInt(const size_t order, const double fullness, const int64_t x, const int64_t y, const int64_t w, const int64_t h, const uint8_t fr, const uint8_t fg, const uint8_t fb, const uint8_t fa, const uint8_t br, const uint8_t bg, const uint8_t bb, const uint8_t ba, const uint8_t thickness, const bool vertical, const bool offset, const size_t line, const char* const errorfile);

	// --- SPECIALIZED DRAW FUNCTIONS ---

	extern void PLEX_drawPanoramaInt(const size_t order, const size_t texture, const size_t line, const char* const errorfile);

	// --- FUNCTIONS TO HIGHLIGHT WIDGETS ---

	extern void PLEX_showAllTooltips();
	extern void PLEX_showTogglesTooltips();
	extern void PLEX_showAllClickables();

	// --- FUNCTIONS TO SET AND UNSET A CURRENT DRAW TARGET --

	extern void PLEX_setTextureTarget(const size_t texture);
	extern void PLEX_unsetTextureTarget();

	// -- EXTRA FUNCTIONS ---

	extern size_t PLEX_takeScreencapInt(const bool offset, const size_t errorline, const char* const errorfile);

	// starts, updates, and ends the graphics system. PLEX_start, PLEX_update, and PLEX_end manages these. Typically, you never need to call these.

	extern bool PLEX_startGraphics();
	extern bool PLEX_updateGraphics(SDL_Window* window, const uint32_t w, const uint32_t h);
	extern void PLEX_endGraphics();
#ifdef __cplusplus
}
#endif

#endif
