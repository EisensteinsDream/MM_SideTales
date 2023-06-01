#ifndef PLEX_FILEPARSE_H
#define PLEX_FILEPARSE_H

#include "utility/fakestrings.h"

typedef struct
{
	PLEX_FAKESTRING_100 line, commentString;
	PLEX_FAKESTRING_50 tokens[PLEX_LIMIT_TOKENLIMIT];
	size_t tokenLen[PLEX_LIMIT_TOKENLIMIT];
	size_t usedTokens, commentLen;
} PLEX_PARSELINE;

typedef struct
{
	PLEX_PARSELINE lines[PLEX_LIMIT_LINEPARSE];
	size_t usedLines;
	uint64_t linebreak, tokenbreak, comment, ignoreBreak;
} PLEX_PARSEDFILE;

#ifdef __cplusplus
extern"C"
{
#endif
	extern PLEX_PARSEDFILE PLEX_parseFile(const uint64_t linebreak, const uint64_t tokenbreak, const uint64_t comment, const uint64_t
	ignoreBreak, const char* const ignore, const char* const fileloc);

	extern PLEX_PARSELINE* PLEX_getParseline(const size_t index, PLEX_PARSEDFILE* parseFile);

	extern bool PLEX_getToken(const size_t lineIndex, const size_t tokenIndex, PLEX_FAKESTRING_50* token, PLEX_PARSEDFILE* parseFile);

	extern void PLEX_exportParsefile(const PLEX_PARSEDFILE parsefile, const char* const lineEnd, const char* const fileloc);

	extern size_t PLEX_parseLineCount(const PLEX_PARSEDFILE* parseFile);

	extern size_t PLEX_parseTokenCount(const size_t index, const PLEX_PARSEDFILE* parseFile);
#ifdef __cplusplus
}
#endif

#endif
