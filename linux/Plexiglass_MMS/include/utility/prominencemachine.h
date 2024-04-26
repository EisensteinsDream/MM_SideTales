#ifndef PLEX_PROMINENCEMACHINE_H
#define PLEX_PROMINENCEMACHINE_H

#include "utility/math.h"

#include "mem/expandarray.h"

// this unit allows the creation of 'prominence machines' which essentially work as weighted randomizer. You create a new entry by passing a 'prominence' which offers a sort of weight to the next entry. It returns a index to that prominence. You then can 'get prominence' at any point in time. It returns a randomized index to which more likely will hit the entries with a higher prominence than ones with a lower prominence.

#define PLEX_genProminenceMachine() PLEX_genProminenceMachineInt(__LINE__, __FILE__)

#define PLEX_addProminence(prominence, machine) PLEX_addProminenceInt(prominence, machine, __LINE__, __FILE__)

// struct that contains the information for a prominence machine.
typedef struct
{
	PLEX_EXPANDARRAY* prominences;
	uint64_t prominenceCap;
} PLEX_PROMINENCEMACHINE;

#ifdef __cplusplus
extern "C"
{
#endif
	// creates a new prominence machine. Prominence machines do not need any information passed for their creation.
	extern PLEX_PROMINENCEMACHINE* PLEX_genProminenceMachineInt(const size_t line, const char* const filename);

	extern void PLEX_destroyProminenceMachine(PLEX_PROMINENCEMACHINE** toDestroy);

	// adds a 'prominence' to the machine. Of course, the larger this number is the more likely it will return that index. If anything goes wrong it returns -1
	extern int64_t PLEX_addProminenceInt(const uint32_t prominence, PLEX_PROMINENCEMACHINE* machine, const size_t linenum, const char* const filename);

	// removes the prominence entry at 'index'
	extern void PLEX_removeProminence(const size_t index, PLEX_PROMINENCEMACHINE* machine);

	// returns generated prominence index from the passed in prominence machine. Returns -1 if something goes wrong.
	extern int64_t PLEX_getProminence(const PLEX_PROMINENCEMACHINE* machine);

	extern size_t PLEX_getProminenceCount(const PLEX_PROMINENCEMACHINE* machine);
#ifdef __cplusplus
}
#endif

#endif
