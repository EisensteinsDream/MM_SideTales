#include "mem/expandarray.h"

// NOTE: you can note all of these functions simply call an equivalent protoExpandArray function. The reason for this is that the alloc and dealloc hooks are passed in on generation. This is mostly done just to prevent syntatical confusion. On all updates and modifications these should remain syncronized.

static size_t p_errorLine = 0;
static const char* p_errorFile = "";

static void* p_expandArrayAlloc(size_t size)
{
	return PLEX_allocInt(size, p_errorLine, p_errorFile);
}

static void p_expandArrayDealloc(void** toFree)
{
	PLEX_dealloc(toFree);
}

// ---------------------------- //
// PUBLIC FUNCTIONS:
// ---------------------------- //

PLEX_EXPANDARRAY* PLEX_genExpandArrayInt(const size_t growthUnit, const size_t unitSize, const size_t errorLine, const char* const errorFile)
{
	p_errorLine = errorLine;
	p_errorFile = errorFile;

	return PLEX_genProtoExpandArray(growthUnit, unitSize, p_expandArrayAlloc, p_expandArrayDealloc);
}

void PLEX_destroyExpandArray(PLEX_EXPANDARRAY** expandArray){ PLEX_destroyProtoExpandArray(expandArray); }

bool PLEX_expandExpandArrayInt(const size_t capacity, PLEX_EXPANDARRAY* expandArray, const size_t errorLine, const char* const errorFile)
{
	p_errorLine = errorLine;
	p_errorFile = errorFile;

	return PLEX_expandProtoExpandArray(capacity, expandArray);
}

bool PLEX_incrementExpandArrayInt(PLEX_EXPANDARRAY* expandArray, const size_t errorLine, const char* const errorFile)
{
	p_errorLine = errorLine;
	p_errorFile = errorFile;

	return PLEX_incrementProtoExpandArray(expandArray);
}

bool PLEX_multiIncrementExpandArrayInt(const size_t by, PLEX_EXPANDARRAY* expandArray, const size_t errorLine, const char* const errorFile)
{
	p_errorLine = errorLine;
	p_errorFile = errorFile;

	return PLEX_multiIncrementProtoExpandArray(by, expandArray);
}

void PLEX_decrementExpandArray(PLEX_EXPANDARRAY* expandArray){ PLEX_decrementProtoExpandArray(expandArray); }

void PLEX_removeExpandArray(const size_t index, PLEX_EXPANDARRAY* expandArray){ PLEX_removeProtoExpandArray(index, expandArray); }

void PLEX_clearExpandArray(PLEX_EXPANDARRAY* expandArray){ PLEX_clearCountProtoExpandArray(expandArray); }

bool PLEX_rearrangeExpandArrayInt(const size_t beg, const size_t beg2, const size_t len, PLEX_EXPANDARRAY* expandArray, const size_t errorLine, const char* const errorFile)
{
	p_errorLine = errorLine;
	p_errorFile = errorFile;

	return PLEX_rearrangeProtoExpandArray(beg, beg2, len, expandArray);
}

bool PLEX_swapExpandArrayInt(const size_t beg, const size_t beg2, const size_t len, PLEX_EXPANDARRAY* expandArray, PLEX_EXPANDARRAY* expandArray2, const size_t errorLine, const char* const errorFile)
{
	p_errorLine = errorLine;
	p_errorFile = errorFile;

	return PLEX_swapProtoExpandArray(beg, beg2, len, expandArray, expandArray2);
}

bool PLEX_cutExpandArray(const size_t beg, const size_t len, PLEX_EXPANDARRAY* expandArray){ return PLEX_cutProtoExpandArray(beg, len, expandArray); }

bool PLEX_reverseExpandArrayInt(const size_t beg, const size_t len, PLEX_EXPANDARRAY* expandArray, const size_t errorLine, const char* const errorFile)
{
	p_errorLine = errorLine;
	p_errorFile = errorFile;

	return PLEX_reverseProtoExpandArray(beg, len, expandArray);
}

bool PLEX_rearrangeExpandArrayPercInt(const double begPerc, const double begPerc2, const double lenPerc, PLEX_EXPANDARRAY* expandArray, const size_t errorLine, const char* const errorFile)
{
	p_errorLine = errorLine;
	p_errorFile = errorFile;

	return PLEX_rearrangeProtoExpandArrayPerc(begPerc, begPerc2, lenPerc, expandArray);
}

bool PLEX_swapExpandArrayPercInt(const double begPerc, const double begPerc2, const double lenPerc, PLEX_EXPANDARRAY* expandArray, PLEX_EXPANDARRAY* expandArray2, const size_t errorLine, const char* const errorFile)
{
	p_errorLine = errorLine;
	p_errorFile = errorFile;

	return PLEX_swapProtoExpandArrayPerc(begPerc, begPerc2, lenPerc, expandArray, expandArray2);
}

bool PLEX_cutExpandArrayPerc(const double begPerc, const double lenPerc, PLEX_PROTOEXPANDARRAY* expandArray){ return PLEX_cutProtoExpandArrayPerc(begPerc, lenPerc, expandArray); }

bool PLEX_reverseExpandArrayPercInt(const double begPerc, const double lenPerc, PLEX_PROTOEXPANDARRAY* expandArray, const size_t errorLine, const char* const errorFile)
{
	p_errorLine = errorLine;
	p_errorFile = errorFile;

	return PLEX_reverseProtoExpandArrayPerc(begPerc, lenPerc, expandArray);
}

void PLEX_setGrowthUnitExpandArray(const size_t growthUnit, PLEX_EXPANDARRAY* expandArray){ PLEX_setGrowthUnitProtoExpandArray(growthUnit, expandArray); }

void* PLEX_getExpandArrayEntry(const size_t index, PLEX_EXPANDARRAY* expandArray){ return PLEX_getProtoExpandArrayEntry(index, expandArray); }

void* PLEX_getExpandArrayLast(PLEX_EXPANDARRAY* expandArray){ return PLEX_getProtoExpandArrayLast(expandArray); }

size_t PLEX_expandArrayCount(const PLEX_EXPANDARRAY* expandArray){ return PLEX_protoExpandArrayCount(expandArray); }

size_t PLEX_expandArrayCapacity(const PLEX_EXPANDARRAY* expandArray){ return PLEX_protoExpandArrayCapacity(expandArray); }

size_t PLEX_expandArrayUnitSize(const PLEX_EXPANDARRAY* expandArray){ return PLEX_protoExpandArrayUnitSize(expandArray); }

size_t PLEX_expandArrayGrowthUnit(const PLEX_EXPANDARRAY* expandArray){ return PLEX_protoExpandArrayGrowthUnit(expandArray); }

size_t PLEX_expandArraySize(const PLEX_EXPANDARRAY* expandArray){ return PLEX_protoExpandArraySize(expandArray); }

size_t PLEX_expandArrayCapacitySize(const PLEX_EXPANDARRAY* expandArray){ return PLEX_protoExpandArrayCapacitySize(expandArray); }

void* PLEX_expandArrayContents(const PLEX_EXPANDARRAY* expandArray){ return PLEX_protoExpandArrayContents(expandArray); }
