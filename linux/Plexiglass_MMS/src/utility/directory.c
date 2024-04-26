#include "utility/directory.h"

#if PLEX_CORE_IS_UNIX
	#include "directory_unix.c.h"
#endif

#if PLEX_CORE_OS == PLEX_CORE_WINDOWS
	#include "directory_windows.c.h"
#endif

PLEX_DIRECTORYCRAWL_DATA PLEX_blankDirectoryCrawlerData()
{
	PLEX_DIRECTORYCRAWL_DATA ret;

	memset(&ret, 0, sizeof(ret));

	return ret;
}

bool PLEX_compDirectoriesInt(const PLEX_DIRECTORY a, const PLEX_DIRECTORY b, const size_t line, const char* const filename)
{
	return PLEX_compStrings100Int(a.fullPath, b.fullPath, false, line, filename);
}

bool PLEX_getDirectoryFromCrawlerDataInt(PLEX_DIRECTORYCRAWL_DATA* output, PLEX_DIRECTORY* directory, const size_t linenum, const char* const filename)
{
	if(directory == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_DIRECTORY, PLEX_ERROR_ISNULL, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_ONGETTINGDIRECTORYFROMCRAWLER, PLEX_ERRORMESS_DIRECTORY);
		return false;
	}

	if(output == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_DIRECTORY, PLEX_ERROR_ISNULL, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_ONGETTINGDIRECTORYFROMCRAWLER, PLEX_ERRORMESS_DIRECTORYCRAWLER);
		return false;
	}

	if(!output->isDirectory)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_DIRECTORY, PLEX_ERROR_FILETYPE, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONGETTINGDIRECTORYFROMCRAWLER, PLEX_ERRORMESS_CRAWLERSDIRECTORY);
		return false;
	}

	if(output->directory == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_DIRECTORY, PLEX_ERROR_ISNULL, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONGETTINGDIRECTORYFROMCRAWLER, PLEX_ERRORMESS_CRAWLERSDIRECTORY);
		return false;
	}

	return PLEX_getDirectoryInt(PLEX_fts(output->path), directory, linenum, filename);
}

bool PLEX_isCrawlerDataDirectory(const PLEX_DIRECTORYCRAWL_DATA output)
{
	return output.isDirectory;
}

bool PLEX_isRedirecDirectory(const PLEX_DIRECTORY directory)
{
	const char* const path = PLEX_fts(directory.path);
	const size_t plen = directory.path.len;

	char symbol = 0;

	if(!plen) return false;
	if(plen == 1) return path[0] == '.';
	if(plen == 2) return path[0] == '.' && path[1] == '.';

	symbol = path[plen - 1];

	if(symbol != '.') return false;

	symbol = path[plen - 2];

	if(symbol == '/' || symbol == '\\') return true;

	if(symbol != '.') return false;

	symbol = path[plen - 3];

	if(symbol != '/' && symbol != '\\') return false;

	return true;
}
