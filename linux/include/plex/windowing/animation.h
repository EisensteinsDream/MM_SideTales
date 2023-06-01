#ifndef PLEX_ANIMATION_H
#define PLEX_ANIMATION_H

#include "windowing/graphics.h"

// This unit contains animations and sprites which PLEX uses. The animations themselves have a variable amounts of frames, custom cycle lengh. They can also be drawn tinted or at different sizes multiple times.

// NOTE: This is pretty extensive. It's a bit of a pain in the ass to handle. Eventually, I'll put a scripting system or some handler functions to make coding for it cleaner. However, it's very powerful and can essentially do anything you want.

// structs that contain the info to manage animation plate (simply metadata for animation). Animation itself and sprite (basically a collection of animations that can be switched between actively).

typedef struct
{
	size_t base;
	uint32_t fw, fh, wcount;
} PLEX_ANIMATION_PLATE;

typedef struct
{
	size_t frames[PLEX_DEFAULT_FRAMEMAX];
	size_t frameCount;
	uint64_t cycle, start, stopTime;
	bool stopped;
} PLEX_ANIMATION;

typedef struct
{
	PLEX_ANIMATION_PLATE plate;
	PLEX_ANIMATION animations[PLEX_DEFAULT_ANIMATIONMAX];
	size_t animationCount;
	bool stopped;
} PLEX_SPRITE;

#ifdef __cplusplus
extern "C"
{
#endif

// --- ANIMATION PLATE ---

	// creats an animnation plate based off image index 'base' and then framewidth by percentage.

	extern PLEX_ANIMATION_PLATE PLEX_createAnimationPlate(const double wPerc, const double hPerc, const size_t base);

	// clears or unclears the texture inside an animation plate. clearing/unclearing explained more clearly in texture.h

	extern void PLEX_clearAnimationPlate(PLEX_ANIMATION_PLATE* plate);
	extern void PLEX_unclearAnimationPlate(PLEX_ANIMATION_PLATE* plate);

	// destroys the texture inside animation plate.

	extern void PLEX_destroyAnimationPlate(PLEX_ANIMATION_PLATE* plate);

// --- ANIMATION ---

	// draws the current frame of an animation. 'order' being the layer drawn with the larger number being the furthest back. tx, ty, tw, th indicate a draw area. 'offset' means it will draw within the area of the bordered screen rather than the screen. Second function does the same but tinting it by r, g, b.

	extern void PLEX_drawFrame(const size_t order, const uint64_t time, const double tx, const double ty, const double tw, const double th, const PLEX_ANIMATION animation, const PLEX_ANIMATION_PLATE plate, const bool offset);
	extern void PLEX_drawFrameTinted(const size_t order, const uint64_t time, const double tx, const double ty, const double tw, const double th, const uint8_t r, const uint8_t g, const uint8_t b, const PLEX_ANIMATION animation, const PLEX_ANIMATION_PLATE plate, const bool offset);

	// adds the texture 'frame' to the animation.

	extern void PLEX_addAnimationFrame(const size_t frame, PLEX_ANIMATION* animation);

	// starts animation at microsecond time 'start'

	extern void PLEX_startAnimation(const uint64_t start, PLEX_ANIMATION* animation);

	// stops animation

	extern void PLEX_stopAnimation(PLEX_ANIMATION* animation);

	// changes the animation cycle speed

	extern void PLEX_changeAnimationCycleSpeed(const uint64_t cycle, const uint64_t time, PLEX_ANIMATION* animation);

	// gets the internal x, y, w, h on time

	extern size_t PLEX_getFrame(const uint64_t time, uint32_t* x, uint32_t* y, uint32_t* w, uint32_t* h, const PLEX_ANIMATION animation, const PLEX_ANIMATION_PLATE plate);

	// returns whether or not the animation is stopped

	extern bool PLEX_isAnimationStopped(const PLEX_ANIMATION animation);

	// returns the entire microsecond length of the animation cycle

	extern uint64_t PLEX_animationCycleLen(const PLEX_ANIMATION animation);

// --- SPRITE ---

	// create of a sprite based on plate

	extern PLEX_SPRITE PLEX_createSprite(const PLEX_ANIMATION_PLATE* plate);

	// adds an animation to the sprite at duration cycle and start animation time all microseconds

	extern void PLEX_addAnimation(PLEX_SPRITE* sprite, const uint64_t cycle, const uint64_t start);

	extern bool PLEX_syncAnimations(const PLEX_ANIMATION a, PLEX_ANIMATION* b);

	// draws sprite which means it draw the internal animation at 'index', look at drawAnimation above for more information. Also a tinted sprite draw function just like the drawAnimationTinted function.

	extern void PLEX_drawSprite(const size_t order, const size_t index, const uint64_t time, const double tx, const double ty, const double tw, const double th, const PLEX_SPRITE sprite, const bool offset);
	extern void PLEX_drawSpriteTinted(const size_t order, const size_t index, const uint64_t time, const double tx, const double ty, const double tw, const double th, const uint8_t r, const uint8_t g, const uint8_t b, const PLEX_SPRITE sprite, const bool offset);

	// starts the sprite animation cycle at 'start' microseconds

	extern void PLEX_startSprite(const uint64_t start, PLEX_SPRITE* sprite);

	// stops the sprite animation cycle

	extern void PLEX_stopSprite(PLEX_SPRITE* sprite);

	// clears the animations in sprite as described above in clearAnimation

	extern void PLEX_clearSprite(PLEX_SPRITE* sprite);

	// unclears the animations in sprite as described above in unclearAnimation

	extern void PLEX_unclearSprite(PLEX_SPRITE* sprite);

	// changes the animation cycle speed

	extern void PLEX_changeSpriteCycleSpeed(const size_t index, const uint64_t cycle, const uint64_t time, PLEX_SPRITE* sprite);

	// returns whether or not the sprite animation cycle is stopped

	extern bool PLEX_isSpriteStopped(const PLEX_SPRITE sprite);

	// returns the entire animation cycle in seconds of animation at 'index' in sprite

	extern uint64_t PLEX_spriteCycleLen(const size_t index, const PLEX_SPRITE sprite);

	extern bool PLEX_syncSprites(const size_t index, const PLEX_SPRITE a, PLEX_SPRITE* b);

	// returns the animation inside sprite at index 'index'

	extern PLEX_ANIMATION* PLEX_getAnimation(const size_t index, PLEX_SPRITE* sprite);
#ifdef __cplusplus
}
#endif

#endif
