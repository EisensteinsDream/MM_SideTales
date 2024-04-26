#ifndef PLEX_DIRECTORY_UNIX_C_H
#define PLEX_DIRECTORY_UNIX_C_H

bool PLEX_startDirectoryCrawler(PLEX_DIRECTORY* directory)
{
	if(directory == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_DIRECTORY, PLEX_ERROR_ISNULL, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_ONSTARTDIRECTORYCRAWLER, PLEX_ERRORMESS_DIRECTORY);
		return false;
	}

	if(!directory->open)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_DIRECTORY, PLEX_ERROR_DIRECTORYOPENSTATE, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_ONSTARTDIRECTORYCRAWLER, PLEX_ERRORMESS_GENERIC_CLOSED);
		return false;
	}

	if(directory->contents == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_DIRECTORY, PLEX_ERROR_CONTENTCORRUPTION, PLEX_ERROR_SEVERITY_FATAL, PLEX_ERRORMESS_ONSTARTDIRECTORYCRAWLER, PLEX_ERRORMESS_DIRECTORYCONTENTS);
		return false;
	}

	rewinddir(directory->contents);

	return true;
}

bool PLEX_crawlDirectoryInt(const bool readOnly, PLEX_DIRECTORY* directory, PLEX_DIRECTORYCRAWL_DATA* output, const size_t line, const char* const filename)
{
	struct dirent* crawler = NULL;

	DIR* testDir = NULL;

	char* path = NULL;
	char* fullpath = NULL;

	size_t namelen = 0;
	size_t direclen = 0;

	if(output == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_DIRECTORY, PLEX_ERROR_ISNULL, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_ONCRAWLDIRECTORY, PLEX_ERRORMESS_DIRECTORYCRAWLER);
		return false;
	}

	if(directory == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_DIRECTORY, PLEX_ERROR_ISNULL, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_ONCRAWLDIRECTORY, PLEX_ERRORMESS_DIRECTORY);
		return false;
	}

	if(!directory->open)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_DIRECTORY, PLEX_ERROR_DIRECTORYOPENSTATE, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_ONCRAWLDIRECTORY, PLEX_ERRORMESS_GENERIC_CLOSED);
		return false;
	}

	if(directory->contents == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_DIRECTORY, PLEX_ERROR_CONTENTCORRUPTION, PLEX_ERROR_SEVERITY_FATAL, PLEX_ERRORMESS_ONCRAWLDIRECTORY, PLEX_ERRORMESS_DIRECTORYCONTENTS);
		return false;
	}

	output->path = PLEX_blankString100();
	output->file = NULL;
	output->directory = NULL;
	output->isDirectory = false;

	crawler = readdir(directory->contents);

	if(crawler == NULL)
	{
		rewinddir(directory->contents);
		return false;
	}

	namelen = strlen(crawler->d_name);
	direclen = directory->path.len;

	path = PLEX_allocInt(PATH_MAX, line, filename);
	fullpath = PLEX_allocInt(PATH_MAX, line, filename);

	memcpy(path, PLEX_fts(directory->path), direclen);
	path[direclen] = '/';
	memcpy(path + direclen + 1, crawler->d_name, namelen);

	realpath(path, fullpath);

	output->path = PLEX_createString100(path);
	output->fullPath = PLEX_createString100(fullpath);

	testDir = opendir(fullpath);

	if(testDir == NULL)
	{
		const char* const fArgs = readOnly ? "rb" : "rab+";

		output->file = fopen(fullpath, fArgs);

		if(output->file)
		{
			PLEX_dealloc(&path);
			PLEX_dealloc(&fullpath);
			// error
			return false;
		}
	}else{
		output->directory = testDir;
		output->isDirectory = true;
	}

	PLEX_dealloc(&path);
	PLEX_dealloc(&fullpath);

	return true;
}

void PLEX_freeDirectoryCrawlerData(PLEX_DIRECTORYCRAWL_DATA* output)
{
	if(output == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_DIRECTORY, PLEX_ERROR_ISNULL, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_ONFREEDIRECTORYCRAWLER, PLEX_ERRORMESS_DIRECTORYCRAWLER);
		return;
	}

	if(output->file != NULL) fclose(output->file);
	if(output->directory != NULL) closedir(output->directory);

	output->file = NULL;
	output->directory = NULL;
}

bool PLEX_getDirectoryInt(const char* const path, PLEX_DIRECTORY* directory, const size_t line, const char* const filename)
{
	char* fullpath = NULL;

	if(path == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_DIRECTORY, PLEX_ERROR_ISNULL, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_ONGETDIRECTORY, PLEX_ERRORMESS_GENERIC_FILEPATH);
		return false;
	}

	if(path[0] == '\0')
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_DIRECTORY, PLEX_ERROR_NOLEN, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_ONGETDIRECTORY, PLEX_ERRORMESS_GENERIC_FILEPATH);
		return false;
	}

	if(directory == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_DIRECTORY, PLEX_ERROR_ISNULL, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_ONGETDIRECTORY, PLEX_ERRORMESS_DIRECTORY);
		return false;
	}

	directory->open = false;
	directory->path = PLEX_blankString100();
	directory->fullPath = PLEX_blankString100();

	directory->contents = opendir(path);

	if(directory->contents == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_DIRECTORY, PLEX_ERROR_CREATE, PLEX_ERROR_SEVERITY_MAJOR, PLEX_ERRORMESS_ONGETDIRECTORY, PLEX_ERRORMESS_DIRECTORYCONTENTS);
		return false;
	}

	fullpath = PLEX_allocInt(PATH_MAX, line, filename);

	realpath(path, fullpath);

	directory->fullPath = PLEX_createString100(fullpath);

	PLEX_dealloc(&fullpath);

	PLEX_startDirectoryCrawler(directory);

	directory->path = PLEX_createString100(path);
	directory->open = true;

	return true;
}

bool PLEX_reopenDirectory(PLEX_DIRECTORY* directory)
{
	if(directory == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_DIRECTORY, PLEX_ERROR_ISNULL, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_REOPENDIRECTORY, PLEX_ERRORMESS_DIRECTORY);
		return false;
	}

	if(directory->open)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_DIRECTORY, PLEX_ERROR_DIRECTORYOPENSTATE, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_REOPENDIRECTORY, PLEX_ERRORMESS_GENERIC_OPENED);
		return false;
	}

	if(directory->contents == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_DIRECTORY, PLEX_ERROR_CREATE, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_REOPENDIRECTORY, PLEX_ERRORMESS_DIRECTORYCONTENTS);
		return false;
	}

	directory->contents = opendir(PLEX_fts(directory->path));

	if(directory->contents == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_DIRECTORY, PLEX_ERROR_CREATE, PLEX_ERROR_SEVERITY_MAJOR, PLEX_ERRORMESS_REOPENDIRECTORY, PLEX_ERRORMESS_DIRECTORYCONTENTS);
		return false;
	}

	directory->open = true;

	PLEX_startDirectoryCrawler(directory);

	return true;
}

bool PLEX_closeDirectory(PLEX_DIRECTORY* directory)
{
	if(directory == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_DIRECTORY, PLEX_ERROR_ISNULL, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_CLOSEDIRECTORY, PLEX_ERRORMESS_DIRECTORY);
		return false;
	}

	if(!directory->open)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_DIRECTORY, PLEX_ERROR_DIRECTORYOPENSTATE, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_CLOSEDIRECTORY, PLEX_ERRORMESS_GENERIC_CLOSED);
		return false;
	}

	if(directory->contents == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_DIRECTORY, PLEX_ERROR_CONTENTCORRUPTION, PLEX_ERROR_SEVERITY_FATAL, PLEX_ERRORMESS_CLOSEDIRECTORY, PLEX_ERRORMESS_DIRECTORYCONTENTS);
		return false;
	}

	if(closedir(directory->contents) < 0)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_DIRECTORY, PLEX_ERROR_BADOPERATION, PLEX_ERROR_SEVERITY_MAJOR, PLEX_ERRORMESS_CLOSEDIRECTORY, PLEX_ERRORMESS_OP_CLOSEDIRECTORY);
		return false;
	}

	directory->contents = NULL;
	directory->open = false;
	return true;
}

bool PLEX_compPathsInt(const char* const a, const char* const b, const size_t line, const char* const filename)
{
	bool ret = false;

	char* fullpathA = NULL;
	char* fullpathB = NULL;

	PLEX_FAKESTRING_100 pathStrA;
	PLEX_FAKESTRING_100 pathStrB;

	if(a == NULL && b == NULL) return true;
	if(a == NULL || b == NULL) return false;

	fullpathA = PLEX_allocInt(PATH_MAX, line, filename);

	if(fullpathA == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_DIRECTORY, PLEX_ERROR_ISNULL, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_ONCOMPDIRECTORYPATHS, PLEX_ERRORMESS_GENERIC_FILEPATH);
		return false;
	}

	fullpathB = PLEX_allocInt(PATH_MAX, line, filename);

	if(fullpathB == NULL)
	{
		PLEX_dealloc(&fullpathA);
		PLEX_error2(PLEX_ERROR_CATEGORY_DIRECTORY, PLEX_ERROR_ISNULL, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_ONCOMPDIRECTORYPATHS, PLEX_ERRORMESS_GENERIC_FILEPATH2);
		return false;
	}

	realpath(a, fullpathA);
	realpath(b, fullpathB);

	pathStrA = PLEX_createString100(fullpathA);
	pathStrB = PLEX_createString100(fullpathB);

	ret = PLEX_compStrings100Int(pathStrA, pathStrB, false, line, filename);

	PLEX_dealloc(&fullpathA);
	PLEX_dealloc(&fullpathB);

	return ret;
}

#endif
