#include "minigame/minigame1.h"

#define MMS1_ANGLERISE 0.5

#define MMS1_OBSTUCTION_LIMIT 32

#define MMS1_MENULAYER 0
#define MMS1_OVERROADLAYER 5000
#define MMS1_ENDLAYER 10000

typedef struct
{
	size_t lane;
	double velocity, leftDrift, rightDrift, driftVelocity, weight, drift, driftAccum, totalVelocity;
	bool deactivate, crashed;
	uint64_t crashedMarker;
	PLEX_RECT space, translatedSpace;
	PLEX_SPRITE sprite;
} MMS1_OBSTRUCTION;

static const double p_pamaleanaAnimationFastestCycle = 100;
static const double p_pamaleanaAnimationSlowestCycle = 1000;

static const double p_carBackAnimationFastestCycle = 50;
static const double p_carBackAnimationSlowestCycle = 200;

static const double p_carWingAnimationFastestCycle = 200;
static const double p_carWingAnimationSlowestCycle = 800;

static const double p_domeAnimationFastestCycle = 50;
static const double p_domeAnimationSlowestCycle = 400;

static const double p_roadAnimationFastestCycle = 50;
static const double p_roadAnimationSlowestCycle = 400;

static const double p_road4AnimationFastestCycle = 200;
static const double p_road4AnimationSlowestCycle = 1600;

static const double p_fullDist = 14000;

static const double p_unrealisticallyLate = 220;
static const double p_unrealisticallyEarly = 120;

static bool p_running = false;

static size_t p_domeParallaxTex = 0;
static size_t p_domeParallaxTex2 = 0;
static size_t p_skyline = 0;

static size_t p_backdrop0 = 0;
static size_t p_backdrop1 = 0;
static size_t p_backdrop2 = 0;
static size_t p_backdrop3 = 0;
static size_t p_backdrop4 = 0;

static size_t p_road0Tex = 0;
static size_t p_road1Tex = 0;
static size_t p_road2Tex = 0;
static size_t p_road3Tex = 0;
static size_t p_road4Tex = 0;

static size_t p_wall3Tex = 0;
static size_t p_wall4Tex = 0;

static size_t p_tutorial = 0;
static size_t p_pauseTex = 0;

static PLEX_SPRITE p_pauseAnim;

static size_t p_windshieldBreak = 0;

static PLEX_SPRITE p_pamaleanaSprite;

static PLEX_SPRITE p_carBack;
static PLEX_SPRITE p_carSeat;
static PLEX_SPRITE p_carWindshield;
static PLEX_SPRITE p_carWingLeft;
static PLEX_SPRITE p_carWingRight;

static PLEX_SPRITE p_road0;
static PLEX_SPRITE p_road1;
static PLEX_SPRITE p_road2;
static PLEX_SPRITE p_road3;
static PLEX_SPRITE p_road4;

static PLEX_SPRITE p_wall4;
static PLEX_SPRITE p_wall3;

static PLEX_SPRITE p_domeParallax;
static PLEX_SPRITE p_domeParallax2;

static size_t p_car0Tex = 0;
static size_t p_car1Tex = 0;
static size_t p_car2Tex = 0;
static size_t p_car3Tex = 0;
static size_t p_car4Tex = 0;
static size_t p_car5Tex = 0;
static size_t p_car6Tex = 0;
static size_t p_car7Tex = 0;
static size_t p_car8Tex = 0;
static size_t p_car9Tex = 0;
static size_t p_car10Tex = 0;
static size_t p_car11Tex = 0;
static size_t p_car12Tex = 0;
static size_t p_car13Tex = 0;
static size_t p_car14Tex = 0;
static size_t p_car15Tex = 0;

static size_t p_bikeTex = 0;
static size_t p_bikerTex = 0;

static size_t p_cactusTex = 0;
static size_t p_tumbleweedTex = 0;

static size_t p_bagTex = 0;
static size_t p_bag2Tex = 0;
static size_t p_barrelTex = 0;
static size_t p_bricksTex = 0;
static size_t p_crateTex = 0;
static size_t p_pipesTex = 0;

static PLEX_SPRITE p_car0, p_car1, p_car2, p_car3, p_car4, p_car5, p_car6, p_car7, p_car8, p_car9, p_car10, p_car11, p_car12, p_car13, p_car14, p_car15;
static PLEX_SPRITE p_bike, p_biker;
static PLEX_SPRITE p_cactus, p_tumbleweed, p_bag, p_bag2, p_barrel, p_bricks, p_crate, p_pipes;

static PLEX_RECT p_playerRegion, p_carDrawArea, p_carBackArea, p_carSeatArea, p_carWindshieldArea, p_carWingLeftArea, p_carWingRightArea;

static double p_backHealth = 1, p_carSeatHealth = 1, p_windshieldHealth = 1, p_carWingLeftHealth = 1, p_carWingRightHealth = 1;

static double p_accel = 0;
static double p_accelMod = 1;
static double p_dist = 0;

static uint8_t p_outcome = 1;

static MMS1_OBSTRUCTION p_obstructions[MMS1_OBSTUCTION_LIMIT];
static size_t p_obstructionCount = 0;

static uint64_t p_crashMarker = 0;
static uint64_t p_crashDuration = 0;

static uint64_t p_objGenMarker[4], p_objGenDuration[4];
static MMS1_OBSTRUCTION p_objGen[4];

static bool p_warnPlayer = false;
static bool p_honkHonk = false;
static bool p_engineStruggle = false;

static size_t p_start = 0;
static uint64_t p_lastTime = 0;
static double p_accumSeconds = 0;

static bool p_enablePause = false;

static int64_t p_pamEngineRevSE = -1;
static int64_t p_pamEngineRunSE = -1;
static int64_t p_pamEngineStruggleSE = -1;

static size_t p_glassShatterLevel = 0;

static size_t p_overroadLayer(const double y, const double h, const size_t carLayer)
{
	const double bottom = y + h;

	return PLEX_WH(true) - bottom + MMS1_MENULAYER + carLayer;
}

static void p_updateAreas()
{
	p_carDrawArea = PLEX_convRect(PLEX_convPoint(p_playerRegion.origin.x - p_playerRegion.dimens.w * 1.5, p_playerRegion.origin.y), PLEX_convDimens(p_playerRegion.dimens.w * 4, p_playerRegion.dimens.h));

	p_carBackArea = p_carDrawArea;

	p_carWingLeftArea = p_carDrawArea;
	p_carWingRightArea = p_carDrawArea;

	p_carBackArea.origin.x += p_carBackArea.dimens.w * 0.2;
	p_carBackArea.dimens.w *= 0.6;

	p_carWindshieldArea = p_carBackArea;
	p_carWindshieldArea.origin.x += p_carSeatArea.dimens.w * 0.25;
	p_carWindshieldArea.dimens.h *= 0.25;
	p_carWindshieldArea.dimens.w *= 0.6;

	p_carSeatArea = p_carWindshieldArea;

	p_carWingLeftArea.origin.x += p_carWingLeftArea.dimens.w * 0.05;
	p_carWingLeftArea.origin.y += p_carWingLeftArea.dimens.h * 0.355;
	p_carWingLeftArea.dimens.w *= 0.3;
	p_carWingLeftArea.dimens.h *= 0.3;

	p_carWingRightArea.origin.x += p_carWingRightArea.dimens.w * 0.7;
	p_carWingRightArea.origin.y += p_carWingRightArea.dimens.h * 0.355;
	p_carWingRightArea.dimens.w *= 0.25;
	p_carWingRightArea.dimens.h *= 0.3;
}

// draws speed bar which is NOT a speedometer

static int8_t p_drawSpeedBar(const double secondsToDest, const double ETA, const double minETA, const double maxETA)
{
	const double x = PLEX_UNIT(0.5);
	const double y = PLEX_UNIT(0.5);
	const double w = PLEX_WW(true) - PLEX_UNIT(1);
	const double h = PLEX_UNIT(2);

	const double cw = PLEX_UNIT(0.2);

	double ETAperc = ETA ? 1 - (ETA - minETA)/(maxETA - minETA) : 0;
	double lateETAPerc = 1 - (p_unrealisticallyLate - minETA)/(maxETA - minETA);
	double earlyETAPerc = 1 - (p_unrealisticallyEarly - minETA)/(maxETA - minETA);

	double cx = 0;

	double lw = 0;

	double ea = 0;
	double ex = 0;
	double ew = 0;

	if(ETAperc < 0) ETAperc = 0;
	if(lateETAPerc < 0) lateETAPerc = 0;
	if(earlyETAPerc < 0) earlyETAPerc = 0;

	if(ETAperc > 1) ETAperc = 1;
	if(lateETAPerc > 1) lateETAPerc = 1;
	if(earlyETAPerc > 1) earlyETAPerc = 1;

	cx = x + (w * ETAperc) - cw/2.0;

	lw = w * lateETAPerc;

	ea = w * earlyETAPerc;
	ex = x + ea;
	ew = w - ea;

	// white zone

	PLEX_fillRect(MMS1_MENULAYER, x, y, w, h, 255, 255, 255, 255, true);

	// red zones

	PLEX_fillRect(MMS1_MENULAYER, x, y, lw, h, 255, 0, 0, 255, true);
	PLEX_fillRect(MMS1_MENULAYER, ex, y, ew, h, 255, 0, 0, 255, true);

	// outline and cursor

	PLEX_fillRect(MMS1_MENULAYER, cx, y, cw, h, 0, 0, 0, 255, true);
	PLEX_drawRect(MMS1_MENULAYER, x, y, w, h, 0, 0, 0, 255, PLEX_UNIT(0.1), true);

	if(cx < lw) return -1;
	if(cx > ex) return 1;

	return 0;
}

static uint64_t p_calcAnimationCycle(const uint64_t min, const uint64_t max)
{
	const double perc = (200 - p_accel)/200;

	return min + (max - min) * perc;
}

static double p_translateLaneProp(const double y, const double h)
{
	const double range = PLEX_HEIGHTUNITMAX() * 0.75;
	const double area = PLEX_HEIGHTUNITMAX() - range;

	const double dist = (y + h - range);

	return dist/area * 0.75 + 0.25;
}

static double p_totalVelocity(const double velocity, const double secondsPass)
{
	const double natural =  0.05 + 0.25 * (p_accel/200.0);

	return velocity + natural;
}

static PLEX_POINT p_translateLane0Move(const double velocity, const PLEX_POINT xy, const double tilt, const double secondsPass, double* totalVelocity)
{
	const double lTotalVelocity = p_totalVelocity(velocity, secondsPass);

	PLEX_POINT ret = xy;

	ret.x -= lTotalVelocity - tilt;
	ret.y += lTotalVelocity * MMS1_ANGLERISE;

	*totalVelocity = lTotalVelocity;

	return ret;
}

static PLEX_POINT p_translateLane1Move(const double velocity, const PLEX_POINT xy, const double tilt, const double secondsPass, double* totalVelocity)
{
	const double lTotalVelocity = p_totalVelocity(velocity, secondsPass);

	PLEX_POINT ret = xy;

	ret.x += tilt;
	ret.y += lTotalVelocity * MMS1_ANGLERISE;

	*totalVelocity = lTotalVelocity;

	return ret;
}

static PLEX_POINT p_translateLane2Move(const double velocity, const PLEX_POINT xy, const double tilt, const double secondsPass, double* totalVelocity)
{
	const double lTotalVelocity = p_totalVelocity(velocity, secondsPass);

	PLEX_POINT ret = xy;

	ret.x += tilt;
	ret.y -= lTotalVelocity * MMS1_ANGLERISE;

	*totalVelocity = lTotalVelocity;

	return ret;
}

static PLEX_POINT p_translateLane3Move(const double velocity, const PLEX_POINT xy, const double tilt, const double secondsPass, double* totalVelocity)
{
	const double lTotalVelocity = p_totalVelocity(velocity, secondsPass);

	PLEX_POINT ret = xy;

	ret.x -= lTotalVelocity - tilt;
	ret.y -= lTotalVelocity * MMS1_ANGLERISE;

	*totalVelocity = lTotalVelocity;

	return ret;
}

static PLEX_POINT p_translateLaneMove(const size_t lane, const double velocity, const PLEX_POINT xy, const double tilt, const double secondsPass, double* totalVelocity)
{
	PLEX_POINT ret = xy;

	if(PLEX_getGameTimePaused()) return ret;

	switch(lane)
	{
		case 0: ret = p_translateLane0Move(velocity, xy, tilt, secondsPass, totalVelocity); break;
		case 1: ret = p_translateLane1Move(velocity, xy, tilt, secondsPass, totalVelocity); break;
		case 2: ret = p_translateLane2Move(velocity, xy, tilt, secondsPass, totalVelocity); break;
		case 3: ret = p_translateLane3Move(velocity, xy, tilt, secondsPass, totalVelocity); break;
	};

	return ret;
}

static PLEX_POINT p_getLaneStart(const size_t playerLane, const size_t lane)
{
	PLEX_POINT ret;

	switch(lane)
	{
		case 0: ret = PLEX_convPoint(PLEX_WIDTHUNITMAX() * 0.4, PLEX_HEIGHTUNITMAX() * 0.75); break;
		case 1:
			if(playerLane != 1) ret = PLEX_convPoint(PLEX_WIDTHUNITMAX() * 0.465, PLEX_HEIGHTUNITMAX() * 0.75);
			else{
				const double x = PLEX_randomNumber(p_playerRegion.origin.x, PLEX_WIDTHUNITMAX() * 0.465);

				ret = PLEX_convPoint(x, PLEX_HEIGHTUNITMAX() * 0.75);
			}
		break;

		case 2:
			if(playerLane != 2) ret = PLEX_convPoint(PLEX_WIDTHUNITMAX() * 0.535, PLEX_HEIGHTUNITMAX() - 1);
			else
			{
				const double x = PLEX_randomNumber(PLEX_WIDTHUNITMAX() * 0.535, p_playerRegion.origin.x);

				ret = PLEX_convPoint(x, PLEX_HEIGHTUNITMAX() - 1);
			}
		break;

		case 3: ret = PLEX_convPoint(PLEX_WIDTHUNITMAX() * 0.9, PLEX_HEIGHTUNITMAX() - 1); break;
	};

	return ret;
}

static bool p_checkEnd(const PLEX_RECT shape)
{
	const double y = shape.origin.y - shape.dimens.h/2;
	const double y2 = shape.origin.y + shape.dimens.h/2;

	return y2 < PLEX_HEIGHTUNITMAX() * 0.75 || y > PLEX_HEIGHTUNITMAX();
}

static void p_placeObstruction(const size_t playerLane, MMS1_OBSTRUCTION* obstruction)
{
	obstruction->space.origin = p_getLaneStart(playerLane, obstruction->lane);

	obstruction->space.dimens.w -= obstruction->space.dimens.w/2;
	obstruction->space.dimens.h -= obstruction->space.dimens.h/2;
}

static void p_updateObstruction(const size_t index, MMS1_OBSTRUCTION* obstruction, const uint64_t time, const double secondsPass)
{
	double drift = 0;
	double translation = 0;

	uint8_t windshieldCollide = 0;
	uint8_t seatCollide = 0;
	uint8_t backCollide = 0;
	uint8_t wingLeftCollide = 0;
	uint8_t wingRightCollide = 0;

	bool playerCollide = false;
	bool objCollide = false;

	drift = obstruction->drift * secondsPass;

	if(obstruction->deactivate) return;

	obstruction->space.origin = p_translateLaneMove(obstruction->lane, obstruction->velocity, obstruction->space.origin, drift, secondsPass, &obstruction->totalVelocity);

	translation = p_translateLaneProp(obstruction->space.origin.y, obstruction->translatedSpace.dimens.h);

	obstruction->space.origin = p_translateLaneMove(obstruction->lane, obstruction->velocity, obstruction->space.origin, drift, secondsPass, &obstruction->totalVelocity);

	obstruction->translatedSpace.dimens.w = obstruction->space.dimens.w * translation;
	obstruction->translatedSpace.dimens.h = obstruction->space.dimens.h * translation;
	obstruction->translatedSpace.origin.x = obstruction->space.origin.x - obstruction->translatedSpace.dimens.w/2;
	obstruction->translatedSpace.origin.y = obstruction->space.origin.y - obstruction->translatedSpace.dimens.h/2;

	if(obstruction->crashed)
	{
		PLEX_drawSprite(p_overroadLayer(p_carDrawArea.origin.y, p_carDrawArea.dimens.h, 7), 2, time, PLEX_UNIT(obstruction->translatedSpace.origin.x), PLEX_UNIT(obstruction->translatedSpace.origin.y), PLEX_UNIT(obstruction->translatedSpace.dimens.w), PLEX_UNIT(obstruction->translatedSpace.dimens.h), obstruction->sprite, true);

		if(time - obstruction->crashedMarker > 5000 || p_checkEnd(obstruction->translatedSpace)) obstruction->deactivate = true;
		return;
	}

	obstruction->driftAccum += drift;

	windshieldCollide = PLEX_rectCollisionNoMove(p_carWindshieldArea, obstruction->translatedSpace) && p_windshieldHealth;
	seatCollide = PLEX_rectCollisionNoMove(p_carSeatArea, obstruction->translatedSpace) && p_carSeatHealth;
	backCollide = PLEX_rectCollisionNoMove(p_carBackArea, obstruction->translatedSpace);
	wingLeftCollide = PLEX_rectCollisionNoMove(p_carWingLeftArea, obstruction->translatedSpace);
	wingRightCollide = PLEX_rectCollisionNoMove(p_carWingRightArea, obstruction->translatedSpace);

	if(drift < 0 && obstruction->driftAccum < -1 * obstruction->leftDrift)
	{
		drift = 0;
		obstruction->driftAccum = 0;
		obstruction->drift *= -1;
	}

	if(drift > 0 && obstruction->driftAccum > obstruction->rightDrift)
	{
		drift = 0;
		obstruction->driftAccum = 0;
		obstruction->drift *= -1;
	}

	if(windshieldCollide && p_windshieldHealth)
	{
		seatCollide = false;
		backCollide = false;
	}

	if(seatCollide && p_carSeatHealth)
	{
		backCollide = false;
	}

	playerCollide = seatCollide || windshieldCollide || backCollide || wingLeftCollide || wingRightCollide;

	for(size_t ze = 0; ze < p_obstructionCount; ++ze)
	{
		MMS1_OBSTRUCTION* obstruction2 = NULL;

		if(ze == index) continue;

		obstruction2 = p_obstructions + ze;

		if(obstruction->weight < obstruction2->weight && PLEX_rectCollisionNoMove(obstruction->translatedSpace, obstruction2->translatedSpace))
		{
			playerCollide = false;
			objCollide = true;
			break;
		}
	}

	PLEX_drawSprite(p_overroadLayer(p_carDrawArea.origin.y, p_carDrawArea.dimens.h, 7), obstruction->lane > 1, time, PLEX_UNIT(obstruction->translatedSpace.origin.x), PLEX_UNIT(obstruction->translatedSpace.origin.y), PLEX_UNIT(obstruction->translatedSpace.dimens.w), PLEX_UNIT(obstruction->translatedSpace.dimens.h), obstruction->sprite, true);

	if(playerCollide || objCollide)
	{
		if(playerCollide)
		{
			const double damage =  obstruction->weight/100.0 * (obstruction->totalVelocity/0.33) * 2;

			const double deccel = damage * 3;

			if(backCollide) p_backHealth -= damage;
			if(seatCollide){ p_carSeatHealth -= damage; }
			if(windshieldCollide) p_windshieldHealth -= damage;
			if(wingLeftCollide) p_carWingLeftHealth -= damage;
			if(wingRightCollide) p_carWingRightHealth -= damage;

			if(p_backHealth < 0) p_backHealth = 0;
			if(p_carSeatHealth < 0) p_carSeatHealth = 0;
			if(p_windshieldHealth < 0) p_windshieldHealth = 0;
			if(p_carWingLeftHealth < 0) p_carWingLeftHealth = 0;
			if(p_carWingRightHealth < 0) p_carWingRightHealth = 0;

			if(!p_crashDuration)
			{
				if(deccel < 1)
				{
					p_accel -= p_accel * deccel;

					p_crashMarker = time;
					p_crashDuration += 1000;
				}else{
					p_accel = 0;
					p_crashDuration += deccel * 3000;
				}
			}

			MM_COMMON_playSoundEffectVolume(obstruction->weight/50.0 + 0.5, MM_COMMON_SOUNDEFFECT_CRASH);
		}

		obstruction->velocity = 0;
		obstruction->crashedMarker = time;
		obstruction->drift = 0;
		obstruction->driftAccum = 0;
		obstruction->crashed = true;
	}

	if(p_checkEnd(obstruction->translatedSpace)) obstruction->deactivate = true;
}

static MMS1_OBSTRUCTION p_genObstruction(const size_t lane, const double w, const double h, const double velocity, const double driftVelocity, const double leftDrift, const double rightDrift, const double weight, const bool startDriftRight, const PLEX_SPRITE sprite)
{
	MMS1_OBSTRUCTION ret;

	memset(&ret, 0, sizeof(ret));

	ret.lane = lane;
	ret.space.dimens.w = w;
	ret.space.dimens.h = h;
	ret.velocity = velocity;
	ret.leftDrift = leftDrift;
	ret.rightDrift = rightDrift;
	ret.driftVelocity = driftVelocity;
	ret.weight = weight;
	ret.sprite = sprite;

	if(rightDrift && startDriftRight) ret.drift = driftVelocity;
	else if(leftDrift && !startDriftRight) ret.drift = driftVelocity * -1;

	return ret;
}

static void p_addObstruction(const size_t lane, const double w, const double h, const double velocity, const double driftVelocity, const double leftDrift, const double rightDrift, const double weight, const bool startDriftRight, const uint64_t duration, const PLEX_SPRITE sprite, const uint64_t time)
{
	if(p_objGenDuration[lane]) return;

	p_objGenDuration[lane] = duration;
	p_objGenMarker[lane] = time;
	p_objGen[lane] = p_genObstruction(lane, w, h, velocity, driftVelocity, leftDrift, rightDrift, weight, startDriftRight, sprite);
}

static void p_checkAddObstruction(const size_t playerLane, const size_t lane, const uint64_t time)
{
	size_t index = p_obstructionCount;

	MMS1_OBSTRUCTION toAdd;

	if(!p_objGenDuration[lane] || time - p_objGenMarker[lane] < p_objGenDuration[lane]) return;

	p_objGenDuration[lane] = 0;

	for(size_t ze = 0; ze < p_obstructionCount; ++ze)
	{
		if(p_obstructions[ze].deactivate)
		{
			index = ze;
			break;
		}
	}

	if(index >= MMS1_OBSTUCTION_LIMIT)
	{
		p_objGenMarker[lane] = time;
		return;
	}

	toAdd = p_objGen[lane];

	p_placeObstruction(playerLane, &toAdd);

	p_obstructions[index] = toAdd;
	if(index == p_obstructionCount) ++p_obstructionCount;

	MM_COMMON_playSoundEffectVolume((lane == 1 || lane == 2) * 0.5 + 0.5, MM_COMMON_SOUNDEFFECT_ZOOMBY);

	p_objGenMarker[lane] = time;
}

static void p_checkObstructionsAllPhases(const size_t playerLane, const uint64_t time)
{
	p_warnPlayer = playerLane > 1 && p_objGenDuration[playerLane];

	for(size_t ze = 0; ze < 4; ++ze) p_checkAddObstruction(playerLane, ze, time);

	if(p_obstructionCount && p_obstructions[p_obstructionCount - 1].deactivate) --p_obstructionCount;
}

static void p_checkObstructions(const size_t phase, const size_t playerLane, const uint64_t time)
{
	const size_t lane0Duration = time - p_objGenMarker[0];
	const size_t lane1Duration = time - p_objGenMarker[1];
	const size_t lane2Duration = time - p_objGenMarker[2];
	const size_t lane3Duration = time - p_objGenMarker[3];

	const double genNum = PLEX_randomNumber(0, 100);

	if(!phase)
	{
		const bool driftAdjust = PLEX_randomNumber(0, 10) < 5;

		if(lane0Duration > 500)
		{
			if(genNum < 10) p_addObstruction(0, 4, 4, 0.05, 5, 2, 2, 5, driftAdjust, 500, p_bike, time);
			else p_addObstruction(0, 5, 2.5, 0.5, 0, 0, 0.5, 10, driftAdjust, 2000, p_car0, time);
		}

		if(lane1Duration > 7500)
		{
			if(genNum < 30) p_addObstruction(1, 4, 4, 0.05, 5, 2, 2, 5, driftAdjust, 500, p_bike, time);
			else p_addObstruction(1, 4, 3, 0.5, 0, 0, 0, 10, driftAdjust, 2000, p_car0, time);
		}

		if(lane2Duration > 10000)
		{
			if(genNum < 50) p_addObstruction(2, 4, 4, 0.05, 5, 2, 2, 5, driftAdjust, 3000, p_bike, time);
			else p_addObstruction(2, 4, 3, 0.5, 0, 0, 0.5, 10, driftAdjust, 3000, p_car0, time);
		}

		if(lane3Duration > 500)
		{
			if(genNum < 90) p_addObstruction(3, 4, 4, 0.05, 5, 2, 2, 5, driftAdjust, 500, p_bike, time);
			else p_addObstruction(3, 4, 3, 0.5, 0, 0, 0, 10, driftAdjust, 2000, p_car0, time);
		}
	}

	if(phase == 1)
	{
		const bool driftAdjust = PLEX_randomNumber(0, 10) < 5;

		if(lane0Duration > 500)
		{
			if(genNum < 10)
			{
				p_addObstruction(0, 2, 4, 0, 0, 0, 0, 10, driftAdjust, 1000, p_cactus, time);
			}else{
				if(genNum < 20)
				{
					p_addObstruction(0, 2, 2, 0, 7, 0, 1000, 2, true, 200, p_tumbleweed, time);
				}else{
					if(genNum < 50)
					{
						p_addObstruction(0, 4, 3, 0.25, 0, 0, 0, 10, driftAdjust, 2000, p_car2, time);
					}else{

						if(genNum < 75)
						{
							p_addObstruction(0, 6, 2.5, 0.02, 3, 0.5, 0.5, 10, driftAdjust, 500, p_biker, time);
						}else{
							p_addObstruction(0, 6, 4, 0.01, 0, 0, 0, 25, driftAdjust, 500, p_car1, time);
						}
					}
				}
			}
		}

		if(lane1Duration > 7500)
		{
			if(genNum < 10)
			{
				p_addObstruction(1, 2, 4, 0, 0, 0, 0, 5, driftAdjust, 1000, p_cactus, time);
			}else{
				if(genNum < 20)
				{
					p_addObstruction(1, 2, 2, 0, 7, 0, 1000, 2, true, 200, p_tumbleweed, time);
				}else{
					if(genNum < 70)
					{
						p_addObstruction(1, 4, 3, 0.25, 0, 0, 0, 10, driftAdjust, 2000, p_car2, time);
					}else{
						p_addObstruction(1, 6, 2.5, 0.02, 3, 0.5, 0.5, 7, driftAdjust, 500, p_biker, time);
					}
				}
			}
		}

		if(lane2Duration > 5000)
		{
			if(genNum < 50)
			{
				p_addObstruction(2, 4, 3, 0.25, 0, 0, 0, 10, driftAdjust, 2000, p_car2, time);
			}else{
				p_addObstruction(2, 6, 2.5, 0.02, 3, 0.5, 0.5, 7, driftAdjust, 500, p_biker, time);
			}
		}

		if(lane3Duration > 500)
		{
			if(genNum < 70)
			{
				p_addObstruction(3, 6, 4, 0.01, 0, 0, 0, 25, driftAdjust, 500, p_car1, time);
			}else{
				if(genNum < 90)
				{
					p_addObstruction(3, 4, 3, 0.25, 0, 0, 0, 10, driftAdjust, 2000, p_car2, time);
				}else{
					p_addObstruction(3, 6, 2.5, 0.02, 3, 0.5, 0.5, 7, driftAdjust, 500, p_biker, time);
				}
			}
		}
	}

	if(phase == 2)
	{
		const size_t gen3Num = genNum/14;

		if(lane0Duration > 100)
		{
			switch(gen3Num)
			{
				case 0:
					p_addObstruction(0, 5, 2.5, 0.5, 0, 0, 0.5, 10, false, 100, p_car0, time);
				break;

				case 1:
					p_addObstruction(0, 6, 4, 0.01, 0, 0, 0, 25, false, 50, p_car1, time);
				break;

				case 2:
					p_addObstruction(0, 4, 3, 0.25, 0, 0, 0, 10, false, 100, p_car2, time);
				break;

				case 3:
					p_addObstruction(0, 4, 3, 0.25, 0, 0, 0, 10, false, 100, p_car3, time);
				break;

				case 4:
					p_addObstruction(0, 4, 3, 0.25, 0, 0, 0, 10, false, 100, p_car4, time);
				break;

				case 5:
					p_addObstruction(0, 4, 3, 0.25, 0, 0, 0, 10, false, 100, p_car5, time);
				break;

				case 6:
					p_addObstruction(0, 4, 3, 0.25, 0, 0, 0, 10, false, 50, p_car6, time);
				break;

				default:
					p_addObstruction(0, 4, 3, 0.25, 0, 0, 0, 10, false, 50, p_car7, time);
			};
		}

		if(lane1Duration > 500)
		{
			switch(gen3Num)
			{
				case 0:
					p_addObstruction(1, 5, 2.5, 0.5, 0, 0, 0.5, 10, false, 100, p_car0, time);
				break;

				case 1:
					p_addObstruction(1, 6, 4, 0.01, 0, 0, 0, 25, false, 50, p_car1, time);
				break;

				case 2:
					p_addObstruction(1, 4, 3, 0.25, 0, 0, 0, 10, false, 100, p_car2, time);
				break;

				case 3:
					p_addObstruction(1, 4, 3, 0.25, 0, 0, 0, 10, false, 100, p_car3, time);
				break;

				case 4:
					p_addObstruction(1, 4, 3, 0.25, 0, 0, 0, 10, false, 100, p_car4, time);
				break;

				case 5:
					p_addObstruction(1, 4, 3, 0.25, 0, 0, 0, 10, false, 100, p_car5, time);
				break;

				case 6:
					p_addObstruction(1, 4, 3, 0.25, 0, 0, 0, 10, false, 50, p_car6, time);
				break;

				default:
					p_addObstruction(1, 4, 3, 0.25, 0, 0, 0, 10, false, 50, p_car7, time);
			};
		}

		if(lane2Duration > 10000)
		{
			switch(gen3Num)
			{
				case 0:
					p_addObstruction(2, 5, 2.5, 0.5, 0, 0, 0.5, 10, false, 500, p_car0, time);
				break;

				case 1:
					p_addObstruction(2, 6, 4, 0.01, 0, 0, 0, 25, false, 250, p_car1, time);
				break;

				case 2:
					p_addObstruction(2, 4, 3, 0.25, 0, 0, 0, 10, false, 500, p_car2, time);
				break;

				case 3:
					p_addObstruction(2, 4, 3, 0.25, 0, 0, 0, 10, false, 500, p_car3, time);
				break;

				case 4:
					p_addObstruction(2, 4, 3, 0.25, 0, 0, 0, 10, false, 500, p_car4, time);
				break;

				case 5:
					p_addObstruction(2, 4, 3, 0.25, 0, 0, 0, 10, false, 500, p_car5, time);
				break;

				case 6:
					p_addObstruction(2, 4, 3, 0.25, 0, 0, 0, 10, false, 500, p_car6, time);
				break;

				default:
					p_addObstruction(2, 4, 3, 0.25, 0, 0, 0, 10, false, 500, p_car7, time);
			};
		}

		if(lane3Duration > 5000)
		{
			switch(gen3Num)
			{
				case 0:
					p_addObstruction(3, 5, 2.5, 0.5, 0, 0, 0.5, 10, false, 1000, p_car0, time);
				break;

				case 1:
					p_addObstruction(3, 6, 4, 0.01, 0, 0, 0, 25, false, 1000, p_car1, time);
				break;

				case 2:
					p_addObstruction(3, 4, 3, 0.25, 0, 0, 0, 10, false, 1000, p_car2, time);
				break;

				case 3:
					p_addObstruction(3, 4, 3, 0.25, 0, 0, 0, 10, false, 1000, p_car3, time);
				break;

				case 4:
					p_addObstruction(3, 4, 3, 0.25, 0, 0, 0, 10, false, 1000, p_car4, time);
				break;

				case 5:
					p_addObstruction(3, 4, 3, 0.25, 0, 0, 0, 10, false, 1000, p_car5, time);
				break;

				case 6:
					p_addObstruction(3, 4, 3, 0.25, 0, 0, 0, 10, false, 1000, p_car6, time);
				break;

				default:
					p_addObstruction(3, 4, 3, 0.25, 0, 0, 0, 10, false, 1000, p_car7, time);
			};
		}
	}

	if(phase == 3)
	{
		if(lane0Duration > 1000)
		{
			if(genNum < 75)
			{
				if(genNum < 12)
				{
					p_addObstruction(0, 2, 2, 0, 0, 0, 0, 5, false, 1000, p_bag, time);
				}else{
					if(genNum < 24)
					{
						p_addObstruction(0, 4, 3, 0, 0, 0, 0, 5, false, 1000, p_bag2, time);
					}else{
						if(genNum < 36)
						{
							p_addObstruction(0, 2, 3, 0, 0, 0, 0, 5, false, 1000, p_barrel, time);
						}else{
							if(genNum < 48)
							{
								p_addObstruction(0, 2, 2, 0, 0, 0, 0, 5, false, 1000, p_bricks, time);
							}else{
								if(genNum < 60)
								{
									p_addObstruction(0, 2, 2, 0, 0, 0, 0, 5, false, 1000, p_crate, time);
								}else{
									p_addObstruction(0, 4, 4, 0, 0, 0, 0, 5, false, 1000, p_pipes, time);
								}
							}
						}
					}
				}
			}else{
				if(genNum < 85)
				{
					p_addObstruction(0, 4, 3, 0.1, 0, 0, 0, 25, false, 250, p_car8, time);
				}else{
					p_addObstruction(0, 4, 3, 0.1, 0, 0, 0, 25, false, 250, p_car9, time);
				}
			}
		}

		if(lane1Duration > 2500)
		{
			if(genNum < 75)
			{
				if(genNum < 12)
				{
					p_addObstruction(1, 2, 2, 0, 0, 0, 0, 5, false, 1000, p_bag, time);
				}else{
					if(genNum < 24)
					{
						p_addObstruction(1, 4, 3, 0, 0, 0, 0, 5, false, 1000, p_bag2, time);
					}else{
						if(genNum < 36)
						{
							p_addObstruction(1, 2, 3, 0, 0, 0, 0, 5, false, 1000, p_barrel, time);
						}else{
							if(genNum < 48)
							{
								p_addObstruction(1, 2, 2, 0, 0, 0, 0, 5, false, 1000, p_bricks, time);
							}else{
								if(genNum < 60)
								{
									p_addObstruction(1, 2, 2, 0, 0, 0, 0, 5, false, 1000, p_crate, time);
								}else{
									p_addObstruction(1, 4, 4, 0, 0, 0, 0, 5, false, 1000, p_pipes, time);
								}
							}
						}
					}
				}
			}else{
				if(genNum < 85)
				{
					p_addObstruction(1, 4, 3, 0.1, 0, 0, 0, 25, false, 250, p_car8, time);
				}else{
					p_addObstruction(1, 4, 3, 0.1, 0, 0, 0, 25, false, 250, p_car9, time);
				}
			}
		}

		if(lane2Duration > 5000)
		{
			if(genNum < 50)
			{
				p_addObstruction(2, 4, 3, 0.1, 0, 0, 0, 25, false, 250, p_car8, time);
			}else{
				p_addObstruction(2, 4, 3, 0.1, 0, 0, 0, 25, false, 250, p_car9, time);
			}
		}

		if(lane3Duration > 7500)
		{
			if(genNum < 50)
			{
				p_addObstruction(3, 4, 3, 0.1, 0, 0, 0, 25, false, 250, p_car8, time);
			}else{
				p_addObstruction(3, 4, 3, 0.1, 0, 0, 0, 25, false, 250, p_car9, time);
			}
		}
	}

	if(phase == 4)
	{
		if(lane0Duration > 1000)
		{
			if(genNum < 20)
			{
				p_addObstruction(0, 4, 3, 0.1, 0, 0, 0, 25, false, 250, p_car10, time);
			}else{
				if(genNum < 40)
				{
					p_addObstruction(0, 4, 3, 0.1, 0, 0, 0, 25, false, 250, p_car11, time);
				}else{
					if(genNum < 60)
					{
						p_addObstruction(0, 4, 3, 0.1, 0, 0, 0, 25, false, 250, p_car12, time);
					}else{
						if(genNum < 80)
						{
							p_addObstruction(0, 4, 3, 0.1, 0, 0, 0, 25, false, 250, p_car13, time);
						}else{
							p_addObstruction(0, 4, 3, 0.1, 0, 0, 0, 25, false, 250, p_car14, time);
						}
					}
				}
			}
		}

		if(lane1Duration > 2500)
		{
			if(genNum < 20)
			{
				p_addObstruction(1, 4, 3, 0.1, 0, 0, 0, 25, false, 250, p_car10, time);
			}else{
				if(genNum < 40)
				{
					p_addObstruction(1, 4, 3, 0.1, 0, 0, 0, 25, false, 250, p_car11, time);
				}else{
					if(genNum < 60)
					{
						p_addObstruction(1, 4, 3, 0.1, 0, 0, 0, 25, false, 250, p_car12, time);
					}else{
						if(genNum < 80)
						{
							p_addObstruction(1, 4, 3, 0.1, 0, 0, 0, 25, false, 250, p_car13, time);
						}else{
							p_addObstruction(1, 4, 3, 0.1, 0, 0, 0, 25, false, 250, p_car14, time);
						}
					}
				}
			}
		}

		if(lane2Duration > 5000)
		{
			if(genNum < 20)
			{
				p_addObstruction(2, 4, 3, 0.1, 0, 0, 0, 25, false, 1000, p_car10, time);
			}else{
				if(genNum < 40)
				{
					p_addObstruction(2, 4, 3, 0.1, 0, 0, 0, 25, false, 1000, p_car11, time);
				}else{
					if(genNum < 60)
					{
						p_addObstruction(2, 4, 3, 0.1, 0, 0, 0, 25, false, 1000, p_car12, time);
					}else{
						if(genNum < 80)
						{
							p_addObstruction(2, 4, 3, 0.1, 0, 0, 0, 25, false, 1000, p_car13, time);
						}else{
							p_addObstruction(2, 4, 3, 0.1, 0, 0, 0, 25, false, 1000, p_car14, time);
						}
					}
				}
			}
		}

		if(lane3Duration > 10000)
		{
			if(genNum < 20)
			{
				p_addObstruction(3, 4, 3, 0.1, 0, 0, 0, 25, false, 250, p_car10, time);
			}else{
				if(genNum < 40)
				{
					p_addObstruction(3, 4, 3, 0.1, 0, 0, 0, 25, false, 250, p_car11, time);
				}else{
					if(genNum < 60)
					{
						p_addObstruction(3, 4, 3, 0.1, 0, 0, 0, 25, false, 250, p_car12, time);
					}else{
						if(genNum < 80)
						{
							p_addObstruction(3, 4, 3, 0.1, 0, 0, 0, 25, false, 250, p_car13, time);
						}else{
							p_addObstruction(3, 4, 3, 0.1, 0, 0, 0, 25, false, 250, p_car14, time);
						}
					}
				}
			}
		}
	}

	p_checkObstructionsAllPhases(playerLane, time);
}

static void p_failCut(const char* const extrapMessage, const size_t pamaleanaSpriteTex, const size_t cubicleBackdrop, const size_t venariusSpriteTex, const size_t extraplocanSpriteTex, const size_t fontPlate, const size_t label, const size_t bubble, const size_t arrow)
{
	MMS_startPamaleanasCubicle(pamaleanaSpriteTex, cubicleBackdrop, venariusSpriteTex, extraplocanSpriteTex, fontPlate, label, bubble, arrow);

	MMS_extrapTalk(extrapMessage);

	MMS_pamTalk(MMS1_LANGUAGE_MINIGAME_PAMFAILEXCUSE);

	while(!PLEX_isSpriteTheaterOver())
	{
		PLEX_updateSpriteTheater(PLEX_getGameTime());
		PLEX_updatePlex();
	}

	PLEX_endSpriteTheater();
}

static void p_tooFastCut(const size_t pamaleanaSpriteTex, const size_t cubicleBackdrop, const size_t venariusSpriteTex, const size_t extraplocanSpriteTex, const size_t fontPlate, const size_t label, const size_t bubble, const size_t arrow)
{
	p_failCut(MMS1_LANGUAGE_MINIGAME_FAILFAST, pamaleanaSpriteTex, cubicleBackdrop, venariusSpriteTex, extraplocanSpriteTex, fontPlate, label, bubble, arrow);
}

static void p_tooSlowCut(const size_t pamaleanaSpriteTex, const size_t cubicleBackdrop, const size_t venariusSpriteTex, const size_t extraplocanSpriteTex, const size_t fontPlate, const size_t label, const size_t bubble, const size_t arrow)
{
	p_failCut(MMS1_LANGUAGE_MINIGAME_FAILSLOW, pamaleanaSpriteTex, cubicleBackdrop, venariusSpriteTex, extraplocanSpriteTex, fontPlate, label, bubble, arrow);
}

// PUBLIC FUNCTIONS:

bool MMS_beginMinigame1(const size_t chapter1SpriteTex, const size_t pamaleanaSpriteTex, const size_t venariusSpriteTex, const size_t extraplocanSpriteTex, const uint64_t time)
{
	// PAMALEANA

	PLEX_ANIMATION_PLATE pamaleanaPlate = PLEX_createAnimationPlate(0.0625, 0.125, pamaleanaSpriteTex);

	PLEX_ANIMATION* pamaleanaStoppedAnim = NULL;

	PLEX_ANIMATION* pamaleanaDriftLeft = NULL;
	PLEX_ANIMATION* pamaleanaDriftRight = NULL;
	PLEX_ANIMATION* pamaleanaDriftForward = NULL;
	PLEX_ANIMATION* pamaleanaDriftBackward = NULL;

	PLEX_ANIMATION* pamaleanaAccelForward = NULL;

	PLEX_ANIMATION* pamaleanaCrash = NULL;

	// CAR

	PLEX_ANIMATION_PLATE carPlate = PLEX_createAnimationPlate(0.2, 0.0334, chapter1SpriteTex);

	PLEX_ANIMATION* carBackHealthyAnim = NULL;
	PLEX_ANIMATION* carBackBroken1Anim = NULL;
	PLEX_ANIMATION* carBackBroken2Anim = NULL;
	PLEX_ANIMATION* carBackBroken3Anim = NULL;

	PLEX_ANIMATION* carBackHealthyBackingAnim = NULL;
	PLEX_ANIMATION* carBackBroken1BackingAnim = NULL;
	PLEX_ANIMATION* carBackBroken2BackingAnim = NULL;
	PLEX_ANIMATION* carBackBroken3BackingAnim = NULL;

	PLEX_ANIMATION* carSeatHealthyAnim = NULL;
	PLEX_ANIMATION* carSeatBroken1Anim = NULL;
	PLEX_ANIMATION* carSeatBroken2Anim = NULL;
	PLEX_ANIMATION* carSeatBroken3Anim = NULL;

	PLEX_ANIMATION* carWindshieldHealthyAnim = NULL;
	PLEX_ANIMATION* carWindshieldBroken1Anim = NULL;
	PLEX_ANIMATION* carWindshieldBroken2Anim = NULL;
	PLEX_ANIMATION* carWindshieldBroken3Anim = NULL;

	PLEX_ANIMATION* carWingLeftHealthyAnim = NULL;
	PLEX_ANIMATION* carWingLeftBroken1Anim = NULL;
	PLEX_ANIMATION* carWingLeftBroken2Anim = NULL;
	PLEX_ANIMATION* carWingLeftBroken3Anim = NULL;

	PLEX_ANIMATION* carWingLeftHealthyBackingAnim = NULL;
	PLEX_ANIMATION* carWingLeftBroken1BackingAnim = NULL;
	PLEX_ANIMATION* carWingLeftBroken2BackingAnim = NULL;
	PLEX_ANIMATION* carWingLeftBroken3BackingAnim = NULL;

	PLEX_ANIMATION* carWingRightHealthyAnim = NULL;
	PLEX_ANIMATION* carWingRightBroken1Anim = NULL;
	PLEX_ANIMATION* carWingRightBroken2Anim = NULL;
	PLEX_ANIMATION* carWingRightBroken3Anim = NULL;

	PLEX_ANIMATION* carWingRightHealthyBackingAnim = NULL;
	PLEX_ANIMATION* carWingRightBroken1BackingAnim = NULL;
	PLEX_ANIMATION* carWingRightBroken2BackingAnim = NULL;
	PLEX_ANIMATION* carWingRightBroken3BackingAnim = NULL;

	// THE DOME

	PLEX_ANIMATION_PLATE domeParallaxPlate;
	PLEX_ANIMATION_PLATE domeParallaxPlate2;

	PLEX_ANIMATION* domeParallaxAnimation = NULL;
	PLEX_ANIMATION* domeParallaxAnimation2 = NULL;

	// THE ROAD

	PLEX_ANIMATION_PLATE road0Plate;
	PLEX_ANIMATION_PLATE road1Plate;
	PLEX_ANIMATION_PLATE road2Plate;
	PLEX_ANIMATION_PLATE road3Plate;
	PLEX_ANIMATION_PLATE road4Plate;

	PLEX_ANIMATION* road0Animation = NULL;
	PLEX_ANIMATION* road1Animation = NULL;
	PLEX_ANIMATION* road2Animation = NULL;
	PLEX_ANIMATION* road3Animation = NULL;
	PLEX_ANIMATION* road4Animation = NULL;

	// THE WALL

	PLEX_ANIMATION_PLATE wall3Plate;
	PLEX_ANIMATION_PLATE wall4Plate;

	PLEX_ANIMATION* wall3Animation = NULL;
	PLEX_ANIMATION* wall4Animation = NULL;

	// OBSTRUCTIONS

	PLEX_ANIMATION_PLATE car0Plate;

	PLEX_ANIMATION* car0Down = NULL;
	PLEX_ANIMATION* car0Up = NULL;
	PLEX_ANIMATION* car0Crash = NULL;

	PLEX_ANIMATION_PLATE car1Plate;

	PLEX_ANIMATION* car1Down = NULL;
	PLEX_ANIMATION* car1Up = NULL;
	PLEX_ANIMATION* car1Crash = NULL;

	PLEX_ANIMATION_PLATE car2Plate;

	PLEX_ANIMATION* car2Down = NULL;
	PLEX_ANIMATION* car2Up = NULL;
	PLEX_ANIMATION* car2Crash = NULL;

	PLEX_ANIMATION_PLATE car3Plate;

	PLEX_ANIMATION* car3Down = NULL;
	PLEX_ANIMATION* car3Up = NULL;
	PLEX_ANIMATION* car3Crash = NULL;

	PLEX_ANIMATION_PLATE car4Plate;

	PLEX_ANIMATION* car4Down = NULL;
	PLEX_ANIMATION* car4Up = NULL;
	PLEX_ANIMATION* car4Crash = NULL;

	PLEX_ANIMATION_PLATE car5Plate;

	PLEX_ANIMATION* car5Down = NULL;
	PLEX_ANIMATION* car5Up = NULL;
	PLEX_ANIMATION* car5Crash = NULL;

	PLEX_ANIMATION_PLATE car6Plate;

	PLEX_ANIMATION* car6Down = NULL;
	PLEX_ANIMATION* car6Up = NULL;
	PLEX_ANIMATION* car6Crash = NULL;

	PLEX_ANIMATION_PLATE car7Plate;

	PLEX_ANIMATION* car7Down = NULL;
	PLEX_ANIMATION* car7Up = NULL;
	PLEX_ANIMATION* car7Crash = NULL;

	PLEX_ANIMATION_PLATE car8Plate;

	PLEX_ANIMATION* car8Down = NULL;
	PLEX_ANIMATION* car8Up = NULL;
	PLEX_ANIMATION* car8Crash = NULL;

	PLEX_ANIMATION_PLATE car9Plate;

	PLEX_ANIMATION* car9Down = NULL;
	PLEX_ANIMATION* car9Up = NULL;
	PLEX_ANIMATION* car9Crash = NULL;

	PLEX_ANIMATION_PLATE car10Plate;

	PLEX_ANIMATION* car10Down = NULL;
	PLEX_ANIMATION* car10Up = NULL;
	PLEX_ANIMATION* car10Crash = NULL;

	PLEX_ANIMATION_PLATE car11Plate;

	PLEX_ANIMATION* car11Down = NULL;
	PLEX_ANIMATION* car11Up = NULL;
	PLEX_ANIMATION* car11Crash = NULL;

	PLEX_ANIMATION_PLATE car12Plate;

	PLEX_ANIMATION* car12Down = NULL;
	PLEX_ANIMATION* car12Up = NULL;
	PLEX_ANIMATION* car12Crash = NULL;

	PLEX_ANIMATION_PLATE car13Plate;

	PLEX_ANIMATION* car13Down = NULL;
	PLEX_ANIMATION* car13Up = NULL;
	PLEX_ANIMATION* car13Crash = NULL;

	PLEX_ANIMATION_PLATE car14Plate;

	PLEX_ANIMATION* car14Down = NULL;
	PLEX_ANIMATION* car14Up = NULL;
	PLEX_ANIMATION* car14Crash = NULL;

	PLEX_ANIMATION_PLATE car15Plate;

	PLEX_ANIMATION* car15Down = NULL;
	PLEX_ANIMATION* car15Up = NULL;
	PLEX_ANIMATION* car15Crash = NULL;

	PLEX_ANIMATION_PLATE bikePlate;

	PLEX_ANIMATION* bikeDown = NULL;
	PLEX_ANIMATION* bikeUp = NULL;
	PLEX_ANIMATION* bikeCrash = NULL;

	PLEX_ANIMATION_PLATE bikerPlate;

	PLEX_ANIMATION* bikerDown = NULL;
	PLEX_ANIMATION* bikerUp = NULL;
	PLEX_ANIMATION* bikerCrash = NULL;

	PLEX_ANIMATION_PLATE cactusPlate;

	PLEX_ANIMATION* cactusDown = NULL;
	PLEX_ANIMATION* cactusUp = NULL;
	PLEX_ANIMATION* cactusCrash = NULL;

	PLEX_ANIMATION_PLATE tumbleweedPlate;

	PLEX_ANIMATION* tumbleweedDown = NULL;
	PLEX_ANIMATION* tumbleweedUp = NULL;
	PLEX_ANIMATION* tumbleweedCrash = NULL;

	PLEX_ANIMATION_PLATE bagPlate;

	PLEX_ANIMATION* bagDown = NULL;
	PLEX_ANIMATION* bagUp = NULL;
	PLEX_ANIMATION* bagCrash = NULL;

	PLEX_ANIMATION_PLATE bag2Plate;

	PLEX_ANIMATION* bag2Down = NULL;
	PLEX_ANIMATION* bag2Up = NULL;
	PLEX_ANIMATION* bag2Crash = NULL;

	PLEX_ANIMATION_PLATE barrelPlate;

	PLEX_ANIMATION* barrelDown = NULL;
	PLEX_ANIMATION* barrelUp = NULL;
	PLEX_ANIMATION* barrelCrash = NULL;

	PLEX_ANIMATION_PLATE bricksPlate;

	PLEX_ANIMATION* bricksDown = NULL;
	PLEX_ANIMATION* bricksUp = NULL;
	PLEX_ANIMATION* bricksCrash = NULL;

	PLEX_ANIMATION_PLATE cratePlate;

	PLEX_ANIMATION* crateDown = NULL;
	PLEX_ANIMATION* crateUp = NULL;
	PLEX_ANIMATION* crateCrash = NULL;

	PLEX_ANIMATION_PLATE pipesPlate;

	PLEX_ANIMATION* pipesDown = NULL;
	PLEX_ANIMATION* pipesUp = NULL;
	PLEX_ANIMATION* pipesCrash = NULL;

	// PAUSE ANIMATION

	PLEX_ANIMATION_PLATE pausePlate;

	PLEX_ANIMATION* pauseAnim = NULL;

	// PAMALEANA

	p_pamaleanaSprite = PLEX_createSprite(&pamaleanaPlate);

	PLEX_addAnimation(&p_pamaleanaSprite, 800, PLEX_getGameTime());

	pamaleanaStoppedAnim = PLEX_getAnimation(0, &p_pamaleanaSprite);

	PLEX_addAnimationFrame(33, pamaleanaStoppedAnim);

	PLEX_addAnimation(&p_pamaleanaSprite, 1000, PLEX_getGameTime());

	pamaleanaDriftLeft = PLEX_getAnimation(1, &p_pamaleanaSprite);

	PLEX_addAnimationFrame(37, pamaleanaDriftLeft);
	PLEX_addAnimationFrame(38, pamaleanaDriftLeft);
	PLEX_addAnimationFrame(39, pamaleanaDriftLeft);
	PLEX_addAnimationFrame(38, pamaleanaDriftLeft);

	PLEX_addAnimation(&p_pamaleanaSprite, 1000, PLEX_getGameTime());

	pamaleanaDriftRight = PLEX_getAnimation(2, &p_pamaleanaSprite);

	PLEX_addAnimationFrame(34, pamaleanaDriftRight);
	PLEX_addAnimationFrame(35, pamaleanaDriftRight);
	PLEX_addAnimationFrame(36, pamaleanaDriftRight);
	PLEX_addAnimationFrame(35, pamaleanaDriftRight);

	PLEX_addAnimation(&p_pamaleanaSprite, 1000, PLEX_getGameTime());

	pamaleanaDriftForward = PLEX_getAnimation(3, &p_pamaleanaSprite);

	PLEX_addAnimationFrame(40, pamaleanaDriftForward);
	PLEX_addAnimationFrame(41, pamaleanaDriftForward);
	PLEX_addAnimationFrame(42, pamaleanaDriftForward);
	PLEX_addAnimationFrame(41, pamaleanaDriftForward);

	PLEX_addAnimation(&p_pamaleanaSprite, 1000, PLEX_getGameTime());

	pamaleanaDriftBackward = PLEX_getAnimation(4, &p_pamaleanaSprite);

	PLEX_addAnimationFrame(43, pamaleanaDriftBackward);
	PLEX_addAnimationFrame(44, pamaleanaDriftBackward);
	PLEX_addAnimationFrame(45, pamaleanaDriftBackward);
	PLEX_addAnimationFrame(44, pamaleanaDriftBackward);

	PLEX_addAnimation(&p_pamaleanaSprite, 250, PLEX_getGameTime());

	pamaleanaAccelForward = PLEX_getAnimation(5, &p_pamaleanaSprite);

	PLEX_addAnimationFrame(40, pamaleanaAccelForward);
	PLEX_addAnimationFrame(46, pamaleanaAccelForward);
	PLEX_addAnimationFrame(47, pamaleanaAccelForward);
	PLEX_addAnimationFrame(46, pamaleanaAccelForward);

	PLEX_addAnimation(&p_pamaleanaSprite, 500, PLEX_getGameTime());

	pamaleanaCrash = PLEX_getAnimation(6, &p_pamaleanaSprite);

	PLEX_addAnimationFrame(48, pamaleanaCrash);
	PLEX_addAnimationFrame(49, pamaleanaCrash);
	PLEX_addAnimationFrame(48, pamaleanaCrash);

	PLEX_startSprite(time, &p_pamaleanaSprite);

	// CAR BACK

	p_carBack = PLEX_createSprite(&carPlate);

	PLEX_addAnimation(&p_carBack, 200, PLEX_getGameTime());

	carBackHealthyAnim = PLEX_getAnimation(0, &p_carBack);

	PLEX_addAnimationFrame(0, carBackHealthyAnim);
	PLEX_addAnimationFrame(5, carBackHealthyAnim);
	PLEX_addAnimationFrame(10, carBackHealthyAnim);
	PLEX_addAnimationFrame(15, carBackHealthyAnim);
	PLEX_addAnimationFrame(20, carBackHealthyAnim);

	PLEX_addAnimation(&p_carBack, 200, PLEX_getGameTime());

	carBackBroken1Anim = PLEX_getAnimation(1, &p_carBack);

	PLEX_addAnimationFrame(25, carBackBroken1Anim);
	PLEX_addAnimationFrame(30, carBackBroken1Anim);
	PLEX_addAnimationFrame(35, carBackBroken1Anim);
	PLEX_addAnimationFrame(40, carBackBroken1Anim);
	PLEX_addAnimationFrame(45, carBackBroken1Anim);

	PLEX_addAnimation(&p_carBack, 200, PLEX_getGameTime());

	carBackBroken2Anim = PLEX_getAnimation(2, &p_carBack);

	PLEX_addAnimationFrame(26, carBackBroken2Anim);
	PLEX_addAnimationFrame(31, carBackBroken2Anim);
	PLEX_addAnimationFrame(36, carBackBroken2Anim);
	PLEX_addAnimationFrame(41, carBackBroken2Anim);
	PLEX_addAnimationFrame(46, carBackBroken2Anim);

	PLEX_addAnimation(&p_carBack, 200, PLEX_getGameTime());

	carBackBroken3Anim = PLEX_getAnimation(3, &p_carBack);

	PLEX_addAnimationFrame(27, carBackBroken3Anim);
	PLEX_addAnimationFrame(32, carBackBroken3Anim);
	PLEX_addAnimationFrame(37, carBackBroken3Anim);
	PLEX_addAnimationFrame(42, carBackBroken3Anim);
	PLEX_addAnimationFrame(47, carBackBroken3Anim);

	PLEX_addAnimation(&p_carBack, 200, PLEX_getGameTime());

	carBackHealthyBackingAnim = PLEX_getAnimation(4, &p_carBack);

	PLEX_addAnimationFrame(20, carBackHealthyBackingAnim);

	PLEX_addAnimation(&p_carBack, 200, PLEX_getGameTime());

	carBackBroken1BackingAnim = PLEX_getAnimation(5, &p_carBack);

	PLEX_addAnimationFrame(45, carBackBroken1BackingAnim);

	PLEX_addAnimation(&p_carBack, 200, PLEX_getGameTime());

	carBackBroken2BackingAnim = PLEX_getAnimation(6, &p_carBack);

	PLEX_addAnimationFrame(46, carBackBroken2BackingAnim);

	PLEX_addAnimation(&p_carBack, 200, PLEX_getGameTime());

	carBackBroken3BackingAnim = PLEX_getAnimation(7, &p_carBack);

	PLEX_addAnimationFrame(47, carBackBroken3BackingAnim);

	PLEX_startSprite(time, &p_carBack);

	// CAR SEAT

	p_carSeat = PLEX_createSprite(&carPlate);

	PLEX_addAnimation(&p_carSeat, 800, PLEX_getGameTime());

	carSeatHealthyAnim = PLEX_getAnimation(0, &p_carSeat);

	PLEX_addAnimationFrame(1, carSeatHealthyAnim);

	PLEX_addAnimation(&p_carSeat, 800, PLEX_getGameTime());

	carSeatBroken1Anim = PLEX_getAnimation(1, &p_carSeat);

	PLEX_addAnimationFrame(6, carSeatBroken1Anim);

	PLEX_addAnimation(&p_carSeat, 800, PLEX_getGameTime());

	carSeatBroken2Anim = PLEX_getAnimation(2, &p_carSeat);

	PLEX_addAnimationFrame(11, carSeatBroken2Anim);

	PLEX_addAnimation(&p_carSeat, 800, PLEX_getGameTime());

	carSeatBroken3Anim = PLEX_getAnimation(3, &p_carSeat);

	PLEX_addAnimationFrame(16, carSeatBroken3Anim);

	// CAR WINDSHIELD

	p_carWindshield = PLEX_createSprite(&carPlate);

	PLEX_addAnimation(&p_carWindshield, 800, PLEX_getGameTime());

	carWindshieldHealthyAnim = PLEX_getAnimation(0, &p_carWindshield);

	PLEX_addAnimationFrame(2, carWindshieldHealthyAnim);

	PLEX_addAnimation(&p_carWindshield, 800, PLEX_getGameTime());

	carWindshieldBroken1Anim = PLEX_getAnimation(1, &p_carWindshield);

	PLEX_addAnimationFrame(7, carWindshieldBroken1Anim);

	PLEX_addAnimation(&p_carWindshield, 800, PLEX_getGameTime());

	carWindshieldBroken2Anim = PLEX_getAnimation(2, &p_carWindshield);

	PLEX_addAnimationFrame(12, carWindshieldBroken2Anim);

	PLEX_addAnimation(&p_carWindshield, 800, PLEX_getGameTime());

	carWindshieldBroken3Anim = PLEX_getAnimation(3, &p_carWindshield);

	PLEX_addAnimationFrame(17, carWindshieldBroken3Anim);

	// windshield effect

	p_windshieldBreak = PLEX_loadTextureFromFile(1920, 3240, 255, 255, 255, 255, 0, "./resources/chapter1/windshieldbreak.png");

	// CAR WING LEFT

	p_carWingLeft = PLEX_createSprite(&carPlate);

	PLEX_addAnimation(&p_carWingLeft, 200, PLEX_getGameTime());

	carWingLeftHealthyAnim = PLEX_getAnimation(0, &p_carWingLeft);

	PLEX_addAnimationFrame(3, carWingLeftHealthyAnim);
	PLEX_addAnimationFrame(8, carWingLeftHealthyAnim);
	PLEX_addAnimationFrame(13, carWingLeftHealthyAnim);
	PLEX_addAnimationFrame(18, carWingLeftHealthyAnim);
	PLEX_addAnimationFrame(23, carWingLeftHealthyAnim);

	PLEX_addAnimation(&p_carWingLeft, 200, PLEX_getGameTime());

	carWingLeftBroken1Anim = PLEX_getAnimation(1, &p_carWingLeft);

	PLEX_addAnimationFrame(50, carWingLeftBroken1Anim);
	PLEX_addAnimationFrame(55, carWingLeftBroken1Anim);
	PLEX_addAnimationFrame(60, carWingLeftBroken1Anim);
	PLEX_addAnimationFrame(65, carWingLeftBroken1Anim);
	PLEX_addAnimationFrame(70, carWingLeftBroken1Anim);

	PLEX_addAnimation(&p_carWingLeft, 200, PLEX_getGameTime());

	carWingLeftBroken2Anim = PLEX_getAnimation(2, &p_carWingLeft);

	PLEX_addAnimationFrame(51, carWingLeftBroken2Anim);
	PLEX_addAnimationFrame(56, carWingLeftBroken2Anim);
	PLEX_addAnimationFrame(61, carWingLeftBroken2Anim);
	PLEX_addAnimationFrame(66, carWingLeftBroken2Anim);
	PLEX_addAnimationFrame(71, carWingLeftBroken2Anim);

	PLEX_addAnimation(&p_carWingLeft, 200, PLEX_getGameTime());

	carWingLeftBroken3Anim = PLEX_getAnimation(3, &p_carWingLeft);

	PLEX_addAnimationFrame(52, carWingLeftBroken3Anim);
	PLEX_addAnimationFrame(57, carWingLeftBroken3Anim);
	PLEX_addAnimationFrame(62, carWingLeftBroken3Anim);
	PLEX_addAnimationFrame(67, carWingLeftBroken3Anim);
	PLEX_addAnimationFrame(72, carWingLeftBroken3Anim);

	PLEX_addAnimation(&p_carWingLeft, 200, PLEX_getGameTime());

	carWingLeftHealthyBackingAnim = PLEX_getAnimation(4, &p_carWingLeft);

	PLEX_addAnimationFrame(23, carWingLeftHealthyBackingAnim);

	PLEX_addAnimation(&p_carWingLeft, 200, PLEX_getGameTime());

	carWingLeftBroken1BackingAnim = PLEX_getAnimation(5, &p_carWingLeft);

	PLEX_addAnimationFrame(70, carWingLeftBroken1BackingAnim);

	PLEX_addAnimation(&p_carWingLeft, 200, PLEX_getGameTime());

	carWingLeftBroken2BackingAnim = PLEX_getAnimation(6, &p_carWingLeft);

	PLEX_addAnimationFrame(71, carWingLeftBroken2BackingAnim);

	PLEX_addAnimation(&p_carWingLeft, 200, PLEX_getGameTime());

	carWingLeftBroken3BackingAnim = PLEX_getAnimation(7, &p_carWingLeft);

	PLEX_addAnimationFrame(72, carWingLeftBroken3BackingAnim);

	PLEX_startSprite(time, &p_carWingLeft);

	// CAR WING RIGHT

	p_carWingRight = PLEX_createSprite(&carPlate);

	PLEX_addAnimation(&p_carWingRight, 200, PLEX_getGameTime());

	carWingRightHealthyAnim = PLEX_getAnimation(0, &p_carWingRight);

	PLEX_addAnimationFrame(4, carWingRightHealthyAnim);
	PLEX_addAnimationFrame(9, carWingRightHealthyAnim);
	PLEX_addAnimationFrame(14, carWingRightHealthyAnim);
	PLEX_addAnimationFrame(19, carWingRightHealthyAnim);
	PLEX_addAnimationFrame(24, carWingRightHealthyAnim);

	PLEX_addAnimation(&p_carWingRight, 200, PLEX_getGameTime());

	carWingRightBroken1Anim = PLEX_getAnimation(1, &p_carWingRight);

	PLEX_addAnimationFrame(28, carWingRightBroken1Anim);
	PLEX_addAnimationFrame(33, carWingRightBroken1Anim);
	PLEX_addAnimationFrame(38, carWingRightBroken1Anim);
	PLEX_addAnimationFrame(43, carWingRightBroken1Anim);
	PLEX_addAnimationFrame(48, carWingRightBroken1Anim);

	PLEX_addAnimation(&p_carWingRight, 200, PLEX_getGameTime());

	carWingRightBroken2Anim = PLEX_getAnimation(2, &p_carWingRight);

	PLEX_addAnimationFrame(29, carWingRightBroken2Anim);
	PLEX_addAnimationFrame(34, carWingRightBroken2Anim);
	PLEX_addAnimationFrame(39, carWingRightBroken2Anim);
	PLEX_addAnimationFrame(44, carWingRightBroken2Anim);
	PLEX_addAnimationFrame(49, carWingRightBroken2Anim);

	PLEX_addAnimation(&p_carWingRight, 200, PLEX_getGameTime());

	carWingRightBroken3Anim = PLEX_getAnimation(3, &p_carWingRight);

	PLEX_addAnimationFrame(53, carWingRightBroken3Anim);
	PLEX_addAnimationFrame(58, carWingRightBroken3Anim);
	PLEX_addAnimationFrame(63, carWingRightBroken3Anim);
	PLEX_addAnimationFrame(68, carWingRightBroken3Anim);
	PLEX_addAnimationFrame(73, carWingRightBroken3Anim);

	PLEX_addAnimation(&p_carWingRight, 200, PLEX_getGameTime());

	carWingRightHealthyBackingAnim = PLEX_getAnimation(4, &p_carWingRight);

	PLEX_addAnimationFrame(24, carWingRightHealthyBackingAnim);

	PLEX_addAnimation(&p_carWingRight, 200, PLEX_getGameTime());

	carWingRightBroken1BackingAnim = PLEX_getAnimation(5, &p_carWingRight);

	PLEX_addAnimationFrame(48, carWingRightBroken1BackingAnim);

	PLEX_addAnimation(&p_carWingRight, 200, PLEX_getGameTime());

	carWingRightBroken2BackingAnim = PLEX_getAnimation(6, &p_carWingRight);

	PLEX_addAnimationFrame(49, carWingRightBroken2BackingAnim);

	PLEX_addAnimation(&p_carWingRight, 200, PLEX_getGameTime());

	carWingRightBroken3BackingAnim = PLEX_getAnimation(7, &p_carWingRight);

	PLEX_addAnimationFrame(73, carWingRightBroken3BackingAnim);

	PLEX_startSprite(time, &p_carWingRight);

	// THE DOME

	p_domeParallaxTex = PLEX_loadTextureFromFile(1560, 200, 255, 255, 255, 255, 0, "./resources/chapter1/parallaxdome.png");
	p_domeParallaxTex2 = PLEX_loadTextureFromFile(1560, 200, 255, 255, 255, 255, 0, "./resources/chapter1/parallaxdome2.png");

	domeParallaxPlate = PLEX_createAnimationPlate(0.5, 1, p_domeParallaxTex);
	domeParallaxPlate2 = PLEX_createAnimationPlate(0.5, 1, p_domeParallaxTex2);

	p_domeParallax = PLEX_createSprite(&domeParallaxPlate);
	p_domeParallax2 = PLEX_createSprite(&domeParallaxPlate2);

	PLEX_addAnimation(&p_domeParallax, 400, PLEX_getGameTime());

	domeParallaxAnimation = PLEX_getAnimation(0, &p_domeParallax);

	PLEX_addAnimationFrame(0, domeParallaxAnimation);
	PLEX_addAnimationFrame(1, domeParallaxAnimation);

	PLEX_startSprite(time, &p_domeParallax);

	PLEX_addAnimation(&p_domeParallax2, 400, PLEX_getGameTime());

	domeParallaxAnimation2 = PLEX_getAnimation(0, &p_domeParallax2);

	PLEX_addAnimationFrame(0, domeParallaxAnimation2);
	PLEX_addAnimationFrame(1, domeParallaxAnimation2);

	PLEX_startSprite(time, &p_domeParallax2);

	// SKYLINE

	p_skyline = PLEX_loadTextureFromFile(800, 500, 255, 255, 255, 255, 0, "./resources/chapter1/skyline.png");

	// THE ROAD

	p_road0Tex = PLEX_loadTextureFromFile(3840, 2466, 255, 255, 255, 255, 0, "./resources/chapter1/road0.png");
	p_road1Tex = PLEX_loadTextureFromFile(3840, 2466, 255, 255, 255, 255, 0, "./resources/chapter1/road1.png");
	p_road2Tex = PLEX_loadTextureFromFile(3840, 2466, 255, 255, 255, 255, 0, "./resources/chapter1/road2.png");
	p_road3Tex = PLEX_loadTextureFromFile(3840, 2466, 255, 255, 255, 255, 0, "./resources/chapter1/road3.png");
	p_road4Tex = PLEX_loadTextureFromFile(3840, 2466, 255, 255, 255, 255, 0, "./resources/chapter1/road4.png");

	road0Plate = PLEX_createAnimationPlate(1, 0.5, p_road0Tex);
	road1Plate = PLEX_createAnimationPlate(1, 0.5, p_road1Tex);
	road2Plate = PLEX_createAnimationPlate(1, 0.5, p_road2Tex);
	road3Plate = PLEX_createAnimationPlate(1, 0.5, p_road3Tex);
	road4Plate = PLEX_createAnimationPlate(1, 0.5, p_road4Tex);

	p_road0 = PLEX_createSprite(&road0Plate);
	p_road1 = PLEX_createSprite(&road1Plate);
	p_road2 = PLEX_createSprite(&road2Plate);
	p_road3 = PLEX_createSprite(&road3Plate);
	p_road4 = PLEX_createSprite(&road4Plate);

	PLEX_addAnimation(&p_road0, 400, PLEX_getGameTime());

	road0Animation = PLEX_getAnimation(0, &p_road0);

	PLEX_addAnimationFrame(0, road0Animation);
	PLEX_addAnimationFrame(1, road0Animation);

	PLEX_startSprite(time, &p_road0);

	PLEX_addAnimation(&p_road1, 400, PLEX_getGameTime());

	road1Animation = PLEX_getAnimation(0, &p_road1);

	PLEX_addAnimationFrame(0, road1Animation);
	PLEX_addAnimationFrame(1, road1Animation);

	PLEX_startSprite(time, &p_road1);

	PLEX_addAnimation(&p_road2, 400, PLEX_getGameTime());

	road2Animation = PLEX_getAnimation(0, &p_road2);

	PLEX_addAnimationFrame(0, road2Animation);
	PLEX_addAnimationFrame(1, road2Animation);

	PLEX_startSprite(time, &p_road2);

	PLEX_addAnimation(&p_road3, 400, PLEX_getGameTime());

	road3Animation = PLEX_getAnimation(0, &p_road3);

	PLEX_addAnimationFrame(0, road3Animation);
	PLEX_addAnimationFrame(1, road3Animation);

	PLEX_startSprite(time, &p_road3);

	PLEX_addAnimation(&p_road4, 400, PLEX_getGameTime());

	road4Animation = PLEX_getAnimation(0, &p_road4);

	PLEX_addAnimationFrame(0, road4Animation);
	PLEX_addAnimationFrame(1, road4Animation);

	PLEX_startSprite(time, &p_road4);

	// WALL

	p_wall3Tex = PLEX_loadTextureFromFile(3840, 2466, 255, 255, 255, 255, 0, "./resources/chapter1/wall3.png");

	wall3Plate = PLEX_createAnimationPlate(1, 0.5, p_wall3Tex);

	p_wall3 = PLEX_createSprite(&wall3Plate);

	PLEX_addAnimation(&p_wall3, 400, PLEX_getGameTime());

	wall3Animation = PLEX_getAnimation(0, &p_wall3);

	PLEX_addAnimationFrame(0, wall3Animation);
	PLEX_addAnimationFrame(1, wall3Animation);

	PLEX_startSprite(time, &p_wall3);

	p_wall4Tex = PLEX_loadTextureFromFile(3840, 9864, 255, 255, 255, 255, 0, "./resources/chapter1/wall4.png");

	wall4Plate = PLEX_createAnimationPlate(1, 0.5, p_wall4Tex);

	p_wall4 = PLEX_createSprite(&wall4Plate);

	PLEX_addAnimation(&p_wall4, 400, PLEX_getGameTime());

	wall4Animation = PLEX_getAnimation(0, &p_wall4);

	PLEX_addAnimationFrame(0, wall4Animation);
	PLEX_addAnimationFrame(1, wall4Animation);

	PLEX_startSprite(time, &p_wall4);

	// SKY BACKDROPS

	p_backdrop0 = PLEX_loadTextureFromFile(3840, 1644, 255, 255, 255, 255, 0, "./resources/chapter1/phase0back.png");
	p_backdrop1 = PLEX_loadTextureFromFile(3840, 1644, 255, 255, 255, 255, 0, "./resources/chapter1/phase1back.png");
	p_backdrop2 = PLEX_loadTextureFromFile(3840, 1644, 255, 255, 255, 255, 0, "./resources/chapter1/phase2back.png");
	p_backdrop3 = PLEX_loadTextureFromFile(3840, 1644, 255, 255, 255, 255, 0, "./resources/chapter1/phase3back.png");
	p_backdrop4 = PLEX_loadTextureFromFile(3840, 1644, 255, 255, 255, 255, 0, "./resources/chapter1/phase4back.png");

	// OBSTRUCTIONS ----------------------------------------------------------------------------------------------------

	// CAR 0

	p_car0Tex = PLEX_loadTextureFromFile(512, 192, 255, 255, 255, 255, 0, "./resources/chapter1/obstructions/phase0/car0.png");

	car0Plate = PLEX_createAnimationPlate(0.25, 0.33333333, p_car0Tex);

	p_car0 = PLEX_createSprite(&car0Plate);

	PLEX_addAnimation(&p_car0, 1000, PLEX_getGameTime());

	car0Down = PLEX_getAnimation(0, &p_car0);

	PLEX_addAnimationFrame(0, car0Down);
	PLEX_addAnimationFrame(1, car0Down);
	PLEX_addAnimationFrame(2, car0Down);
	PLEX_addAnimationFrame(3, car0Down);

	PLEX_addAnimation(&p_car0, 1000, PLEX_getGameTime());

	car0Up = PLEX_getAnimation(1, &p_car0);

	PLEX_addAnimationFrame(4, car0Up);
	PLEX_addAnimationFrame(5, car0Up);
	PLEX_addAnimationFrame(6, car0Up);
	PLEX_addAnimationFrame(7, car0Up);

	PLEX_addAnimation(&p_car0, 1000, PLEX_getGameTime());

	car0Crash = PLEX_getAnimation(2, &p_car0);

	PLEX_addAnimationFrame(8, car0Crash);
	PLEX_addAnimationFrame(9, car0Crash);
	PLEX_addAnimationFrame(10, car0Crash);
	PLEX_addAnimationFrame(11, car0Crash);

	PLEX_startSprite(time, &p_car0);

	// CAR 1

	p_car1Tex = PLEX_loadTextureFromFile(512, 192, 255, 255, 255, 255, 0, "./resources/chapter1/obstructions/phase1/car1.png");

	car1Plate = PLEX_createAnimationPlate(0.25, 0.33333333, p_car1Tex);

	p_car1 = PLEX_createSprite(&car1Plate);

	PLEX_addAnimation(&p_car1, 1000, PLEX_getGameTime());

	car1Down = PLEX_getAnimation(0, &p_car1);

	PLEX_addAnimationFrame(0, car1Down);
	PLEX_addAnimationFrame(1, car1Down);
	PLEX_addAnimationFrame(2, car1Down);
	PLEX_addAnimationFrame(3, car1Down);

	PLEX_addAnimation(&p_car1, 1000, PLEX_getGameTime());

	car1Up = PLEX_getAnimation(1, &p_car1);

	PLEX_addAnimationFrame(4, car1Up);
	PLEX_addAnimationFrame(5, car1Up);
	PLEX_addAnimationFrame(6, car1Up);
	PLEX_addAnimationFrame(7, car1Up);

	PLEX_addAnimation(&p_car1, 1000, PLEX_getGameTime());

	car1Crash = PLEX_getAnimation(2, &p_car1);

	PLEX_addAnimationFrame(8, car1Crash);
	PLEX_addAnimationFrame(9, car1Crash);
	PLEX_addAnimationFrame(10, car1Crash);
	PLEX_addAnimationFrame(11, car1Crash);

	PLEX_startSprite(time, &p_car1);

	// CAR 2

	p_car2Tex = PLEX_loadTextureFromFile(512, 192, 255, 255, 255, 255, 0, "./resources/chapter1/obstructions/phase1/car2.png");

	car2Plate = PLEX_createAnimationPlate(0.25, 0.33333333, p_car2Tex);

	p_car2 = PLEX_createSprite(&car2Plate);

	PLEX_addAnimation(&p_car2, 1000, PLEX_getGameTime());

	car2Down = PLEX_getAnimation(0, &p_car2);

	PLEX_addAnimationFrame(0, car2Down);
	PLEX_addAnimationFrame(1, car2Down);
	PLEX_addAnimationFrame(2, car2Down);
	PLEX_addAnimationFrame(3, car2Down);

	PLEX_addAnimation(&p_car2, 1000, PLEX_getGameTime());

	car2Up = PLEX_getAnimation(1, &p_car2);

	PLEX_addAnimationFrame(4, car2Up);
	PLEX_addAnimationFrame(5, car2Up);
	PLEX_addAnimationFrame(6, car2Up);
	PLEX_addAnimationFrame(7, car2Up);

	PLEX_addAnimation(&p_car2, 1000, PLEX_getGameTime());

	car2Crash = PLEX_getAnimation(2, &p_car2);

	PLEX_addAnimationFrame(8, car2Crash);
	PLEX_addAnimationFrame(9, car2Crash);
	PLEX_addAnimationFrame(10, car2Crash);
	PLEX_addAnimationFrame(11, car2Crash);

	PLEX_startSprite(time, &p_car2);

	// CAR 3

	p_car3Tex = PLEX_loadTextureFromFile(512, 192, 255, 255, 255, 255, 0, "./resources/chapter1/obstructions/phase2/car3.png");

	car3Plate = PLEX_createAnimationPlate(0.25, 0.33333333, p_car3Tex);

	p_car3 = PLEX_createSprite(&car3Plate);

	PLEX_addAnimation(&p_car3, 1000, PLEX_getGameTime());

	car3Down = PLEX_getAnimation(0, &p_car3);

	PLEX_addAnimationFrame(0, car3Down);
	PLEX_addAnimationFrame(1, car3Down);
	PLEX_addAnimationFrame(2, car3Down);
	PLEX_addAnimationFrame(3, car3Down);

	PLEX_addAnimation(&p_car3, 1000, PLEX_getGameTime());

	car3Up = PLEX_getAnimation(1, &p_car3);

	PLEX_addAnimationFrame(4, car3Up);
	PLEX_addAnimationFrame(5, car3Up);
	PLEX_addAnimationFrame(6, car3Up);
	PLEX_addAnimationFrame(7, car3Up);

	PLEX_addAnimation(&p_car3, 1000, PLEX_getGameTime());

	car3Crash = PLEX_getAnimation(2, &p_car3);

	PLEX_addAnimationFrame(8, car3Crash);
	PLEX_addAnimationFrame(9, car3Crash);
	PLEX_addAnimationFrame(10, car3Crash);
	PLEX_addAnimationFrame(11, car3Crash);

	PLEX_startSprite(time, &p_car3);

	// CAR 4

	p_car4Tex = PLEX_loadTextureFromFile(512, 192, 255, 255, 255, 255, 0, "./resources/chapter1/obstructions/phase2/car4.png");

	car4Plate = PLEX_createAnimationPlate(0.25, 0.33333333, p_car4Tex);

	p_car4 = PLEX_createSprite(&car4Plate);

	PLEX_addAnimation(&p_car4, 1000, PLEX_getGameTime());

	car4Down = PLEX_getAnimation(0, &p_car4);

	PLEX_addAnimationFrame(0, car4Down);
	PLEX_addAnimationFrame(1, car4Down);
	PLEX_addAnimationFrame(2, car4Down);
	PLEX_addAnimationFrame(3, car4Down);

	PLEX_addAnimation(&p_car4, 1000, PLEX_getGameTime());

	car4Up = PLEX_getAnimation(1, &p_car4);

	PLEX_addAnimationFrame(4, car4Up);
	PLEX_addAnimationFrame(5, car4Up);
	PLEX_addAnimationFrame(6, car4Up);
	PLEX_addAnimationFrame(7, car4Up);

	PLEX_addAnimation(&p_car4, 1000, PLEX_getGameTime());

	car4Crash = PLEX_getAnimation(2, &p_car4);

	PLEX_addAnimationFrame(8, car4Crash);
	PLEX_addAnimationFrame(9, car4Crash);
	PLEX_addAnimationFrame(10, car4Crash);
	PLEX_addAnimationFrame(11, car4Crash);

	PLEX_startSprite(time, &p_car4);

	// CAR 5

	p_car5Tex = PLEX_loadTextureFromFile(512, 192, 255, 255, 255, 255, 0, "./resources/chapter1/obstructions/phase2/car5.png");

	car5Plate = PLEX_createAnimationPlate(0.25, 0.33333333, p_car5Tex);

	p_car5 = PLEX_createSprite(&car5Plate);

	PLEX_addAnimation(&p_car5, 1000, PLEX_getGameTime());

	car5Down = PLEX_getAnimation(0, &p_car5);

	PLEX_addAnimationFrame(0, car5Down);
	PLEX_addAnimationFrame(1, car5Down);
	PLEX_addAnimationFrame(2, car5Down);
	PLEX_addAnimationFrame(3, car5Down);

	PLEX_addAnimation(&p_car5, 1000, PLEX_getGameTime());

	car5Up = PLEX_getAnimation(1, &p_car5);

	PLEX_addAnimationFrame(4, car5Up);
	PLEX_addAnimationFrame(5, car5Up);
	PLEX_addAnimationFrame(6, car5Up);
	PLEX_addAnimationFrame(7, car5Up);

	PLEX_addAnimation(&p_car5, 1000, PLEX_getGameTime());

	car5Crash = PLEX_getAnimation(2, &p_car5);

	PLEX_addAnimationFrame(8, car5Crash);
	PLEX_addAnimationFrame(9, car5Crash);
	PLEX_addAnimationFrame(10, car5Crash);
	PLEX_addAnimationFrame(11, car5Crash);

	PLEX_startSprite(time, &p_car5);

	// CAR 6

	p_car6Tex = PLEX_loadTextureFromFile(512, 192, 255, 255, 255, 255, 0, "./resources/chapter1/obstructions/phase2/car6.png");

	car6Plate = PLEX_createAnimationPlate(0.25, 0.33333333, p_car6Tex);

	p_car6 = PLEX_createSprite(&car6Plate);

	PLEX_addAnimation(&p_car6, 1000, PLEX_getGameTime());

	car6Down = PLEX_getAnimation(0, &p_car6);

	PLEX_addAnimationFrame(0, car6Down);
	PLEX_addAnimationFrame(1, car6Down);
	PLEX_addAnimationFrame(2, car6Down);
	PLEX_addAnimationFrame(3, car6Down);

	PLEX_addAnimation(&p_car6, 1000, PLEX_getGameTime());

	car6Up = PLEX_getAnimation(1, &p_car6);

	PLEX_addAnimationFrame(4, car6Up);
	PLEX_addAnimationFrame(5, car6Up);
	PLEX_addAnimationFrame(6, car6Up);
	PLEX_addAnimationFrame(7, car6Up);

	PLEX_addAnimation(&p_car6, 1000, PLEX_getGameTime());

	car6Crash = PLEX_getAnimation(2, &p_car6);

	PLEX_addAnimationFrame(8, car6Crash);
	PLEX_addAnimationFrame(9, car6Crash);
	PLEX_addAnimationFrame(10, car6Crash);
	PLEX_addAnimationFrame(11, car6Crash);

	PLEX_startSprite(time, &p_car6);

	// CAR 7

	p_car7Tex = PLEX_loadTextureFromFile(512, 192, 255, 255, 255, 255, 0, "./resources/chapter1/obstructions/phase2/car7.png");

	car7Plate = PLEX_createAnimationPlate(0.25, 0.33333333, p_car7Tex);

	p_car7 = PLEX_createSprite(&car7Plate);

	PLEX_addAnimation(&p_car7, 1000, PLEX_getGameTime());

	car7Down = PLEX_getAnimation(0, &p_car7);

	PLEX_addAnimationFrame(0, car7Down);
	PLEX_addAnimationFrame(1, car7Down);
	PLEX_addAnimationFrame(2, car7Down);
	PLEX_addAnimationFrame(3, car7Down);

	PLEX_addAnimation(&p_car7, 1000, PLEX_getGameTime());

	car7Up = PLEX_getAnimation(1, &p_car7);

	PLEX_addAnimationFrame(4, car7Up);
	PLEX_addAnimationFrame(5, car7Up);
	PLEX_addAnimationFrame(6, car7Up);
	PLEX_addAnimationFrame(7, car7Up);

	PLEX_addAnimation(&p_car7, 1000, PLEX_getGameTime());

	car7Crash = PLEX_getAnimation(2, &p_car7);

	PLEX_addAnimationFrame(8, car7Crash);
	PLEX_addAnimationFrame(9, car7Crash);
	PLEX_addAnimationFrame(10, car7Crash);
	PLEX_addAnimationFrame(11, car7Crash);

	PLEX_startSprite(time, &p_car7);

	// CAR 8

	p_car8Tex = PLEX_loadTextureFromFile(512, 192, 255, 255, 255, 255, 0, "./resources/chapter1/obstructions/phase3/car8.png");

	car8Plate = PLEX_createAnimationPlate(0.25, 0.33333333, p_car8Tex);

	p_car8 = PLEX_createSprite(&car8Plate);

	PLEX_addAnimation(&p_car8, 1000, PLEX_getGameTime());

	car8Down = PLEX_getAnimation(0, &p_car8);

	PLEX_addAnimationFrame(0, car8Down);
	PLEX_addAnimationFrame(1, car8Down);
	PLEX_addAnimationFrame(2, car8Down);
	PLEX_addAnimationFrame(3, car8Down);

	PLEX_addAnimation(&p_car8, 1000, PLEX_getGameTime());

	car8Up = PLEX_getAnimation(1, &p_car8);

	PLEX_addAnimationFrame(4, car8Up);
	PLEX_addAnimationFrame(5, car8Up);
	PLEX_addAnimationFrame(6, car8Up);
	PLEX_addAnimationFrame(7, car8Up);

	PLEX_addAnimation(&p_car8, 1000, PLEX_getGameTime());

	car8Crash = PLEX_getAnimation(2, &p_car8);

	PLEX_addAnimationFrame(8, car8Crash);
	PLEX_addAnimationFrame(9, car8Crash);
	PLEX_addAnimationFrame(10, car8Crash);
	PLEX_addAnimationFrame(11, car8Crash);

	PLEX_startSprite(time, &p_car8);

	// CAR 9

	p_car9Tex = PLEX_loadTextureFromFile(512, 192, 255, 255, 255, 255, 0, "./resources/chapter1/obstructions/phase3/car9.png");

	car9Plate = PLEX_createAnimationPlate(0.25, 0.33333333, p_car9Tex);

	p_car9 = PLEX_createSprite(&car9Plate);

	PLEX_addAnimation(&p_car9, 1000, PLEX_getGameTime());

	car9Down = PLEX_getAnimation(0, &p_car9);

	PLEX_addAnimationFrame(0, car9Down);
	PLEX_addAnimationFrame(1, car9Down);
	PLEX_addAnimationFrame(2, car9Down);
	PLEX_addAnimationFrame(3, car9Down);

	PLEX_addAnimation(&p_car9, 1000, PLEX_getGameTime());

	car9Up = PLEX_getAnimation(1, &p_car9);

	PLEX_addAnimationFrame(4, car9Up);
	PLEX_addAnimationFrame(5, car9Up);
	PLEX_addAnimationFrame(6, car9Up);
	PLEX_addAnimationFrame(7, car9Up);

	PLEX_addAnimation(&p_car9, 1000, PLEX_getGameTime());

	car9Crash = PLEX_getAnimation(2, &p_car9);

	PLEX_addAnimationFrame(8, car9Crash);
	PLEX_addAnimationFrame(9, car9Crash);
	PLEX_addAnimationFrame(10, car9Crash);
	PLEX_addAnimationFrame(11, car9Crash);

	PLEX_startSprite(time, &p_car9);

	// CAR 10

	p_car10Tex = PLEX_loadTextureFromFile(512, 192, 255, 255, 255, 255, 0, "./resources/chapter1/obstructions/phase4/car10.png");

	car10Plate = PLEX_createAnimationPlate(0.25, 0.33333333, p_car10Tex);

	p_car10 = PLEX_createSprite(&car10Plate);

	PLEX_addAnimation(&p_car10, 1000, PLEX_getGameTime());

	car10Down = PLEX_getAnimation(0, &p_car10);

	PLEX_addAnimationFrame(0, car10Down);
	PLEX_addAnimationFrame(1, car10Down);
	PLEX_addAnimationFrame(2, car10Down);
	PLEX_addAnimationFrame(3, car10Down);

	PLEX_addAnimation(&p_car10, 1000, PLEX_getGameTime());

	car10Up = PLEX_getAnimation(1, &p_car10);

	PLEX_addAnimationFrame(4, car10Up);
	PLEX_addAnimationFrame(5, car10Up);
	PLEX_addAnimationFrame(6, car10Up);
	PLEX_addAnimationFrame(7, car10Up);

	PLEX_addAnimation(&p_car10, 1000, PLEX_getGameTime());

	car10Crash = PLEX_getAnimation(2, &p_car10);

	PLEX_addAnimationFrame(8, car10Crash);
	PLEX_addAnimationFrame(9, car10Crash);
	PLEX_addAnimationFrame(10, car10Crash);
	PLEX_addAnimationFrame(11, car10Crash);

	PLEX_startSprite(time, &p_car10);

	// CAR 11

	p_car11Tex = PLEX_loadTextureFromFile(512, 192, 255, 255, 255, 255, 0, "./resources/chapter1/obstructions/phase4/car11.png");

	car11Plate = PLEX_createAnimationPlate(0.25, 0.33333333, p_car11Tex);

	p_car11 = PLEX_createSprite(&car11Plate);

	PLEX_addAnimation(&p_car11, 1000, PLEX_getGameTime());

	car11Down = PLEX_getAnimation(0, &p_car11);

	PLEX_addAnimationFrame(0, car11Down);
	PLEX_addAnimationFrame(1, car11Down);
	PLEX_addAnimationFrame(2, car11Down);
	PLEX_addAnimationFrame(3, car11Down);

	PLEX_addAnimation(&p_car11, 1000, PLEX_getGameTime());

	car11Up = PLEX_getAnimation(1, &p_car11);

	PLEX_addAnimationFrame(4, car11Up);
	PLEX_addAnimationFrame(5, car11Up);
	PLEX_addAnimationFrame(6, car11Up);
	PLEX_addAnimationFrame(7, car11Up);

	PLEX_addAnimation(&p_car11, 1000, PLEX_getGameTime());

	car11Crash = PLEX_getAnimation(2, &p_car11);

	PLEX_addAnimationFrame(8, car11Crash);
	PLEX_addAnimationFrame(9, car11Crash);
	PLEX_addAnimationFrame(10, car11Crash);
	PLEX_addAnimationFrame(11, car11Crash);

	PLEX_startSprite(time, &p_car11);

	// CAR 12

	p_car12Tex = PLEX_loadTextureFromFile(512, 192, 255, 255, 255, 255, 0, "./resources/chapter1/obstructions/phase4/car12.png");

	car12Plate = PLEX_createAnimationPlate(0.25, 0.33333333, p_car12Tex);

	p_car12 = PLEX_createSprite(&car12Plate);

	PLEX_addAnimation(&p_car12, 1000, PLEX_getGameTime());

	car12Down = PLEX_getAnimation(0, &p_car12);

	PLEX_addAnimationFrame(0, car12Down);
	PLEX_addAnimationFrame(1, car12Down);
	PLEX_addAnimationFrame(2, car12Down);
	PLEX_addAnimationFrame(3, car12Down);

	PLEX_addAnimation(&p_car12, 1000, PLEX_getGameTime());

	car12Up = PLEX_getAnimation(1, &p_car12);

	PLEX_addAnimationFrame(4, car12Up);
	PLEX_addAnimationFrame(5, car12Up);
	PLEX_addAnimationFrame(6, car12Up);
	PLEX_addAnimationFrame(7, car12Up);

	PLEX_addAnimation(&p_car12, 1000, PLEX_getGameTime());

	car12Crash = PLEX_getAnimation(2, &p_car12);

	PLEX_addAnimationFrame(8, car12Crash);
	PLEX_addAnimationFrame(9, car12Crash);
	PLEX_addAnimationFrame(10, car12Crash);
	PLEX_addAnimationFrame(11, car12Crash);

	PLEX_startSprite(time, &p_car12);

	// CAR 13

	p_car13Tex = PLEX_loadTextureFromFile(512, 192, 255, 255, 255, 255, 0, "./resources/chapter1/obstructions/phase4/car13.png");

	car13Plate = PLEX_createAnimationPlate(0.25, 0.33333333, p_car13Tex);

	p_car13 = PLEX_createSprite(&car13Plate);

	PLEX_addAnimation(&p_car13, 1000, PLEX_getGameTime());

	car13Down = PLEX_getAnimation(0, &p_car13);

	PLEX_addAnimationFrame(0, car13Down);
	PLEX_addAnimationFrame(1, car13Down);
	PLEX_addAnimationFrame(2, car13Down);
	PLEX_addAnimationFrame(3, car13Down);

	PLEX_addAnimation(&p_car13, 1000, PLEX_getGameTime());

	car13Up = PLEX_getAnimation(1, &p_car13);

	PLEX_addAnimationFrame(4, car13Up);
	PLEX_addAnimationFrame(5, car13Up);
	PLEX_addAnimationFrame(6, car13Up);
	PLEX_addAnimationFrame(7, car13Up);

	PLEX_addAnimation(&p_car13, 1000, PLEX_getGameTime());

	car13Crash = PLEX_getAnimation(2, &p_car13);

	PLEX_addAnimationFrame(8, car13Crash);
	PLEX_addAnimationFrame(9, car13Crash);
	PLEX_addAnimationFrame(10, car13Crash);
	PLEX_addAnimationFrame(11, car13Crash);

	PLEX_startSprite(time, &p_car13);

	// CAR 14

	p_car14Tex = PLEX_loadTextureFromFile(512, 192, 255, 255, 255, 255, 0, "./resources/chapter1/obstructions/phase4/car14.png");

	car14Plate = PLEX_createAnimationPlate(0.25, 0.33333333, p_car14Tex);

	p_car14 = PLEX_createSprite(&car14Plate);

	PLEX_addAnimation(&p_car14, 1000, PLEX_getGameTime());

	car14Down = PLEX_getAnimation(0, &p_car14);

	PLEX_addAnimationFrame(0, car14Down);
	PLEX_addAnimationFrame(1, car14Down);
	PLEX_addAnimationFrame(2, car14Down);
	PLEX_addAnimationFrame(3, car14Down);

	PLEX_addAnimation(&p_car14, 1000, PLEX_getGameTime());

	car14Up = PLEX_getAnimation(1, &p_car14);

	PLEX_addAnimationFrame(4, car14Up);
	PLEX_addAnimationFrame(5, car14Up);
	PLEX_addAnimationFrame(6, car14Up);
	PLEX_addAnimationFrame(7, car14Up);

	PLEX_addAnimation(&p_car14, 1000, PLEX_getGameTime());

	car14Crash = PLEX_getAnimation(2, &p_car14);

	PLEX_addAnimationFrame(8, car14Crash);
	PLEX_addAnimationFrame(9, car14Crash);
	PLEX_addAnimationFrame(10, car14Crash);
	PLEX_addAnimationFrame(11, car14Crash);

	PLEX_startSprite(time, &p_car14);

	// CAR 15

	p_car15Tex = PLEX_loadTextureFromFile(512, 192, 255, 255, 255, 255, 0, "./resources/chapter1/obstructions/phase4/car15.png");

	car15Plate = PLEX_createAnimationPlate(0.25, 0.33333333, p_car15Tex);

	p_car15 = PLEX_createSprite(&car15Plate);

	PLEX_addAnimation(&p_car15, 1000, PLEX_getGameTime());

	car15Down = PLEX_getAnimation(0, &p_car15);

	PLEX_addAnimationFrame(0, car15Down);
	PLEX_addAnimationFrame(1, car15Down);
	PLEX_addAnimationFrame(2, car15Down);
	PLEX_addAnimationFrame(3, car15Down);

	PLEX_addAnimation(&p_car15, 1000, PLEX_getGameTime());

	car15Up = PLEX_getAnimation(1, &p_car15);

	PLEX_addAnimationFrame(4, car15Up);
	PLEX_addAnimationFrame(5, car15Up);
	PLEX_addAnimationFrame(6, car15Up);
	PLEX_addAnimationFrame(7, car15Up);

	PLEX_addAnimation(&p_car15, 1000, PLEX_getGameTime());

	car15Crash = PLEX_getAnimation(2, &p_car15);

	PLEX_addAnimationFrame(8, car15Crash);
	PLEX_addAnimationFrame(9, car15Crash);
	PLEX_addAnimationFrame(10, car15Crash);
	PLEX_addAnimationFrame(11, car15Crash);

	PLEX_startSprite(time, &p_car15);

	// BIKE

	p_bikeTex = PLEX_loadTextureFromFile(256, 192, 255, 255, 255, 255, 0, "./resources/chapter1/obstructions/phase0/bike.png");

	bikePlate = PLEX_createAnimationPlate(0.25, 0.33333333, p_bikeTex);

	p_bike = PLEX_createSprite(&bikePlate);

	PLEX_addAnimation(&p_bike, 1000, PLEX_getGameTime());

	bikeDown = PLEX_getAnimation(0, &p_bike);

	PLEX_addAnimationFrame(0, bikeDown);
	PLEX_addAnimationFrame(1, bikeDown);
	PLEX_addAnimationFrame(2, bikeDown);
	PLEX_addAnimationFrame(3, bikeDown);

	PLEX_addAnimation(&p_bike, 1000, PLEX_getGameTime());

	bikeUp = PLEX_getAnimation(1, &p_bike);

	PLEX_addAnimationFrame(4, bikeUp);
	PLEX_addAnimationFrame(5, bikeUp);
	PLEX_addAnimationFrame(6, bikeUp);
	PLEX_addAnimationFrame(7, bikeUp);

	PLEX_addAnimation(&p_bike, 1000, PLEX_getGameTime());

	bikeCrash = PLEX_getAnimation(2, &p_bike);

	PLEX_addAnimationFrame(8, bikeCrash);
	PLEX_addAnimationFrame(9, bikeCrash);

	PLEX_startSprite(time, &p_bike);

	// BIKER

	p_bikerTex = PLEX_loadTextureFromFile(384, 192, 255, 255, 255, 255, 0, "./resources/chapter1/obstructions/phase1/biker.png");

	bikerPlate = PLEX_createAnimationPlate(0.33333333, 0.33333333, p_bikerTex);

	p_biker = PLEX_createSprite(&bikerPlate);

	PLEX_addAnimation(&p_biker, 1000, PLEX_getGameTime());

	bikerDown = PLEX_getAnimation(0, &p_biker);

	PLEX_addAnimationFrame(0, bikerDown);
	PLEX_addAnimationFrame(1, bikerDown);
	PLEX_addAnimationFrame(2, bikerDown);

	PLEX_addAnimation(&p_biker, 1000, PLEX_getGameTime());

	bikerUp = PLEX_getAnimation(1, &p_biker);

	PLEX_addAnimationFrame(3, bikerUp);
	PLEX_addAnimationFrame(4, bikerUp);
	PLEX_addAnimationFrame(5, bikerUp);

	PLEX_addAnimation(&p_biker, 1000, PLEX_getGameTime());

	bikerCrash = PLEX_getAnimation(2, &p_biker);

	PLEX_addAnimationFrame(7, bikerCrash);
	PLEX_addAnimationFrame(6, bikerCrash);

	PLEX_startSprite(time, &p_biker);

	// CACTUS

	p_cactusTex = PLEX_loadTextureFromFile(64, 192, 255, 255, 255, 255, 0, "./resources/chapter1/obstructions/phase1/cactus.png");

	cactusPlate = PLEX_createAnimationPlate(1, 0.33333333, p_cactusTex);

	p_cactus = PLEX_createSprite(&cactusPlate);

	PLEX_addAnimation(&p_cactus, 1000, PLEX_getGameTime());

	cactusDown = PLEX_getAnimation(0, &p_cactus);

	PLEX_addAnimationFrame(0, cactusDown);

	PLEX_addAnimation(&p_cactus, 1000, PLEX_getGameTime());

	cactusUp = PLEX_getAnimation(1, &p_cactus);

	PLEX_addAnimationFrame(1, cactusUp);

	PLEX_addAnimation(&p_cactus, 1000, PLEX_getGameTime());

	cactusCrash = PLEX_getAnimation(2, &p_cactus);

	PLEX_addAnimationFrame(2, cactusCrash);

	PLEX_startSprite(time, &p_cactus);

	// TUMBLEWEED

	p_tumbleweedTex = PLEX_loadTextureFromFile(256, 192, 255, 255, 255, 255, 0, "./resources/chapter1/obstructions/phase1/tumbleweed.png");

	tumbleweedPlate = PLEX_createAnimationPlate(0.25, 0.33333333, p_tumbleweedTex);

	p_tumbleweed = PLEX_createSprite(&tumbleweedPlate);

	PLEX_addAnimation(&p_tumbleweed, 1000, PLEX_getGameTime());

	tumbleweedDown = PLEX_getAnimation(0, &p_tumbleweed);

	PLEX_addAnimationFrame(0, tumbleweedDown);
	PLEX_addAnimationFrame(1, tumbleweedDown);
	PLEX_addAnimationFrame(2, tumbleweedDown);
	PLEX_addAnimationFrame(3, tumbleweedDown);

	PLEX_addAnimation(&p_tumbleweed, 1000, PLEX_getGameTime());

	tumbleweedUp = PLEX_getAnimation(1, &p_tumbleweed);

	PLEX_addAnimationFrame(4, tumbleweedUp);
	PLEX_addAnimationFrame(5, tumbleweedUp);
	PLEX_addAnimationFrame(6, tumbleweedUp);
	PLEX_addAnimationFrame(7, tumbleweedUp);

	PLEX_addAnimation(&p_tumbleweed, 1000, PLEX_getGameTime());

	tumbleweedCrash = PLEX_getAnimation(2, &p_tumbleweed);

	PLEX_addAnimationFrame(8, tumbleweedCrash);
	PLEX_addAnimationFrame(9, tumbleweedCrash);

	PLEX_startSprite(time, &p_tumbleweed);

	// BAG

	p_bagTex = PLEX_loadTextureFromFile(64, 192, 255, 255, 255, 255, 0, "./resources/chapter1/obstructions/phase3/bag.png");

	bagPlate = PLEX_createAnimationPlate(1, 0.33333333, p_bagTex);

	p_bag = PLEX_createSprite(&bagPlate);

	PLEX_addAnimation(&p_bag, 1000, PLEX_getGameTime());

	bagDown = PLEX_getAnimation(0, &p_bag);

	PLEX_addAnimationFrame(0, bagDown);

	PLEX_addAnimation(&p_bag, 1000, PLEX_getGameTime());

	bagUp = PLEX_getAnimation(1, &p_bag);

	PLEX_addAnimationFrame(1, bagUp);

	PLEX_addAnimation(&p_bag, 1000, PLEX_getGameTime());

	bagCrash = PLEX_getAnimation(2, &p_bag);

	PLEX_addAnimationFrame(2, bagCrash);

	PLEX_startSprite(time, &p_bag);

	// BAG2

	p_bag2Tex = PLEX_loadTextureFromFile(64, 192, 255, 255, 255, 255, 0, "./resources/chapter1/obstructions/phase3/bag2.png");

	bag2Plate = PLEX_createAnimationPlate(1, 0.33333333, p_bag2Tex);

	p_bag2 = PLEX_createSprite(&bag2Plate);

	PLEX_addAnimation(&p_bag2, 1000, PLEX_getGameTime());

	bag2Down = PLEX_getAnimation(0, &p_bag2);

	PLEX_addAnimationFrame(0, bag2Down);

	PLEX_addAnimation(&p_bag2, 1000, PLEX_getGameTime());

	bag2Up = PLEX_getAnimation(1, &p_bag2);

	PLEX_addAnimationFrame(1, bag2Up);

	PLEX_addAnimation(&p_bag2, 1000, PLEX_getGameTime());

	bag2Crash = PLEX_getAnimation(2, &p_bag2);

	PLEX_addAnimationFrame(2, bag2Crash);

	PLEX_startSprite(time, &p_bag2);

	// BARREL

	p_barrelTex = PLEX_loadTextureFromFile(64, 192, 255, 255, 255, 255, 0, "./resources/chapter1/obstructions/phase3/barrel.png");

	barrelPlate = PLEX_createAnimationPlate(1, 0.33333333, p_barrelTex);

	p_barrel = PLEX_createSprite(&barrelPlate);

	PLEX_addAnimation(&p_barrel, 1000, PLEX_getGameTime());

	barrelDown = PLEX_getAnimation(0, &p_barrel);

	PLEX_addAnimationFrame(0, barrelDown);

	PLEX_addAnimation(&p_barrel, 1000, PLEX_getGameTime());

	barrelUp = PLEX_getAnimation(1, &p_barrel);

	PLEX_addAnimationFrame(1, barrelUp);

	PLEX_addAnimation(&p_barrel, 1000, PLEX_getGameTime());

	barrelCrash = PLEX_getAnimation(2, &p_barrel);

	PLEX_addAnimationFrame(2, barrelCrash);

	PLEX_startSprite(time, &p_barrel);

	// BRICKS

	p_bricksTex = PLEX_loadTextureFromFile(64, 192, 255, 255, 255, 255, 0, "./resources/chapter1/obstructions/phase3/bricks.png");

	bricksPlate = PLEX_createAnimationPlate(1, 0.33333333, p_bricksTex);

	p_bricks = PLEX_createSprite(&bricksPlate);

	PLEX_addAnimation(&p_bricks, 1000, PLEX_getGameTime());

	bricksDown = PLEX_getAnimation(0, &p_bricks);

	PLEX_addAnimationFrame(0, bricksDown);

	PLEX_addAnimation(&p_bricks, 1000, PLEX_getGameTime());

	bricksUp = PLEX_getAnimation(1, &p_bricks);

	PLEX_addAnimationFrame(1, bricksUp);

	PLEX_addAnimation(&p_bricks, 1000, PLEX_getGameTime());

	bricksCrash = PLEX_getAnimation(2, &p_bricks);

	PLEX_addAnimationFrame(2, bricksCrash);

	PLEX_startSprite(time, &p_bricks);

	// CRATE

	p_crateTex = PLEX_loadTextureFromFile(64, 192, 255, 255, 255, 255, 0, "./resources/chapter1/obstructions/phase3/crate.png");

	cratePlate = PLEX_createAnimationPlate(1, 0.33333333, p_crateTex);

	p_crate = PLEX_createSprite(&cratePlate);

	PLEX_addAnimation(&p_crate, 1000, PLEX_getGameTime());

	crateDown = PLEX_getAnimation(0, &p_crate);

	PLEX_addAnimationFrame(0, crateDown);

	PLEX_addAnimation(&p_crate, 1000, PLEX_getGameTime());

	crateUp = PLEX_getAnimation(1, &p_crate);

	PLEX_addAnimationFrame(1, crateUp);

	PLEX_addAnimation(&p_crate, 1000, PLEX_getGameTime());

	crateCrash = PLEX_getAnimation(2, &p_crate);

	PLEX_addAnimationFrame(2, crateCrash);

	PLEX_startSprite(time, &p_crate);

	// PIPES

	p_pipesTex = PLEX_loadTextureFromFile(64, 192, 255, 255, 255, 255, 0, "./resources/chapter1/obstructions/phase3/pipes.png");

	pipesPlate = PLEX_createAnimationPlate(1, 0.33333333, p_pipesTex);

	p_pipes = PLEX_createSprite(&pipesPlate);

	PLEX_addAnimation(&p_pipes, 1000, PLEX_getGameTime());

	pipesDown = PLEX_getAnimation(0, &p_pipes);

	PLEX_addAnimationFrame(0, pipesDown);

	PLEX_addAnimation(&p_pipes, 1000, PLEX_getGameTime());

	pipesUp = PLEX_getAnimation(1, &p_pipes);

	PLEX_addAnimationFrame(1, pipesUp);

	PLEX_addAnimation(&p_pipes, 1000, PLEX_getGameTime());

	pipesCrash = PLEX_getAnimation(2, &p_pipes);

	PLEX_addAnimationFrame(2, pipesCrash);

	PLEX_startSprite(time, &p_pipes);

	// PAUSE ANIM

	p_pauseTex = PLEX_loadTextureFromFile(1000, 2000, 255, 255, 255, 255, 0, "./resources/chapter1/paused.png");

	pausePlate = PLEX_createAnimationPlate(1, 0.25, p_pauseTex);

	p_pauseAnim = PLEX_createSprite(&pausePlate);

	PLEX_addAnimation(&p_pauseAnim, 1000, time);

	pauseAnim = PLEX_getAnimation(0, &p_pauseAnim);

	PLEX_addAnimationFrame(0, pauseAnim);
	PLEX_addAnimationFrame(1, pauseAnim);
	PLEX_addAnimationFrame(2, pauseAnim);
	PLEX_addAnimationFrame(3, pauseAnim);
	PLEX_addAnimationFrame(2, pauseAnim);
	PLEX_addAnimationFrame(1, pauseAnim);

	PLEX_startSprite(PLEX_getNoPauseTime(), &p_pauseAnim);

	// ------------------------------------------------------------------------------------------------------------

	// NON-ANIMATION STUFF

	p_playerRegion = PLEX_convRect(PLEX_convPoint(PLEX_WIDTHUNITMAX()/2 - 0.25 + 1.5, PLEX_HEIGHTUNITMAX() - 2), PLEX_convDimens(0.75, 1.5));

	p_dist = 0;
	p_accel = 50;
	p_accelMod = 1;

	p_outcome = 1; // < 0 is too early, 1 is just right, 2 is too late, 3 is reset

	p_backHealth = 1;
	p_carSeatHealth = 1;
	p_windshieldHealth = 1;
	p_carWingLeftHealth = 1;
	p_carWingRightHealth = 1;

	p_crashDuration = 0;

	p_lastTime = 0;
	p_obstructionCount = 0;
	p_accumSeconds = 0;

	p_glassShatterLevel = 0;

	p_pamEngineRevSE = -1;
	p_pamEngineRunSE = -1;
	p_pamEngineStruggleSE = -1;

	p_tutorial = PLEX_loadTextureFromFile(1827, 1370, 255, 255, 255, 255, 0, "./resources/chapter1/tutorial.png");

	while(!PLEX_leftClick() && !PLEX_isTerminated() && !PLEX_CONTROLLER_down(0, SDL_CONTROLLER_BUTTON_A))
	{
		PLEX_printTexture(MMS1_MENULAYER, p_tutorial, 0, 0, 1827, 1370, 0, 0, PLEX_WW(true), PLEX_WH(true), true);

		PLEX_updatePlex();

		if(PLEX_askingToQuit()) PLEX_terminate();
	}

	p_start = time;

	for(size_t ze = 0; ze < 4; ++ze)
	{
		p_objGenDuration[ze] = 0;
		p_objGenMarker[ze] = time - p_start; // < real time
	}

	MM_COMMON_playTheme((size_t)MM_COMMON_THEMEID_PANIC);

	if(PLEX_pauseDisabled())
	{
		PLEX_enablePause();
		p_enablePause = true;
	}

	return true;
}

bool MMS_updateMinigame1(const size_t fontPlate, const uint64_t time)
{
	const uint64_t realTime = time - p_start;

	const bool pressReset = !PLEX_KEY_ctrl() && PLEX_KEY_fdown(1);

	const bool pressAccel = PLEX_KEY_down('w') || PLEX_CONTROLLER_down(0, SDL_CONTROLLER_BUTTON_A);
	const bool pressBrake = PLEX_KEY_down('s') || PLEX_CONTROLLER_down(0, SDL_CONTROLLER_BUTTON_B);
	const bool pressDriftLeft = !pressBrake && (PLEX_KEY_down('a') || PLEX_CONTROLLER_axis(0, 0) < -1000 || PLEX_CONTROLLER_down(0, SDL_CONTROLLER_BUTTON_DPAD_LEFT));
	const bool pressDriftRight = !pressBrake && (PLEX_KEY_down('d') || PLEX_CONTROLLER_axis(0, 0) > 1000 || PLEX_CONTROLLER_down(0, SDL_CONTROLLER_BUTTON_DPAD_RIGHT));

	const double distLeft = p_fullDist - p_dist;
	const double secondsToDest = p_accel && p_accelMod ? distLeft/(p_accel * p_accelMod) : distLeft;
	const double ETA = p_accumSeconds + secondsToDest;

	const double distLeftPerc = distLeft/(double)p_fullDist;
	const double completePerc = 1 - distLeftPerc;

	const double minETA = p_accumSeconds + distLeft/200.0;
	const double maxETA = p_accumSeconds + distLeft/20.0;

	const double skylineY = PLEX_WH(true) * 0.75 - PLEX_WH(true) * 0.75 * completePerc;
	const uint8_t skylineTint = 255 * completePerc;

	const size_t phase = completePerc/0.2;
	const double phasePerc = (completePerc - (phase * 0.2))/0.2;

	size_t crashCycle = realTime - p_crashMarker;

	double lanePerc = (p_playerRegion.origin.x + p_playerRegion.dimens.w/2)/(double)PLEX_WIDTHUNITMAX();
	size_t lane = 0;

	double secondsPass = 0;

	size_t backing = 0;
	size_t pamAniIndex = 0;

	size_t phaseBack = 0;
	size_t nextPhaseBack = 0;

	int8_t speedRank = 0;

	PLEX_SPRITE road;
	PLEX_SPRITE nextRoad;

	size_t backAniIndex = p_backHealth ? (1 - p_backHealth)/0.25 : 3;
	size_t carSeatAniIndex = p_carSeatHealth ? (1 - p_carSeatHealth)/0.25 : 3;
	size_t windshieldAniIndex = p_windshieldHealth ? (1 - p_windshieldHealth)/0.25 : 3;
	size_t carWingLeftAniIndex = p_carWingLeftHealth ? (1 - p_carWingLeftHealth)/0.25 : 3;
	size_t carWingRightAniIndex = p_carWingRightHealth ? (1 - p_carWingRightHealth)/0.25 : 3;

	double damageDrift = 0;

	uint32_t roadCycleSpeed = 0;
	uint32_t road4CycleSpeed = 0;

	bool accelerate = false;

	if(pressReset)
	{
		p_outcome = 3; // outcome 3 == reset
		return false;
	}

	if(!p_lastTime) p_lastTime = realTime;

	secondsPass = (realTime - p_lastTime)/1000.0;

	damageDrift = (1 - p_carSeatHealth) * secondsPass;

	if(lanePerc < 0.2) lane = 0;
	else if(lanePerc < 0.5) lane = 1;
	else if(lanePerc < 0.8) lane = 2;
	else lane = 3;

	if(realTime - p_crashMarker > p_crashDuration)
	{
		PLEX_startSprite(realTime, &p_carBack);
		PLEX_startSprite(realTime, &p_carSeat);
		PLEX_startSprite(realTime, &p_carWindshield);
		PLEX_startSprite(realTime, &p_carWingLeft);
		PLEX_startSprite(realTime, &p_carWingRight);

		p_crashDuration = 0;
	}

	p_engineStruggle = false;

	if(!PLEX_getGameTimePaused())
	{
		if(!p_crashDuration)
		{
			if(pressAccel)
			{
				p_accel += 0.25;

				pamAniIndex = 5;

				if(p_pamEngineRevSE == -1) p_pamEngineRevSE = MM_COMMON_loopSoundEffect(MM_COMMON_SOUNDEFFECT_PAMENGINEREV);

				accelerate = true;
			}else{
				if(pressBrake)
				{
					backing = 4;
					pamAniIndex = 4;

					p_accel -= 0.25;

					if(p_accel < 0) p_accel = 0;
				}else{
					if(p_accel < 25) p_accel += 0.25;
					else if(p_accel < 50) p_accel += 0.1;
					else if(p_accel < 100) p_accel -= 0.1;
					else p_accel -= 0.1;
				}
			}

			if(p_accel > 200) p_accel = 200;

			if(pressDriftLeft)
			{
				p_playerRegion.origin.x -= 0.1 + (p_carWingLeftHealth * 0.05);

				pamAniIndex = 1;

				if(p_playerRegion.origin.x < 1) p_playerRegion.origin.x = 1;
			}else{
				if(pressDriftRight)
				{
					p_playerRegion.origin.x += 0.1 + (p_carWingRightHealth * 0.05);

					pamAniIndex = 2;

					if(p_playerRegion.origin.x > PLEX_WIDTHUNITMAX() - 1) p_playerRegion.origin.x = PLEX_WIDTHUNITMAX() - 1;
				}
			}

			p_playerRegion.origin.x -= damageDrift;

			if(p_accel <= 0) pamAniIndex = 0;
			else if(!pamAniIndex)
			{
				if(p_accel <= 100) pamAniIndex = 3;
				else pamAniIndex = 5;
			}

			PLEX_changeSpriteCycleSpeed(pamAniIndex, p_calcAnimationCycle(p_pamaleanaAnimationFastestCycle, p_pamaleanaAnimationSlowestCycle), realTime, &p_pamaleanaSprite);
		}else{
			if(pressAccel || pressDriftLeft || pressDriftRight) p_engineStruggle = true;

			if(crashCycle < PLEX_spriteCycleLen(6, p_pamaleanaSprite)) pamAniIndex = 6;
			else PLEX_stopSprite(&p_pamaleanaSprite);

			PLEX_stopSprite(&p_carBack);
			PLEX_stopSprite(&p_carSeat);
			PLEX_stopSprite(&p_carWindshield);
			PLEX_stopSprite(&p_carWingLeft);
			PLEX_stopSprite(&p_carWingRight);
		}
	}else{
		PLEX_drawSprite(0, 0, PLEX_getNoPauseTime(), PLEX_UNIT(2), PLEX_WH(true)/2 - PLEX_WH(true)/8, PLEX_WW(true) - PLEX_UNIT(4), PLEX_WH(true)/4, p_pauseAnim, true);
	}

	if(!accelerate && p_pamEngineRevSE != -1)
	{
		MM_COMMON_stopSoundEffect(p_pamEngineRevSE);
		p_pamEngineRevSE = -1;
	}

	if(p_accel)
	{
		if(p_pamEngineRunSE == -1) p_pamEngineRunSE = MM_COMMON_loopSoundEffect(MM_COMMON_SOUNDEFFECT_PAMENGINERUN);
	}else{
		if(p_pamEngineRunSE != -1)
		{
			MM_COMMON_stopSoundEffect(p_pamEngineRunSE);
			p_pamEngineRunSE = -1;
		}
	}

	MM_COMMON_setSoundEffectVolume(p_accel/200.0, p_pamEngineRunSE);

	if(p_playerRegion.origin.x + p_playerRegion.dimens.w/2 - p_carDrawArea.dimens.w/2 < 0) p_playerRegion.origin.x = p_carDrawArea.dimens.w/2 - p_playerRegion.dimens.w/2;
	if(p_playerRegion.origin.x + p_playerRegion.dimens.w/2 + p_carDrawArea.dimens.w/2 > PLEX_WIDTHUNITMAX()) p_playerRegion.origin.x = PLEX_WIDTHUNITMAX() - p_carDrawArea.dimens.w/2 - p_playerRegion.dimens.w/2;

	p_updateAreas();

	// CAR SPRITES

	PLEX_changeSpriteCycleSpeed(carWingLeftAniIndex + backing, p_calcAnimationCycle(p_carWingAnimationFastestCycle, p_carWingAnimationSlowestCycle), realTime, &p_carWingLeft);
	PLEX_changeSpriteCycleSpeed(carWingRightAniIndex + backing, p_calcAnimationCycle(p_carWingAnimationFastestCycle, p_carWingAnimationSlowestCycle), realTime, &p_carWingRight);
	PLEX_changeSpriteCycleSpeed(backAniIndex + backing, p_calcAnimationCycle(p_carBackAnimationFastestCycle, p_carBackAnimationSlowestCycle), realTime, &p_carBack);

	PLEX_drawSprite(p_overroadLayer(p_carDrawArea.origin.y, p_carDrawArea.dimens.h, 0), carWingLeftAniIndex + backing, realTime, PLEX_UNIT(p_carDrawArea.origin.x), PLEX_UNIT(p_carDrawArea.origin.y), PLEX_UNIT(p_carDrawArea.dimens.w), PLEX_UNIT(p_carDrawArea.dimens.h), p_carWingLeft, true);

	PLEX_drawSprite(p_overroadLayer(p_carDrawArea.origin.y, p_carDrawArea.dimens.h, 0), carWingRightAniIndex + backing, realTime, PLEX_UNIT(p_carDrawArea.origin.x), PLEX_UNIT(p_carDrawArea.origin.y), PLEX_UNIT(p_carDrawArea.dimens.w), PLEX_UNIT(p_carDrawArea.dimens.h), p_carWingRight, true);

	PLEX_drawSprite(p_overroadLayer(p_carDrawArea.origin.y, p_carDrawArea.dimens.h, 1), backAniIndex + backing, realTime, PLEX_UNIT(p_carDrawArea.origin.x), PLEX_UNIT(p_carDrawArea.origin.y), PLEX_UNIT(p_carDrawArea.dimens.w), PLEX_UNIT(p_carDrawArea.dimens.h), p_carBack, true);

	// PAM-PAM sprite

	PLEX_drawSprite(p_overroadLayer(p_carDrawArea.origin.y, p_carDrawArea.dimens.h, 2), pamAniIndex, realTime, PLEX_UNIT(p_playerRegion.origin.x), PLEX_UNIT(p_playerRegion.origin.y), PLEX_UNIT(p_playerRegion.dimens.w), PLEX_UNIT(p_playerRegion.dimens.h), p_pamaleanaSprite, true);

	// MORE CAR SPRITES

	PLEX_drawSprite(p_overroadLayer(p_carDrawArea.origin.y, p_carDrawArea.dimens.h, 3), carSeatAniIndex, realTime, PLEX_UNIT(p_carDrawArea.origin.x), PLEX_UNIT(p_carDrawArea.origin.y), PLEX_UNIT(p_carDrawArea.dimens.w), PLEX_UNIT(p_carDrawArea.dimens.h), p_carSeat, true);

	PLEX_drawSprite(p_overroadLayer(p_carDrawArea.origin.y, p_carDrawArea.dimens.h, 6), windshieldAniIndex, realTime, PLEX_UNIT(p_carDrawArea.origin.x), PLEX_UNIT(p_carDrawArea.origin.y), PLEX_UNIT(p_carDrawArea.dimens.w), PLEX_UNIT(p_carDrawArea.dimens.h), p_carWindshield, true);

	// THE DOME sprite

	PLEX_changeSpriteCycleSpeed(0, p_calcAnimationCycle(p_domeAnimationFastestCycle, p_domeAnimationSlowestCycle), realTime, &p_domeParallax);
	PLEX_changeSpriteCycleSpeed(0, p_calcAnimationCycle(p_domeAnimationFastestCycle, p_domeAnimationSlowestCycle), realTime, &p_domeParallax2);

	PLEX_drawSprite(MMS1_ENDLAYER - 2, 0, realTime, 0, 0, PLEX_WW(true), PLEX_WH(true)/4.5, p_domeParallax, true);
	PLEX_drawSprite(MMS1_ENDLAYER - 2, 0, realTime, 0, PLEX_WH(true)/4.5, PLEX_WW(true), PLEX_WH(true) * 0.75 - PLEX_WH(true)/4.5, p_domeParallax2, true);

	// skyline

	PLEX_printTextureTinted(MMS1_ENDLAYER - 3, p_skyline, 0, 0, 800, 500, 0, skylineY, PLEX_WW(true), PLEX_WH(true) * 0.75, skylineTint, skylineTint, skylineTint, true);

	// THE WALL

	roadCycleSpeed = p_calcAnimationCycle(p_roadAnimationFastestCycle, p_roadAnimationSlowestCycle);
	road4CycleSpeed = p_calcAnimationCycle(p_road4AnimationFastestCycle, p_road4AnimationSlowestCycle);

	if(phase != 4) PLEX_changeSpriteCycleSpeed(0, roadCycleSpeed, realTime, &road);
	else PLEX_changeSpriteCycleSpeed(0, road4CycleSpeed, realTime, &road);

	if(phase != 3) PLEX_changeSpriteCycleSpeed(0, roadCycleSpeed, realTime, &nextRoad);
	else PLEX_changeSpriteCycleSpeed(0, road4CycleSpeed, realTime, &nextRoad);

	PLEX_syncSprites(0, p_road3, &p_wall3);
	PLEX_syncSprites(0, p_road4, &p_wall4);

	if(phase == 3 || (phase == 2 && phasePerc > 0.8))
	{
		PLEX_drawSprite(MMS1_ENDLAYER - 5, 0, realTime, 0, PLEX_WH(true)/2, PLEX_WW(true), PLEX_WH(true)/4, p_wall3, true);
	}

	if(phase == 4 || (phase == 3 && phasePerc > 0.8))
	{
		const double hperc = phase == 3 ? (phasePerc - 0.8)/0.2 : 1;

		const uint32_t wallheight = hperc * PLEX_WH(true);

		PLEX_drawSprite(MMS1_ENDLAYER - 6, 0, realTime, 0, PLEX_WH(true) * 0.75 - wallheight, PLEX_WW(true), wallheight, p_wall4, true);
	}

	// THE ROAD sprite

	switch(phase)
	{
		case 0: road = p_road0; nextRoad = p_road1; break;
		case 1: road = p_road1; nextRoad = p_road2; break;
		case 2: road = p_road2; nextRoad = p_road3; break;
		case 3: road = p_road3; nextRoad = p_road4; break;
		default: road = p_road4; break;
	};

	if(phasePerc > 0.8 && phase != 4)
	{
		const double overlapPerc = (phasePerc - 0.8)/0.2;

		PLEX_drawSprite(MMS1_ENDLAYER - 5, 0, realTime, 0, PLEX_WH(true) * 0.75, PLEX_WW(true), PLEX_WH(true)/4 * overlapPerc, nextRoad, true);
	}

	PLEX_drawSprite(MMS1_ENDLAYER - 4, 0, realTime, 0, PLEX_WH(true) * 0.75, PLEX_WW(true), PLEX_WH(true)/4, road, true);

	// BACKDROP

	switch(phase)
	{
		case 0: phaseBack = p_backdrop0; nextPhaseBack = p_backdrop1; break;
		case 1: phaseBack = p_backdrop1; nextPhaseBack = p_backdrop2; break;
		case 2: phaseBack = p_backdrop2; nextPhaseBack = p_backdrop3; break;
		case 3: phaseBack = p_backdrop3; nextPhaseBack = p_backdrop4; break;
		case 4: phaseBack = p_backdrop4; break;
	}

	// WINDSHIELD EFFECT

	if(windshieldAniIndex > 0)
	{
		const size_t index = windshieldAniIndex - 1;

		PLEX_printTexture(MMS1_MENULAYER + 1, p_windshieldBreak, 0, 1080 * index, 1920, 1080, 0, 0, PLEX_WW(true), PLEX_WH(true), true);

		if(windshieldAniIndex > p_glassShatterLevel)
		{
			MM_COMMON_playSoundEffect(MM_COMMON_SOUNDEFFECT_SURREALGLASSCRACK);
			p_glassShatterLevel = windshieldAniIndex;
		}
	}

	// STOP ANIMATION OF NO ACCEL

	PLEX_setTextureAlpha(phaseBack, 255);
	PLEX_printTexture(MMS1_ENDLAYER, phaseBack, 0, 0, 3840, 1644, 0, 0, PLEX_WW(true), PLEX_WH(true) * 0.75, true);

	if(phase != 4)
	{
		PLEX_setTextureAlpha(nextPhaseBack, 255 * phasePerc);
		PLEX_printTexture(MMS1_ENDLAYER, nextPhaseBack, 0, 0, 3840, 1644, 0, 0, PLEX_WW(true), PLEX_WH(true) * 0.75, true);
	}

	speedRank = p_drawSpeedBar(secondsPass, ETA, minETA, maxETA);

	if((realTime % 666) < 333)
	{
		if(lane < 2)
		{
			PLEX_drawTextBoxTinted(MMS1_MENULAYER, MMS1_LANGUAGE_MINIGAME_WRONGLANEWARNING, PLEX_STANDARD_FW, PLEX_STANDARD_FH, 255, 255, 50, 255, 0, 0, 0, 255, PLEX_UNIT(1), PLEX_UNIT(3), PLEX_UNIT(7), PLEX_UNIT(1), PLEX_UNIT(0.5), PLEX_UNIT(1), 0.125, 0.125, 1, 1, PLEX_UNIT(0.05), PLEX_LANGUAGE_ALPHASTRING, fontPlate, 0, 0, 0, 0, 0, 0, true);
		}

		if(speedRank == -1)
		{
			PLEX_drawTextBoxTinted(MMS1_MENULAYER, MMS1_LANGUAGE_MINIGAME_TOOSLOWWARNING, PLEX_STANDARD_FW, PLEX_STANDARD_FH, 200, 50, 50, 255, 0, 0, 0, 255, PLEX_WW(true) - PLEX_UNIT(8), PLEX_UNIT(3), PLEX_UNIT(7), PLEX_UNIT(1), PLEX_UNIT(0.5), PLEX_UNIT(1), 0.125, 0.125, 1, 1, PLEX_UNIT(0.05), PLEX_LANGUAGE_ALPHASTRING, fontPlate, 0, 0, 0, 0, 0, 0, true);
		}

		if(speedRank == 1)
		{
			PLEX_drawTextBoxTinted(MMS1_MENULAYER, MMS1_LANGUAGE_MINIGAME_TOOFASTWARNING, PLEX_STANDARD_FW, PLEX_STANDARD_FH, 200, 50, 50, 255, 0, 0, 0, 255, PLEX_WW(true) - PLEX_UNIT(8), PLEX_UNIT(3), PLEX_UNIT(7), PLEX_UNIT(1), PLEX_UNIT(0.5), PLEX_UNIT(1), 0.125, 0.125, 1, 1, PLEX_UNIT(0.05), PLEX_LANGUAGE_ALPHASTRING, fontPlate, 0, 0, 0, 0, 0, 0, true);
		}

		if(p_engineStruggle)
		{
			PLEX_drawTextBoxTinted(MMS1_MENULAYER, MMS1_LANGUAGE_MINIGAME_ENGINEREV, PLEX_STANDARD_FW, PLEX_STANDARD_FH, 0, 0, 0, 100, 100, 100, 100, 100, PLEX_UNIT(1), PLEX_UNIT(5), PLEX_WW(true) - PLEX_UNIT(1), PLEX_UNIT(1), PLEX_UNIT(0.5), PLEX_UNIT(1), 0.125, 0.125, 1, 1, PLEX_UNIT(0.05), PLEX_LANGUAGE_ALPHASTRING, fontPlate, 0, 0, 0, 0, 0, 0, true);

			if(p_pamEngineStruggleSE == -1) p_pamEngineStruggleSE = MM_COMMON_loopSoundEffect(MM_COMMON_SOUNDEFFECT_PAMENGINESTRUGGLE);
		}

		if(p_warnPlayer)
		{
			PLEX_drawTextBoxTinted(MMS1_MENULAYER, MMS1_LANGUAGE_MINIGAME_ONCOMINGTRAFFIC, PLEX_STANDARD_FW, PLEX_STANDARD_FH, 200, 50, 50, 255, 0, 0, 0, 255, PLEX_WW(true)/2 - PLEX_UNIT(4), PLEX_UNIT(3), PLEX_UNIT(7), PLEX_UNIT(1), PLEX_UNIT(0.5), PLEX_UNIT(1), 0.125, 0.125, 1, 1, PLEX_UNIT(0.05), PLEX_LANGUAGE_ALPHASTRING, fontPlate, 0, 0, 0, 0, 0, 0, true);

			if(!p_honkHonk)
			{
				MM_COMMON_playSoundEffect(MM_COMMON_SOUNDEFFECT_SURREALHONKHONK);
				p_honkHonk = true;
			}
		}
	}else p_honkHonk = false;

	if(!p_engineStruggle && p_pamEngineStruggleSE)
	{
		MM_COMMON_stopSoundEffect(p_pamEngineStruggleSE);
		p_pamEngineStruggleSE = -1;
	}

	p_accelMod = (p_backHealth * 0.5) + 0.5;
	p_dist += p_accel * p_accelMod * secondsPass;

	// STOP ANIMATION OF CAR STOPS

	if(p_accel)
	{
		if(PLEX_isSpriteStopped(p_road0)) PLEX_startSprite(realTime, &p_road0);
		if(PLEX_isSpriteStopped(p_road1)) PLEX_startSprite(realTime, &p_road1);
		if(PLEX_isSpriteStopped(p_road2)) PLEX_startSprite(realTime, &p_road2);
		if(PLEX_isSpriteStopped(p_road3)) PLEX_startSprite(realTime, &p_road3);
		if(PLEX_isSpriteStopped(p_road4)) PLEX_startSprite(realTime, &p_road4);
		if(PLEX_isSpriteStopped(p_domeParallax)) PLEX_startSprite(realTime, &p_domeParallax);
		if(PLEX_isSpriteStopped(p_domeParallax2)) PLEX_startSprite(realTime, &p_domeParallax2);
		if(PLEX_isSpriteStopped(p_wall3)) PLEX_startSprite(realTime, &p_wall3);
		if(PLEX_isSpriteStopped(p_wall4)) PLEX_startSprite(realTime, &p_wall4);
	}else{
		PLEX_stopSprite(&p_road0);
		PLEX_stopSprite(&p_road1);
		PLEX_stopSprite(&p_road2);
		PLEX_stopSprite(&p_road3);
		PLEX_stopSprite(&p_road4);
		PLEX_stopSprite(&p_domeParallax);
		PLEX_stopSprite(&p_domeParallax2);
		PLEX_stopSprite(&p_wall3);
		PLEX_stopSprite(&p_wall4);
	}

	// UPDATE OBSTRUCTIONS

	p_checkObstructions(phase, lane, realTime);

	for(size_t ze = 0; ze < p_obstructionCount; ++ze) p_updateObstruction(ze, p_obstructions + ze, realTime, secondsPass);

	// OUTCOME

	p_outcome = 1;

	if(ETA > p_unrealisticallyLate) p_outcome = 0;
	if(ETA < p_unrealisticallyEarly) p_outcome = 2;

	if(p_dist >= p_fullDist || minETA > p_unrealisticallyLate) return false;

	p_lastTime = realTime;

	p_accumSeconds += secondsPass;

	MM_COMMON_updateTheme();

	return true;
}

void MMS_endMinigame1()
{
	if(p_enablePause)
	{
		PLEX_disablePause();
		p_enablePause = false;
	}

	PLEX_destroyTexture(p_domeParallaxTex);
	PLEX_destroyTexture(p_domeParallaxTex2);
	PLEX_destroyTexture(p_skyline);

	PLEX_destroyTexture(p_windshieldBreak);

	PLEX_destroyTexture(p_backdrop0);
	PLEX_destroyTexture(p_backdrop1);
	PLEX_destroyTexture(p_backdrop2);
	PLEX_destroyTexture(p_backdrop3);
	PLEX_destroyTexture(p_backdrop4);

	PLEX_destroyTexture(p_road0Tex);
	PLEX_destroyTexture(p_road1Tex);

	PLEX_destroyTexture(p_wall3Tex);
	PLEX_destroyTexture(p_wall4Tex);

	PLEX_destroyTexture(p_car0Tex);
	PLEX_destroyTexture(p_car1Tex);
	PLEX_destroyTexture(p_car2Tex);
	PLEX_destroyTexture(p_car3Tex);
	PLEX_destroyTexture(p_car4Tex);
	PLEX_destroyTexture(p_car5Tex);
	PLEX_destroyTexture(p_car6Tex);
	PLEX_destroyTexture(p_car7Tex);
	PLEX_destroyTexture(p_car8Tex);
	PLEX_destroyTexture(p_car9Tex);
	PLEX_destroyTexture(p_car10Tex);
	PLEX_destroyTexture(p_car11Tex);
	PLEX_destroyTexture(p_car12Tex);
	PLEX_destroyTexture(p_car13Tex);
	PLEX_destroyTexture(p_car14Tex);
	PLEX_destroyTexture(p_car15Tex);

	PLEX_destroyTexture(p_bikeTex);
	PLEX_destroyTexture(p_bikerTex);

	PLEX_destroyTexture(p_cactusTex);
	PLEX_destroyTexture(p_tumbleweedTex);
	PLEX_destroyTexture(p_bagTex);
	PLEX_destroyTexture(p_bag2Tex);
	PLEX_destroyTexture(p_barrelTex);
	PLEX_destroyTexture(p_bricksTex);
	PLEX_destroyTexture(p_crateTex);
	PLEX_destroyTexture(p_pipesTex);

	PLEX_destroyTexture(p_tutorial);
	PLEX_destroyTexture(p_pauseTex);

	p_domeParallaxTex = 0;
	p_domeParallaxTex2 = 0;

	p_skyline = 0;

	p_windshieldBreak = 0;

	p_backdrop0 = 0;
	p_backdrop1 = 0;
	p_backdrop2 = 0;
	p_backdrop3 = 0;
	p_backdrop4 = 0;

	p_road0Tex = 0;
	p_road1Tex = 0;

	p_wall3Tex = 0;
	p_wall4Tex = 0;

	p_car0Tex = 0;
	p_car1Tex = 0;
	p_car2Tex = 0;
	p_car3Tex = 0;
	p_car4Tex = 0;
	p_car5Tex = 0;
	p_car6Tex = 0;
	p_car7Tex = 0;
	p_car8Tex = 0;
	p_car9Tex = 0;
	p_car10Tex = 0;
	p_car11Tex = 0;
	p_car12Tex = 0;
	p_car13Tex = 0;
	p_car14Tex = 0;
	p_car15Tex = 0;

	p_bikeTex = 0;
	p_bikerTex = 0;

	p_cactusTex = 0;
	p_tumbleweedTex = 0;
	p_bagTex = 0;
	p_bag2Tex = 0;
	p_barrelTex = 0;
	p_bricksTex = 0;
	p_crateTex = 0;
	p_pipesTex = 0;

	p_tutorial = 0;
	p_pauseTex = 0;

	MM_COMMON_playTheme(MM_COMMON_THEMEID_NONE);

	MM_COMMON_stopSoundEffect(p_pamEngineRevSE);
	MM_COMMON_stopSoundEffect(p_pamEngineRunSE);
	MM_COMMON_stopSoundEffect(p_pamEngineStruggleSE);

	p_pamEngineRevSE = -1;
	p_pamEngineRunSE = -1;
	p_pamEngineStruggleSE = -1;
}

bool MMS_isMinigame1Running(){ return p_running; }

void MMS_runMinigame1(const size_t chapter1SpriteTex, const size_t pamaleanaSpriteTex, const size_t cubicleBackdrop, const size_t venariusSpriteTex, const size_t extraplocanSpriteTex, const size_t fontPlate, const size_t label, const size_t bubble, const size_t arrow)
{
	uint64_t time = PLEX_getGameTime();

	p_running = false;

	while(!PLEX_isTerminated())
	{
		MMS_beginMinigame1(chapter1SpriteTex, pamaleanaSpriteTex, venariusSpriteTex, extraplocanSpriteTex, time);

		while(MMS_updateMinigame1(fontPlate, time) && !PLEX_isTerminated())
		{
			PLEX_updatePlex();
			if(PLEX_askingToQuit()) PLEX_terminate();
			time = PLEX_getGameTime();
		}

		MMS_endMinigame1();

		if(PLEX_isTerminated()) return;

		switch(p_outcome)
		{
			case 0: p_tooSlowCut(pamaleanaSpriteTex, cubicleBackdrop, venariusSpriteTex, extraplocanSpriteTex, fontPlate, label, bubble, arrow); break;
			case 1: return;
			case 2: p_tooFastCut(pamaleanaSpriteTex, cubicleBackdrop, venariusSpriteTex, extraplocanSpriteTex, fontPlate, label, bubble, arrow); break;
			default: break;
		}
	}
}
