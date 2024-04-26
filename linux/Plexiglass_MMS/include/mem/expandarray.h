#ifndef PLEX_EXPANDARRAY_H
#define PLEX_EXPANDARRAY_H

#include "mem.h"

// A simple lowlevel dynamic array based off of protoExpandArray. If needed, this one is preferred for usage.

// NOTE: This is basically just protoExpandArray but fixed with the built in c 'malloc' and 'free' allocation functions.

// expandarray is essentially a typedef'd protoexpandarray. Still, it's not good practice to interchange them.
typedef PLEX_PROTOEXPANDARRAY PLEX_EXPANDARRAY;


#define PLEX_genExpandArray(growthUnit, unitSize) PLEX_genExpandArrayInt(growthUnit, unitSize, __LINE__, __FILE__);
#define PLEX_expandExpandArray(capacity, expandArray) PLEX_expandExpandArrayInt(capacity, expandArray, __LINE__, __FILE__);
#define PLEX_incrementExpandArray(expandArray) PLEX_incrementExpandArrayInt(expandArray, __LINE__, __FILE__)
#define PLEX_multiIncrementExpandArray(by, expandArray) PLEX_multiIncrementExpandArrayInt(by, expandArray, __LINE__, __FILE__)
#define PLEX_rearrangeExpandArray(beg, beg2, len, expandArray) PLEX_rearrangeExpandArrayInt(beg, beg2, len, expandArray, __LINE__, __FILE__)
#define PLEX_swapExpandArray(beg, beg2, len, expandArray, expandArray2) PLEX_swapExpandArrayInt(beg, beg2, len, expandArray, expandArray2, __LINE__, __FILE__)
#define PLEX_reverseExpandArray(beg, len, expandArray) PLEX_reverseExpandArrayInt(beg, len, expandArray, __LINE__, __FILE__)
#define PLEX_rearrangeExpandArrayPerc(begPerc, begPerc2, lenPerc, expandArray) PLEX_rearrangeExpandArrayPercInt(begPerc, begPerc2, lenPerc, expandArray, __LINE__, __FILE__)
#define PLEX_swapExpandArrayPerc(begPerc, begPerc2, lenPerc, expandArray, expandArray2) PLEX_swapExpandArrayPercInt(begPerc, begPerc2, lenPerc, expandArray, expandArray2, __LINE__, __FILE__)
#define PLEX_reverseExpandArrayPerc(begPerc, lenPerc, expandArray) PLEX_reverseExpandArrayPercInt(begPerc, lenPerc, expandArray, __LINE__, __FILE__)

#ifdef __cplusplus
extern"C"
{
#endif
	// generates the expand array. 'growthUnit' is the amount of units needed to be added before expanding an expandArray internally. 'unit size' being the byte size of an individual contained unit.

	extern PLEX_EXPANDARRAY* PLEX_genExpandArrayInt(const size_t growthUnit, const size_t unitSize, const size_t line, const char* const file);

	// destroys the expandArray by a pointer to an allocated pointer. Frees all content, frees the expandArray allocation, and sets it to NULL.

	extern void PLEX_destroyExpandArray(PLEX_EXPANDARRAY** expandArray);

	// expands a expandArray up to capacity (number of bytes = capacity * unitSize). If the capacity is more than the current capacity, then it expands. If it is not, it doesn't bother expanding. Returns true whether it expands or not, but returns false if anything goes wrong.

	extern bool PLEX_expandExpandArrayInt(const size_t capacity, PLEX_EXPANDARRAY* expandArray, const size_t line, const char* const file);

	// increments the expandArray by one unit, expanding it internally if needed. Returns false if anything goes wrong.

	extern bool PLEX_incrementExpandArrayInt(PLEX_EXPANDARRAY* expandArray, const size_t line, const char* const file);

	// increments the expandArray by 'by' number of units, expanding internaly if needed. Returns false if anything goes wrong.

	extern bool PLEX_multiIncrementExpandArrayInt(const size_t by, PLEX_EXPANDARRAY* expandArray, const size_t line, const char* const file);

	// decrements the expandArray by one unit.

	extern void PLEX_decrementExpandArray(PLEX_EXPANDARRAY* expandArray);

	// removes an entry from the protoExpandArray

	extern void PLEX_removeExpandArray(const size_t index, PLEX_EXPANDARRAY* expandArray);

	// clears the expandArray of all values

	extern void PLEX_clearExpandArray(PLEX_EXPANDARRAY* expandArray);

	// takes one piece of expandArray to another. From beg to beg2 with a chunk as big as 'len'. len may be shrunk for safety reasons. Returns false if anything goes wrong.

	extern bool PLEX_rearrangeExpandArrayInt(const size_t beg, const size_t beg2, const size_t len, PLEX_EXPANDARRAY* expandArray, const size_t line, const char* const file);

	// moves a piece of a piece of one expandArray to another expandArray from beg to beg2. The size is up to len but will be shrunk by the size of either expandArray's contents for safety. Returns false if anything goes wrong.

	extern bool PLEX_swapExpandArrayInt(const size_t beg, const size_t beg2, const size_t len, PLEX_EXPANDARRAY* expandArray, PLEX_EXPANDARRAY* expandArray2, const size_t line, const char* const file);

	// cuts out a piece of expandArray at beg size of length, decreasing the size of protoExpandArray. Also, reducing len if needed for safety reasons. Returns false if anything goes wrong.

	extern bool PLEX_cutExpandArray(const size_t beg, const size_t len, PLEX_EXPANDARRAY* expandArray);

	// reverses the order of a section of the expandArray

	extern bool PLEX_reverseExpandArrayInt(const size_t beg, const size_t len, PLEX_EXPANDARRAY* expandArray, const size_t line, const char* const file);

	// same as rearrange except by percentages rather than determined lengths. Of course, the percentages are related to the length of the expandArray. Returns false if anything goes wrong.

	extern bool PLEX_rearrangeExpandArrayPercInt(const double begPerc, const double begPerc2, const double lenPerc, PLEX_EXPANDARRAY* expandArray, const size_t line, const char* const file);

	// same as swap except by percentages rather than determined lengths. Of course, the percentages are related to the length of the expandArray. Returns false if anything goes wrong.

	extern bool PLEX_swapExpandArrayPercInt(const double begPerc, const double begPerc2, const double lenPerc, PLEX_EXPANDARRAY* expandArray, PLEX_EXPANDARRAY* expandArray2, const size_t line, const char* const file);

	// same as cut except by percentages rather than determined lengths. Of course, the percentages are related to the length of the expandArray. Returns false if anything goes wrong.

	extern bool PLEX_cutExpandArrayPerc(const double begPerc, const double lenPerc, PLEX_PROTOEXPANDARRAY* expandArray);

	// same as rearrange except by percentages rather than determined lengths. Of course, the percentages are related to the length of the protoExpandArray. Returns false if anything goes wrong.

	extern bool PLEX_reverseExpandArrayPercInt(const double begPerc, const double lenPerc, PLEX_EXPANDARRAY* expandArray, const size_t line, const char* const file);

	// sets the growth unit to something else

	extern void PLEX_setGrowthUnitExpandArray(const size_t growthUnit, PLEX_EXPANDARRAY* expandArray);

	// returns a pointer to an entry to the expandArray by index. Returns NULL if anything goes wrong.

	extern void* PLEX_getExpandArrayEntry(const size_t index, PLEX_EXPANDARRAY* expandArray);

	// returns the last pointer to an entry to the expandArray. Returns NULL if anything goes wrong.

	extern void* PLEX_getExpandArrayLast(PLEX_EXPANDARRAY* expandArray);

	// returns the number of entries (each size of 'unit size') contained in the expandArray.

	extern size_t PLEX_expandArrayCount(const PLEX_EXPANDARRAY* expandArray);

	// returns the 'capacity' contained in the expandArray. (ie. the potential number of units needed before expanding)

	extern size_t PLEX_expandArrayCapacity(const PLEX_EXPANDARRAY* expandArray);

	// returns the 'unit size' of a expandArray.

	extern size_t PLEX_expandArrayUnitSize(const PLEX_EXPANDARRAY* expandArray);

	// returns the 'growth unit' of a expandArray.

	extern size_t PLEX_expandArrayGrowthUnit(const PLEX_EXPANDARRAY* expandArray);

	// returns the 'size' of a expandArray which means the byte size of the count. Whereas the 'count' is the number of units.

	extern size_t PLEX_expandArraySize(const PLEX_EXPANDARRAY* expandArray);

	// returns the 'capacity size' of an expandarray which is the byte size of the capacity. Whereas the 'capacity' is the number of units.

	extern size_t PLEX_expandArrayCapacitySize(const PLEX_EXPANDARRAY* expandArray);

	// returns the pointer to the contents of a expandArray, use sparringly.

	extern void* PLEX_expandArrayContents(const PLEX_EXPANDARRAY* expandArray);
#ifdef __cplusplus
}
#endif


#endif
