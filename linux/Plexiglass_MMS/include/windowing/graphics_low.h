#ifndef PLEX_GRAPHICS_LOW_H
#define PLEX_GRAPHICS_LOW_H

#include "windowing/texture_low.h"

// This is the header for the low level graphics system. This is a branch of multiple potential graphics systems which can be switched through the macro PLEX_RENDER_TYPE.

// NOTE: There is also a high level graphics system in the graphics unit and header. This is not for drawing functions or other directly usable graphics function you want the graphics.h and graphics.c files NOT graphics_low. This is basically for the branching between different renderers. For the moment there is only opengl.

// definitions for sdl renderer
#if (PLEX_RENDERER_TYPE == PLEX_RENDERER_TYPE_SDL)
	#define PLEX_LOW_NOTARGET NULL
#endif

// NOTE: more definitions in texture_low.h

#ifdef __cplusplus
extern "C"
{
#endif
	// creates the window based on whatever designated renderer

	extern SDL_Window* PLEX_low_createSDLWindow(const char* const title, const uint32_t width, const uint32_t height, const bool allowResize);
	extern void PLEX_low_destroySDLWindow(SDL_Window* const window);

	// updates the window based on whatever designated renderer

	extern void PLEX_low_updateRenderer(SDL_Window* const window, const uint32_t w, const uint32_t h);

	// sets the rendering target (no target = the screen)

	extern void PLEX_low_setRenderTarget(PLEX_LOW_TEXTURE target);

	// clears the screen texture

	extern void PLEX_low_clear(const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a);

	// low level drawing functions

	extern void PLEX_low_fillRect(const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a, const SDL_Rect* s, const SDL_BlendMode blendMode);

	extern void PLEX_low_fillRects(const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a, const SDL_Rect* s, const size_t count, const SDL_BlendMode blendMode);

	// tints the texture

	extern void PLEX_low_tintTexturesRGB(PLEX_LOW_TEXTURE* texture, const uint8_t r, const uint8_t g, const uint8_t b);

	// draws the texture onto the screen

	extern void PLEX_low_copyTexture(PLEX_LOW_TEXTURE texture, const SDL_Rect* from, const SDL_Rect* to, const SDL_BlendMode blendMode);

	extern void PLEX_low_copyTextureRotated(PLEX_LOW_TEXTURE texture, const SDL_Rect* from, const SDL_Rect* to, const double angle, const SDL_BlendMode blendMode);

	extern void PLEX_low_setBlendTarget(const SDL_BlendMode blendMode);

	// returns the tint rgb of a texture

	extern void PLEX_low_getTintTexturesRGB(PLEX_LOW_TEXTURE texture, uint8_t* r, uint8_t* g, uint8_t* b);

	// extern SDL_Renderer* PLEX_low_getRenderer(); < forward declared in texture_low. doesn't exist in all implementations.

	// returns the render target

	extern PLEX_LOW_TEXTURE PLEX_low_getRenderTarget();

	// returns the current target's blend mode

	extern SDL_BlendMode PLEX_low_getRenderBlendMode();
#ifdef __cplusplus
}
#endif

#endif
