#include "utility/spritetheater.h"

static bool p_running = false;
static bool p_dispPause = false;

static size_t p_backdrop = 0;
static size_t p_font = 0;
static size_t p_labelPlate = 0;
static size_t p_bubble = 0;
static size_t p_arrow = 0;

static uint32_t p_labelPlateX = 0;
static uint32_t p_labelPlateY = 0;
static uint32_t p_labelPlateW = 0;
static uint32_t p_labelPlateH = 0;

static uint32_t p_bubbleW = 0;
static uint32_t p_bubbleH = 0;

static uint32_t p_arrowW = 0;
static uint32_t p_arrowH = 0;

static uint8_t p_labelR = 0;
static uint8_t p_labelG = 0;
static uint8_t p_labelB = 0;

static uint8_t p_messageR = 0;
static uint8_t p_messageG = 0;
static uint8_t p_messageB = 0;

static uint64_t p_theaterStart = 0;

static const char* p_stageName = NULL;
static const char* p_message = NULL;

typedef struct
{
	const char* message;
	uint8_t r, g, b;
	int64_t actor;
	uint64_t startTime, waitTime;
	bool pauseForWait;
} PLEX_MESSAGE_SLIDE;

typedef struct
{
	PLEX_SPRITE* sprite;
	PLEX_RECT space;
	bool hidden;
	size_t aniIndex;
} PLEX_MESSAGE_ACTOR;

typedef enum
{
	PLEX_MESSAGE_ACTIONCHANGE_HIDE = 0,
	PLEX_MESSAGE_ACTIONCHANGE_UNHIDE = 1,
	PLEX_MESSAGE_ACTIONCHANGE_CHANGEANIMATION = 2,
	PLEX_MESSAGE_ACTIONCHANGE_MOVE = 3,
	PLEX_MESSAGE_ACTIONCHANGE_BACKGROUND = 4,
	PLEX_MESSAGE_ACTIONCHANGE_RESIZE = 5
} PLEX_MESSAGE_ACTORCHANGEEVENT;

typedef struct
{
	int64_t actor;
	size_t slide, backdrop;
	PLEX_MESSAGE_ACTORCHANGEEVENT event;
	double amount, amount2;
	bool used;
	const char* stageName;
} PLEX_MESSAGE_ACTORCHANGE;

typedef enum
{
	PLEX_MESSAGE_MUSICSTOP = 0,
	PLEX_MESSAGE_MUSICPLAY = 1,
	PLEX_MESSAGE_SOUNDEFFECT = 2
} PLEX_MESSAGE_MUSICCHANGEEVENT;

typedef struct
{
	size_t slide, id;
	PLEX_MESSAGE_MUSICCHANGEEVENT event;
	bool used;
} PLEX_MESSAGE_MUSICCHANGE;

static PLEX_EXPANDARRAY* p_slides = NULL;
static PLEX_EXPANDARRAY* p_actors = NULL;

static PLEX_EXPANDARRAY* p_changes = NULL;

static PLEX_EXPANDARRAY* p_musicChanges = NULL;

static size_t p_slideIndex = 0;

static int64_t p_actorIndex = -1;

static size_t p_currentSong = 0;

static bool p_waitStopMusic = false;

static bool p_checkExpandArrays(const size_t line, const char* const filename)
{
	if(p_slides == NULL)
	{
		p_slides = PLEX_genExpandArrayInt(PLEX_getMemProfileSpriteTheaterSlide(), sizeof(PLEX_MESSAGE_SLIDE), line, filename);

		if(p_slides == NULL)
		{
			PLEX_error2(PLEX_ERROR_CATEGORY_SPRITETHEATER, PLEX_ERROR_ACQUIRE, PLEX_ERROR_SEVERITY_FATAL, PLEX_ERRORMESS_SPRITETHEATER_ONSPRITETHEATERCHECKEXPANDARRAYS, PLEX_ERRORMESS_SPRITETHEATER_SLIDES);
			return false;
		}
	}

	if(p_actors == NULL)
	{
		p_actors = PLEX_genExpandArrayInt(PLEX_getMemProfileActor(), sizeof(PLEX_MESSAGE_ACTOR), line, filename);

		if(p_actors == NULL)
		{
			PLEX_destroyExpandArray(&p_slides);

			PLEX_error2(PLEX_ERROR_CATEGORY_SPRITETHEATER, PLEX_ERROR_ACQUIRE, PLEX_ERROR_SEVERITY_FATAL, PLEX_ERRORMESS_SPRITETHEATER_ONSPRITETHEATERCHECKEXPANDARRAYS, PLEX_ERRORMESS_SPRITETHEATER_ACTORS);
			return false;
		}
	}

	if(p_changes == NULL)
	{
		p_changes = PLEX_genExpandArrayInt(PLEX_getMemProfileActorChange(), sizeof(PLEX_MESSAGE_ACTORCHANGE), line, filename);

		if(p_changes == NULL)
		{
			PLEX_destroyExpandArray(&p_slides);
			PLEX_destroyExpandArray(&p_actors);

			PLEX_error2(PLEX_ERROR_CATEGORY_SPRITETHEATER, PLEX_ERROR_ACQUIRE, PLEX_ERROR_SEVERITY_FATAL, PLEX_ERRORMESS_SPRITETHEATER_ONSPRITETHEATERCHECKEXPANDARRAYS, PLEX_ERRORMESS_SPRITETHEATER_CHANGES);
			return false;
		}
	}

	if(p_musicChanges == NULL)
	{
		p_musicChanges = PLEX_genExpandArrayInt(PLEX_getMemProfileMusicChange(), sizeof(PLEX_MESSAGE_MUSICCHANGE), line, filename);

		if(p_musicChanges == NULL)
		{
			PLEX_destroyExpandArray(&p_slides);
			PLEX_destroyExpandArray(&p_actors);
			PLEX_destroyExpandArray(&p_changes);

			PLEX_error2(PLEX_ERROR_CATEGORY_SPRITETHEATER, PLEX_ERROR_ACQUIRE, PLEX_ERROR_SEVERITY_FATAL, PLEX_ERRORMESS_SPRITETHEATER_ONSPRITETHEATERCHECKEXPANDARRAYS, PLEX_ERRORMESS_SPRITETHEATER_MUSICCHANGES);
			return false;
		}
	}

	return true;
}

static void p_setChange(const PLEX_MESSAGE_ACTORCHANGE change, const size_t line, const char* const filename)
{
	PLEX_MESSAGE_ACTORCHANGE* toChange = NULL;

	if(!p_checkExpandArrays(line, filename))
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_SPRITETHEATER, PLEX_ERROR_EXPAND, PLEX_ERROR_SEVERITY_FATAL, PLEX_ERRORMESS_SPRITETHEATER_ONSPRITEETHEATERSETACTORCHANGE, PLEX_ERRORMESS_SPRITETHEATER_CHANGES);
		return;
	}

	if(!PLEX_incrementExpandArrayInt(p_changes, line, filename))
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_SPRITETHEATER, PLEX_ERROR_INCREMENT, PLEX_ERROR_SEVERITY_FATAL, PLEX_ERRORMESS_SPRITETHEATER_ONSPRITEETHEATERSETACTORCHANGE, PLEX_ERRORMESS_SPRITETHEATER_CHANGES);
		return;
	}

	toChange = PLEX_getExpandArrayLast(p_changes);

	*toChange = change;
}

static void p_setChangeMusic(const PLEX_MESSAGE_MUSICCHANGE change, const size_t line, const char* const filename)
{
	PLEX_MESSAGE_MUSICCHANGE* toChange = NULL;

	if(!p_checkExpandArrays(line, filename))
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_SPRITETHEATER, PLEX_ERROR_EXPAND, PLEX_ERROR_SEVERITY_FATAL, PLEX_ERRORMESS_SPRITETHEATER_ONSPRITEETHEATERSETMUSICCHANGE, PLEX_ERRORMESS_SPRITETHEATER_MUSICCHANGES);
		return;
	}

	if(!PLEX_incrementExpandArrayInt(p_musicChanges, line, filename))
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_SPRITETHEATER, PLEX_ERROR_INCREMENT, PLEX_ERROR_SEVERITY_FATAL, PLEX_ERRORMESS_SPRITETHEATER_ONSPRITEETHEATERSETMUSICCHANGE, PLEX_ERRORMESS_SPRITETHEATER_MUSICCHANGES);
		return;
	}

	toChange = PLEX_getExpandArrayLast(p_musicChanges);

	*toChange = change;
}


// ---------------------------- //
// PUBLIC FUNCTIONS:
// ---------------------------- //

int64_t PLEX_createActorInt(PLEX_SPRITE* sprite, const PLEX_RECT space, const bool hidden, const size_t line, const char* const filename)
{
	PLEX_MESSAGE_ACTOR* actor = NULL;

	if(!p_checkExpandArrays(line, filename))
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_SPRITETHEATER, PLEX_ERROR_EXPAND, PLEX_ERROR_SEVERITY_FATAL, PLEX_ERRORMESS_SPRITETHEATER_ONCREATEACTOR, PLEX_ERRORMESS_SPRITETHEATER_ACTORS);
		return -1;
	}

	if(!PLEX_incrementExpandArrayInt(p_actors, line, filename))
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_SPRITETHEATER, PLEX_ERROR_INCREMENT, PLEX_ERROR_SEVERITY_FATAL, PLEX_ERRORMESS_SPRITETHEATER_ONCREATEACTOR, PLEX_ERRORMESS_SPRITETHEATER_ACTORS);
		return -1;
	}

	actor = PLEX_getExpandArrayLast(p_actors);

	actor->sprite = sprite;
	actor->space = space;
	actor->hidden = hidden;
	actor->aniIndex = 0;

	return PLEX_expandArrayCount(p_actors) - 1;
}

void PLEX_addSpriteTheaterSlideInt(const uint8_t r, const uint8_t g, const uint8_t b, const char* const message, const int64_t actor, const uint64_t waitTime, const bool pauseForWait, const size_t line, const char* const filename)
{
	// do not null check 'message'

	PLEX_MESSAGE_SLIDE* slide = NULL;

	if(!p_checkExpandArrays(line, filename))
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_SPRITETHEATER, PLEX_ERROR_EXPAND, PLEX_ERROR_SEVERITY_FATAL, PLEX_ERRORMESS_SPRITETHEATER_ONADDSPRITETHEATERSLIDE, PLEX_ERRORMESS_SPRITETHEATER_SLIDES);
		return;
	}

	if(!PLEX_incrementExpandArrayInt(p_slides, line, filename))
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_SPRITETHEATER, PLEX_ERROR_INCREMENT, PLEX_ERROR_SEVERITY_FATAL, PLEX_ERRORMESS_SPRITETHEATER_ONADDSPRITETHEATERSLIDE, PLEX_ERRORMESS_SPRITETHEATER_SLIDES);
		return;
	}

	slide = PLEX_getExpandArrayLast(p_slides);

	if(message == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_SPRITETHEATER, PLEX_ERROR_ISNULL, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_SPRITETHEATER_ONADDSPRITETHEATERSLIDE, PLEX_ERRORMESS_GENERIC_MESSAGE);
		memset(slide, 0, sizeof(PLEX_MESSAGE_SLIDE));
		return;
	}

	slide->r = r;
	slide->g = g;
	slide->b = b;
	slide->message = message;
	slide->actor = actor;
	slide->waitTime = waitTime;
	slide->pauseForWait = pauseForWait;

	if(!p_slideIndex)
	{
		p_messageR = r;
		p_messageB = b;
		p_messageG = g;
		p_message = message;
		p_actorIndex = actor;

		++p_slideIndex;
	}
}

void PLEX_nextSlide()
{
	PLEX_MESSAGE_SLIDE* slide = NULL;

	if(p_slideIndex < PLEX_expandArrayCount(p_slides))
	{
		slide = PLEX_getExpandArrayEntry(p_slideIndex, p_slides);

		p_messageR = slide->r;
		p_messageG = slide->g;
		p_messageB = slide->b;
		p_message = slide->message;
		p_actorIndex = slide->actor;
	}

	++p_slideIndex;
}

void PLEX_hideActorChangeInt(const size_t actor, const size_t line, const char* const filename)
{
	PLEX_MESSAGE_ACTORCHANGE change;

	change.event = PLEX_MESSAGE_ACTIONCHANGE_HIDE;
	change.actor = actor;
	change.slide = PLEX_expandArrayCount(p_slides);
	change.used = false;

	p_setChange(change, line, filename);
}

void PLEX_unhideActorChangeInt(const size_t actor, const size_t line, const char* const filename)
{
	PLEX_MESSAGE_ACTORCHANGE change;

	change.event = PLEX_MESSAGE_ACTIONCHANGE_UNHIDE;
	change.actor = actor;
	change.slide = PLEX_expandArrayCount(p_slides);
	change.used = false;

	p_setChange(change, line, filename);
}

void PLEX_animationActorChangeInt(const size_t actor, const size_t index, const size_t line, const char* const filename)
{
	PLEX_MESSAGE_ACTORCHANGE change;

	change.event = PLEX_MESSAGE_ACTIONCHANGE_CHANGEANIMATION;
	change.actor = actor;
	change.slide = PLEX_expandArrayCount(p_slides);
	change.amount = index;
	change.used = false;

	p_setChange(change, line, filename);
}

void PLEX_moveActorChangeInt(const size_t actor, const double x, const double y, const size_t line, const char* const filename)
{
	PLEX_MESSAGE_ACTORCHANGE change;

	change.event = PLEX_MESSAGE_ACTIONCHANGE_MOVE;
	change.actor = actor;
	change.slide = PLEX_expandArrayCount(p_slides);
	change.amount = x;
	change.amount2 = y;
	change.used = false;

	p_setChange(change, line, filename);
}

void PLEX_spriteTheaterBackgroundChangeInt(const size_t backdrop, const char* const name, const size_t line, const char* const filename)
{
	PLEX_MESSAGE_ACTORCHANGE change;

	if(name == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_SPRITETHEATER, PLEX_ERROR_OVERLIMIT, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_SPRITETHEATER_ONBACKGROUNDCHANGE, PLEX_ERRORMESS_SPRITETHEATER_ONCHANGESPRITETHEATERBACKGROUND);
		return;
	}

	change.event = PLEX_MESSAGE_ACTIONCHANGE_BACKGROUND;
	change.backdrop = backdrop;
	change.stageName = name;
	change.slide = PLEX_expandArrayCount(p_slides);
	change.used = false;

	p_setChange(change, line, filename);
}

void PLEX_actorResizeInt(const size_t actor, const double w, const double h, const size_t line, const char* const filename)
{
	PLEX_MESSAGE_ACTORCHANGE change;

	change.event = PLEX_MESSAGE_ACTIONCHANGE_RESIZE;
	change.actor = actor;
	change.slide = PLEX_expandArrayCount(p_slides);
	change.amount = w;
	change.amount2 = h;
	change.used = false;

	p_setChange(change, line, filename);
}

void PLEX_spriteTheaterMusicChangeInt(const size_t ID, const size_t line, const char* const filename)
{
	PLEX_MESSAGE_MUSICCHANGE change;

	change.event = PLEX_MESSAGE_MUSICPLAY;
	change.id = ID;
	change.slide = PLEX_expandArrayCount(p_slides);
	change.used = false;

	p_setChangeMusic(change, line, filename);
}

void PLEX_spriteTheaterMusicStopInt(const size_t line, const char* const filename)
{
	PLEX_MESSAGE_MUSICCHANGE change;

	change.event = PLEX_MESSAGE_MUSICSTOP;
	change.slide = PLEX_expandArrayCount(p_slides);
	change.used = false;

	p_setChangeMusic(change, line, filename);
}

void PLEX_spriteTheaterSoundEffectInt(const size_t ID, const size_t line, const char* const filename)
{
	PLEX_MESSAGE_MUSICCHANGE change;

	change.event = PLEX_MESSAGE_SOUNDEFFECT;
	change.id = ID;
	change.slide = PLEX_expandArrayCount(p_slides);
	change.used = false;

	p_setChangeMusic(change, line, filename);
}

bool PLEX_beginSpriteTheater(const char* const stageName, const size_t backdrop, const uint32_t labelPlateX, const uint32_t labelPlateY, const uint32_t labelPlateW, const uint32_t labelPlateH, const uint8_t labelR, const uint8_t labelG, const uint8_t labelB, const size_t font, const size_t labelPlate, const size_t bubble, const size_t arrow, const uint64_t time)
{
	if(p_running)
	{
		PLEX_errorNoDesc(PLEX_ERROR_CATEGORY_SPRITETHEATER, PLEX_ERROR_DOUBLESTART, PLEX_ERROR_SEVERITY_MINOR);
		return true;
	}

	if(stageName == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_SPRITETHEATER, PLEX_ERROR_ISNULL, PLEX_ERROR_SEVERITY_MAJOR, PLEX_ERRORMESS_SPRITETHEATER_ONSPRITETHEATERSOUNDEFFECT, PLEX_ERRORMESS_SPRITETHEATER_STAGENAME);
		return false;
	}

	p_backdrop = backdrop;
	p_theaterStart = time;
	p_stageName = stageName;

	p_labelPlateX = labelPlateX;
	p_labelPlateY = labelPlateY;
	p_labelPlateW = labelPlateW;
	p_labelPlateH = labelPlateH;

	p_labelR = labelR;
	p_labelG = labelG;
	p_labelB = labelB;

	p_font = font;
	p_labelPlate = labelPlate;

	p_bubble = bubble;
	p_arrow = arrow;

	if(bubble) PLEX_textureWH(bubble, &p_bubbleW, &p_bubbleH);
	if(arrow) PLEX_textureWH(arrow, &p_arrowW, &p_arrowH);

	p_slideIndex = 0;
	p_actorIndex = -1;

	p_running = true;
	p_dispPause = false;

	if(!PLEX_pauseDisabled())
	{
		PLEX_disablePause();
		p_dispPause = true;
	}

	return true;
}

bool PLEX_updateSpriteTheater(const uint64_t time)
{
	bool slideWait = false;

	uint32_t bh = PLEX_UNIT(4);

	if(!p_running)
	{
		PLEX_errorNoDesc(PLEX_ERROR_CATEGORY_SPRITETHEATER, PLEX_ERROR_UPDATEBEFORESTART, PLEX_ERROR_SEVERITY_MINOR);
		return false;
	}

	if(p_slideIndex < PLEX_expandArrayCount(p_slides) && p_slideIndex > 0)
	{
		PLEX_MESSAGE_SLIDE* slide = PLEX_getExpandArrayEntry(p_slideIndex - 1, p_slides);

		if(!slide->startTime) slide->startTime = time;

		slideWait = time - slide->startTime < slide->waitTime;

		if(slideWait && slide->pauseForWait)
		{
			PLEX_signalUniversalThemeStop();
			p_waitStopMusic = true;
		}else{
			if(p_waitStopMusic)
			{
				if(p_currentSong) PLEX_setUniversalThemeID(p_currentSong);
				p_waitStopMusic = false;
			}
		}
	}

	if(p_backdrop) PLEX_drawPanorama(10000, p_backdrop);

	if(p_stageName != NULL && p_font && p_labelPlate) PLEX_drawLabel(3, p_stageName, PLEX_STANDNUM_FW, PLEX_STANDNUM_FH, p_labelR, p_labelG, p_labelB, 255, PLEX_UNIT(0.25), PLEX_UNIT(0.25), PLEX_UNIT(12), PLEX_UNIT(1.5), 0.05, 0.1, 0.9, 0.9, PLEX_UNIT(0.05), PLEX_LANGUAGE_ALPHASTRING, p_font, p_labelPlate, p_labelPlateX, p_labelPlateY, p_labelPlateW, p_labelPlateH, true);

	if(!slideWait && p_message != NULL)
	{
		if(p_font)
		{
			const uint32_t ox = PLEX_UNIT(2);
			const uint32_t oy = PLEX_UNIT(2.75);

			const uint32_t fw = PLEX_UNIT(0.5);
			const uint32_t fh = PLEX_UNIT(1);

			const uint32_t ex = PLEX_WW(true) - ox;

			size_t index = 0;
			char next = p_message[index];

			uint32_t x = ox;
			uint32_t y = oy;

			char wbuffer[1024];
			size_t wit = 0;

			wbuffer[0] = '\0';

			while(next != '\0')
			{
				if(next == '\t' || next == ' ')
				{
					const uint32_t wordWidth = fw * wit;

					if(next == '\t') x += fw * 5;
					else x += fw;

					if(x + wordWidth >= ex)
					{
						x = ox;
						y += fh;
					}

					if(wit) PLEX_printFontStringTinted(0, PLEX_STANDNUM_FW, PLEX_STANDNUM_FH, x, y, fw, fh, p_messageR, p_messageG, p_messageB, (char*)wbuffer, PLEX_LANGUAGE_ALPHASTRING, p_font, true);

					x += wordWidth;
					wit = 0;
					wbuffer[0] = '\0';
				}else{
					if(next == '\n' || next == '\r')
					{
						x = ox;
						y += fh;
					}else{
						wbuffer[wit] = next;
						wbuffer[wit + 1] = '\0';
						++wit;
					}
				}

				++index;
				next = p_message[index];
			}

			if(x + fw * wit >= ex)
			{
				x = ox;
				y += fh;
			}

			x += fw;

			if(wit) PLEX_printFontStringTinted(0, PLEX_STANDNUM_FW, PLEX_STANDNUM_FH, x, y, fw, fh, p_messageR, p_messageG, p_messageB, (char*)wbuffer, PLEX_LANGUAGE_ALPHASTRING, p_font, true);

			if(y >= PLEX_UNIT(0.25) + bh) bh = PLEX_UNIT(0.25) + y;
		}

		if(!slideWait && p_bubble) PLEX_printTexture(1, p_bubble, 0, 0, p_bubbleW, p_bubbleH, PLEX_UNIT(0.25), PLEX_UNIT(2.5), PLEX_WW(true) - PLEX_UNIT(0.5), bh, true);

		if(p_arrow && p_actorIndex >= 0)
		{
			const PLEX_MESSAGE_ACTOR* actor = PLEX_getExpandArrayEntry(p_actorIndex, p_actors);

			const double ax = PLEX_UNIT(actor->space.origin.x);
			const double ay = PLEX_UNIT(actor->space.origin.y);
			const double aw = PLEX_UNIT(actor->space.dimens.w);

			const double w = aw * 3;

			PLEX_printTexture(2, p_arrow, 0, 0, p_arrowW, p_arrowH, (ax + aw/2) - w/2, bh, w, (ay - bh) * 0.75, true);
		}


		for(size_t ze = 0; ze < PLEX_expandArrayCount(p_changes); ++ze)
		{
			PLEX_MESSAGE_ACTORCHANGE* change = PLEX_getExpandArrayEntry(ze, p_changes);

			if(change->slide < p_slideIndex && !change->used)
			{
				PLEX_MESSAGE_ACTOR* actor = NULL;

				if(change->actor < PLEX_expandArrayCount(p_actors) && change->actor >= 0) actor = PLEX_getExpandArrayEntry(change->actor, p_actors);

				switch(change->event)
				{
					case PLEX_MESSAGE_ACTIONCHANGE_HIDE: if(actor != NULL) actor->hidden = true; break;
					case PLEX_MESSAGE_ACTIONCHANGE_UNHIDE: if(actor != NULL) actor->hidden = false; break;
					case PLEX_MESSAGE_ACTIONCHANGE_CHANGEANIMATION: if(actor != NULL) actor->aniIndex = change->amount; break;
					case PLEX_MESSAGE_ACTIONCHANGE_MOVE: if(actor != NULL) actor->space.origin = PLEX_convPoint(change->amount, change->amount2); break;

					case PLEX_MESSAGE_ACTIONCHANGE_BACKGROUND:
						p_backdrop = change->backdrop;
						p_stageName = change->stageName;
					break;

					case PLEX_MESSAGE_ACTIONCHANGE_RESIZE: if(actor != NULL) actor->space.dimens = PLEX_convDimens(change->amount, change->amount2); break;
				};

				change->used = true;
			}
		}

		for(size_t ze = 0; ze < PLEX_expandArrayCount(p_musicChanges); ++ze)
		{
			PLEX_MESSAGE_MUSICCHANGE* change = PLEX_getExpandArrayEntry(ze, p_musicChanges);

			if(change->slide < p_slideIndex && !change->used)
			{
				switch(change->event)
				{
					case PLEX_MESSAGE_MUSICPLAY:
						if(change->id) PLEX_setUniversalThemeID(change->id); else PLEX_signalUniversalThemeStop();
						p_currentSong = change->id;

					break;

					case PLEX_MESSAGE_MUSICSTOP: PLEX_signalUniversalThemeStop(); p_currentSong = 0; break;
					case PLEX_MESSAGE_SOUNDEFFECT: PLEX_setUniversalSoundEffect(change->id); break;
				};

				change->used = true;
			}
		}
	}

	for(size_t ze = 0; ze < PLEX_expandArrayCount(p_actors); ++ze)
	{
		PLEX_MESSAGE_ACTOR* actor = PLEX_getExpandArrayEntry(ze, p_actors);

		double x = PLEX_UNIT(actor->space.origin.x);
		double y = PLEX_UNIT(actor->space.origin.y);
		double w = PLEX_UNIT(actor->space.dimens.w);
		double h = PLEX_UNIT(actor->space.dimens.h);

		if(actor->hidden) continue;

		if(PLEX_isSpriteStopped(actor->sprite)) PLEX_startSprite(time, actor->sprite);

		PLEX_drawSpriteInt(3, actor->aniIndex, x, y, w, h, actor->sprite, true, time, __LINE__, __FILE__);
	}

	if(!slideWait && (PLEX_leftClick() || PLEX_KEY_press('\n') || PLEX_CONTROLLER_press(0, SDL_CONTROLLER_BUTTON_DPAD_RIGHT))) PLEX_nextSlide();

	return true;
}

void PLEX_endSpriteTheater()
{
	if(!p_running)
	{
		PLEX_errorNoDesc(PLEX_ERROR_CATEGORY_SPRITETHEATER, PLEX_ERROR_ENDBEFORESTART, PLEX_ERROR_SEVERITY_MINOR);
		return;
	}

	if(p_dispPause) PLEX_enablePause();

	PLEX_signalUniversalThemeStop();

	if(p_slides != NULL) PLEX_destroyExpandArray(&p_slides);
	if(p_actors != NULL) PLEX_destroyExpandArray(&p_actors);
	if(p_changes != NULL) PLEX_destroyExpandArray(&p_changes);
	if(p_musicChanges != NULL) PLEX_destroyExpandArray(&p_musicChanges);

	p_dispPause = false;
	p_running = false;
}

bool PLEX_inSpriteTheater(){ return p_running; }

bool PLEX_isSpriteTheaterOver()
{
	if(!p_running) return true; // < not running == over, no need for error checks.

	return p_slideIndex > PLEX_expandArrayCount(p_slides);
}
