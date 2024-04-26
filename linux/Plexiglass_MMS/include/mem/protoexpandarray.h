#ifndef PLEX_PROTOEXPANDARRAY_H
#define PLEX_PROTOEXPANDARRAY_H

#include "utility/error.h"

// This unit handles customizable dynamic low level arrays. Useful mostly as a base for more specific dynamic arrays as it utilizes custom allocation and deallocation methods.

// NOTE: There really isn't a reason you should use this unit unless you're making your own custom expand array type. Typically you should use 'expandArray' or create your own instead.

// the basic custom allocation and deallocation function types to be passed into expand arrays
typedef void* (PLEX_F_MEMHOOK)(const size_t);
typedef void (PLEX_F_FREEHOOK)(void**);

// the structs that contain protoexpandarray information
typedef struct
{
	char* contents;
	size_t count, capacity, growthUnit, unitSize;
	PLEX_F_MEMHOOK* memHook;
	PLEX_F_FREEHOOK* freeHook;
} PLEX_PROTOEXPANDARRAY;

#ifdef __cplusplus
extern"C"
{
#endif
	// allocates a new protoexpandarray. Growth Unit indicates how many entries are allocated at beginning and how many will be allocated as the array expands. The unitSize would be the size of the individual unit in bytes contained within. memHook is the memory allocation unit for hooks, freeHook is the memory unit deallocation for hooks. Hook function types are described above.

	extern PLEX_PROTOEXPANDARRAY* PLEX_genProtoExpandArray(const size_t growthUnit, const size_t unitSize, PLEX_F_MEMHOOK memHook, PLEX_F_FREEHOOK freeHook);

	// destroys the protoExpandArray by a pointer to an allocated pointer. Frees all content (by the freeHook), frees the protoExpandArray allocation, and sets it to NULL.

	extern void PLEX_destroyProtoExpandArray(PLEX_PROTOEXPANDARRAY** expandArray);

	// expands a protoExpandArray up to capacity (number of bytes = capacity * unitSize). If the capacity is more than the current capacity, then it expands. If it is not, it doesn't bother expanding. Returns true whether it expands or not, but returns false if anything goes wrong.

	extern bool PLEX_expandProtoExpandArray(const size_t capacity, PLEX_PROTOEXPANDARRAY* expandArray);

	// increments the protoExpandArray by one unit, expanding it internally if needed. Returns false if anything goes wrong.

	extern bool PLEX_incrementProtoExpandArray(PLEX_PROTOEXPANDARRAY* expandArray);

	// increments the protoExpandArray by 'by' number of units, expanding internaly if needed. Returns false if anything goes wrong.

	extern bool PLEX_multiIncrementProtoExpandArray(const size_t by, PLEX_PROTOEXPANDARRAY* expandArray);

	// decrements the protoExpandArray by one unit.

	extern void PLEX_decrementProtoExpandArray(PLEX_PROTOEXPANDARRAY* expandArray);

	// removes an entry from the protoExpandArray

	extern void PLEX_removeProtoExpandArray(const size_t index, PLEX_PROTOEXPANDARRAY* expandArray);

	// clears the protoExpandArray of all values

	extern void PLEX_clearCountProtoExpandArray(PLEX_PROTOEXPANDARRAY* expandArray);

	// takes one piece of protoExpandArray to another. From beg to beg2 with a chunk as big as 'len'. len may be shrunk for safety reasons. Returns false if anything goes wrong.

	extern bool PLEX_rearrangeProtoExpandArray(const size_t beg, const size_t beg2, const size_t len, PLEX_PROTOEXPANDARRAY* expandArray);

	// moves a piece of a piece of one protoExpandArray to another protoExpandArray from beg to beg2. The size is up to len but will be shrunk by the size of either protoExpandArray's contents for safety. Returns false if anything goes wrong.

	extern bool PLEX_swapProtoExpandArray(const size_t beg, const size_t beg2, const size_t len, PLEX_PROTOEXPANDARRAY* expandArray, PLEX_PROTOEXPANDARRAY* expandArray2);

	// cuts out a piece of protoExpandArray at beg size of length, decreasing the size of protoExpandArray. Also, reducing len if needed for safety reasons. Returns false if anything goes wrong.

	extern bool PLEX_cutProtoExpandArray(const size_t beg, const size_t len, PLEX_PROTOEXPANDARRAY* expandArray);

	// reverses the order of a section of the protoExpandArray

	extern bool PLEX_reverseProtoExpandArray(const size_t beg, const size_t len, PLEX_PROTOEXPANDARRAY* expandArray);

	// same as rearrange except by percentages rather than determined lengths. Of course, the percentages are related to the length of the protoExpandArray. Returns false if anything goes wrong.

	extern bool PLEX_rearrangeProtoExpandArrayPerc(const double begPerc, const double begPerc2, const double lenPerc, PLEX_PROTOEXPANDARRAY* expandArray);

	// same as swap except by percentages rather than determined lengths. Of course, the percentages are related to the length of the protoExpandArray. Returns false if anything goes wrong.

	extern bool PLEX_swapProtoExpandArrayPerc(const double begPerc, const double begPerc2, const double lenPerc, PLEX_PROTOEXPANDARRAY* expandArray, PLEX_PROTOEXPANDARRAY* expandArray2);

	// same as cut except by percentages rather than determined lengths. Of course, the percentages are related to the length of the protoExpandArray. Returns false if anything goes wrong.

	extern bool PLEX_cutProtoExpandArrayPerc(const double begPerc, const double lenPerc, PLEX_PROTOEXPANDARRAY* expandArray);

	// same as rearramge except by percentages rather than determined lengths. Of course, the percentages are related to the length of the protoExpandArray. Returns false if anything goes wrong.

	extern bool PLEX_reverseProtoExpandArrayPerc(const double begPerc, const double lenPerc, PLEX_PROTOEXPANDARRAY* expandArray);

	// sets the growth unit to something else

	extern void PLEX_setGrowthUnitProtoExpandArray(const size_t growthUnit, PLEX_PROTOEXPANDARRAY* expandArray);

	// returns a pointer to an entry to the protoExpandArray by index. Returns NULL if anything goes wrong.

	extern void* PLEX_getProtoExpandArrayEntry(const size_t index, PLEX_PROTOEXPANDARRAY* expandArray);

	// returns the last pointer to an entry to the protoExpandArray. Returns NULL if anything goes wrong.

	extern void* PLEX_getProtoExpandArrayLast(PLEX_PROTOEXPANDARRAY* expandArray);

	// returns the number of entries (each size of 'unit size') contained in the protoExpandArray.

	extern size_t PLEX_protoExpandArrayCount(const PLEX_PROTOEXPANDARRAY* expandArray);

	// returns the 'capacity' contained in the protoExpandArray. (ie. the potential number of units needed before expanding)

	extern size_t PLEX_protoExpandArrayCapacity(const PLEX_PROTOEXPANDARRAY* expandArray);

	// returns the 'unit size' of a protoExpandArray.

	extern size_t PLEX_protoExpandArrayUnitSize(const PLEX_PROTOEXPANDARRAY* expandArray);

	// returns the 'growth unit' of a protoExpandArray.

	extern size_t PLEX_protoExpandArrayGrowthUnit(const PLEX_PROTOEXPANDARRAY* expandArray);

	// returns the 'size' of a protoExpandArray which means the byte size of the count. Where as 'count' is the number of units.

	extern size_t PLEX_protoExpandArraySize(const PLEX_PROTOEXPANDARRAY* expandArray);

	// returns the 'capacity size' of a protoExpandArray which means the byte size of the capacity. Where as 'capacity' is the number of units capacity.

	extern size_t PLEX_protoExpandArrayCapacitySize(const PLEX_PROTOEXPANDARRAY* expandArray);

	// returns the pointer to the contents of a protoExpandArray, use sparringly.

	extern void* PLEX_protoExpandArrayContents(const PLEX_PROTOEXPANDARRAY* expandArray);
#ifdef __cplusplus
}
#endif

#endif
