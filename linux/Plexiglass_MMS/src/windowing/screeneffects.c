#include "windowing/screeneffects.h"

typedef struct
{
	PLEX_SCREENEFFECT_TYPE type;
	PLEX_RECT range;
	bool loop, active, screenOffset, ranged, initial, over;
	uint64_t start, duration;
	uint8_t r, g, b, a;
	double minPerc;
} PLEX_SCREENEFFECT;

static bool p_running = false;

static PLEX_EXPANDARRAY* p_effects = NULL;

static bool p_disableScreenEffects = false;

static void p_tintFadeEffect(const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a, const double perc, const double minPerc, const bool ranged, const PLEX_RECT range, const bool screenOffset, const size_t linenum, const char* const filename)
{
	const double alpha = a * minPerc + a * perc * (1 - minPerc);

	const uint32_t x = ranged ? range.origin.x : 0;
	const uint32_t y = ranged ? range.origin.y : 0;
	const uint32_t w = ranged ? range.dimens.w : PLEX_WW(screenOffset);
	const uint32_t h = ranged ? range.dimens.h : PLEX_WH(screenOffset);

	PLEX_fillRectInt(0, x, y, w, h, r, g, b, alpha, screenOffset, linenum, filename);
}

static void p_splatterEffect(const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a, const double perc, const double minPerc, const bool ranged, const PLEX_RECT range, const bool screenOffset, const bool initial, const size_t linenum, const char* const filename)
{

	const PLEX_RECT arange = ranged ? range : PLEX_convRect(PLEX_convPoint(PLEX_WOFFSET(), PLEX_HOFFSET()), PLEX_convDimens(PLEX_WW(true), PLEX_WH(true)));
	const double mx = range.origin.x + range.dimens.w/2;
	const double my = range.origin.y + range.dimens.h/2;

	const PLEX_POINT midpoint = PLEX_convPoint(mx, my);

	const double adaptPerc = (perc - minPerc)/(1 - minPerc);

	static double lastBoundary = 0;
	static PLEX_RECT splatterRects[16];

	PLEX_MPREC lenx = 0;
	PLEX_MPREC leny = 0;
	PLEX_MPREC halfDist = 0;
	PLEX_MPREC boundary = 0;

	halfDist = PLEX_distance(arange.origin, midpoint, &lenx, &leny);
	boundary = halfDist * adaptPerc;

	for(size_t ze = 0; ze < 16; ++ze)
	{
		const size_t direc = ze % 8;

		PLEX_MPREC bu = 0;
		PLEX_MPREC bdu = 0;
		PLEX_MPREC bux = 0;
		PLEX_MPREC buy = 0;

		double bnx = 0;
		double bny = 0;
		double rpw = 0;
		double rph = 0;

		PLEX_RECT* splatterRect = splatterRects + ze;

		if(initial)
		{
			splatterRect->origin.x = mx + boundary * PLEX_randomNumber(-0.1, 0.25);
			splatterRect->origin.y = my + boundary * PLEX_randomNumber(-0.1, 0.25);
			splatterRect->dimens.w = boundary * 0.1;
			splatterRect->dimens.h = boundary * 0.1;
			lastBoundary = 0;
		}

		bnx = PLEX_randomNumber(-0.25, 0.25);
		bny = PLEX_randomNumber(-0.25, 0.25);
		rpw = PLEX_randomNumber(0, 1);
		rph = PLEX_randomNumber(0, 1);

		bu = boundary - lastBoundary;

		bdu = bu;

		bux = bu * (rpw - bnx);
		buy = bu * (rph - bny);

		splatterRect->dimens.w += bdu * (1 - rpw);
		splatterRect->dimens.h += bdu * (1 - rph);

		switch(direc)
		{
			case 0:
				splatterRect->origin.x -= bux/2;
				splatterRect->origin.y -= buy/2;
			break;

			case 1:
				splatterRect->origin.y -= buy;
			break;

			case 2:
				splatterRect->origin.x += bux/2;
				splatterRect->origin.y -= buy/2;
			break;

			case 3:
				splatterRect->origin.x += bux;
			break;

			case 4:
				splatterRect->origin.x += buy/2;
				splatterRect->origin.y += buy/2;
			break;

			case 5:
				splatterRect->origin.y -= buy;
			break;

			case 6:
				splatterRect->origin.x -= bux/2;
				splatterRect->origin.y += buy/2;
			break;

			case 7:
				splatterRect->origin.x -= bux;
			break;
		};

		PLEX_fillRectInt(0, splatterRect->origin.x, splatterRect->origin.y, splatterRect->dimens.w, splatterRect->dimens.h, r, g, b, a, screenOffset, linenum, filename);
	}

	lastBoundary = boundary;
}

static void p_triggerEffect(const PLEX_SCREENEFFECT_TYPE type, const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a, const double perc, const double minPerc, const bool ranged, const PLEX_RECT range, const bool screenOffset, const bool initial, const size_t linenum, const char* const filename)
{
	if(p_disableScreenEffects) return;

	switch(type)
	{
		case PLEX_SCREENEFFECT_TINTFADE:
			p_tintFadeEffect(r, g, b, a, perc, minPerc, ranged, range, screenOffset, linenum, filename);
		break;

		case PLEX_SCREENEFFECT_SPLATTER:
			p_splatterEffect(r, g, b, a, perc, minPerc, ranged, range, screenOffset, initial, linenum, filename);
		break;
	};
}

static int64_t p_doScreenEffectBase(const PLEX_SCREENEFFECT_TYPE type, const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a, const bool loop, const bool screenOffset, const uint64_t duration, const double minPerc, const uint64_t time, const bool ranged, const PLEX_RECT range, const size_t linenum, const char* const filename)
{
	PLEX_SCREENEFFECT* effect = NULL;

	size_t index = 0;

	if(p_effects == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_SCREENEFFECTS, PLEX_ERROR_ISNULL, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONDOINGSCREENEFFECT, PLEX_ERRORMESS_SCREENEFFECTSTACK);
		return 0;
	}

	if(!PLEX_incrementExpandArrayInt(p_effects, linenum, filename))
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_SCREENEFFECTS, PLEX_ERROR_INCREMENT, PLEX_ERROR_SEVERITY_FATAL, PLEX_ERRORMESS_ONDOINGSCREENEFFECT, PLEX_ERRORMESS_SCREENEFFECTSTACK);
		return 0;
	}

	index = PLEX_expandArrayCount(p_effects) - 1;

	effect = PLEX_getExpandArrayLast(p_effects);

	memset(effect, 0, sizeof(PLEX_SCREENEFFECT));

	effect->type = type;
	effect->loop = loop;
	effect->screenOffset = screenOffset;
	effect->duration = duration;
	effect->start = time;
	effect->r = r;
	effect->g = g;
	effect->b = b;
	effect->a = a;
	effect->minPerc = minPerc;
	effect->ranged = ranged;
	effect->range = range;
	effect->active = true;
	effect->initial = true;

	return index;
}

// ---------------------------- //
// PUBLIC FUNCTIONS:
// ---------------------------- //

int64_t PLEX_doScreenEffectInt(const PLEX_SCREENEFFECT_TYPE type, const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a, const bool loop, const bool screenOffset, const uint64_t duration, const double minPerc, const uint64_t time, const size_t linenum, const char* const filename)
{
	PLEX_RECT range;

	if(!p_running)
	{
		PLEX_error(PLEX_ERROR_CATEGORY_SCREENEFFECTS, PLEX_ERROR_ACTIONBEFORESTART, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONDOINGSCREENEFFECT);
		return -1;
	}

	range = PLEX_convRect(PLEX_convPoint(0,0), PLEX_convDimens(0,0));

	return p_doScreenEffectBase(type, r, g, b, a, loop, screenOffset, duration, minPerc, time, false, range, linenum, filename);
}

int64_t PLEX_doScreenEffectRangedInt(const PLEX_SCREENEFFECT_TYPE type, const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a, const bool loop, const bool screenOffset, const uint64_t duration, const double minPerc, const PLEX_RECT range, const uint64_t time, const size_t linenum, const char* const filename)
{
	if(!p_running)
	{
		PLEX_error(PLEX_ERROR_CATEGORY_SCREENEFFECTS, PLEX_ERROR_ACTIONBEFORESTART, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONDOINGRANGEDSCREENEFFECT);
		return -1;
	}

	return p_doScreenEffectBase(type, r, g, b, a, loop, screenOffset, duration, minPerc, time, true, range, linenum, filename);
}

void PLEX_ceaseScreenEffect(const int64_t index)
{
	PLEX_SCREENEFFECT* effect = NULL;

	if(!p_running)
	{
		PLEX_error(PLEX_ERROR_CATEGORY_SCREENEFFECTS, PLEX_ERROR_ACTIONBEFORESTART, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONCEASINGSCREENEFFECT);
		return;
	}

	if(index < 0)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_SCREENEFFECTS, PLEX_ERROR_UNDERLIMIT, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONCEASINGSCREENEFFECT, PLEX_ERRORMESS_GENERIC_INDEX);
		return;
	}

	if(index >= PLEX_expandArrayCount(p_effects))
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_SCREENEFFECTS, PLEX_ERROR_OVERLIMIT, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONCEASINGSCREENEFFECT, PLEX_ERRORMESS_GENERIC_INDEX);
		return;
	}

	effect = PLEX_getExpandArrayEntry(index, p_effects);

	effect->active = false;
}

void PLEX_disableScreenEffects(){ p_disableScreenEffects = true; }
void PLEX_enableScreenEffects(){ p_disableScreenEffects = false; }

bool PLEX_startScreenEffects(const uint64_t time)
{
	if(p_running)
	{
		PLEX_errorNoDesc(PLEX_ERROR_CATEGORY_SCREENEFFECTS, PLEX_ERROR_DOUBLESTART, PLEX_ERROR_SEVERITY_BADSTYLE);
		return true;
	}

	p_effects = PLEX_genExpandArray(PLEX_getMemProfileScreenEffectsPreload(), sizeof(PLEX_SCREENEFFECT));

	p_running = true;

	return true;
}

bool PLEX_updateScreenEffects(const uint64_t time)
{
	size_t count = 0;

	if(!p_running)
	{
		PLEX_errorNoDesc(PLEX_ERROR_CATEGORY_SCREENEFFECTS, PLEX_ERROR_UPDATEBEFORESTART, PLEX_ERROR_SEVERITY_MINOR);
		return false;
	}

	count = PLEX_expandArrayCount(p_effects);

	for(size_t ze = 0; ze < count; ++ze)
	{
		PLEX_SCREENEFFECT* effect = PLEX_getExpandArrayEntry(ze, p_effects);

		double duration = 0;

		if(!effect->active) continue;

		duration = time - effect->start;

		if(duration <= effect->duration)
		{
			const double perc = duration/(double)effect->duration;

			p_triggerEffect(effect->type, effect->r, effect->g, effect->b, effect->a, perc, effect->minPerc, effect->ranged, effect->range, effect->screenOffset, effect->initial, __LINE__, __FILE__);
			effect->initial = false;
		}else{
			p_triggerEffect(effect->type, effect->r, effect->g, effect->b, effect->a, 1, effect->minPerc, effect->ranged, effect->range, effect->screenOffset, effect->initial, __LINE__, __FILE__);

			if(effect->loop) effect->start = time;
			else
			{
				effect->active = false;
				effect->over = true;
			}
		}
	}

	return true;
}

void PLEX_endScreenEffects()
{
	if(!p_running)
	{
		PLEX_errorNoDesc(PLEX_ERROR_CATEGORY_SCREENEFFECTS, PLEX_ERROR_ENDBEFORESTART, PLEX_ERROR_SEVERITY_MINOR);
		return;
	}

	PLEX_destroyExpandArray(&p_effects);
}

bool PLEX_screenEffectOver(const int64_t index, const uint64_t time)
{
	PLEX_SCREENEFFECT* effect = NULL;

	if(!p_running)
	{
		PLEX_errorNoDesc(PLEX_ERROR_CATEGORY_SCREENEFFECTS, PLEX_ERROR_ACTIONBEFORESTART, PLEX_ERROR_SEVERITY_MINOR);
		return false;
	}

	if(index < 0 || p_disableScreenEffects) return true;

	effect = PLEX_getExpandArrayEntry(index, p_effects);

	return effect->over;
}

bool PLEX_screenEffectsDisabled(){ return p_disableScreenEffects; }
