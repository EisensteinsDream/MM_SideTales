#ifndef PLEX_TEXTURES_H
#define PLEX_TEXTURES_H

#include "windowing/window.h"

#include "windowing/texture.h"

#include "utility/fakestrings.h"

#include "mem/mem.h"

// this unit contains PLEX's texture management system. The way that PLEX manages textures is mostly internally. So everything is managed by index's as size_t's returned as they are created. It also recycles textures whenever it can get away with it.

// NOTE: this system may come off as kind of weird but the idea is that it conceals multiple potential rendering systems. It's somewhat similar to how gl works with gluints. Almost a state machine, I guess.

#define PLEX_createGeneratedTexture(w, h, r, g, b, a) PLEX_createGeneratedTextureInt(w, h, r, g, b, a, __LINE__, __FILE__)
#define PLEX_loadTextureFromFile(w, h, r, g, b, a, tint, fileloc) PLEX_loadTextureFromFileInt(w, h, r, g, b, a, tint, fileloc, __LINE__, __FILE__)
#define PLEX_copyTexture(from, cx, cy, cw, ch) PLEX_copyTextureInt(from, cx, cy, cw, ch, __LINE__, __FILE__)
#define PLEX_loadTextureFromHeaderless(fileloc) PLEX_loadTextureFromHeaderlessInt(fileloc, __LINE__, __FILE__)
#define PLEX_loadTextureFromFileOrHeaderless(w, h, r, g, b, a, tint, fileloc, headerlessLoc) PLEX_loadTextureFromFileOrHeaderlessInt(w, h, r, g, b, a, tint, fileloc, headerlessLoc, __LINE__, __FILE__)

#ifdef __cplusplus
extern "C"
{
#endif

	// internally creates a blank texture filled with rgba and returns the index of the created texture

	extern size_t PLEX_createGeneratedTextureInt(const uint32_t w, const uint32_t h, const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a, const size_t linenum, const char* const filename);

	// internally creates a texture from an image file and returns the index of teh created texture

	extern size_t PLEX_loadTextureFromFileInt(const uint32_t w, const uint32_t h, const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a, const double tint, const char* const fileloc, const size_t linenum, const char* const filename);

	// converts a surface loaded from an image into a headerless uncompressed image

	extern bool PLEX_convToHeaderlessImage(const char* const filelocFrom, const char* const filelocTo, const size_t linenum, const char* const filename);

	// loads a heaaderless image file into a texture

	extern size_t PLEX_loadTextureFromHeaderlessInt(const char* const fileloc, const size_t linenum, const char* const filename);

	// loads headerless if it's not available, otherwise creates a new one

	extern size_t PLEX_loadTextureFromFileOrHeaderlessInt(const uint32_t w, const uint32_t h, const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a, const double tint, const char* const fileloc, const char* const headerlessLoc, const size_t linenum, const char* const filename);

	// copies a texture from the range cx, cy, cw, ch into a new space and returns it's index. This hasn't been tested for awhile, so it's technically considered experimental.

	extern size_t PLEX_copyTextureInt(const size_t from, const uint32_t cx, const uint32_t cy, const uint32_t cw, const uint32_t ch, const size_t linenum, const char* const filename);

	// internally destroys the texture by index. Of course, doesn't clear whatever size_t index is passed into it

	extern void PLEX_destroyTexture(const size_t index);

	// destroys a texture without destroying it's meta information so it can be regenerated

	extern void PLEX_clearTexture(const size_t index);

	// regenerates a cleared texture

	extern void PLEX_unclearTexture(const size_t index);

	// sets an alpha mod to the texture for real time alpha

	extern void PLEX_setTextureAlpha(const size_t texture, const uint8_t a);

	// the functions to start, update, and end the texture system. Typically, you'll never call these. They are automatically managed with PLEX_start, PLEX_update, PLEX_end.

	extern bool PLEX_startTextures();
	extern bool PLEX_updateTextures();
	extern void PLEX_endTextures();

	// returns a 'null' texture. Not a blank texture but a signifier of a non-texture,

	extern size_t PLEX_nullTexture();

	// returns true if a texture at index is NULL

	extern bool PLEX_textureIsNull(const size_t index);

	// reads the width and height of a texture at index

	extern void PLEX_textureWH(const size_t index, uint32_t* w, uint32_t* h);

	// returns the low level texture inside the texture at index

	extern PLEX_LOW_TEXTURE PLEX_innerTexture(const size_t index);

	// returns whether or not the texture at index is cleared

	extern bool PLEX_textureIsCleared(const size_t index);
#ifdef __cplusplus
}
#endif

#endif
