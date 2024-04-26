#ifndef PLEX_DIRECTORY_WINDOWS_C_H
#define PLEX_DIRECTORY_WINDOWS_C_H

bool PLEX_startDirectoryCrawler(PLEX_DIRECTORY* directory)
{
	PLEX_FAKESTRING_100 dirPath;

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

	if(directory->contents == INVALID_HANDLE_VALUE)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_DIRECTORY, PLEX_ERROR_CONTENTCORRUPTION, PLEX_ERROR_SEVERITY_FATAL, PLEX_ERRORMESS_ONSTARTDIRECTORYCRAWLER, PLEX_ERRORMESS_DIRECTORYCONTENTS);
		return false;
	}

	dirPath = PLEX_concatString100(directory->path, '\\', 1);
	dirPath = PLEX_concatString100(dirPath, '*', 1);

	directory->contents = FindFirstFile(PLEX_fts(dirPath), &directory->fdata);

	return true;
}

bool PLEX_crawlDirectoryInt(const bool readOnly, PLEX_DIRECTORY* directory, PLEX_DIRECTORYCRAWL_DATA* output, const size_t line, const char* const filename)
{
	PLEX_FAKESTRING_100 path;

	char fullPath[101]; // < 101 because it's stored in a 100 sized fake string with a null terminator

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

	if(directory->contents == INVALID_HANDLE_VALUE)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_DIRECTORY, PLEX_ERROR_CONTENTCORRUPTION, PLEX_ERROR_SEVERITY_FATAL, PLEX_ERRORMESS_ONCRAWLDIRECTORY, PLEX_ERRORMESS_DIRECTORYCONTENTS);
		return false;
	}

	if(output->done) return false;

	if(output->directory == INVALID_HANDLE_VALUE || output->directory == NULL)
	{
		output->fdata = directory->fdata;
		output->directory = directory->contents;
	}

	if(output->file != NULL) fclose(output->file);
	output->file = NULL;

	output->path = PLEX_createString100(output->fdata.cFileName);

	path = directory->path;
	path = PLEX_concatString100(path, '/', 1);

	GetFullPathNameA(PLEX_fts(path), 100, fullPath, NULL);

	output->fullPath = PLEX_createString100(fullPath);
	output->fullPath = PLEX_mergeString100(output->fullPath, output->path);

	output->isDirectory = output->fdata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY;

	if(!output->isDirectory)
	{
		const char* const fstr = readOnly ? "rb" : "ab+";

		output->file = fopen(output->fdata.cFileName, fstr);

		if(output->file == NULL)
		{
			// error
			return false;
		}
	}

	if(!FindNextFile(output->directory, &output->fdata)) output->done = true;

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
	// windows API specifies the directory from FindNextFile does not need to be closed

	output->file = NULL;
	output->done = false;

	output->directory = INVALID_HANDLE_VALUE;
}

bool PLEX_getDirectoryInt(const char* const path, PLEX_DIRECTORY* directory, const size_t line, const char* const filename)
{
	PLEX_FAKESTRING_100 dirPath;

	char fullPath[101]; // < 101 because it's stored in a 100 sized fake string with a null terminator

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
	directory->path = PLEX_createString100(path);
	directory->fullPath = PLEX_blankString100();

	dirPath = PLEX_concatString100(directory->path, '\\', 1);
	dirPath = PLEX_concatString100(dirPath, '*', 1);

	directory->contents = FindFirstFile(PLEX_fts(dirPath), &directory->fdata);

	if(directory->contents == INVALID_HANDLE_VALUE)
	{
		directory->path = PLEX_blankString100();
		PLEX_error2(PLEX_ERROR_CATEGORY_DIRECTORY, PLEX_ERROR_CREATE, PLEX_ERROR_SEVERITY_MAJOR, PLEX_ERRORMESS_ONGETDIRECTORY, PLEX_ERRORMESS_DIRECTORYCONTENTS);
		return false;
	}

	GetFullPathNameA(path, 100, fullPath, NULL);

	directory->fullPath = PLEX_createString100(fullPath);
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

	directory->contents = FindFirstFile(PLEX_fts(directory->fullPath), &directory->fdata);

	if(directory->contents == INVALID_HANDLE_VALUE)
	{
		directory->path = PLEX_blankString100();
		PLEX_error2(PLEX_ERROR_CATEGORY_DIRECTORY, PLEX_ERROR_CREATE, PLEX_ERROR_SEVERITY_MAJOR, PLEX_ERRORMESS_REOPENDIRECTORY, PLEX_ERRORMESS_DIRECTORYCONTENTS);
		return false;
	}

	directory->open = true;

	return false;
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

	if(directory->contents == INVALID_HANDLE_VALUE)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_DIRECTORY, PLEX_ERROR_CONTENTCORRUPTION, PLEX_ERROR_SEVERITY_FATAL, PLEX_ERRORMESS_CLOSEDIRECTORY, PLEX_ERRORMESS_DIRECTORYCONTENTS);
		return false;
	}

	FindClose(directory->contents);

	directory->contents = INVALID_HANDLE_VALUE;
	directory->open = false;

	return false;
}

bool PLEX_compPathsInt(const char* const a, const char* const b, const size_t line, const char* const filename)
{
	bool ret = false;

	char fullpathA[100];
	char fullpathB[100];

	PLEX_FAKESTRING_100 pathStrA;
	PLEX_FAKESTRING_100 pathStrB;

	if(a == NULL && b == NULL) return true;
	if(a == NULL || b == NULL) return false;

	GetFullPathNameA(a, 100, fullpathA, NULL);
	GetFullPathNameA(b, 100, fullpathB, NULL);

	pathStrA = PLEX_createString100(fullpathA);
	pathStrB = PLEX_createString100(fullpathB);

	ret = PLEX_compStrings100Int(pathStrA, pathStrB, false, line, filename);

	return true;
}

#endif
