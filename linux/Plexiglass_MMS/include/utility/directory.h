#ifndef PLEX_DIRECTORY_H
#define PLEX_DIRECTORY_H

#include "utility/fakestrings.h"

#define PLEX_crawlDirectory(readOnly, directory, output) PLEX_crawlDirectoryInt(readOnly, directory, output, __LINE__, __FILE__)
#define PLEX_getDirectory(path, directory) PLEX_getDirectoryInt(path, directory, __LINE__, __FILE__)
#define PLEX_compPaths(a, b) PLEX_compPathsInt(a, b, __LINE__, __FILE__)
#define PLEX_compDirectories(a, b) PLEX_compDirectoriesInt(a, b, __LINE__, __FILE__)
#define PLEX_getDirectoryFromCrawlerData(output, directory) PLEX_getDirectoryFromCrawlerDataInt(output, directory, __LINE__, __FILE__)

typedef struct
{
	PLEX_FAKESTRING_100 path, fullPath;
	bool open;

	#if PLEX_CORE_IS_UNIX
		DIR* contents;
	#endif

	#if PLEX_CORE_OS == PLEX_CORE_WINDOWS
		HANDLE contents;
		WIN32_FIND_DATA fdata;
	#endif

} PLEX_DIRECTORY;

typedef struct
{
	PLEX_FAKESTRING_100 path, fullPath;
	FILE* file;

	#if PLEX_CORE_IS_UNIX
		DIR* directory;
	#endif

	#if PLEX_CORE_OS == PLEX_CORE_WINDOWS
		HANDLE directory;
		WIN32_FIND_DATA fdata;
		bool done;
	#endif

	bool isDirectory;
} PLEX_DIRECTORYCRAWL_DATA;


// NOTE: some of these functions are described in the specific 'directory_unix.c'/'directory_windows.c' source files some simply in 'directory.c'
#ifdef __cplusplus
extern "C"
{
#endif
	extern PLEX_DIRECTORYCRAWL_DATA PLEX_blankDirectoryCrawlerData();

	extern bool PLEX_startDirectoryCrawler(PLEX_DIRECTORY* directory);

	extern bool PLEX_crawlDirectoryInt(const bool readOnly, PLEX_DIRECTORY* directory, PLEX_DIRECTORYCRAWL_DATA* output, const size_t linenum, const char* const filename);

	// directory crawler data should always be free'd with this function
	extern void PLEX_freeDirectoryCrawlerData(PLEX_DIRECTORYCRAWL_DATA* output);

	extern bool PLEX_getDirectoryFromCrawlerDataInt(PLEX_DIRECTORYCRAWL_DATA* output, PLEX_DIRECTORY* directory, const size_t line, const char* const filename);

	extern bool PLEX_getDirectoryInt(const char* const path, PLEX_DIRECTORY* directory, const size_t line, const char* const filename);

	extern bool PLEX_reopenDirectory(PLEX_DIRECTORY* directory);

	extern bool PLEX_closeDirectory(PLEX_DIRECTORY* directory);

	extern bool PLEX_compDirectoriesInt(const PLEX_DIRECTORY a, const PLEX_DIRECTORY b, const size_t line, const char* const filename);

	extern bool PLEX_compPathsInt(const char* const a, const char* const b, const size_t line, const char* const filename);

	extern bool PLEX_isCrawlerDataDirectory(const PLEX_DIRECTORYCRAWL_DATA output);

	// checks whether the directory is the '.' or '..' directories. It's available in windows, but only really relevant to unix based systems.
	extern bool PLEX_isRedirecDirectory(const PLEX_DIRECTORY directory);
#ifdef __cplusplus
}
#endif


#endif
