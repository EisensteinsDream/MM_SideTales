#ifndef PLEX_GENERATOR_H
#define PLEX_GENERATOR_H

#include "utility/math.h"

// This unit creates 'generators' which are essentially a means to generate a number of x/y coordinates over a period of time. This serves multiple functions within various PLEX-based projects.

//NOTE: Essentially this gives you a series of 'spawn points' but not in a particularly sophisticated way (ie. within a range but not according to walls or other obstructions). It's still relatively useful for various things.

// Struct which contains an individual generated unit within the generator. Generally, there's technical reasons why this isn't contained invisibly within the 'unit', but it kind of should be treated as it should be. You shouldn't access these directly for any reason, really.
typedef struct
{
	double startX, startY, endX, endY;
	size_t phaseLen, phaseStart;
	uint64_t duration;
} PLEX_GENERATED;

// The struct which contains all the information the generator needs to generate.
typedef struct
{
	double minX, maxX, minY, maxY, minEndX, maxEndX, minEndY, maxEndY;
	uint64_t duration, minWait, maxWait, start, minGenDuration, maxGenDuration;

	PLEX_GENERATED generated[PLEX_LIMIT_GENMAX];
	uint64_t waits[PLEX_LIMIT_GENMAX];
	size_t generatedCount;
	bool recycle;
} PLEX_GENERATOR;

#ifdef __cplusplus
extern "C"
{
#endif
	// creates a 'generator' struct. Contains a range for x and y. Contains the duration which chooses how long the generator will continue generating. A minimum and maximum wait time before generation starts. The minimum and maximum amount of time before a coordinate generates. If 'recycle' is true then the generator resets itself. All times in microseconds.
	extern PLEX_GENERATOR PLEX_createGenerator(const double minX, const double maxX, const double minY, const double maxY, const double minEndX, const double maxEndX, const double minEndY, const double maxEndY, const uint64_t duration, const uint64_t minWait, const uint64_t maxWait, const uint64_t minGenDuration, const uint64_t maxGenDuration, const bool recycle);

	// starts the generator at the marked time.
	extern void PLEX_startGenerator(const uint64_t time, PLEX_GENERATOR* generator);

	// updates the generator at the marked time.
	extern void PLEX_updateGenerator(const uint64_t time, PLEX_GENERATOR* generator);

	// pulls the last x/y from the generator. Returns 'false' if the generation is not ready to pull yet. Also checks by 'index' which should be externally incremented.
	extern bool PLEX_pullGenerated(const size_t index, const uint64_t time, double* x, double* y, PLEX_GENERATOR* generator);

	// changes the generator's generation area
	extern void PLEX_changeGeneratorArea(const double minX, const double maxX, const double minY, const double maxY, const double minEndX, const double maxEndX, const double minEndY, const double maxEndY, PLEX_GENERATOR* generator);

	// extends the duration time of the generator by 'by' microseconds
	extern void PLEX_extendGeneratorDuration(const uint64_t by, PLEX_GENERATOR* generator);

	// acquires the generator's generation area
	extern void PLEX_getGeneratorArea(double* minX, double* maxX, double* minY, double* maxY, double* minEndX, double* maxEndX, double* minEndY, double* maxEndY, PLEX_GENERATOR* generator);

	// returns the number of coordinates generated
	extern size_t PLEX_generatedCount(const PLEX_GENERATOR generator);

	// returns whether or not the generator is finished
	extern bool PLEX_generationFinished(const uint64_t time, const PLEX_GENERATOR generator);
#ifdef __cplusplus
}
#endif

#endif
