#include "minigame/minigame0.h"

static bool p_running = false;

static PLEX_SPRITE p_pamaleanaSprite;
static PLEX_SPRITE p_table;
static PLEX_SPRITE p_coffeIcon;
static PLEX_SPRITE p_thePrize;

static size_t p_direction = 2; // up, right, down, left
static bool p_moving = false;
static bool p_pushing = false;

static PLEX_RECT p_space;
static PLEX_RECT p_prizeRect;

static PLEX_MPREC p_prizeMomentum = 0;
static PLEX_MPREC p_prizeDirection = 3; // left, right, nodirec

static uint64_t p_lastTime = 0;
static uint64_t p_pushTime = 0;
static uint64_t p_startTime = 0;

static PLEX_RECT p_tableSpace[4];
static PLEX_RECT p_playerCrashSpace[4];

static PLEX_MPREC p_tableMomentumX[4];
static PLEX_MPREC p_tableMomentumY[4];
static size_t p_tableDirec[4];
static size_t p_crashDirection[4] = { 5, 5, 5, 5 };

static size_t p_tutorial = 0;
static bool p_tutorialOn = true;

static double p_coffee = 1;

static double p_theLine = 0;
static double p_theLine2 = 0;

static PLEX_SPRITE p_enemySprite;

typedef struct
{
	PLEX_RECT rect;
	bool dead, vert;
} MMS0_ENEMY;

static const size_t p_stopGenKilled = 20;

static MMS0_ENEMY p_enemies[10];
static size_t p_enemyCount = 0, p_enemiesUsed = 0, p_enemiesKilled = 0;
static uint8_t p_lastEnemySpot = 0;

// this function contains what's needed for a cheat to unlock MMS1. Why you would employ such a cheat, I don't know.
static const uint64_t* p_MMS1Keyword()
{
	static const uint8_t codeWord[8] = { 'M', 'O', 'R', 'M', 'O', 'N', '1', '3' };

	return (uint64_t*)codeWord;
}

static void p_genMMS1UnlockFile()
{
	const uint64_t* const cwint = p_MMS1Keyword();

	FILE* unlock = fopen("unlocker", "r");

	if(unlock == NULL)
	{
		unlock = fopen("unlocker", "w+");

		if(unlock != NULL)
		{
			fwrite((void*)cwint, 1, sizeof(uint64_t), unlock);
			fclose(unlock);
		}
	}
}

static void p_generateEnemy()
{
	size_t index = p_enemyCount;

	MMS0_ENEMY* enemy = NULL;

	bool inc = true;

	if(p_enemiesUsed >= 10) return;

	for(size_t ze = 0; ze < p_enemyCount; ++ze)
	{
		enemy = p_enemies + ze;

		if(enemy->dead)
		{
			inc = false;
			index = ze;
			break;
		}
	}

	if(index > 9) return;

	enemy = p_enemies + index;

	enemy->dead = false;
	enemy->vert = PLEX_randomNumber(0,2);

	switch(p_lastEnemySpot)
	{
		case 0: enemy->rect.origin.x = 3.25; enemy->rect.origin.y = PLEX_HEIGHTUNITMAX() - 3.25; break;
		case 1: enemy->rect.origin.x = PLEX_WIDTHUNITMAX()/2; enemy->rect.origin.y = PLEX_HEIGHTUNITMAX() - 3.25; break;
		case 2: enemy->rect.origin.x = PLEX_WIDTHUNITMAX() - 3.25; enemy->rect.origin.y = PLEX_HEIGHTUNITMAX() - 3.25; break;
	}

	enemy->rect.dimens.w = 1.5;
	enemy->rect.dimens.h = 3;

	++p_enemiesUsed;
	if(inc) ++p_enemyCount;

	p_lastEnemySpot = (p_lastEnemySpot + 1) % 3;
}

static void p_drawFloor(const double perc)
{
	const double revPerc = 1 - perc;

	const uint32_t tileD = PLEX_UNIT(1.5);

	for(uint32_t x = 0; x < PLEX_WW(true); x += tileD)
	{
		const bool nudge = x/tileD % 2 == 1;

		for(int64_t y = 0; y < PLEX_WW(true); y += tileD)
		{
			const bool nudge2 = y/tileD % 2 == 1;

			const bool nudgeOp = (nudge && nudge2) || (!nudge && !nudge2);

			const uint8_t fr = 0;
			const uint8_t fg = 0;
			const uint8_t fb = 0;

			const uint8_t br = 60 * perc;
			const uint8_t bg = nudgeOp ? 30 * revPerc : 60 * revPerc;
			const uint8_t bb = nudgeOp ? 60 * revPerc : 30 * revPerc;

			PLEX_fillRect(10000, x, y, tileD, tileD, br, bg, bb, 255, true);
			PLEX_drawRect(9999, x, y, tileD, tileD, fr, fg, fb, 255, PLEX_UNIT(0.05), true);
		}
	}
}

static bool p_checkUnstuck(const bool resSpace, const PLEX_RECT newSpace, const PLEX_RECT crashSpace, const size_t direction, PLEX_RECT* check)
{
	const PLEX_RECT begin = *check;

	if(!resSpace) return false;

	if(PLEX_rectCollision(begin, crashSpace, 0, 0))
	{
		double nx = check->origin.x;
		double ny = check->origin.y;

		switch(direction)
		{
			case 0: ny = newSpace.origin.y + newSpace.dimens.h; break;
			case 1: if(begin.origin.x + begin.dimens.w > newSpace.origin.x) nx = newSpace.origin.x - check->dimens.w; break;
			case 2: ny = newSpace.origin.y - check->dimens.h; break;
		};

		check->origin.x = nx;
		check->origin.y = ny;

		return true;
	}

	return false;
}

bool MMS_beginMinigame0(const size_t chapter0spriteTex, const size_t pamaleanaSpriteTex, const uint64_t time)
{
	if(p_running) return true;

	PLEX_ANIMATION_PLATE pamaleanaPlate = PLEX_createAnimationPlate(0.0625, 0.125, pamaleanaSpriteTex);
	PLEX_ANIMATION_PLATE table = PLEX_createAnimationPlate(0.125, 0.125, chapter0spriteTex);
	PLEX_ANIMATION_PLATE coffee = PLEX_createAnimationPlate(0.125, 0.125, chapter0spriteTex);
	PLEX_ANIMATION_PLATE prize = PLEX_createAnimationPlate(0.125, 0.125, chapter0spriteTex);

	PLEX_ANIMATION_PLATE enemyPlate = PLEX_createAnimationPlate(0.0625, 0.125, chapter0spriteTex);

	PLEX_ANIMATION* pamaleanaAnimDown = NULL;
	PLEX_ANIMATION* pamaleanaAnimUp = NULL;
	PLEX_ANIMATION* pamaleanaAnimLeft = NULL;
	PLEX_ANIMATION* pamaleanaAnimRight = NULL;

	PLEX_ANIMATION* pamaleanaAnimPushDown = NULL;
	PLEX_ANIMATION* pamaleanaAnimPushUp = NULL;
	PLEX_ANIMATION* pamaleanaAnimPushLeft = NULL;
	PLEX_ANIMATION* pamaleanaAnimPushRight = NULL;

	PLEX_ANIMATION* tableAnim = NULL;

	PLEX_ANIMATION* coffeeAnim = NULL;

	PLEX_ANIMATION* prizeAnim = NULL;

	PLEX_ANIMATION* enemyAnimDown = NULL;
	PLEX_ANIMATION* enemyAnimUp = NULL;
	PLEX_ANIMATION* enemyAnimLeft = NULL;
	PLEX_ANIMATION* enemyAnimRight = NULL;

	PLEX_ANIMATION* enemyAnimDrink = NULL;

	// tutorial

	// 1.333 - four thirds
	p_tutorial = PLEX_loadTextureFromFile(PLEX_WH(true) * 1.3333, PLEX_WH(true), 255, 255, 255, 255, 0, "./resources/chapter0/tutorial.png");

	p_pamaleanaSprite = PLEX_createSprite(&pamaleanaPlate);

	PLEX_addAnimation(&p_pamaleanaSprite, 800, PLEX_getGameTime());
	PLEX_addAnimation(&p_pamaleanaSprite, 800, PLEX_getGameTime());
	PLEX_addAnimation(&p_pamaleanaSprite, 800, PLEX_getGameTime());
	PLEX_addAnimation(&p_pamaleanaSprite, 800, PLEX_getGameTime());

	PLEX_addAnimation(&p_pamaleanaSprite, 300, PLEX_getGameTime());
	PLEX_addAnimation(&p_pamaleanaSprite, 300, PLEX_getGameTime());
	PLEX_addAnimation(&p_pamaleanaSprite, 300, PLEX_getGameTime());
	PLEX_addAnimation(&p_pamaleanaSprite, 300, PLEX_getGameTime());

	pamaleanaAnimUp = PLEX_getAnimation(0, &p_pamaleanaSprite);
	pamaleanaAnimRight = PLEX_getAnimation(1, &p_pamaleanaSprite);
	pamaleanaAnimDown = PLEX_getAnimation(2, &p_pamaleanaSprite);
	pamaleanaAnimLeft = PLEX_getAnimation(3, &p_pamaleanaSprite);

	// MOVING

	PLEX_addAnimationFrame(4, pamaleanaAnimUp);
	PLEX_addAnimationFrame(5, pamaleanaAnimUp);
	PLEX_addAnimationFrame(6, pamaleanaAnimUp);
	PLEX_addAnimationFrame(5, pamaleanaAnimUp);
	PLEX_addAnimationFrame(4, pamaleanaAnimUp);
	PLEX_addAnimationFrame(7, pamaleanaAnimUp);
	PLEX_addAnimationFrame(8, pamaleanaAnimUp);
	PLEX_addAnimationFrame(7, pamaleanaAnimUp);

	PLEX_addAnimationFrame(9, pamaleanaAnimRight);
	PLEX_addAnimationFrame(10, pamaleanaAnimRight);
	PLEX_addAnimationFrame(11, pamaleanaAnimRight);
	PLEX_addAnimationFrame(12, pamaleanaAnimRight);
	PLEX_addAnimationFrame(11, pamaleanaAnimRight);

	PLEX_addAnimationFrame(13, pamaleanaAnimDown);
	PLEX_addAnimationFrame(14, pamaleanaAnimDown);
	PLEX_addAnimationFrame(15, pamaleanaAnimDown);
	PLEX_addAnimationFrame(14, pamaleanaAnimDown);
	PLEX_addAnimationFrame(13, pamaleanaAnimDown);
	PLEX_addAnimationFrame(16, pamaleanaAnimDown);
	PLEX_addAnimationFrame(13, pamaleanaAnimDown);
	PLEX_addAnimationFrame(16, pamaleanaAnimDown);

	PLEX_addAnimationFrame(18, pamaleanaAnimLeft);
	PLEX_addAnimationFrame(19, pamaleanaAnimLeft);
	PLEX_addAnimationFrame(20, pamaleanaAnimLeft);
	PLEX_addAnimationFrame(21, pamaleanaAnimLeft);
	PLEX_addAnimationFrame(19, pamaleanaAnimLeft);

	// PUSHING

	pamaleanaAnimPushUp = PLEX_getAnimation(4, &p_pamaleanaSprite);
	pamaleanaAnimPushRight = PLEX_getAnimation(5, &p_pamaleanaSprite);
	pamaleanaAnimPushDown = PLEX_getAnimation(6, &p_pamaleanaSprite);
	pamaleanaAnimPushLeft = PLEX_getAnimation(7, &p_pamaleanaSprite);

	PLEX_addAnimationFrame(22, pamaleanaAnimPushUp);
	PLEX_addAnimationFrame(23, pamaleanaAnimPushUp);

	PLEX_addAnimationFrame(24, pamaleanaAnimPushRight);
	PLEX_addAnimationFrame(25, pamaleanaAnimPushRight);

	PLEX_addAnimationFrame(26, pamaleanaAnimPushDown);
	PLEX_addAnimationFrame(27, pamaleanaAnimPushDown);

	PLEX_addAnimationFrame(28, pamaleanaAnimPushLeft);
	PLEX_addAnimationFrame(29, pamaleanaAnimPushLeft);

	PLEX_startSprite(time, &p_pamaleanaSprite);

	// TABLE

	p_table = PLEX_createSprite(&table);

	PLEX_addAnimation(&p_table, 1, PLEX_getGameTime());

	tableAnim = PLEX_getAnimation(0, &p_table);

	PLEX_addAnimationFrame(32, tableAnim);

	// COFFEE

	p_coffeIcon = PLEX_createSprite(&coffee);

	PLEX_addAnimation(&p_coffeIcon, 1, PLEX_getGameTime());

	coffeeAnim = PLEX_getAnimation(0, &p_coffeIcon);

	PLEX_addAnimationFrame(33, coffeeAnim);

	// ENEMY SPRITE

	p_enemySprite = PLEX_createSprite(&enemyPlate);

	PLEX_addAnimation(&p_enemySprite, 800, PLEX_getGameTime());
	PLEX_addAnimation(&p_enemySprite, 800, PLEX_getGameTime());
	PLEX_addAnimation(&p_enemySprite, 800, PLEX_getGameTime());
	PLEX_addAnimation(&p_enemySprite, 800, PLEX_getGameTime());
	PLEX_addAnimation(&p_enemySprite, 2000, PLEX_getGameTime());

	enemyAnimUp = PLEX_getAnimation(0, &p_enemySprite);
	enemyAnimRight = PLEX_getAnimation(1, &p_enemySprite);
	enemyAnimDown = PLEX_getAnimation(2, &p_enemySprite);
	enemyAnimLeft = PLEX_getAnimation(3, &p_enemySprite);
	enemyAnimDrink = PLEX_getAnimation(4, &p_enemySprite);

	PLEX_addAnimationFrame(41, enemyAnimUp);
	PLEX_addAnimationFrame(42, enemyAnimUp);
	PLEX_addAnimationFrame(43, enemyAnimUp);
	PLEX_addAnimationFrame(44, enemyAnimUp);
	PLEX_addAnimationFrame(45, enemyAnimUp);

	PLEX_addAnimationFrame(46, enemyAnimRight);
	PLEX_addAnimationFrame(47, enemyAnimRight);
	PLEX_addAnimationFrame(48, enemyAnimRight);
	PLEX_addAnimationFrame(49, enemyAnimRight);

	PLEX_addAnimationFrame(50, enemyAnimDown);
	PLEX_addAnimationFrame(51, enemyAnimDown);
	PLEX_addAnimationFrame(52, enemyAnimDown);
	PLEX_addAnimationFrame(53, enemyAnimDown);
	PLEX_addAnimationFrame(54, enemyAnimDown);

	PLEX_addAnimationFrame(55, enemyAnimLeft);
	PLEX_addAnimationFrame(56, enemyAnimLeft);
	PLEX_addAnimationFrame(57, enemyAnimLeft);
	PLEX_addAnimationFrame(58, enemyAnimLeft);

	PLEX_addAnimationFrame(59, enemyAnimDrink);
	PLEX_addAnimationFrame(60, enemyAnimDrink);

	PLEX_startSprite(PLEX_getGameTime(), &p_enemySprite);

	// PRIZE

	p_thePrize = PLEX_createSprite(&prize);

	PLEX_addAnimation(&p_thePrize, 1, PLEX_getGameTime());

	prizeAnim = PLEX_getAnimation(0, &p_thePrize);

	PLEX_addAnimationFrame(34, prizeAnim);

	p_space.origin = PLEX_convPoint(PLEX_WIDTHUNITMAX()/2, PLEX_HEIGHTUNITMAX()/4);
	p_space.dimens = PLEX_convDimens(1.5, 3);

	memset(p_tableMomentumX, 0, sizeof(PLEX_MPREC) * 4); // < note: this is an array
	memset(p_tableMomentumY, 0, sizeof(PLEX_MPREC) * 4);

	for(size_t ze = 0; ze < 4; ++ze) p_tableDirec[ze] = 5;

	p_prizeRect.dimens = PLEX_convDimens(4, 3);
	p_prizeRect.origin = PLEX_convPoint(PLEX_WIDTHUNITMAX()/2 - p_prizeRect.dimens.w/2, 0);

	p_theLine = p_prizeRect.dimens.h + p_space.dimens.h + 2;
	p_theLine2 = PLEX_HEIGHTUNITMAX() - p_theLine;

	for(size_t ze = 0; ze < 4; ++ze)
	{
		const double w = 4;
		const double h = 3;

		const uint32_t x = (ze % 2) *  PLEX_WIDTHUNITMAX()/2 + PLEX_WIDTHUNITMAX()/4 - w/2;
		const uint32_t y = p_theLine + ((ze >= 2) * (p_theLine2 - p_theLine));

		p_tableSpace[ze] = PLEX_convRect(PLEX_convPoint(x, y), PLEX_convDimens(w, h));
		p_playerCrashSpace[ze] = p_tableSpace[ze];
	}

	p_prizeDirection = 3;

	p_enemyCount = 0;
	p_enemiesUsed = 0;

	p_direction = 2;

	p_running = true;
	p_lastTime = time;
	p_startTime = time;
	p_tutorialOn = true;

	p_coffee = 1;

	p_enemiesKilled = 0;

	PLEX_hideCursor();

	return true;
}

bool MMS_updateMinigame0(bool* victory, const uint64_t time)
{
	const uint64_t turnTime = time - p_lastTime;

	const double momentum = turnTime * 0.008; // 8/1000
	const double enemyMomentum = turnTime * 0.002; // 2/1000
	const double tableMomentum = turnTime * 0.00005; // 0.05/1000.0

	double perc = p_enemiesKilled/(double)p_stopGenKilled;

	size_t enemyExpectation = perc/0.125 + 1;

	PLEX_POINT newOrigin = p_space.origin;
	PLEX_POINT newPrizeOrigin = p_prizeRect.origin;

	PLEX_MPREC momX = 0;
	PLEX_MPREC momY = 0;

	if(!p_running) return false;

	if(p_tutorialOn)
	{
		uint32_t fw = 0, fh = 0;

		p_startTime = time;
		p_lastTime = time;

		PLEX_textureWH(p_tutorial, &fw, &fh);

		p_drawFloor(perc);

		PLEX_printTexture(0, p_tutorial, 0, 0, fw, fh, 0, 0, PLEX_WW(true), PLEX_WH(true), true);

		if(PLEX_leftClick() || PLEX_KEY_press('\n'))
		{
			MM_COMMON_playTheme((size_t)MM_COMMON_THEMEID_PANIC);
			p_tutorialOn = false;
		}else PLEX_getUniversalThemeStopSignal();

		return true;
	}

	if(perc >= 1)
	{
		perc = 1;
		enemyExpectation = 0;

		if(!p_enemiesUsed)
		{
			*victory = true;
			return false;
		}
	}

	if(p_coffee <= 0){ *victory = false; return false; }

	p_moving = false;

	PLEX_drawSprite(0, 0, time, PLEX_UNIT(1), PLEX_UNIT(1), PLEX_UNIT(2), PLEX_UNIT(2), p_coffeIcon, true);

	PLEX_drawChargeBar(0, p_coffee, PLEX_UNIT(1), PLEX_UNIT(4), PLEX_UNIT(2), PLEX_WH(true) - PLEX_UNIT(6), 255, 255, 255, 200, 60, 50, 30, 200, PLEX_UNIT(0.05), true, true);

	PLEX_drawSprite(1, 0, time, PLEX_UNIT(p_prizeRect.origin.x), PLEX_UNIT(p_prizeRect.origin.y), PLEX_UNIT(p_prizeRect.dimens.w), PLEX_UNIT(p_prizeRect.dimens.h), p_thePrize, true);

	if(p_prizeDirection != 3)
	{
		if(!p_prizeDirection) p_prizeMomentum -= tableMomentum;
		else p_prizeMomentum += tableMomentum;

		newPrizeOrigin.x += p_prizeMomentum;
	}

	if(newPrizeOrigin.x < p_space.dimens.w)
	{
		newPrizeOrigin.x = p_space.dimens.w + momentum;
		p_prizeMomentum = 0;
		p_prizeDirection = 3;
	}

	if(newPrizeOrigin.x + p_prizeRect.dimens.w > PLEX_WIDTHUNITMAX() - p_space.dimens.w)
	{
		newPrizeOrigin.x = PLEX_WIDTHUNITMAX() - p_space.dimens.w - p_prizeRect.dimens.w - momentum;
		p_prizeMomentum = 0;
		p_prizeDirection = 3;
	}

	if(p_direction == 3 && p_pushing && p_space.origin.x > newPrizeOrigin.x + p_prizeRect.dimens.w && p_space.origin.y < p_prizeRect.dimens.h)
	{
		p_prizeDirection = 0;
	}

	if(p_direction == 1 && p_pushing && p_space.origin.x < newPrizeOrigin.x && p_space.origin.y < p_prizeRect.dimens.h)
	{
		p_prizeDirection = 1;
	}

	p_prizeRect.origin = newPrizeOrigin;

	if(!p_pushing)
	{
		if(PLEX_KEY_press('q'))
		{
			p_pushing = true;
			p_pushTime = time;
		}else{
			if(PLEX_KEY_down('w')){ p_direction = 0; p_moving = true; momY -= momentum; }
			if(PLEX_KEY_down('d')){ p_direction = 1; p_moving = true; momX += momentum; }
			if(PLEX_KEY_down('s')){ p_direction = 2; p_moving = true; momY += momentum; }
			if(PLEX_KEY_down('a')){ p_direction = 3; p_moving = true; momX -= momentum; }
		}

		if(p_moving)
		{
			if(PLEX_isSpriteStopped(p_pamaleanaSprite)) PLEX_startSprite(time, &p_pamaleanaSprite);
		}else{
			if(!PLEX_isSpriteStopped(p_pamaleanaSprite)) PLEX_stopSprite(&p_pamaleanaSprite);
		}
	}else{
		p_moving = false;

		if(PLEX_isSpriteStopped(p_pamaleanaSprite)) PLEX_startSprite(time, &p_pamaleanaSprite);

		if(time - p_pushTime >= 300) p_pushing = false;

		for(size_t ze = 0; ze < 4; ++ze)
		{
			const PLEX_MPREC dx = p_space.origin.x - p_tableSpace[ze].origin.x;
			const PLEX_MPREC dy = p_space.origin.y - p_tableSpace[ze].origin.y;

			const double x = p_space.origin.x;
			const double y = p_space.origin.y;
			const double w = p_space.dimens.w;
			const double h = p_space.dimens.h;

			const double tx = p_tableSpace[ze].origin.x;
			const double ty = p_tableSpace[ze].origin.y;
			const double tw = p_tableSpace[ze].dimens.w;
			const double th = p_tableSpace[ze].dimens.h;

			switch(p_direction)
			{
				case 0: if(dy > 0 && dy < 1 + th && x + w >= tx && x < tx + tw) p_tableDirec[ze] = 0; break;
				case 1: if(dx < 0 && dx > -2 && y + h > ty && y < ty + th) p_tableDirec[ze] = 1; break;
				case 2: if(dy < 0 && dy > 0 - (1 + th) && x + w >= tx && x < tx + tw) p_tableDirec[ze] = 2; break;
				case 3: if(dx > tw && dx < tw + 2 && y + h > ty && y < ty + th) p_tableDirec[ze] = 3; break;
			}
		}
	}

	for(size_t ze = 0; ze < 4; ++ze)
	{
		PLEX_RECT space = p_tableSpace[ze];

		PLEX_POINT newTableOrigin = space.origin;

		if(space.origin.x < 0) space.origin.x = 0;
		if(space.origin.y < 0) space.origin.y = 0;

		PLEX_drawSprite(1, 0, time, PLEX_UNIT(space.origin.x), PLEX_UNIT(space.origin.y), PLEX_UNIT(space.dimens.w), PLEX_UNIT(space.dimens.h), p_table, true);

		if(p_tableDirec[ze] != 5)
		{
			switch(p_tableDirec[ze])
			{
				case 0: p_tableMomentumY[ze] -= tableMomentum; break;
				case 1: p_tableMomentumX[ze] += tableMomentum; break;
				case 2: p_tableMomentumY[ze] += tableMomentum; break;
				case 3: p_tableMomentumX[ze] -= tableMomentum; break;
			};

			newTableOrigin.x += p_tableMomentumX[ze];
			newTableOrigin.y += p_tableMomentumY[ze];

			if(p_tableMomentumX[ze] < 0) p_playerCrashSpace[ze].origin.x += p_tableMomentumX[ze];
			if(p_tableMomentumY[ze] < 0) p_playerCrashSpace[ze].origin.y += p_tableMomentumY[ze];

			p_playerCrashSpace[ze].dimens.w += p_tableMomentumX[ze] * PLEX_signum(p_tableMomentumX[ze]);
			p_playerCrashSpace[ze].dimens.h += p_tableMomentumY[ze] * PLEX_signum(p_tableMomentumY[ze]);

			p_crashDirection[ze] = p_tableDirec[ze];
		}else{
			PLEX_RECT newSpace;

			bool resSpace = false;

			newTableOrigin.x -= p_tableMomentumX[ze];
			newTableOrigin.y -= p_tableMomentumY[ze];

			if(newTableOrigin.y < p_theLine){ newTableOrigin.y = p_theLine; resSpace = true; }
			if(newTableOrigin.y + space.dimens.h > p_theLine2){ newTableOrigin.y = p_theLine2; resSpace = true; }

			newSpace.origin = newTableOrigin;
			newSpace.dimens = space.dimens;

			if(p_checkUnstuck(resSpace, newSpace, p_playerCrashSpace[ze], p_crashDirection[ze], &p_space)) newOrigin = p_space.origin;

			for(size_t uh = 0; uh < p_enemiesUsed; ++uh)
			{
				MMS0_ENEMY* enemy = p_enemies + uh;

				if(p_checkUnstuck(resSpace, newSpace, p_playerCrashSpace[ze], p_crashDirection[ze], &enemy->rect))
				{
					enemy->vert = !enemy->vert;
				}
			}

			p_tableMomentumX[ze] = 0;
			p_tableMomentumY[ze] = 0;

			p_playerCrashSpace[ze] = p_tableSpace[ze];
			p_crashDirection[ze] = 5;
		}

		if(newTableOrigin.x < 0 || newTableOrigin.y < 0 || newTableOrigin.x + space.dimens.w > PLEX_WIDTHUNITMAX() || newTableOrigin.y + space.dimens.h > PLEX_HEIGHTUNITMAX()) p_tableDirec[ze] = 5;

		for(size_t uh = 0; uh < 4; ++uh)
		{
			const PLEX_RECT crashSpace = p_tableSpace[uh];

			if(uh == ze) continue;

			if(PLEX_rectCollision(space, crashSpace, p_tableMomentumX[ze], p_tableMomentumY[ze]))
			{
				p_tableDirec[ze] = 5;
				break;
			}
		}

		if(PLEX_rectCollision(space, p_prizeRect, p_tableMomentumX[ze], p_tableMomentumY[ze]))
		{
			p_tableDirec[ze] = 5;
		}

		p_tableSpace[ze].origin = newTableOrigin;

		// KEEP LAST

		if(PLEX_rectCollision(p_space, space, momX, momY))
		{
			momX = 0;
			momY = 0;
		}
	}

	if(PLEX_rectCollision(p_space, p_prizeRect, momX, momY))
	{
		momX = 0;
		momY = 0;
	}

	newOrigin.x += momX;
	newOrigin.y += momY;

	if(newOrigin.x < 0) newOrigin.x = 0;
	if(newOrigin.y < 0) newOrigin.y = 0;
	if(newOrigin.x + p_space.dimens.w > PLEX_WIDTHUNITMAX()) newOrigin.x = PLEX_WIDTHUNITMAX() - p_space.dimens.w;
	if(newOrigin.y + p_space.dimens.h > PLEX_HEIGHTUNITMAX()) newOrigin.y = PLEX_HEIGHTUNITMAX() - p_space.dimens.h;

	if(p_enemiesUsed < enemyExpectation) p_generateEnemy();

	for(size_t ze = 0; ze < p_enemyCount; ++ze)
	{
		MMS0_ENEMY* enemy = p_enemies + ze;

		if(!enemy->dead)
		{
			PLEX_POINT enewPoint = enemy->rect.origin;

			double emomX = 0;
			double emomY = 0;

			bool collision = false;
			bool destination = false;
			bool nomiddle = false;
			bool drinking = false;

			size_t direction = 0;

			if(p_pushing)
			{
				PLEX_RECT pushRect = p_space;
				PLEX_RECT oldRect = enemy->rect;

				switch(p_direction)
				{
					case 0: pushRect.origin.y -= 1; break;
					case 1: pushRect.origin.x += 1; break;
					case 2: pushRect.origin.y += 1; break;
					case 3: pushRect.origin.x -= 1; break;
				};

				if(PLEX_rectCollision(enemy->rect, pushRect, 0, 0))
				{
					switch(p_direction)
					{
						case 0: enemy->rect.origin.y -= 3; break;
						case 1: enemy->rect.origin.x += 3; break;
						case 2: enemy->rect.origin.y += 3; break;
						case 3: enemy->rect.origin.x -= 3; break;
					};

					if(enemy->rect.origin.x < 0) enemy->rect.origin.x = 0;
					if(enemy->rect.origin.y < p_theLine - enemy->rect.dimens.h) enemy->rect.origin.y = p_theLine - enemy->rect.dimens.h;
					if(enemy->rect.origin.x + enemy->rect.dimens.w > PLEX_WIDTHUNITMAX()) enemy->rect.origin.x = PLEX_WIDTHUNITMAX() - enemy->rect.dimens.w;
					if(enemy->rect.origin.y + enemy->rect.dimens.h > PLEX_HEIGHTUNITMAX()) enemy->rect.origin.y = PLEX_HEIGHTUNITMAX() - enemy->rect.dimens.h;

					if(PLEX_rectCollision(p_space, enemy->rect, 0, 0)) enemy->rect = oldRect;

					for(size_t uh = 0; uh < 4; ++uh){ if(PLEX_rectCollision(p_tableSpace[uh], enemy->rect, 0, 0)) enemy->rect = oldRect; }

					continue;
				}
			}

			if(enemy->vert)
			{
				emomY -= enemyMomentum;
			}else{
				if(enemy->rect.origin.x + emomX > p_prizeRect.origin.x) emomX -= enemyMomentum;
				else emomX += enemyMomentum;
			}

			if(PLEX_rectCollision(p_space, enemy->rect, momX, momY))
			{
				momX = 0;
				momY = 0;
				newOrigin = p_space.origin;
			}

			if(enemy->rect.origin.y <= p_prizeRect.dimens.h + 1)
			{
				enemy->rect.origin.y = p_prizeRect.dimens.h + 1;
				enemy->vert = false;
				emomY = 0;
				destination = true;

				if(p_prizeDirection == 3 && enemy->rect.origin.x > p_prizeRect.origin.x && enemy->rect.origin.x + enemy->rect.dimens.w < p_prizeRect.origin.x + p_prizeRect.dimens.w){ p_coffee -= 0.00075; drinking = true; }
			}

			if(PLEX_rectCollision(enemy->rect, p_space, emomX, emomY))
			{
				emomX = 0;
				emomY = 0;
				momX = 0;
				momY = 0;
				collision = true;
			}

			if(!enemy->vert && enemy->rect.origin.y >= PLEX_HEIGHTUNITMAX() - 6.0 && enemy->rect.origin.x > PLEX_WIDTHUNITMAX()/2 - enemy->rect.dimens.w/2 && enemy->rect.origin.x < PLEX_WIDTHUNITMAX()/2 + enemy->rect.dimens.w/2){ enemy->vert = true; nomiddle = true; }

			if(!enemy->vert && enemy->rect.origin.y >= PLEX_HEIGHTUNITMAX() - 6.0)
			{
				if(emomX > 0 && enemy->rect.origin.x > PLEX_WIDTHUNITMAX()/2) emomX = 0 - enemyMomentum;
				if(emomX < 0 && enemy->rect.origin.x < PLEX_WIDTHUNITMAX()/2) emomX = enemyMomentum;

				nomiddle = true;
			}

			for(size_t ze = 0; ze < 4; ++ze)
			{
				const PLEX_RECT tableSpace = p_tableSpace[ze];

				if(PLEX_rectCollision(enemy->rect, tableSpace, emomX, emomY))
				{
					bool kill = false;

					switch(p_crashDirection[ze])
					{
						case 0: kill = enemy->rect.origin.y + enemy->rect.dimens.h < tableSpace.origin.y + tableSpace.dimens.h; break;
						case 1: kill = enemy->rect.origin.x > tableSpace.origin.x; break;
						case 2: kill = enemy->rect.origin.y < tableSpace.origin.y; break;
						case 3: kill = enemy->rect.origin.x + enemy->rect.dimens.w < tableSpace.origin.x + tableSpace.dimens.w; break;
					};

					if(kill)
					{
						enemy->dead = true;
						++p_enemiesKilled;
						if(p_enemiesUsed) --p_enemiesUsed;
					}else collision = true;

					if(p_crashDirection[ze] == 5) kill = true;
				}
			}

			if(emomY < 0) direction = 0;
			if(emomX > 0) direction = 1;
			if(emomY > 0) direction = 2;
			if(emomX < 0) direction = 3;

			if(drinking) direction = 4;

			if(!collision)
			{
				const double diff = enewPoint.x - p_prizeRect.origin.x;

				if(!nomiddle && !destination && diff > 0 - 2 && diff < 2) enemy->vert = true;

				enewPoint.x += emomX;
				enewPoint.y += emomY;

				enemy->rect.origin = enewPoint;
			}else enemy->vert = !enemy->vert;

			PLEX_drawSprite(1, direction, time, PLEX_UNIT(enemy->rect.origin.x), PLEX_UNIT(enemy->rect.origin.y), PLEX_UNIT(enemy->rect.dimens.w), PLEX_UNIT(enemy->rect.dimens.h), p_enemySprite, true);
		}
	}

	p_space.origin = newOrigin;

	if(p_pushing) PLEX_drawSprite(1, p_direction + 4, time, PLEX_UNIT(p_space.origin.x), PLEX_UNIT(p_space.origin.y), PLEX_UNIT(p_space.dimens.w), PLEX_UNIT(p_space.dimens.h), p_pamaleanaSprite, true);
	else PLEX_drawSprite(1, p_direction, time, PLEX_UNIT(p_space.origin.x), PLEX_UNIT(p_space.origin.y), PLEX_UNIT(p_space.dimens.w), PLEX_UNIT(p_space.dimens.h), p_pamaleanaSprite, true);

	p_drawFloor(perc);

	p_lastTime = time;

	MM_COMMON_updateTheme();

	return true;
}

void MMS_endMinigame0()
{
	if(!p_running) return;

	/* DO NOT END SPRITES HERE. */

	PLEX_destroyTexture(p_tutorial);
	p_tutorial = 0;

	p_running = false;

	PLEX_unhideCursor();
}


void MMS_runMinigame0(const size_t chapter0spriteTex, const size_t pamaleanaSpriteTex)
{
	uint64_t time = PLEX_getGameTime();

	bool victory = false;

	p_running = false;

	while(!victory && !PLEX_isTerminated())
	{
		MMS_beginMinigame0(chapter0spriteTex, pamaleanaSpriteTex, time);

		while(MMS_updateMinigame0(&victory, time) && !PLEX_isTerminated())
		{
			PLEX_updatePlex();
			if(PLEX_askingToQuit()) PLEX_terminate();
			time = PLEX_getGameTime();
		}

		if(victory) p_genMMS1UnlockFile();

		MMS_endMinigame0();
	}
}

bool MMS_isMinigame0Running()
{
	return p_running;
}

bool MMS_MMS1Unlocked()
{
	const uint64_t* const cwint = p_MMS1Keyword();

	FILE* unlock = fopen("unlocker", "r");

	uint64_t cwint2 = 0;

	if(unlock == NULL) return false;

	fread((void*)&cwint2, 1, sizeof(uint64_t), unlock);

	fclose(unlock);

	return *cwint == cwint2;
}
