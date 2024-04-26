#include "utility/fileparse.h"

// NOTE: function descriptions in header.

static bool p_addLine(PLEX_PARSEDFILE* pfile, const size_t errorline, const char* const errorfile)
{
	PLEX_PARSELINE* next = NULL;

	if(!PLEX_incrementExpandArrayInt(pfile->lines, errorline, errorfile))
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_FILEPARSE, PLEX_ERROR_INCREMENT, PLEX_ERROR_SEVERITY_FATAL, PLEX_ERRORMESS_FILEPARSE_ONADDLINE, PLEX_ERRORMESS_GENERIC_LINES);
		return false;
	}

	next = PLEX_getExpandArrayLast(pfile->lines);

	if(next == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_FILEPARSE, PLEX_ERROR_ACQUIRE, PLEX_ERROR_SEVERITY_FATAL, PLEX_ERRORMESS_FILEPARSE_ONADDLINE, PLEX_ERRORMESS_GENERIC_NEXTLINE);
		return false;
	}

	next->tokens = PLEX_genExpandArrayInt(PLEX_getMemProfileTokenPreload(), sizeof(PLEX_FAKESTRING_50), errorline, errorfile);

	if(next->tokens == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_FILEPARSE, PLEX_ERROR_ALLOCFAIL, PLEX_ERROR_SEVERITY_FATAL, PLEX_ERRORMESS_FILEPARSE_ONADDLINE, PLEX_ERRORMESS_FILEPARSE_NEXTLINESTOKEN);
		return false;
	}

	next->commentString = PLEX_blankString500();

	return true;
}

static bool p_addToken(PLEX_PARSELINE* line, const PLEX_FAKESTRING_50 token, const size_t errorline, const char* const errorfile)
{
	PLEX_FAKESTRING_50* nextToken = NULL;

	if(!PLEX_incrementExpandArrayInt(line->tokens, errorline, errorfile))
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_FILEPARSE, PLEX_ERROR_INCREMENT, PLEX_ERROR_SEVERITY_FATAL, PLEX_ERRORMESS_FILEPARSE_ONADDTOKEN, PLEX_ERRORMESS_GENERIC_TOKENS);
		return false;
	}

	nextToken = PLEX_getExpandArrayLast(line->tokens);

	if(nextToken == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_FILEPARSE, PLEX_ERROR_ACQUIRE, PLEX_ERROR_SEVERITY_FATAL, PLEX_ERRORMESS_FILEPARSE_ONADDTOKEN, PLEX_ERRORMESS_GENERIC_NEXTTOKEN);
		return false;
	}

	*nextToken = token;

	return true;
}

static int p_nextChar(FILE* file, const char* const buffer, const size_t iterator, const bool isFile)
{
	if(isFile) return fgetc(file);

	return buffer[iterator];
}

static uint64_t p_loadNext(FILE* file, const char* const buffer, uint64_t* symbolLen, size_t* iterator, const bool isFile)
{
	uint64_t fint = 0;
	uint64_t ret = 0;
	int32_t first = 0;
	size_t lSymbolLen = 0;
	char* cfint = (char*)&fint;

	size_t literator = *iterator;

	first = p_nextChar(file, buffer, literator, isFile);
	++literator;

	if(first == EOF) return EOF;

	cfint[0] = first;

	if(first < 0x80)
	{
		ret = first;
		lSymbolLen = 1;
	}else{
		cfint[1] = p_nextChar(file, buffer, literator, isFile);
		++literator;

		if(first < 0xE0)
		{
			ret = fint;
			lSymbolLen = 2;
		}else{
			cfint[2] = p_nextChar(file, buffer, literator, isFile);
			++literator;

			if(first < 0xF0)
			{
				uint64_t fint3 = 0;

				memcpy(&fint3, &fint, 3);

				ret = fint3;
				lSymbolLen = 3;
			}else{
				cfint[3] = p_nextChar(file, buffer, literator, isFile);
				++literator;

				if(first < 0xF8)
				{
					uint64_t fint4 = 0;

					memcpy(&fint4, &fint, 4);
					ret = fint4;
					lSymbolLen = 4;
				}else{
					cfint[4] = p_nextChar(file, buffer, literator, isFile);
					++literator;

					if(first < 0xFC)
					{
						uint64_t fint5 = 0;

						memcpy(&fint5, &fint, 5);
						ret = fint5;
						lSymbolLen = 5;
					}else{
						cfint[5] = p_nextChar(file, buffer, literator, isFile);
						++literator;

						if(first < 0xFE)
						{
							uint64_t fint6 = 0;

							memcpy(&fint6, &fint, 6);
							ret = fint6;
							lSymbolLen = 6;
						}else{
							// probably never ever will be used but it's theoretically possible //

							uint64_t fint7 = 0;

							++literator;

							cfint[6] = p_nextChar(file, buffer, literator, isFile);

							memcpy(&fint7, &fint, 7);
							ret = fint7;
							lSymbolLen = 7;
						}
					}
				}
			}
		}
	}

	*symbolLen = lSymbolLen;
	*iterator = literator;

	return ret;
}

// ---------------------------- //
// PUBLIC FUNCTIONS:
// ---------------------------- //

PLEX_PARSEDFILE* PLEX_parseFileInt(const uint64_t linebreak, const uint64_t tokenbreak, const uint64_t comment, const uint64_t escape, const uint64_t ignoreBreak, const uint64_t multiline, const char* const ignore, const char* const fileloc, const size_t errorline, const char* const errorfile)
{
	uint64_t next = 0;
	uint64_t last = 0;
	uint64_t symbolLen = 0;

	size_t iterator = 0;
	size_t lineCount = 0;

	bool commented = false;
	bool ignoreTokenBreak = false;
	bool isMultiline = false;

	FILE* file = NULL;

	PLEX_EXPANDARRAY* line = NULL;

	PLEX_FAKESTRING_50 token = PLEX_blankString50();

	PLEX_PARSEDFILE* ret = NULL;

	if(fileloc == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_FILEPARSE, PLEX_ERROR_ISNULL, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_FILEPARSE_ONPARSEFILE, PLEX_ERRORMESS_GENERIC_FILEPATH);
		return NULL;
	}

	file = fopen(fileloc, "rb+");

	if(file == NULL)
	{
		const PLEX_FAKESTRING_100 targMessage = PLEX_fileLoadErrorPath(fileloc, PLEX_ERRORMESS_GENERIC_FILE);

		PLEX_error2(PLEX_ERROR_CATEGORY_FILEPARSE, PLEX_ERROR_FILELOAD, PLEX_ERROR_SEVERITY_MAJOR, PLEX_ERRORMESS_FILEPARSE_ONPARSEFILE, PLEX_fts(targMessage));
		return NULL;
	}

	ret = PLEX_allocInt(sizeof(PLEX_PARSEDFILE), errorline, errorfile);

	if(ret == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_FILEPARSE, PLEX_ERROR_ALLOCFAIL, PLEX_ERROR_SEVERITY_FATAL, PLEX_ERRORMESS_FILEPARSE_ONPARSEFILE, PLEX_ERRORMESS_GENERIC_FILE);
		fclose(file);
		return NULL;
	}

	ret->lines = PLEX_genExpandArrayInt(PLEX_getMemProfileLineParsePreload(), sizeof(PLEX_PARSELINE), errorline, errorfile);

	if(ret->lines == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_FILEPARSE, PLEX_ERROR_ALLOCFAIL, PLEX_ERROR_SEVERITY_FATAL, PLEX_ERRORMESS_FILEPARSE_ONPARSEFILE, PLEX_ERRORMESS_GENERIC_LINES);
		fclose(file);
		PLEX_dealloc(&ret);
		return NULL;
	}

	lineCount = PLEX_expandArrayCount(ret->lines);

	if(!p_addLine(ret, errorline, errorfile))
	{
		fclose(file);
		PLEX_destroyExpandArray(&ret->lines);
		PLEX_dealloc(&ret);

		PLEX_error2(PLEX_ERROR_CATEGORY_FILEPARSE, PLEX_ERROR_CREATE, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_FILEPARSE_ONPARSEFILE, PLEX_ERRORMESS_GENERIC_LINES);
		return NULL;
	}

	if(!lineCount)
	{
		// error
		return NULL;
	}

	line = PLEX_getExpandArrayEntry(0, ret->lines);

	if(line == NULL)
	{
		fclose(file);
		PLEX_destroyExpandArray(&ret->lines);
		PLEX_dealloc(&ret);

		PLEX_error2(PLEX_ERROR_CATEGORY_FILEPARSE, PLEX_ERROR_ACQUIRE, PLEX_ERROR_SEVERITY_FATAL, PLEX_ERRORMESS_FILEPARSE_ONPARSEFILE, PLEX_ERRORMESS_GENERIC_LINE);
		return NULL;
	}

	ret->linebreak = linebreak;
	ret->tokenbreak = tokenbreak;
	ret->comment = comment;
	ret->ignoreBreak = ignoreBreak;

	next = p_loadNext(file, NULL, &symbolLen, &iterator, true);

	while(next != EOF)
	{
		const bool addToken = next == tokenbreak;

		const bool addLine = next == linebreak || next == '\r' || next == '\n';

		const bool escaped = last == escape && next != escape;

		bool addNext = true;

		if(!escaped && !commented && addToken)
		{
			if(!ignoreTokenBreak) addNext = false;
		}

		if(!escaped && !commented && next == multiline)
		{
			isMultiline = true;
			addNext = false;
		}

		if(!escaped && !commented && next == escape) addNext = false;

		if(!escaped && !commented && next == ignoreBreak)
		{
			ignoreTokenBreak = !ignoreTokenBreak;
			addNext = false;
		}

		if(!escaped && !commented && next == comment)
		{
			commented = true;
			addNext = false;
		}

		if(!escaped && next == linebreak)
		{
			addNext = false;
		}

		if(addNext)
		{
			size_t ignoreIt = 0;

			uint64_t ignoreNext = ignore[ignoreIt];

			// eventually adjust to utf-8

			while(ignoreNext != '\0')
			{
				if(next == ignoreNext)
				{
					addNext = false;
					break;
				}

				++ignoreIt;

				ignoreNext = ignore[ignoreIt];
			}
		}

		if(addNext) token = PLEX_concatString50(token, next, symbolLen);

		if((!ignoreTokenBreak && addToken) || (addLine && token.len))
		{
			if(token.len)
			{
				PLEX_PARSELINE* nextLine = PLEX_getExpandArrayLast(ret->lines);

				PLEX_FAKESTRING_500 bigToken = PLEX_createString500(PLEX_fts(token));

				bigToken = PLEX_concatString500(bigToken, ' ', 1);

				if(commented)
				{
					nextLine->commentString = PLEX_mergeString500(nextLine->commentString, bigToken);
				}else{
					nextLine->line = PLEX_mergeString500(nextLine->line, bigToken);

					if(!p_addToken(nextLine, token, errorline, errorfile))
					{
						PLEX_error2(PLEX_ERROR_CATEGORY_FILEPARSE, PLEX_ERROR_CREATE, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_FILEPARSE_ONPARSEFILE, PLEX_ERRORMESS_GENERIC_TOKEN);
						return false;
					}
				}
			}

			token = PLEX_blankString50();
		}

		if(addLine)
		{
			if(!isMultiline)
			{
				const size_t lineCount = PLEX_expandArrayCount(ret->lines);

				if(lineCount) p_addLine(ret, errorline, errorfile);

				commented = false;
				ignoreTokenBreak = false;
			}

			isMultiline = false;
		}

		last = next;
		next = p_loadNext(file, NULL, &symbolLen, &iterator, true);
	}

	if(token.len)
	{
		PLEX_PARSELINE* nextLine = PLEX_getExpandArrayLast(ret->lines);

		PLEX_FAKESTRING_500 bigToken = PLEX_createString500(PLEX_fts(token));

		bigToken = PLEX_concatString500(bigToken, ' ', 1);

		if(commented) nextLine->commentString = PLEX_mergeString500(nextLine->commentString, bigToken);
		else{
			nextLine->line = PLEX_mergeString500(nextLine->line, bigToken);

			if(!p_addToken(nextLine, token, errorline, errorfile))
			{
				PLEX_error2(PLEX_ERROR_CATEGORY_FILEPARSE, PLEX_ERROR_CREATE, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_FILEPARSE_ONPARSEFILE, PLEX_ERRORMESS_GENERIC_TOKEN);
				return false;
			}
		}
	}

	fclose(file);

	return ret;
}

void PLEX_destroyParseFile(PLEX_PARSEDFILE** pFile)
{
	PLEX_PARSEDFILE* pFilePtr = NULL;

	size_t lineCount = 0;

	if(pFile == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_FILEPARSE, PLEX_ERROR_FREENULL, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_FILEPARSE_ONPARSEFILEDESTROY, PLEX_ERRORMESS_GENERIC_FILEPTR);
		return;
	}

	pFilePtr = *pFile;

	if(pFilePtr == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_FILEPARSE, PLEX_ERROR_FREENULL, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_FILEPARSE_ONPARSEFILEDESTROY, PLEX_ERRORMESS_GENERIC_FILE);
		return;
	}

	lineCount = PLEX_expandArrayCount(pFilePtr->lines);

	for(size_t ze = 0; ze < lineCount; ++ze)
	{
		PLEX_PARSELINE* line = PLEX_getExpandArrayEntry(ze, pFilePtr->lines);

		PLEX_destroyExpandArray(&line->tokens);
	}

	PLEX_destroyExpandArray(&pFilePtr->lines);

	PLEX_dealloc(&pFilePtr);

	*pFile = NULL;
}

PLEX_PARSELINE* PLEX_getParseline(const size_t index, PLEX_PARSEDFILE* parseFile)
{
	size_t lineCount = 0;

	PLEX_PARSELINE* ret = 0;

	if(parseFile == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_FILEPARSE, PLEX_ERROR_GETNULL, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_FILEPARSE_ONGETPARSELINE, PLEX_ERRORMESS_GENERIC_FILEPATH);
		return NULL;
	}

	lineCount = PLEX_expandArrayCount(parseFile->lines);

	if(index >= lineCount)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_FILEPARSE, PLEX_ERROR_OVERLIMIT, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_FILEPARSE_ONGETPARSELINE, PLEX_ERRORMESS_GENERIC_LINES);
		return NULL;
	}

	ret = PLEX_getExpandArrayEntry(index, parseFile->lines);

	if(ret == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_FILEPARSE, PLEX_ERROR_ACQUIRE, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_FILEPARSE_ONGETPARSELINE, PLEX_ERRORMESS_GENERIC_FILEPATH);
		return NULL;
	}

	return ret;
}

bool PLEX_getToken(const size_t lineIndex, const size_t tokenIndex, PLEX_FAKESTRING_50* token, PLEX_PARSEDFILE* parseFile)
{
	PLEX_PARSELINE* parseLine = NULL;

	size_t tokenCount = 0;

	PLEX_FAKESTRING_50* token2 = NULL;

	if(parseFile == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_FILEPARSE, PLEX_ERROR_GETNULL, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_FILEPARSE_ONGETTOKEN, PLEX_ERRORMESS_GENERIC_FILEPATH);
		return false;
	}

	parseLine = PLEX_getParseline(lineIndex, parseFile);

	if(parseLine == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_FILEPARSE, PLEX_ERROR_CREATE, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_FILEPARSE_ONGETTOKEN, PLEX_ERRORMESS_GENERIC_LINE);
		return false;
	}

	tokenCount = PLEX_expandArrayCount(parseLine->tokens);

	if(tokenIndex >= tokenCount)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_FILEPARSE, PLEX_ERROR_OVERLIMIT, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_FILEPARSE_ONGETTOKEN, PLEX_ERRORMESS_GENERIC_TOKENS);
		return false;
	}

	token2 = PLEX_getExpandArrayEntry(tokenIndex, parseLine->tokens);

	if(token2 == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_FILEPARSE, PLEX_ERROR_ACQUIRE, PLEX_ERROR_SEVERITY_FATAL, PLEX_ERRORMESS_FILEPARSE_ONGETTOKEN, PLEX_ERRORMESS_GENERIC_TOKEN2);
		return false;
	}

	*token = *token2;

	return true;
}

void PLEX_exportParsefile(const PLEX_PARSEDFILE parsefile, const char* const lineEnd, const char* const fileloc)
{
	// must have forgot to finish...
}

size_t PLEX_parseLineCount(const PLEX_PARSEDFILE* parseFile)
{
	if(parseFile == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_FILEPARSE, PLEX_ERROR_GETNULL, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_FILEPARSE_ONGETPARSELINECOUNT, PLEX_ERRORMESS_GENERIC_FILE);
		return 0;
	}

	if(parseFile->lines == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_FILEPARSE, PLEX_ERROR_CONTENTCORRUPTION, PLEX_ERROR_SEVERITY_IMPOSSIBLE, PLEX_ERRORMESS_FILEPARSE_ONGETPARSELINECOUNT, PLEX_ERRORMESS_GENERIC_LINES);
		return 0;
	}

	return PLEX_expandArrayCount(parseFile->lines);
}

size_t PLEX_parseTokenCount(const size_t index, PLEX_PARSEDFILE* parseFile)
{
	PLEX_PARSELINE* line = NULL;

	size_t lineCount = 0;

	if(parseFile == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_FILEPARSE, PLEX_ERROR_GETNULL, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_FILEPARSE_ONGETTOKENCCOUNT, PLEX_ERRORMESS_GENERIC_FILE);
		return 0;
	}

	lineCount = PLEX_expandArrayCount(parseFile->lines);

	if(index >= lineCount)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_FILEPARSE, PLEX_ERROR_OVERLIMIT, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_FILEPARSE_ONGETTOKENCCOUNT, PLEX_ERRORMESS_GENERIC_LINES);
		return 0;
	}

	line = PLEX_getParseline(index, parseFile);

	return PLEX_expandArrayCount(line->tokens);
}
