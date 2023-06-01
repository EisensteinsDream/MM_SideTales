#ifndef PLEX_MEM_H
#define PLEX_MEM_H

#include "mem/protoexpandarray.h"

#include "meta/hardlimits.h"

#include "utility/error.h"

// this unit contains the plex allocation system. The difference between PLEX's allocation and the typical 'malloc' and 'free' is that PLEX's memory allocation system preallocates and reuses allocated memory. This is NOT a garbage collector. It does not create memory leaks because it frees all memory at termination of program. However, it can create memory hangs as none of the memory is automatically managed. Essentially, it's your typical c-style allocation but with some light safety and efficiency improvements.

#ifdef __cplusplus
extern"C"
{
#endif
	// starts, ends, and updates the memory system. Typically, you never have to call these. They are called in 'startPlex', 'updatePlex', and 'endPlex'.
	extern bool PLEX_startMem();
	extern bool PLEX_updateMem();
	extern void PLEX_endMem();

	// the plex allocation and deallocation method. Essentially just for allocating and deallocating dynamic memory. It's used sparringly within PLEX and should be used sparringly in general. Again, also, this is not a garbage collector so you should make sure to 'PLEX_dealloc' every time you 'PLEX_alloc'.
	extern void* PLEX_alloc(size_t count);
	extern void PLEX_dealloc(void* toFree);
#ifdef __cplusplus
}
#endif

#endif
