#ifndef PLEX_TEXTURE_LOW_H
#define PLEX_TEXTURE_LOW_H

#include "utility/defaults.h"

// This is the header for the low level texturing system. This is a branch of multiple potential graphics systems which can be switched through the macro PLEX_RENDER_TYPE. More information at graphics_low.h.

// NOTE: There is also a high level texturing system in the texture unit and header. If you're looking for any function involving loading or utilizing textures then texture.h and texture.c not this texture_low.h or texture_low.c.

// Indicies for different rendering types:

#define PLEX_RENDERER_TYPE_SDL 0
  
// defaults to SDL rendering type if the compiler hasn't set one:

#ifndef PLEX_RENDERER_TYPE
	#define PLEX_RENDERER_TYPE PLEX_RENDERER_TYPE_SDL // < defaults to sdl hardware
#endif

// definitions if SDL hardware rendering:


#if (PLEX_RENDERER_TYPE == PLEX_RENDERER_TYPE_SDL)
	#define PLEX_LOW_TEXTURE SDL_Texture*
	#define PLEX_LOW_NOTEXTURE NULL
#endif

#ifdef __cplusplus
extern "C"
{
#endif
	// generate low level blank texture

	extern PLEX_LOW_TEXTURE PLEX_low_generateTexture(const Uint32 format, const int access, const int w, const int h);

	// generates a texture of color r, g, b, a

	extern void PLEX_low_setGeneratedTexture(PLEX_LOW_TEXTURE texture, const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a);

	// destroys the low level texture

	extern void PLEX_low_destroyTexture(PLEX_LOW_TEXTURE* texture);

	// clears low level texture to blank

	extern void PLEX_low_clearTexture(PLEX_LOW_TEXTURE texture);

	// copies texture from onto to

	extern void PLEX_low_textureOverTexture(PLEX_LOW_TEXTURE from, PLEX_LOW_TEXTURE to, SDL_Rect area, SDL_Rect toArea);

	// alpha mods the texture to draw translucent

	extern void PLEX_low_textureSetAlpha(PLEX_LOW_TEXTURE* toSet, const uint8_t alpha);

	// compares whether texture a or b are equal, or whether a is less than b. This is mostly for sorting

	extern bool PLEX_low_compTextures(const PLEX_LOW_TEXTURE a, const PLEX_LOW_TEXTURE b);
	extern bool PLEX_low_lessTextures(const PLEX_LOW_TEXTURE a, const PLEX_LOW_TEXTURE b);

	// converts 'surface' into 'texture' with alpha

	extern void PLEX_low_textureFromSurface(SDL_Surface* surface, PLEX_LOW_TEXTURE* texture, const uint8_t a);

	// returns the width and height of a texture

	extern void PLEX_low_textureWH(PLEX_LOW_TEXTURE texture, uint32_t* w, uint32_t* h);

	// returns the blend mode of a texture

	extern SDL_BlendMode PLEX_low_getTextureBlendMode(PLEX_LOW_TEXTURE texture);

	// returns whether or not a texture is null

	extern bool PLEX_low_isTextureNull(const PLEX_LOW_TEXTURE texture);

	// if set to SDL rendering returns SDL_Renderer

	#if (PLEX_RENDERER_TYPE == PLEX_RENDERER_TYPE_SDL)
		extern SDL_Renderer* PLEX_low_getRenderer();
		extern void PLEX_low_destroyRenderer();
	#endif
#ifdef __cplusplus
}
#endif

#endif
