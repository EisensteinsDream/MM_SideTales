#ifndef PLEX_SCRIPTING_H
#define PLEX_SCRIPTING_H

#include "utility/fileparse.h"

#include "mem/expandarray.h"

typedef struct
{
	PLEX_FAKESTRING_50 name, contents;
} PLEX_SCRIPT_ARGUMENT;

typedef struct
{
	PLEX_EXPANDARRAY* contents;
} PLEX_SCRIPT_ARGUMENTLIST;

typedef struct
{
	PLEX_FAKESTRING_50 sectionName, subSectionName, commandName;
	PLEX_SCRIPT_ARGUMENTLIST args;
} PLEX_SCRIPT_COMMANDTYPE;

typedef struct
{
	PLEX_FAKESTRING_50 name;
	PLEX_EXPANDARRAY* commandTypes;
} PLEX_SCRIPT;

typedef bool (PLEX_F_COMMANDFACTOR)(const PLEX_SCRIPT_ARGUMENTLIST args);

typedef struct
{
	PLEX_FAKESTRING_50 name; // commandName
	PLEX_F_COMMANDFACTOR* factor;
} PLEX_SCRIPT_COMMAND;

typedef struct
{
	PLEX_FAKESTRING_50 name; //  subSectionName
	PLEX_EXPANDARRAY* commands;
} PLEX_SCRIPT_COMMANDROW;

typedef struct
{
	PLEX_FAKESTRING_50 name; //  sectionName
	PLEX_EXPANDARRAY* commandRows;
} PLEX_SCRIPT_COMMANDCOLUMN;

#endif
