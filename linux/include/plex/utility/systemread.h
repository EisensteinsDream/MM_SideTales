#ifndef PLEX_SYSTEMREAD_H
#define PLEX_SYSTEMREAD_H

#include "string/string.h"

#include "mem/stack.h"

// NOTE: this unit will contain a branched system of reading your PC's hardware information. At the moment, it's incomplete and disconnected from the library's compilation process. However, the beginnings of the source are here.

#ifdef __cplusplus
extern "C"
{
#endif
	extern bool PLEX_startSystemRead();
	extern bool PLEX_updateSystemRead();
	extern bool PLEX_endSystemRead();

	extern PLEX_FAKESTRING_50 PLEX_readOSName();
	extern PLEX_FAKESTRING_50 PLEX_readOSVersion();
	extern PLEX_FAKESTRING_50 PLEX_readSystemName();
	extern PLEX_FAKESTRING_50 PLEX_readAssemblyVersion();
	extern PLEX_FAKESTRING_50 PLEX_readCPUInfo(const size_t index);
	extern PLEX_FAKESTRING_50 PLEX_readCPUMHZ(const size_t index);

	extern size_t PLEX_readSystemRamUsage();
	extern size_t PLEX_cpuCount();
	extern double PLEX_cpuAvg();

	extern double PLEX_cpuUsagePerc();
#ifdef __cplusplus
}
#endif

#endif
