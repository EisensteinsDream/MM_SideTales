#include "windowing/input.h"

static bool p_keyDown[127];
static bool p_keyPress[127];
static bool p_fkeyPress[25];
static bool p_fkeyDown[25];

static bool p_shift = false;
static bool p_lshift = false;
static bool p_rshift = false;
static bool p_ctrl = false;
static bool p_lctrl = false;
static bool p_rctrl = false;

static bool p_upKey = false;
static bool p_downKey = false;
static bool p_leftKey = false;
static bool p_rightKey = false;

static bool p_upKeyPress = false;
static bool p_downKeyPress = false;
static bool p_leftKeyPress = false;
static bool p_rightKeyPress = false;

static bool p_anyKeyPress = false;

static bool p_left = false;
static bool p_leftClick = false;
static bool p_right = false;
static bool p_rightClick = false;
static bool p_midButton = false;
static bool p_midButtonClick = false;

static bool p_mouseMove = false;

static bool p_disablePause = false;

static int64_t p_mx = 0;
static int64_t p_my = 0;

static double p_mouseScroll = 0;

static int16_t p_axis[PLEX_LIMIT_CONTROLLERS][SDL_CONTROLLER_AXIS_MAX];

static SDL_GameController* p_controller[PLEX_LIMIT_CONTROLLERS];
static size_t p_controllerCount = 0;

static size_t p_customCursor = 0;
static size_t p_customCursorClick = 0;
static size_t p_customCursorDown = 0;

static bool p_hideCursor = false;

static uint32_t p_cursorW = 0;
static uint32_t p_cursorH = 0;
static uint32_t p_cursorFX = 0;
static uint32_t p_cursorFY = 0;
static uint32_t p_cursorFW = 0;
static uint32_t p_cursorFH = 0;

static uint8_t p_cursorR = 0;
static uint8_t p_cursorG = 0;
static uint8_t p_cursorB = 0;

static bool p_controllerButtonDown[PLEX_LIMIT_CONTROLLERS][SDL_CONTROLLER_BUTTON_MAX];
static bool p_controllerButtonPress[PLEX_LIMIT_CONTROLLERS][SDL_CONTROLLER_BUTTON_MAX];

static bool p_running = false;

static void p_pollController()
{
	p_controllerCount = 0;

	// do not give error for this
	if(PLEX_DEF_disableController()) return;

	for(size_t ze = 0; ze < SDL_NumJoysticks(); ++ze)
	{
		if(SDL_IsGameController(ze))
		{
			p_controller[ze] = SDL_GameControllerOpen(ze);
			++p_controllerCount;
		}
	}

	for(size_t ze = 0; ze < PLEX_LIMIT_CONTROLLERS; ++ze)
	{
		memset(p_controllerButtonDown + ze, false, SDL_CONTROLLER_BUTTON_MAX);
		memset(p_controllerButtonPress + ze, false, SDL_CONTROLLER_BUTTON_MAX);
	}
}

static void p_checkNextKey(const size_t nextKey, const size_t start, const size_t end, const size_t start2, const bool result, bool* target)
{
	for(size_t ze = start; ze <= end; ++ze)
	{
		const size_t it = ze - start;

		if(nextKey == ze)
		{
			target[start2 + it] = result;
			return;
		}
	}
}

static void p_checkFKey(const size_t nextKey, const bool result, bool* target)
{
	static const size_t f1Key = SDLK_F1;

	for(size_t ze = 0; ze <= 24; ++ze)
	{
		if(nextKey == f1Key + ze)
		{
			target[ze] = result;
			return;
		}
	}
}

// ---------------------------- //
// PUBLIC FUNCTIONS:
// ---------------------------- //

void PLEX_disablePause(){ p_disablePause = true; }
void PLEX_enablePause(){ p_disablePause = false; }

void PLEX_hideCursor(){ p_hideCursor = true; }
void PLEX_unhideCursor(){ p_hideCursor = false; }
void PLEX_toggleHideCursor(){ p_hideCursor = !p_hideCursor; }
bool PLEX_cursorIsHidden(){ return p_hideCursor; }

void PLEX_setCustomCursor(const size_t tex, const int64_t w, const int64_t h, const int64_t fx, const int64_t fy, const int64_t fw, const int64_t fh)
{
	PLEX_setCustomCursorTinted(tex, w, h, fx, fy, fw, fh, 255, 255, 255);
}

void PLEX_setCustomCursorTinted(const size_t tex, const int64_t w, const int64_t h, const int64_t fx, const int64_t fy, const int64_t fw, const int64_t fh, const uint8_t r, const uint8_t g, const uint8_t b)
{
	if(!tex)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_INPUT, PLEX_ERROR_ISNONTEXTURE, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONSETCUSTOMCURSORTINTED, PLEX_ERRORMESS_CURSORTEX);
		return;
	}

	p_customCursor = tex;
	p_customCursorClick = tex;
	p_customCursorDown = tex;

	p_cursorW = w;
	p_cursorH = h;

	p_cursorFX = fx;
	p_cursorFY = fy;
	p_cursorFW = fw;
	p_cursorFH = fh;

	p_cursorR = r;
	p_cursorG = g;
	p_cursorB = b;
}

void PLEX_setCustomCursorClick(const size_t tex)
{
	if(!p_customCursor)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_INPUT, PLEX_ERROR_ZEROVALUE, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONSETCUSTOMCURSOR, PLEX_ERRORMESS_CUSTOMCURSOR);
		return;
	}

	p_customCursorClick = tex;
}

void PLEX_setCustomCursorDown(const size_t tex)
{
	if(!p_customCursor)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_INPUT, PLEX_ERROR_ISNULL, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONSETCUSTOMCURSORDOWN, PLEX_ERRORMESS_CUSTOMCURSOR);
		return;
	}

	p_customCursorDown = tex;
}

void PLEX_unsetCustomCursor()
{
	p_customCursor = 0;
	p_customCursorClick = 0;
	p_customCursorDown = 0;
}

bool PLEX_startInput()
{
	if(p_running)
	{
		PLEX_errorNoDesc(PLEX_ERROR_CATEGORY_INPUT, PLEX_ERROR_DOUBLESTART, PLEX_ERROR_SEVERITY_BADSTYLE);
		return true;
	}

	if(SDL_InitSubSystem(SDL_INIT_EVENTS)) // remember 0 == success with this method
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_SDL, PLEX_ERROR_LOADEXTERNALMODULE, PLEX_ERROR_SEVERITY_FATAL, PLEX_ERRORMESS_ONINPUTSTART, PLEX_ERRORMESS_SDLMODULE_INPUT);
		PLEX_errorSDL(PLEX_ERROR_SEVERITY_FATAL);
		return false;
	}

	if(SDL_InitSubSystem(SDL_INIT_GAMECONTROLLER))
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_SDL, PLEX_ERROR_LOADEXTERNALMODULE, PLEX_ERROR_SEVERITY_MAJOR, PLEX_ERRORMESS_ONINPUTSTART, PLEX_ERRORMESS_SDLMODULE_CONTROLLER);
		PLEX_errorSDL(PLEX_ERROR_SEVERITY_MAJOR);
	}

	p_disablePause = false;

	p_customCursor = 0;
	p_customCursorClick = 0;
	p_customCursorDown = 0;

	memset(p_fkeyDown, false, 24);
	memset(p_keyDown, false, 127);

	p_upKey = false;
	p_rightKey = false;
	p_downKey = false;
	p_leftKey = false;

	p_cursorR = PLEX_DEF_cursorR();
	p_cursorG = PLEX_DEF_cursorG();
	p_cursorB = PLEX_DEF_cursorB();

	p_pollController();

	p_hideCursor = false;

#ifndef PLEX_DEBUGMOUSEMODE
	SDL_ShowCursor(SDL_DISABLE);
#endif

	p_running = true;

	return true;
}

bool PLEX_updateInput()
{
	const bool keyDisable = PLEX_DEF_disableKeyboard() && ((!PLEX_DEF_disableController() && p_controllerCount) || !PLEX_DEF_disableMouse());

	int mx = 0;
	int my = 0;

	double amx = 0;
	double amy = 0;
	double amw = 0;
	double amh = 0;

	double mpw = 0;
	double mph = 0;

	SDL_Event ev;

	Uint32 nextKey = 0;

	bool pauseToggle = false;

	if(!p_running)
	{
		PLEX_errorNoDesc(PLEX_ERROR_CATEGORY_INPUT, PLEX_ERROR_UPDATEBEFORESTART, PLEX_ERROR_SEVERITY_MINOR);
		return false;
	}

	if(!p_controllerCount) p_pollController();

	SDL_GetMouseState(&mx, &my);

	amx = (double)mx - PLEX_WOFFSET();
	amy = (double)my - PLEX_HOFFSET();

	amw = PLEX_WW(true);
	amh = PLEX_WH(true);

	mpw = amx/amw;
	mph = amy/amh;

	p_mx = mpw * PLEX_WW(false);
	p_my = mph * PLEX_WH(false);

	memset(p_keyPress, false, 127);
	memset(p_fkeyPress, false, 24);

	p_keyDown['\n'] = false;

	for(size_t ze = 0; ze < PLEX_LIMIT_CONTROLLERS; ++ze) memset(p_controllerButtonPress + ze, false, SDL_CONTROLLER_BUTTON_MAX);

	p_shift = false;
	p_ctrl = false;

	p_upKeyPress = false;
	p_downKeyPress = false;
	p_leftKeyPress = false;
	p_rightKeyPress = false;

	p_leftClick = false;
	p_rightClick = false;

	p_mouseMove = false;

	p_anyKeyPress = false;

	p_mouseScroll = 0;

	SDL_PumpEvents();

	PLEX_unmarkSizing(); // < keep before polling

	while(SDL_PollEvent(&ev))
	{
		switch(ev.type)
		{
			case SDL_WINDOWEVENT:
				if(PLEX_DEF_killOnClose() && ev.window.event == SDL_WINDOWEVENT_CLOSE) PLEX_askQuit();

				if(ev.window.event == SDL_WINDOWEVENT_RESIZED)
				{
					PLEX_updateWindowResize();
					PLEX_markSizing();
				}

				if(!PLEX_DEF_allowMaximize() && ev.window.event == SDL_WINDOWEVENT_MAXIMIZED)
				{
					PLEX_restoreWindow();
					PLEX_markSizing();
				}
			break;

			case SDL_KEYDOWN:
				nextKey = ev.key.keysym.sym;

				if(PLEX_DEF_killOnEscape() && nextKey == SDLK_ESCAPE) PLEX_askQuit();

				if(!keyDisable)
				{
					if(nextKey == SDLK_SPACE)
					{
						if(p_disablePause) p_keyPress[' '] = true;
						else pauseToggle = true;
					}

					if(nextKey == SDLK_RSHIFT) p_rshift = true;
					if(nextKey == SDLK_LSHIFT) p_lshift = true;
					if(nextKey == SDLK_RCTRL) p_rctrl = true;
					if(nextKey == SDLK_LCTRL) p_lctrl = true;

					p_checkNextKey(nextKey, SDLK_0, SDLK_9, '0', true, p_keyPress);
					p_checkNextKey(nextKey, SDLK_a, SDLK_z, 'a', true, p_keyPress);

					p_checkNextKey(nextKey, SDLK_0, SDLK_9, '0', true, p_keyDown);
					p_checkNextKey(nextKey, SDLK_a, SDLK_z, 'a', true, p_keyDown);

					p_checkFKey(nextKey, true, p_fkeyPress);
					p_checkFKey(nextKey, true, p_fkeyDown);

					if(nextKey == SDLK_RETURN || nextKey == SDLK_KP_ENTER)
					{
						p_keyPress['\n'] = true;
						p_keyDown['\n'] = true;
					}

					if(nextKey == SDLK_LEFT){ p_leftKey  = true; p_leftKeyPress = true; }
					if(nextKey == SDLK_RIGHT){ p_rightKey = true; p_rightKeyPress = true; }
					if(nextKey == SDLK_UP){ p_upKey = true; p_upKeyPress = true; }
					if(nextKey == SDLK_DOWN){  p_downKey  = true; p_downKeyPress = true; }
				}

				p_anyKeyPress = true;
			break;

			case SDL_KEYUP:
				nextKey = ev.key.keysym.sym;

				if(!keyDisable)
				{
					if(nextKey == SDLK_SPACE && p_disablePause) p_keyDown[' '] = false;

					p_checkNextKey(nextKey, SDLK_0, SDLK_9, '0', false, p_keyDown);
					p_checkNextKey(nextKey, SDLK_a, SDLK_z, 'a', false, p_keyDown);

					p_checkFKey(nextKey, false, p_fkeyDown);

					if(nextKey == SDLK_RETURN || nextKey == SDLK_KP_ENTER) p_keyDown['\n'] = false;

					if(nextKey == SDLK_LEFT) p_leftKey = false;
					if(nextKey == SDLK_RIGHT) p_rightKey = false;
					if(nextKey == SDLK_UP) p_upKey = false;
					if(nextKey == SDLK_DOWN) p_downKey = false;
				}
			break;

			case SDL_MOUSEBUTTONDOWN:
				if(ev.button.button == SDL_BUTTON_LEFT) p_left = true;
				if(ev.button.button == SDL_BUTTON_RIGHT) p_right = true;
				if(ev.button.button == SDL_BUTTON_MIDDLE) p_midButton = true;
			break;

			case SDL_MOUSEBUTTONUP:
				if(ev.button.button == SDL_BUTTON_LEFT)
				{
					p_left = false;
					p_leftClick = true;
				}

				if(ev.button.button == SDL_BUTTON_RIGHT)
				{
					p_right = false;
					p_rightClick = true;
				}

				if(ev.button.button == SDL_BUTTON_MIDDLE)
				{
					p_midButton = false;
					p_midButtonClick = true;
				}
			break;

			case SDL_MOUSEWHEEL:
				p_mouseScroll = ev.wheel.preciseY;
			break;

			case SDL_MOUSEMOTION:
				p_mouseMove = true;
			break;

			case SDL_CONTROLLERBUTTONDOWN:
				if(ev.cbutton.which < p_controllerCount)
				{
					p_controllerButtonDown[ev.cbutton.which][ev.cbutton.button] = true;
					p_controllerButtonPress[ev.cbutton.which][ev.cbutton.button] = true;
				}
			break;

			case SDL_CONTROLLERBUTTONUP:
				if(ev.cbutton.which < p_controllerCount)
				{
					if(PLEX_DEF_controllerPause() && ev.cbutton.button == SDL_CONTROLLER_BUTTON_START) pauseToggle = true;
					else p_controllerButtonDown[ev.cbutton.which][ev.cbutton.button] = false;
				}
			break;

			case SDL_CONTROLLERAXISMOTION:
				p_axis[ev.caxis.which][ev.caxis.axis] = ev.caxis.value;
			break;
		};
	};

	if(pauseToggle)
	{
		if(PLEX_getGameTimePaused()) PLEX_unpauseGameTime();
		else PLEX_pauseGameTime();
	}

	p_shift = p_rshift || p_lshift;
	p_ctrl = p_rctrl || p_lctrl;

	return true;
}

void PLEX_endInput()
{
	if(!p_running)
	{
		PLEX_errorNoDesc(PLEX_ERROR_CATEGORY_INPUT, PLEX_ERROR_ENDBEFORESTART, PLEX_ERROR_SEVERITY_BADSTYLE);
		return;
	}

	for(size_t ze = 0; ze < p_controllerCount; ++ze)
	{
		if(SDL_IsGameController(ze)) SDL_GameControllerClose(p_controller[ze]);
	}

	p_controllerCount = 0;

	SDL_QuitSubSystem(SDL_INIT_EVENTS);

	p_running = false;
}

bool PLEX_pauseDisabled(){ return p_disablePause; }

void PLEX_cursorDimens(uint32_t* w, uint32_t* h, uint32_t* fx, uint32_t* fy, uint32_t* fw, uint32_t* fh)
{
	*w = p_cursorW;
	*h = p_cursorH;

	*fx = p_cursorFX;
	*fy = p_cursorFY;
	*fw = p_cursorFW;
	*fh = p_cursorFH;
}

bool PLEX_usingCustomCursor(){ return p_customCursor; }

size_t PLEX_customCursor()
{
	if(p_leftClick) return p_customCursorClick;
	if(p_left) return p_customCursorDown;

	return p_customCursor;
}

bool PLEX_KEY_down(const uint8_t key){ return p_keyDown[key]; }
bool PLEX_KEY_press(const uint8_t key){ return p_keyPress[key]; }

bool PLEX_KEY_fdown(const size_t number){ if(!number || number > 24) return false; return p_fkeyDown[number - 1]; }
bool PLEX_KEY_fpress(const size_t number){ if(!number || number > 24) return false; return p_fkeyPress[number - 1]; }

bool PLEX_mouseInside(const int64_t x, const int64_t y, const uint32_t w, const uint32_t h, const bool offset)
{
	const PLEX_POINT mousePoint = PLEX_convPoint(p_mx, p_my);

	const PLEX_RECT region = PLEX_convRect(PLEX_convPoint(x + (offset * PLEX_WOFFSET()), y + (offset * PLEX_HOFFSET())), PLEX_convDimens(w, h));

	return PLEX_insideRect(mousePoint, region);
}

void PLEX_clearMouseClicks()
{
	p_left = false;
	p_right = false;
	p_leftClick = false;
	p_rightClick = false;
	p_midButton = false;
	p_midButtonClick = false;
}

bool PLEX_KEY_rshift(){ return p_rshift; }
bool PLEX_KEY_lshift(){ return p_lshift; }
bool PLEX_KEY_shift(){ return p_shift; }

bool PLEX_KEY_rctrl(){ return p_rctrl; }
bool PLEX_KEY_lctrl(){ return p_lctrl; }
bool PLEX_KEY_ctrl(){ return p_ctrl; }

bool PLEX_upKey(){ return p_upKey; }
bool PLEX_leftKey(){ return p_leftKey; }
bool PLEX_downKey(){ return p_downKey; }
bool PLEX_rightKey(){ return p_rightKey; }

bool PLEX_upKeyPress(){ return p_upKeyPress; }
bool PLEX_leftKeyPress(){ return p_leftKeyPress; }
bool PLEX_downKeyPress(){ return p_downKeyPress; }
bool PLEX_rightKeyPress(){ return p_rightKeyPress; }

int64_t PLEX_MX()
{
	if(PLEX_DEF_disableMouse()) return 0;

	return p_mx;
}

int64_t PLEX_MY()
{
	if(PLEX_DEF_disableMouse()) return 0;

	return p_my;
}

bool PLEX_left(){ if(PLEX_DEF_disableMouse()) return false; return p_left; }
bool PLEX_leftClick(){ if(PLEX_DEF_disableMouse()) return false; return p_leftClick; }

bool PLEX_right(){ if(PLEX_DEF_disableMouse()) return false; return p_right; }
bool PLEX_rightClick(){ if(PLEX_DEF_disableMouse()) return false; return p_rightClick; }

bool PLEX_midButton(){ return p_midButton; }
bool PLEX_midButtonClick(){ return p_midButtonClick; }

double PLEX_mouseScroll(){ return p_mouseScroll; }

bool PLEX_mouseMoved(){ return p_mouseMove; }

bool PLEX_CONTROLLER_down(const size_t controller, const uint8_t button)
{
	if(controller >= PLEX_LIMIT_CONTROLLERS)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_INPUT, PLEX_ERROR_OVERLIMIT, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONINPUTUPDATE, PLEX_ERRORMESS_CONTROLLER);
		return false;
	}

	if(button >= SDL_CONTROLLER_BUTTON_MAX)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_INPUT, PLEX_ERROR_OVERLIMIT, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONINPUTUPDATE, PLEX_ERRORMESS_GENERIC_INDEX);
		return false;
	}

	if(PLEX_DEF_disableController())
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_INPUT, PLEX_ERROR_DISABLED, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_ONINPUTUPDATE, PLEX_ERRORMESS_CONTROLLER);
		return false;
	}

	return p_controllerButtonDown[controller][button];
}

bool PLEX_CONTROLLER_press(const size_t controller, const uint8_t button)
{
	if(controller >= PLEX_LIMIT_CONTROLLERS)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_INPUT, PLEX_ERROR_OVERLIMIT, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONCONTROLLERPRESS, PLEX_ERRORMESS_CONTROLLER);
		return false;
	}

	if(button >= SDL_CONTROLLER_BUTTON_MAX)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_INPUT, PLEX_ERROR_OVERLIMIT, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONCONTROLLERPRESS, PLEX_ERRORMESS_GENERIC_INDEX);
		return false;
	}

	if(PLEX_DEF_disableController())
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_INPUT, PLEX_ERROR_DISABLED, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_ONCONTROLLERPRESS, PLEX_ERRORMESS_CONTROLLER);
		return false;
	}

	return p_controllerButtonPress[controller][button];
}

bool PLEX_KEY_anyPress(){ return p_anyKeyPress; }

bool PLEX_CONTROLLER_axis(const size_t controller, const size_t axisNum, const double sensitivity)
{
	if(controller >= p_controllerCount)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_INPUT, PLEX_ERROR_OVERLIMIT, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_ONCONTROLLERAXIS, PLEX_ERRORMESS_GENERIC_INDEX);
		return 0;
	}

	if(axisNum >= SDL_CONTROLLER_AXIS_MAX)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_INPUT, PLEX_ERROR_OVERLIMIT, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONCONTROLLERAXIS, PLEX_ERRORMESS_AXISVALUE);
		return 0;
	}

	if(PLEX_DEF_disableController())
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_INPUT, PLEX_ERROR_DISABLED, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_ONCONTROLLERAXIS, PLEX_ERRORMESS_CONTROLLER);
		return 0;
	}

	return sensitivity < 0 ? p_axis[controller][axisNum] < sensitivity : p_axis[controller][axisNum] > sensitivity;
}

bool PLEX_CONTROLLER_axisUp(const size_t controller)
{
	if(controller >= p_controllerCount)
	{
		// error
		return 0;
	}

	if(PLEX_DEF_disableController())
	{
		// error
		return 0;
	}

	return PLEX_CONTROLLER_axis(controller, 1, -10000);
}

bool PLEX_CONTROLLER_axisRight(const size_t controller)
{
	if(controller >= p_controllerCount)
	{
		// error
		return 0;
	}

	if(PLEX_DEF_disableController())
	{
		// error
		return 0;
	}

	return PLEX_CONTROLLER_axis(controller, 0, 10000);
}

bool PLEX_CONTROLLER_axisDown(const size_t controller)
{
	if(controller >= p_controllerCount)
	{
		// error
		return 0;
	}

	if(PLEX_DEF_disableController())
	{
		// error
		return 0;
	}

	return PLEX_CONTROLLER_axis(controller, 1, 10000);
}

bool PLEX_CONTROLLER_axisLeft(const size_t controller)
{
	if(controller >= p_controllerCount)
	{
		// error
		return 0;
	}

	if(PLEX_DEF_disableController())
	{
		// error
		return 0;
	}

	return PLEX_CONTROLLER_axis(controller, 0, -10000);
}

size_t PLEX_CONTROLLER_count()
{
	if(PLEX_DEF_disableController())
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_INPUT, PLEX_ERROR_DISABLED, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_ONCONTROLLERCOUNT, PLEX_ERRORMESS_CONTROLLER);
		return 0;
	}

	return p_controllerCount;
}
