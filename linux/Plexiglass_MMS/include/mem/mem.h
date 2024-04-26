#ifndef PLEX_MEM_H
#define PLEX_MEM_H

#include "mem/memprofile.h"

#include "mem/protoexpandarray.h"

#include "meta/hardlimits.h"

#include "utility/error.h"

// this unit contains the plex allocation system. The difference between PLEX's allocation and the typical 'malloc' and 'free' is that PLEX's memory allocation system preallocates and reuses allocated memory. This is NOT a garbage collector. It does not create memory leaks because it frees all memory at termination of program. However, it can create memory hangs as none of the memory is automatically managed. Essentially, it's your typical c-style allocation but with some light safety and efficiency improvements.

// allocate or deallocate smart memory
#define PLEX_alloc(size) PLEX_allocInt(size, __LINE__, __FILE__)
#define PLEX_dealloc(toFree) PLEX_deallocInt((void**)toFree)

#ifdef __cplusplus
extern"C"
{
#endif
	// starts, ends, and updates the memory system. Typically, you never have to call these. They are called in 'startPlex', 'updatePlex', and 'endPlex'.
	extern bool PLEX_startMem();
	extern bool PLEX_updateMem();
	extern void PLEX_endMem();

	// never call these directly. Use the PLEX_alloc and PLEX_dealloc macros instead
	extern void* PLEX_allocInt(const size_t count, const size_t line, const char* const filename);
	extern void PLEX_deallocInt(void** toFree);

	// really only used for defaults, updates all the precaching to the current memory profile output
	extern void PLEX_updateMemProfile();

	// you will never need to call these. They're just for default values.
	extern void PLEX_flagMemReport();
	extern void PLEX_flagMemReportToFile();
#ifdef __cplusplus
}
#endif

#endif
