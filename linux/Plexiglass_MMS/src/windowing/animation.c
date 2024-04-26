#include "windowing/animation.h"

// NOTE: function descriptions in header.

typedef struct
{
	PLEX_ANIMATION* animation;
} P_ANIMATION_CONTAINER;

// ---------------------------- //
// PUBLIC FUNCTIONS:
// ---------------------------- //

// --- ANIMATION PLATE ---

PLEX_ANIMATION_PLATE PLEX_createAnimationPlate(const double wPerc, const double hPerc, const size_t base)
{
	PLEX_ANIMATION_PLATE ret;

	uint32_t w = 0;
	uint32_t h = 0;

	memset(&ret, 0, sizeof(ret));

	if(!wPerc)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ANIMATION, PLEX_ERROR_ZEROVALUE, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ANIMATION_ONCREATEANIMATION, PLEX_ERRORMESS_GENERIC_WPERC);
		return ret;
	}

	if(!hPerc)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ANIMATION, PLEX_ERROR_ZEROVALUE, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ANIMATION_ONCREATEANIMATION, PLEX_ERRORMESS_GENERIC_HPERC);
		return ret;
	}

	if(!base)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ANIMATION, PLEX_ERROR_ZEROVALUE, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ANIMATION_ONCREATEANIMATION, PLEX_ERRORMESS_TEXTURE_TEXTURE);
		return ret;
	}

	ret.base = base;

	PLEX_textureWH(base, &w, &h);

	if(!w)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ANIMATION, PLEX_ERROR_ZEROVALUE, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ANIMATION_ONCREATEANIMATION, PLEX_ERRORMESS_GENERIC_WIDTH);
		return ret;
	}

	if(!h)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ANIMATION, PLEX_ERROR_ZEROVALUE, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ANIMATION_ONCREATEANIMATION, PLEX_ERRORMESS_GENERIC_HEIGHT);
		return ret;
	}

	ret.fw = w * wPerc;
	ret.fh = h * hPerc;
	ret.wcount = w/ret.fw;
	ret.wperc = wPerc;
	ret.hperc = hPerc;

	return ret;
}

void PLEX_clearAnimationPlate(PLEX_ANIMATION_PLATE* plate)
{
	if(plate == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ANIMATION, PLEX_ERROR_ISNULL, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ANIMATION_ONCLEARINGANIMATION, PLEX_ERRORMESS_ANIMATION_ANIMATIONPLATE);
		return;
	}

	PLEX_clearTexture(plate->base);
}

void PLEX_unclearAnimationPlate(PLEX_ANIMATION_PLATE* plate)
{
	if(plate == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ANIMATION, PLEX_ERROR_ISNULL, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ANIMATION_ONUNCLEARINGANIMATION, PLEX_ERRORMESS_ANIMATION_ANIMATIONPLATE);
		return;
	}

	PLEX_unclearTexture(plate->base);
}

void PLEX_destroyAnimationPlate(PLEX_ANIMATION_PLATE** plate)
{
	PLEX_ANIMATION_PLATE* platePtr = NULL;

	if(plate == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ANIMATION, PLEX_ERROR_ISNULL, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ANIMATION_ONDESTROYINGANIMATION, PLEX_ERRORMESS_ANIMATION_ANIMATIONPLATE);
		return;
	}

	platePtr = *plate;

	if(platePtr == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ANIMATION, PLEX_ERROR_ISNULL, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ANIMATION_ONDESTROYINGANIMATION, PLEX_ERRORMESS_ANIMATION_ANIMATIONPLATE);
		return;
	}

	PLEX_destroyTexture(platePtr->base);
	platePtr->base = 0;

	*plate = NULL;
}

size_t PLEX_animationPlateTex(const PLEX_ANIMATION_PLATE plate)
{
	return plate.base;
}

double PLEX_animationPlateWPerc(const PLEX_ANIMATION_PLATE plate)
{
	return plate.wperc;
}

double PLEX_animationPlateHPerc(const PLEX_ANIMATION_PLATE plate)
{
	return plate.hperc;
}

// --- ANIMATION ---

void PLEX_drawFrameInt(const size_t order,  const double tx, const double ty, const double tw, const double th, const PLEX_ANIMATION* animation, const PLEX_ANIMATION_PLATE plate, const bool offset, const uint64_t time, const size_t errorline, const char* const errorfile)
{
	uint32_t fx = 0;
	uint32_t fy = 0;
	uint32_t fw = 0;
	uint32_t fh = 0;

	if(animation == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ANIMATION, PLEX_ERROR_ISNULL, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_ANIMATION_ONDRAWFRAME, PLEX_ERRORMESS_ANIMATION_ANIMATION);
		return;
	}

	PLEX_getFrame(&fx, &fy, &fw, &fh, animation, plate, time);

	PLEX_printTextureInt(order, plate.base, fx, fy, fw, fh, tx, ty, tw, th, offset, errorline, errorfile);
}

void PLEX_drawFrameTintedInt(const size_t order, const double tx, const double ty, const double tw, const double th, const uint8_t r, const uint8_t g, const uint8_t b, const PLEX_ANIMATION* animation, const PLEX_ANIMATION_PLATE plate, const bool offset, const uint64_t time, const size_t errorline, const char* const errorfile)
{
	uint32_t fx = 0;
	uint32_t fy = 0;
	uint32_t fw = 0;
	uint32_t fh = 0;

	if(animation == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ANIMATION, PLEX_ERROR_ISNULL, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_ANIMATION_ONDRAWFRAMETINTED, PLEX_ERRORMESS_ANIMATION_ANIMATION);
		return;
	}

	PLEX_getFrame(&fx, &fy, &fw, &fh, animation, plate, time);

	PLEX_printTextureTintedInt(order, plate.base, fx, fy, fw, fh, tx, ty, tw, th, r, g, b, offset, errorline, errorfile);
}

void PLEX_addAnimationFrameInt(const size_t frame, PLEX_ANIMATION* animation, const size_t errorline, const char* const errorfile)
{
	size_t* framePtr = NULL;

	if(animation == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ANIMATION, PLEX_ERROR_ISNULL, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ANIMATION_ONADDANIMATIONFRAME, PLEX_ERRORMESS_ANIMATION_ANIMATIONPLATE);
		return;
	}

	if(!PLEX_incrementExpandArrayInt(animation->frames, errorline, errorfile))
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ANIMATION, PLEX_ERROR_INCREMENT, PLEX_ERROR_SEVERITY_FATAL, PLEX_ERRORMESS_ANIMATION_ONADDANIMATIONFRAME, PLEX_ERRORMESS_ANIMATION_FRAMES);
		return;
	}

	framePtr = PLEX_getExpandArrayLast(animation->frames);

	if(framePtr == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ANIMATION, PLEX_ERROR_ACQUIRE, PLEX_ERROR_SEVERITY_FATAL, PLEX_ERRORMESS_ANIMATION_ONADDANIMATIONFRAME, PLEX_ERRORMESS_ANIMATION_FRAMES);
		return;
	}

	*framePtr = frame;
}

void PLEX_startAnimation(const uint64_t start, PLEX_ANIMATION* animation)
{
	if(animation == NULL)
	{
		PLEX_errorNoDesc(PLEX_ERROR_CATEGORY_ANIMATION, PLEX_ERROR_DOUBLESTART, PLEX_ERROR_SEVERITY_MINOR);
		return;
	}

	if(animation->stopped) animation->start = start;
	animation->stopped = false;
}

void PLEX_stopAnimation(PLEX_ANIMATION* animation)
{
	if(animation == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ANIMATION, PLEX_ERROR_ISNULL, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ANIMATION_ONSTOPANIMATION, PLEX_ERRORMESS_ANIMATION_ANIMATION);
		return;
	}

	animation->stopped = true;
}

void PLEX_changeAnimationCycleSpeed(const uint64_t cycle, PLEX_ANIMATION* animation, const uint64_t time)
{
	uint64_t cycleDiff = 0;

	double cyclePerc = 0;

	if(animation == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ANIMATION, PLEX_ERROR_ISNULL, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_ANIMATION_ANIMATION, PLEX_ERRORMESS_ANIMATION_ONCYCLESPEEDCHANGENANIMATION);
		return;
	}

	if(cycle == animation->cycle)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ANIMATION, PLEX_ERROR_SAMEVALUE, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_ANIMATION_ANIMATIONCYCLE, PLEX_ERRORMESS_ANIMATION_ONCYCLESPEEDCHANGENANIMATION);
		return;
	}

	if(animation->cycle == cycle) return;

	cycleDiff = time - animation->start;
	cyclePerc = cycle ? animation->cycle/(double)cycle : 0;

	animation->start = time - (cycleDiff * cyclePerc);
	animation->cycle = cycle;
}

size_t PLEX_getFrame(uint32_t* x, uint32_t* y, uint32_t* w, uint32_t* h, const PLEX_ANIMATION* animation, const PLEX_ANIMATION_PLATE plate, const uint64_t time)
{
	uint64_t runTime = 0;

	uint64_t frameLen = 0;

	size_t frameCount = 0;
	size_t frameIndex = 0;

	size_t index = 0;
	size_t column = 0;
	size_t row = 0;

	if(animation == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ANIMATION, PLEX_ERROR_GETNULL, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_ANIMATION_ONGETFRAMEANIMATION, PLEX_ERRORMESS_ANIMATION_ANIMATION);
		return 0;
	}

	runTime = animation->cycle ? (time - animation->start) % animation->cycle : 0;

	if(x == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ANIMATION, PLEX_ERROR_GETNULL, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ANIMATION_ONGETFRAMEANIMATION, PLEX_ERRORMESS_GENERIC_DIMENX);
		return 0;
	}

	if(y == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ANIMATION, PLEX_ERROR_GETNULL, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ANIMATION_ONGETFRAMEANIMATION, PLEX_ERRORMESS_GENERIC_DIMENY);
		return 0;
	}

	if(w == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ANIMATION, PLEX_ERROR_ISNULL, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ANIMATION_ONGETFRAMEANIMATION, PLEX_ERRORMESS_GENERIC_DIMENW);
		return 0;
	}

	if(h == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ANIMATION, PLEX_ERROR_ISNULL, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ANIMATION_ONGETFRAMEANIMATION, PLEX_ERRORMESS_GENERIC_DIMENH);
		return 0;
	}

	frameCount = PLEX_expandArrayCount(animation->frames);
	frameLen = frameCount ? animation->cycle/frameCount : 0;

	if(!animation->stopped)
	{
		frameIndex = frameLen ? runTime/frameLen : 0;
		if(frameIndex >= frameCount) frameIndex = 0; // < fixes weird glitch
	}

	if(!frameCount)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ANIMATION, PLEX_ERROR_ZEROVALUE, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_ANIMATION_ONGETFRAMEANIMATION, PLEX_ERRORMESS_GENERIC_COUNT);
		return 0;
	}

	index = *((size_t*)PLEX_getExpandArrayEntry(frameIndex, animation->frames));
	column = plate.wcount ? index % plate.wcount : 0;
	row = plate.wcount ? index/plate.wcount : 0;

	*x = column * plate.fw;
	*y = row * plate.fh;
	*w = plate.fw;
	*h = plate.fh;

	return index;
}

bool PLEX_isAnimationStopped(const PLEX_ANIMATION* animation)
{
	if(animation == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ANIMATION, PLEX_ERROR_ISNULL, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_ANIMATION_ONISANIMATIONSTOPPED, PLEX_ERRORMESS_ANIMATION_ANIMATION);
		return false;
	}

	return animation->stopped;
}

uint64_t PLEX_animationCycleLen(const PLEX_ANIMATION* animation)
{
	if(animation == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ANIMATION, PLEX_ERROR_ISNULL, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_ANIMATION_ONISANIMATIONCYCLE, PLEX_ERRORMESS_ANIMATION_ANIMATION);
		return 0;
	}

	return animation->cycle;
}

// -- SPRITE ---

PLEX_SPRITE* PLEX_genSpriteInt(const PLEX_ANIMATION_PLATE* plate, const size_t errorline, const char* const errorfile)
{
	PLEX_SPRITE* ret = NULL;

	if(plate == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ANIMATION, PLEX_ERROR_ISNULL, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ANIMATION_ONCREATESPRITE, PLEX_ERRORMESS_ANIMATION_ANIMATIONPLATE);
		return ret;
	}

	ret = PLEX_allocInt(sizeof(PLEX_SPRITE), errorline, errorfile);

	if(ret == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ANIMATION, PLEX_ERROR_ALLOCFAIL, PLEX_ERROR_SEVERITY_FATAL, PLEX_ERRORMESS_ANIMATION_ONCREATESPRITE, PLEX_ERRORMESS_ANIMATION_ANIMATION);
		return NULL;
	}

	ret->animations = PLEX_genExpandArrayInt(PLEX_getMemProfileAnimationPreload(), sizeof(P_ANIMATION_CONTAINER), errorline, errorfile);

	if(ret->animations == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ANIMATION, PLEX_ERROR_ALLOCFAIL, PLEX_ERROR_SEVERITY_FATAL, PLEX_ERRORMESS_ANIMATION_ONCREATESPRITE, PLEX_ERRORMESS_ANIMATION_ANIMATION);
		return NULL;
	}

	memcpy(&ret->plate, plate, sizeof(ret->plate));

	ret->stopped = false;

	return ret;
}

void PLEX_destroySprite(PLEX_SPRITE** sprite)
{
	PLEX_SPRITE* spritePtr = NULL;

	size_t animationCount = 0;

	if(sprite == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ANIMATION, PLEX_ERROR_FREENULL, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_ANIMATION_ONDESTROYSPRITE, PLEX_ERRORMESS_ANIMATION_SPRITE);
		return;
	}

	spritePtr = *sprite;

	if(spritePtr == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ANIMATION, PLEX_ERROR_FREENULL, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_ANIMATION_ONDESTROYSPRITE, PLEX_ERRORMESS_ANIMATION_SPRITE);
		return;
	}

	animationCount = PLEX_expandArrayCount(spritePtr->animations);

	for(size_t ze = 0; ze < animationCount; ++ze)
	{
		P_ANIMATION_CONTAINER* animation = PLEX_getExpandArrayEntry(ze, spritePtr->animations);

		if(animation == NULL)
		{
			PLEX_error2(PLEX_ERROR_CATEGORY_ANIMATION, PLEX_ERROR_ACQUIRE, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ANIMATION_ONDESTROYSPRITE, PLEX_ERRORMESS_ANIMATION_ANIMATION); // < do not make 'fatal' or 'impossible'
			continue;
		}

		if(animation->animation == NULL)
		{
			PLEX_error2(PLEX_ERROR_CATEGORY_ANIMATION, PLEX_ERROR_CONTENTCORRUPTION, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ANIMATION_ONDESTROYSPRITE, PLEX_ERRORMESS_ANIMATION_ANIMATION); // < do not make 'fatal' or 'impossible'
			continue;
		}

		PLEX_destroyExpandArray(&animation->animation->frames);

		PLEX_dealloc(&animation->animation);
	}

	PLEX_destroyExpandArray(&spritePtr->animations);

	PLEX_dealloc(sprite);
}

PLEX_SPRITE* PLEX_copySpriteInt(const PLEX_SPRITE* const sprite, const size_t errorline, const char* const errorfile)
{
	PLEX_SPRITE* ret = 0;

	size_t animationCount = 0;

	if(sprite == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ANIMATION, PLEX_ERROR_ISNULL, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_ANIMATION_ONCOPYSPRITE, PLEX_ERRORMESS_ANIMATION_SPRITE);
		return NULL;
	}

	if(sprite->animations == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ANIMATION, PLEX_ERROR_CONTENTCORRUPTION, PLEX_ERROR_SEVERITY_IMPOSSIBLE, PLEX_ERRORMESS_ANIMATION_ONCOPYSPRITE, PLEX_ERRORMESS_ANIMATION_SPRITE);
		return NULL;
	}

	ret = PLEX_allocInt(sizeof(PLEX_SPRITE), errorline, errorfile);

	if(ret == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ANIMATION, PLEX_ERROR_ALLOCFAIL, PLEX_ERROR_SEVERITY_FATAL, PLEX_ERRORMESS_ANIMATION_ONCOPYSPRITE, PLEX_ERRORMESS_ANIMATION_SPRITE);
		return NULL;
	}

	ret->plate = sprite->plate;
	ret->stopped = sprite->stopped;

	animationCount = PLEX_expandArrayCount(sprite->animations);

	ret->animations = PLEX_genExpandArrayInt(PLEX_getMemProfileAnimationPreload(), sizeof(P_ANIMATION_CONTAINER), errorline, errorfile);

	if(ret->animations == NULL)
	{
		PLEX_dealloc(&ret);

		PLEX_error2(PLEX_ERROR_CATEGORY_ANIMATION, PLEX_ERROR_ALLOCFAIL, PLEX_ERROR_SEVERITY_FATAL, PLEX_ERRORMESS_ANIMATION_ONCOPYSPRITE, PLEX_ERRORMESS_ANIMATION_ANIMATIONS);
		return NULL;
	}

	if(!PLEX_multiIncrementExpandArrayInt(animationCount, ret->animations, errorline, errorfile))
	{
		PLEX_destroyExpandArray(&ret->animations);
		PLEX_dealloc(&ret);

		PLEX_error2(PLEX_ERROR_CATEGORY_ANIMATION, PLEX_ERROR_INCREMENT, PLEX_ERROR_SEVERITY_FATAL, PLEX_ERRORMESS_ANIMATION_ONCOPYSPRITE, PLEX_ERRORMESS_ANIMATION_SPRITE);
		return NULL;
	}

	for(size_t ze = 0; ze < animationCount; ++ze)
	{
		P_ANIMATION_CONTAINER* from = PLEX_getExpandArrayEntry(ze, sprite->animations);
		P_ANIMATION_CONTAINER* to = NULL;

		size_t frameCount = 0;

		if(from == NULL)
		{
			PLEX_destroyExpandArray(&ret->animations);
			PLEX_dealloc(&ret);

			PLEX_error2(PLEX_ERROR_CATEGORY_ANIMATION, PLEX_ERROR_CREATE, PLEX_ERROR_SEVERITY_FATAL, PLEX_ERRORMESS_ANIMATION_ONCOPYSPRITE, PLEX_ERRORMESS_ANIMATION_ANIMATIONS);
			return NULL;
		}

		frameCount = PLEX_expandArrayCount(from->animation->frames);

		to = PLEX_getExpandArrayEntry(ze, ret->animations);

		if(to == NULL)
		{
			PLEX_destroyExpandArray(&ret->animations);
			PLEX_dealloc(&ret);

			PLEX_error2(PLEX_ERROR_CATEGORY_ANIMATION, PLEX_ERROR_CREATE, PLEX_ERROR_SEVERITY_FATAL, PLEX_ERRORMESS_ANIMATION_ONCOPYSPRITE, PLEX_ERRORMESS_ANIMATION_ANIMATIONS);
			return NULL;
		}

		to->animation = PLEX_allocInt(sizeof(PLEX_ANIMATION), errorline, errorfile);

		if(to->animation == NULL)
		{
			for(size_t uh = 0; uh < ze; ++uh)
			{
				P_ANIMATION_CONTAINER* destroy = PLEX_getExpandArrayEntry(uh, ret->animations);

				PLEX_destroyExpandArray(&destroy->animation->frames);

				PLEX_dealloc(&destroy->animation);
			}

			PLEX_destroyExpandArray(&ret->animations);
			PLEX_dealloc(&ret);

			PLEX_dealloc(&to->animation);
			PLEX_dealloc(&to);

			PLEX_error2(PLEX_ERROR_CATEGORY_ANIMATION, PLEX_ERROR_CONTENTCORRUPTION, PLEX_ERROR_SEVERITY_FATAL, PLEX_ERRORMESS_ANIMATION_ONCOPYSPRITE, PLEX_ERRORMESS_ANIMATION_ANIMATION);
			return NULL;
		}

		to->animation->frames = PLEX_genExpandArrayInt(PLEX_getMemProfileFramePreload(), sizeof(size_t), errorline, errorfile);

		if(to->animation->frames == NULL || !PLEX_multiIncrementExpandArrayInt(frameCount, to->animation->frames, errorline, errorfile))
		{
			if(to->animation->frames == NULL)
			{
				PLEX_error2(PLEX_ERROR_CATEGORY_ANIMATION, PLEX_ERROR_CONTENTCORRUPTION, PLEX_ERROR_SEVERITY_FATAL, PLEX_ERRORMESS_ANIMATION_ONCOPYSPRITE, PLEX_ERRORMESS_ANIMATION_FRAMES);
			}

			if(!PLEX_multiIncrementExpandArrayInt(frameCount, to->animation->frames, errorline, errorfile))
			{
				PLEX_error2(PLEX_ERROR_CATEGORY_ANIMATION, PLEX_ERROR_INCREMENT, PLEX_ERROR_SEVERITY_FATAL, PLEX_ERRORMESS_ANIMATION_ONCOPYSPRITE, PLEX_ERRORMESS_ANIMATION_FRAMES);
			}

			for(size_t uh = 0; uh < ze; ++uh)
			{
				P_ANIMATION_CONTAINER* destroy = PLEX_getExpandArrayEntry(uh, ret->animations);

				PLEX_destroyExpandArray(&destroy->animation->frames);

				PLEX_dealloc(&destroy->animation);
			}

			PLEX_dealloc(&to->animation);
			PLEX_dealloc(&to);

			PLEX_destroyExpandArray(&ret->animations);
			PLEX_dealloc(&ret);

			return NULL;
		}

		to->animation->cycle = from->animation->cycle;
		to->animation->start = from->animation->start;
		to->animation->stopped = from->animation->stopped;

		for(size_t uh = 0; uh < frameCount; ++uh)
		{
			const size_t* const fromFrame = PLEX_getExpandArrayEntry(uh, from->animation->frames);
			size_t* toFrame = PLEX_getExpandArrayEntry(uh, to->animation->frames);

			if(toFrame == NULL)
			{
				for(size_t uh = 0; uh < ze; ++uh)
				{
					P_ANIMATION_CONTAINER* destroy = PLEX_getExpandArrayEntry(uh, ret->animations);

					PLEX_destroyExpandArray(&destroy->animation->frames);

					PLEX_dealloc(&destroy->animation);
				}

				PLEX_dealloc(&to->animation);
				PLEX_dealloc(&to);

				PLEX_destroyExpandArray(&ret->animations);
				PLEX_dealloc(&ret);
			}

			*toFrame = *fromFrame;
		}
	}

	return ret;
}

void PLEX_addAnimationInt(PLEX_SPRITE* sprite, const uint64_t cycle, const uint64_t start, const size_t errorline, const char* const errorfile)
{
	P_ANIMATION_CONTAINER* target = NULL;

	if(sprite == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ANIMATION, PLEX_ERROR_ISNULL, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ANIMATION_ONADDINGANIMATIONTOSPRITE, PLEX_ERRORMESS_ANIMATION_SPRITE);
		return;
	}

	if(sprite->animations == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ANIMATION, PLEX_ERROR_CONTENTCORRUPTION, PLEX_ERROR_SEVERITY_IMPOSSIBLE, PLEX_ERRORMESS_ANIMATION_ONADDINGANIMATIONTOSPRITE, PLEX_ERRORMESS_ANIMATION_SPRITE);
		return;
	}

	if(!PLEX_incrementExpandArrayInt(sprite->animations, errorline, errorfile))
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ANIMATION, PLEX_ERROR_INCREMENT, PLEX_ERROR_SEVERITY_FATAL, PLEX_ERRORMESS_ANIMATION_ONADDINGANIMATIONTOSPRITE, PLEX_ERRORMESS_ANIMATION_ANIMATIONS);
		return;
	}

	target = PLEX_getExpandArrayLast(sprite->animations);

	if(target == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ANIMATION, PLEX_ERROR_ACQUIRE, PLEX_ERROR_SEVERITY_FATAL, PLEX_ERRORMESS_ANIMATION_ONADDINGANIMATIONTOSPRITE, PLEX_ERRORMESS_ANIMATION_ANIMATION);
		return;
	}

	target->animation = PLEX_allocInt(sizeof(PLEX_ANIMATION), errorline, errorfile);

	if(target->animation == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ANIMATION, PLEX_ERROR_ALLOCFAIL, PLEX_ERROR_SEVERITY_FATAL, PLEX_ERRORMESS_ANIMATION_ONADDINGANIMATIONTOSPRITE, PLEX_ERRORMESS_ANIMATION_ANIMATION);
		return;
	}

	target->animation->cycle = cycle;
	target->animation->start = start;
	target->animation->stopped = true;

	target->animation->frames = PLEX_genExpandArrayInt(PLEX_getMemProfileFramePreload(), sizeof(size_t), errorline, errorfile);

	if(target->animation->frames == NULL)
	{
		PLEX_dealloc(&target->animation);

		PLEX_error2(PLEX_ERROR_CATEGORY_ANIMATION, PLEX_ERROR_ALLOCFAIL, PLEX_ERROR_SEVERITY_FATAL, PLEX_ERRORMESS_ANIMATION_ONADDINGANIMATIONTOSPRITE, PLEX_ERRORMESS_ANIMATION_FRAMES);
		return;
	}
}

bool PLEX_syncAnimations(const PLEX_ANIMATION a, PLEX_ANIMATION* b)
{
	if(b == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ANIMATION, PLEX_ERROR_SAMEVALUE, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ANIMATION_ONSPRITESYNCANIMATIONS, PLEX_ERRORMESS_ANIMATION_ANIMATION2);
		return false;
	}

	b->start = a.start;
	b->cycle = a.cycle;
	b->stopped = a.stopped;

	return true;
}

void PLEX_drawSpriteInt(const size_t order, const size_t index, const double tx, const double ty, const double tw, const double th, const PLEX_SPRITE* sprite, const bool offset, const uint64_t time, const size_t errorline, const char* const errorfile)
{
	P_ANIMATION_CONTAINER* animation = NULL;

	size_t count = 0;

	if(sprite == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ANIMATION, PLEX_ERROR_ISNULL, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_ANIMATION_ONDRAWSPRITE, PLEX_ERRORMESS_ANIMATION_SPRITE);
		return;
	}

	count = PLEX_expandArrayCount(sprite->animations);

	if(index >= count)
	{
		// error
		return;
	}

	animation = PLEX_getExpandArrayEntry(index, sprite->animations);

	if(animation == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ANIMATION, PLEX_ERROR_ACQUIRE, PLEX_ERROR_SEVERITY_FATAL, PLEX_ERRORMESS_ANIMATION_ONDRAWSPRITE, PLEX_ERRORMESS_ANIMATION_ANIMATION);
		return;
	}

	PLEX_drawFrameInt(order, tx, ty, tw, th, animation->animation, sprite->plate, offset, time, errorline, errorfile);
}

void PLEX_drawSpriteTintedInt(const size_t order, const size_t index, const double tx, const double ty, const double tw, const double th, const uint8_t r, const uint8_t g, const uint8_t b, const PLEX_SPRITE* sprite, const bool offset, const uint64_t time, const size_t errorline, const char* const errorfile)
{
	P_ANIMATION_CONTAINER* animation = NULL;

	size_t count = 0;

	if(sprite == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ANIMATION, PLEX_ERROR_ISNULL, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_ANIMATION_ONDRAWSPRITETINTED, PLEX_ERRORMESS_ANIMATION_SPRITE);
		return;
	}

	count = PLEX_expandArrayCount(sprite->animations);

	if(index >= count)
	{
		// error
		return;
	}

	animation = PLEX_getExpandArrayEntry(index, sprite->animations);

	if(animation == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ANIMATION, PLEX_ERROR_ACQUIRE, PLEX_ERROR_SEVERITY_FATAL, PLEX_ERRORMESS_ANIMATION_ONDRAWSPRITETINTED, PLEX_ERRORMESS_ANIMATION_ANIMATION);
		return;
	}

	PLEX_drawFrameTintedInt(order, tx, ty, tw, th, r, g, b, animation->animation, sprite->plate, offset, time, errorline, errorfile);
}

void PLEX_startSprite(const uint64_t start, PLEX_SPRITE* sprite)
{
	size_t animationCount = 0;

	if(sprite == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ANIMATION, PLEX_ERROR_ISNULL, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ANIMATION_ONSTARTSPRITE, PLEX_ERRORMESS_ANIMATION_SPRITE);
		return;
	}

	animationCount = PLEX_expandArrayCount(sprite->animations);

	for(size_t ze = 0; ze < animationCount; ++ze)
	{
		P_ANIMATION_CONTAINER* animation = PLEX_getExpandArrayEntry(ze, sprite->animations);

		if(animation == NULL)
		{
			PLEX_error2(PLEX_ERROR_CATEGORY_ANIMATION, PLEX_ERROR_ACQUIRE, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ANIMATION_ONSTARTSPRITE, PLEX_ERRORMESS_ANIMATION_ANIMATION);
			return;
		}

		PLEX_startAnimation(start, animation->animation);
	}

	sprite->stopped = false;
}

void PLEX_stopSprite(PLEX_SPRITE* sprite)
{
	size_t animationCount = 0;

	if(sprite == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ANIMATION, PLEX_ERROR_ISNULL, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ANIMATION_ONSTOPSPRITE, PLEX_ERRORMESS_ANIMATION_SPRITE);
		return;
	}

	animationCount = PLEX_expandArrayCount(sprite->animations);

	for(size_t ze = 0; ze < animationCount; ++ze)
	{
		P_ANIMATION_CONTAINER* animation = PLEX_getExpandArrayEntry(ze, sprite->animations);

		if(animation == NULL)
		{
			PLEX_error2(PLEX_ERROR_CATEGORY_ANIMATION, PLEX_ERROR_ACQUIRE, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ANIMATION_ONSTOPSPRITE, PLEX_ERRORMESS_ANIMATION_ANIMATION);
			return;
		}

		PLEX_stopAnimation(animation->animation);
	}

	sprite->stopped = true;
}

void PLEX_clearSprite(PLEX_SPRITE* sprite)
{
	if(sprite == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ANIMATION, PLEX_ERROR_ISNULL, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ANIMATION_ONCLEARSPRITE, PLEX_ERRORMESS_ANIMATION_SPRITE);
		return;
	}

	PLEX_clearAnimationPlate(&sprite->plate);
}

void PLEX_unclearSprite(PLEX_SPRITE* sprite)
{
	if(sprite == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ANIMATION, PLEX_ERROR_ISNULL, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ANIMATION_ONUNCLEARSPRITE, PLEX_ERRORMESS_ANIMATION_SPRITE);
		return;
	}

	PLEX_unclearAnimationPlate(&sprite->plate);
}

void PLEX_changeSpriteCycleSpeed(const size_t index, const uint64_t cycle, PLEX_SPRITE* sprite, const uint64_t time)
{
	P_ANIMATION_CONTAINER* animation = NULL;

	size_t animationCount = 0;

	if(sprite == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ANIMATION, PLEX_ERROR_ISNULL, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ANIMATION_ONSPRITECYCLESPEEDCHANGE, PLEX_ERRORMESS_ANIMATION_SPRITE);
		return;
	}

	animationCount = PLEX_expandArrayCount(sprite->animations);

	if(index >= animationCount)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ANIMATION, PLEX_ERROR_OVERLIMIT, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ANIMATION_ONSPRITECYCLESPEEDCHANGE, PLEX_ERRORMESS_GENERIC_COUNT);
		return;
	}

	animation = PLEX_getExpandArrayEntry(index, sprite->animations);

	PLEX_changeAnimationCycleSpeed(cycle, animation->animation, time);
}

bool PLEX_isSpriteStopped(const PLEX_SPRITE* sprite)
{
	if(sprite == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ANIMATION, PLEX_ERROR_ISNULL, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_ANIMATION_ONGETSPRITESTOPPED, PLEX_ERRORMESS_ANIMATION_SPRITE);
		return false;
	}

	return sprite->stopped;
}

uint64_t PLEX_spriteCycleLen(const size_t index, const PLEX_SPRITE* sprite)
{
	P_ANIMATION_CONTAINER* animation = NULL;

	if(sprite == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ANIMATION, PLEX_ERROR_ISNULL, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_ANIMATION_ONGETSPRITECYCLELEN, PLEX_ERRORMESS_ANIMATION_SPRITE);
		return 0;
	}

	animation = PLEX_getExpandArrayEntry(index, sprite->animations);

	if(animation == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ANIMATION, PLEX_ERROR_ACQUIRE, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ANIMATION_ONGETSPRITECYCLELEN, PLEX_ERRORMESS_ANIMATION_SPRITE);
		return 0;
	}

	return PLEX_animationCycleLen(animation->animation);
}

bool PLEX_syncSprites(const size_t index, const PLEX_SPRITE* a, PLEX_SPRITE* b)
{
	P_ANIMATION_CONTAINER* animA = NULL;
	P_ANIMATION_CONTAINER* animB = NULL;

	size_t animationCountA = 0;
	size_t animationCountB = 0;

	if(a == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ANIMATION, PLEX_ERROR_ISNULL, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_ANIMATION_ONSYNCINGSPRITES, PLEX_ERRORMESS_ANIMATION_ANIMATION);
		return false;
	}

	if(b == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ANIMATION, PLEX_ERROR_ISNULL, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ANIMATION_ONSYNCINGSPRITES, PLEX_ERRORMESS_ANIMATION_ANIMATION2);
		return false;
	}

	animationCountA = PLEX_expandArrayCount(a->animations);

	if(index >= animationCountA)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ANIMATION, PLEX_ERROR_OVERLIMIT, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ANIMATION_ONSYNCINGSPRITES, PLEX_ERRORMESS_GENERIC_COUNT);
		return false;
	}

	animationCountB = PLEX_expandArrayCount(b->animations);

	if(index >= animationCountB)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ANIMATION, PLEX_ERROR_OVERLIMIT, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ANIMATION_ONSYNCINGSPRITES, PLEX_ERRORMESS_GENERIC_COUNT2);
		return false;
	}

	animA = PLEX_getExpandArrayEntry(index, a->animations);

	if(animA == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ANIMATION, PLEX_ERROR_ACQUIRE, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ANIMATION_ONSYNCINGSPRITES, PLEX_ERRORMESS_ANIMATION_ANIMATION);
		return false;
	}

	animB = PLEX_getExpandArrayEntry(index, b->animations);

	PLEX_syncAnimations(*animA->animation, animB->animation);

	return true;
}

PLEX_ANIMATION* PLEX_getAnimation(const size_t index, PLEX_SPRITE* sprite)
{
	P_ANIMATION_CONTAINER* container = NULL;

	size_t animationCount = 0;

	if(sprite == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ANIMATION, PLEX_ERROR_GETNULL, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ANIMATION_ONGETTINGANIMATION, PLEX_ERRORMESS_ANIMATION_SPRITE);
		return NULL;
	}

	animationCount = PLEX_expandArrayCount(sprite->animations);

	if(index >= animationCount)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ANIMATION, PLEX_ERROR_OVERLIMIT, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ANIMATION_ONGETTINGANIMATION, PLEX_ERRORMESS_GENERIC_COUNT);
		return NULL;
	}

	container = PLEX_getExpandArrayEntry(index, sprite->animations);

	if(container == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ANIMATION, PLEX_ERROR_ACQUIRE, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ANIMATION_ONGETTINGANIMATION, PLEX_ERRORMESS_ANIMATION_ANIMATION);
		return NULL;
	}

	return container->animation;
}

size_t PLEX_spriteTex(const PLEX_SPRITE* sprite)
{
	if(sprite == NULL)
	{
		// error
		return 0;
	}

	return PLEX_animationPlateWPerc(sprite->plate);
}

double PLEX_spriteWPerc(const PLEX_SPRITE* sprite)
{
	if(sprite == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ANIMATION, PLEX_ERROR_ACQUIRE, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ANIMATOIN_ONGETSPRITEWPERC, PLEX_ERRORMESS_ANIMATION_SPRITE);
		return 0;
	}

	return PLEX_animationPlateWPerc(sprite->plate);
}

double PLEX_spriteHPerc(const PLEX_SPRITE* sprite)
{
	if(sprite == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ANIMATION, PLEX_ERROR_ACQUIRE, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ANIMATOIN_ONGETSPRITEHPERC, PLEX_ERRORMESS_ANIMATION_SPRITE);
		return 0;
	}

	return PLEX_animationPlateHPerc(sprite->plate);
}
