#ifndef PLEX_MATH_H
#define PLEX_MATH_H

#include "utility/error.h"

// This is a unit that contains basic reusable math functions.

// NOTE: Some of the functions in this are largely or partially unused. Some of them are incomplete. Eventually PLEX will host 3 types of collision detection: A) rectangular (pretty much in use), circular (all the basic elements but incomplete), and polygonal (incomplete/works in a limited way). The latter 2 are not particularly polished at this point but the basics for their operation are in here. This is one unit I'll be adding as much as I can do but not all of it will be immediately practical or extensively tested. With that said, it's a very safe set of functions. Barely uses pointers, has no memory allocation, etc. However, it is essential for PLEX to kind of work. Also, there might be a more refined, easy access, universalized 'collision' unit built on it someday.

#include "meta/cinclude.h"

// insures that pi exists for equations that need them

#ifdef M_PI
	#define p_PI M_PI
#else
	#define p_PI 3.1415926
#endif

// indicates the precision type for math. This should either be a double or a float. There may eventually be a default to chose the precision between. In most cases a double should be fine but on older systems changing to a float might give a bit of a performance boost.

#ifndef PLEX_MPREC_TYPE
	#define PLEX_MPREC_TYPE double
#endif

typedef PLEX_MPREC_TYPE PLEX_MPREC;

// a point is a coordinate, dimens are usually the dimensions of rectangles

typedef struct
{
	PLEX_MPREC x, y;
} PLEX_POINT;

typedef struct
{
	PLEX_MPREC w, h;
} PLEX_DIMENS;

// a struct to hold information for a line. This is not used as extensively in PLEX as I woud like.

typedef struct
{
	PLEX_POINT origin, intercept;
	PLEX_MPREC slope, length, lenx, leny, rise, run;
} PLEX_LINE;

// a rectangle, basically contains an 'origin' for the point of origin and dimens for the dimensions.

typedef struct
{
	PLEX_POINT origin;
	PLEX_DIMENS dimens;
} PLEX_RECT;


typedef struct
{
	PLEX_POINT points[PLEX_LIMIT_POLYSIDES];
	size_t pointCount;
} PLEX_POLYGON;

#ifdef __cplusplus
extern"C"
{
#endif
	// START MATH FUNCTION:

	// there is no upload or end math function yet, and possibly never will be. Mainly here to seed square root table. Does not need to return boolean.

	extern void PLEX_startMath();

	// BASIC UNIVERSAL MATH:

	// signum and inverted signum. I have no idea why the C doesn't have these in it's standard library.
	extern PLEX_MPREC PLEX_signum(const PLEX_MPREC num);
	extern PLEX_MPREC PLEX_invSignum(const PLEX_MPREC num);

	// functions to convert radians into degrees and degrees into radians
	extern PLEX_MPREC PLEX_radians(const PLEX_MPREC degrees);
	extern PLEX_MPREC PLEX_degrees(const PLEX_MPREC radians);

	// square and square root.
	extern PLEX_MPREC PLEX_squareRoot(const PLEX_MPREC num);
	extern PLEX_MPREC PLEX_square(const PLEX_MPREC num);

	// the difference between two numbers. Like subtraction except for it gets the actual difference if one or both is negative
	extern PLEX_MPREC PLEX_difference(const PLEX_MPREC a, const PLEX_MPREC b);

	// RANDOM GEN MATH:

	// takes away the built in random seed for random generation and sets it to a fixed number. Of course, use the same number and all random functions will always return the same thing.
	extern void PLEX_rigSeed(const uint8_t fixedNumber);

	// sets the seed for randomness away from being rigged to a certain number and back to a time-based system
	extern void PLEX_unrigSeed();

	// returns a random percentage (0.0-1.0)
	extern PLEX_MPREC PLEX_randomPercentage();

	// returns a random number between min and max. It is near impossible to actually return 'max' value.
	extern PLEX_MPREC PLEX_randomNumber(const PLEX_MPREC min, const PLEX_MPREC max);

	// RECTANGLE AND LINE MATH:

	// creates a point, dimens, line, or rect from coorcinate values
	extern PLEX_POINT PLEX_convPoint(const PLEX_MPREC x, const PLEX_MPREC y);
	extern PLEX_DIMENS PLEX_convDimens(const PLEX_MPREC w, const PLEX_MPREC h);
	extern PLEX_LINE PLEX_convLine(const PLEX_POINT a, const PLEX_POINT b);
	extern PLEX_RECT PLEX_convRect(const PLEX_POINT origin, const PLEX_DIMENS dimens);

	// convert rectangle to polygon

	extern PLEX_POLYGON PLEX_convRectToPoly(const PLEX_RECT rect);

	// checks if a point is inside a rectangle
	extern bool PLEX_insideRect(const PLEX_POINT point, const PLEX_RECT rect);

	// check if rectangle is inside rectangle
	extern bool PLEX_rectInsideRect(const PLEX_RECT rectA, const PLEX_RECT rectB);

	// checks if there's a collision if rect moves by moveX and moveY coordinates. If it returns a 0 there's no collision. A return value of 1 = downward, 2 = leftward, 3 = upward, 4 = rightward. Does not return diagonal. Returns 5 if the moveX and moveY are 0 but there's still a collision.
	extern uint8_t PLEX_rectCollision(const PLEX_RECT rect, const PLEX_RECT rect2, const PLEX_MPREC moveX, const PLEX_MPREC moveY);
	extern uint8_t PLEX_rectCollisionNoMove(const PLEX_RECT rect, const PLEX_RECT rect2);

	// polygonal collision detection - experimental/untested
	extern uint8_t PLEX_pointToPolygonCollision(const PLEX_POINT point, const PLEX_POLYGON polygon, const PLEX_MPREC moveX, const PLEX_MPREC moveY);
	extern uint8_t PLEX_polygonToPolygonCollision(const PLEX_POLYGON a, const PLEX_POLYGON b, const PLEX_MPREC moveX, const PLEX_MPREC moveY);
	extern uint8_t PLEX_pointToPolygonCollisionNoMove(const PLEX_POINT point, const PLEX_POLYGON polygon);
	extern uint8_t PLEX_polygonToPolygonCollisionNoMove(const PLEX_POLYGON a, const PLEX_POLYGON b);

	// line calculations. distance, slope, converting slope to radians, and getting the radians of a line created from two points
	extern PLEX_MPREC PLEX_distance(const PLEX_POINT point, const PLEX_POINT point2, PLEX_MPREC* lenx, PLEX_MPREC* leny);
	extern PLEX_MPREC PLEX_slope(const PLEX_POINT point, const PLEX_POINT point2, PLEX_MPREC* rise, PLEX_MPREC* run);
	extern PLEX_MPREC PLEX_slopeToRads(const PLEX_MPREC slope);
	extern PLEX_MPREC PLEX_slopeToRads2(const PLEX_MPREC rise, const PLEX_MPREC run);
	extern PLEX_MPREC PLEX_radsBetweenPoints(const PLEX_POINT point, const PLEX_POINT point2);

	// more line calculations. returns the X intercept and Y intercept of a point on a slope. Used mainly for polygonal collision detection which is still experimental. It does, however, work with small closed polygons.
	extern PLEX_MPREC PLEX_xIntercept(const PLEX_POINT point, const PLEX_MPREC slope);
	extern PLEX_MPREC PLEX_yIntercept(const PLEX_POINT point, const PLEX_MPREC slope);

	// returns the point of interception for a point at a slope. Also, primarily for polygonal collision detection
	extern PLEX_POINT PLEX_intercept(const PLEX_POINT point, const PLEX_MPREC slope);

	// gets the end point of a line and the halfway point of a line.
	extern PLEX_POINT PLEX_endPoint(const PLEX_LINE line);
	extern PLEX_POINT PLEX_halfPoint(const PLEX_LINE line);

	// If you want to calculate an angular movement correctly use these. Like, for example, lets say you want to move something north by 1. You'd subtract -1 from that coordinate. But if you wanted to move it northeast you'd subtract the y by PLEX_diagonalMoveAmount and add the x PLEX_diagonalMoveAmount and you'll have gone the distance of 1. Sometimes I'm lazy with this and just ignore this feature and make it x + 0.5 and y + 0.5. But this is more accurate. As for the 'rev' it's basically the same but in reverse. So, applying the same numbers would end up going southwest rather than northeast.
	extern PLEX_MPREC PLEX_revDiagonalMoveAmount();
	extern PLEX_MPREC PLEX_diagonalMoveAmount();

	// CIRCLE COLLISION MATH:

	// This is not extensively in use yet but it essentially takes a percentage and gives you the length in radians of an angle over a slice of a circle. Like, for example, if you wanted the angle for a slice which covers a quarter circle you would pass in 0.25. Eventually, PLEX will include circular based collision detection.
	extern PLEX_MPREC PLEX_circlePerc(const PLEX_MPREC perc);

	// return various radiant values for slices of a circle.
	extern PLEX_MPREC PLEX_eighthCircleRads();
	extern PLEX_MPREC PLEX_quarterCircleRads();
	extern PLEX_MPREC PLEX_halfCircleRads();
	extern PLEX_MPREC PLEX_threeQuarterCircleRads();
	extern PLEX_MPREC PLEX_circleRads();

	// POLYGON COLLISION MATH:

	// determines whether or not a line is 'below' a point. This is an UNTESTED EXPERIMENTAL FUNCTION for the mostly untested polygon collision system. It's in here, but should not be relied on.
	extern bool PLEX_pointBelowLine(const PLEX_POINT point, const PLEX_LINE line);
#ifdef __cplusplus
}
#endif

#endif
