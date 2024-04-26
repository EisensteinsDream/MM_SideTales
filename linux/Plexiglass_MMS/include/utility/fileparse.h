#ifndef PLEX_FILEPARSE_H
#define PLEX_FILEPARSE_H

#include "utility/fakestrings.h"

#include "mem/expandarray.h"

#define PLEX_parseFile(linebreak, tokenbreak, comment, escape, ignoreBreak, multiline, ignore, fileloc) PLEX_parseFileInt(linebreak, tokenbreak, comment, escape, ignoreBreak, multiline, ignore, fileloc, __LINE__, __FILE__)

typedef struct
{
	PLEX_FAKESTRING_500 line, commentString;
	PLEX_EXPANDARRAY* tokens;
} PLEX_PARSELINE;

typedef struct
{
	PLEX_EXPANDARRAY* lines;
	uint64_t linebreak, tokenbreak, comment, ignoreBreak;
} PLEX_PARSEDFILE;

#ifdef __cplusplus
extern"C"
{
#endif
	extern PLEX_PARSEDFILE* PLEX_parseFileInt(const uint64_t linebreak, const uint64_t tokenbreak, const uint64_t comment, const uint64_t escape, const uint64_t
	ignoreBreak, const uint64_t multiline, const char* const ignore, const char* const fileloc, const size_t errorline, const char* const errorfile);

	extern void PLEX_destroyParseFile(PLEX_PARSEDFILE** pFile);

	extern PLEX_PARSELINE* PLEX_getParseline(const size_t index, PLEX_PARSEDFILE* parseFile);

	extern bool PLEX_getToken(const size_t lineIndex, const size_t tokenIndex, PLEX_FAKESTRING_50* token, PLEX_PARSEDFILE* parseFile);

	extern void PLEX_exportParsefile(const PLEX_PARSEDFILE parsefile, const char* const lineEnd, const char* const fileloc);

	extern size_t PLEX_parseLineCount(const PLEX_PARSEDFILE* parseFile);

	extern size_t PLEX_parseTokenCount(const size_t index, PLEX_PARSEDFILE* parseFile);
#ifdef __cplusplus
}
#endif

#endif
