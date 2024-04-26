#include "utility/error.h"

// This is a header for the error subsystem.

static PLEX_ERROR_SEVERITY p_printThreshold = PLEX_ERROR_SEVERITY_MINOR;
static PLEX_ERROR_SEVERITY p_showThreshold = PLEX_ERROR_SEVERITY_MAJOR;
static PLEX_ERROR_SEVERITY p_fatalThreshold = PLEX_ERROR_SEVERITY_FATAL;

static bool p_printErrors = true;
static bool p_showErrors = true;
static bool p_dumpErrors = false;

static bool p_sdlErrorFlag = false;

static uint16_t p_year = 0;
static uint8_t p_month = 0;
static uint8_t p_day = 0;
static uint8_t p_hour = 0;
static uint8_t p_minute = 0;
static uint8_t p_second = 0;

static size_t p_shownErrorCount = 0;

static FILE* p_dumpFile = NULL;
static FILE* p_suppressFile = NULL;

// base function for both terminate and non-terminate errors

static bool p_addErrorString(const char* string, size_t* it, char* buffer)
{
	const size_t messLen = strlen(string);

	size_t lit = *it;

	if(lit + messLen >= PLEX_LIMIT_ERROR_MESSLEN - 1) return false;

	memcpy(buffer + lit, string, messLen);

	lit += messLen;

	*it = lit;

	return true;
}

static void p_readErrorPopupTitle(const PLEX_ERROR toPrint, char* buffer)
{
	size_t it = 0;

	p_addErrorString(PLEX_getCategoryString(toPrint.category), &it, buffer);
	p_addErrorString(PLEX_ERRORMESS_ERROR_POPUPTITLE, &it, buffer);
}

static void p_readErrorPopup(const PLEX_ERROR toPrint, char* buffer)
{
	size_t it = 0;

	if(toPrint.message[0] != '\0')
	{
		p_addErrorString(toPrint.message, &it, buffer);
		p_addErrorString(PLEX_ERRORMESS_ERROR_EXCLAMATION, &it, buffer);
	}

	if(toPrint.message2[0] != '\0')
	{
		if(toPrint.message[0] != '\0') p_addErrorString(PLEX_ERRORMESS_ERROR_MESSDIVIDE, &it, buffer);
		p_addErrorString(toPrint.message2, &it, buffer);
		p_addErrorString(PLEX_ERRORMESS_ERROR_EXCLAMATION, &it, buffer);
	}

	p_addErrorString(PLEX_getTypeStringLong(toPrint.type), &it, buffer);
	p_addErrorString(PLEX_ERRORMESS_ERROR_EXCLAMATION, &it, buffer);
}

static void p_readErrorString(const PLEX_ERROR toPrint, char* buffer)
{
	char numBuff[6];

	size_t it = 0;

	sprintf(numBuff, "%u", toPrint.lineNumber);

	p_addErrorString(PLEX_ERRORMESS_ERROR_HEADER, &it, buffer);
	p_addErrorString(PLEX_ERRORMESS_ERROR_LINE, &it, buffer);
	p_addErrorString(numBuff, &it, buffer);
	p_addErrorString(PLEX_ERRORMESS_ERROR_FILE, &it, buffer);
	p_addErrorString(toPrint.fileloc, &it, buffer);

	p_addErrorString(PLEX_ERRORMESS_ERROR_CATEGORY, &it, buffer);
	p_addErrorString(PLEX_getCategoryString(toPrint.category), &it, buffer);

	p_addErrorString(PLEX_ERRORMESS_ERROR_MESSAGESTART, &it, buffer);
	p_addErrorString(PLEX_getTypeString(toPrint.type), &it, buffer);
	p_addErrorString(PLEX_ERRORMESS_ERROR_MESSAGESPLIT, &it, buffer);
	p_addErrorString(PLEX_getTypeStringLong(toPrint.type), &it, buffer);

	if(toPrint.message[0] != '\0')
	{
		p_addErrorString(PLEX_ERRORMESS_ERROR_MESSAGESPLIT2, &it, buffer);
		p_addErrorString(toPrint.message, &it, buffer);
	}

	if(toPrint.message2[0] != '\0')
	{
		if(toPrint.message[0] != '\0') p_addErrorString(PLEX_ERRORMESS_ERROR_MESSDIVIDE, &it, buffer);
		p_addErrorString(toPrint.message2, &it, buffer);
	}

	p_addErrorString(PLEX_ERRORMESS_ERROR_MESSAGEEND, &it, buffer);

	p_addErrorString(PLEX_ERRORMESS_ERROR_SEVERITY, &it, buffer);
	p_addErrorString(PLEX_getSeverityString(toPrint.severity), &it, buffer);

	p_addErrorString(PLEX_ERRORMESS_ERROR_SEVERITY_LONG, &it, buffer);
	p_addErrorString(PLEX_getSeverityStringLong(toPrint.severity), &it, buffer);

	p_addErrorString(PLEX_ERRORMESS_ERROR_FOOTER, &it, buffer);

	buffer[it] = '\0';
}

// ---------------------------- //
// PUBLIC FUNCTIONS:
// ---------------------------- //

//PLEX errors:

void PLEX_addErrorInt(const PLEX_ERROR_CATEGORY category, const PLEX_ERROR_TYPE type, const PLEX_ERROR_SEVERITY severity, const char* const message, const char* const message2, const char* const fileloc, const size_t lineNumber)
{
	PLEX_ERROR toAdd;

	size_t thresholdAt = 0;

	bool printed = false;
	bool shown = false;

	if(category == PLEX_ERROR_CATEGORY_NONE)
	{
		if(p_printErrors) printf("%s\r\n", PLEX_ERRORMESS_ERROR_ATTEMPTNOCATEGORY);
		return;
	}

	if(severity == PLEX_ERROR_SEVERITY_NONE)
	{
		if(p_printErrors) printf("%s\r\n", PLEX_ERRORMESS_ERROR_NOSEVERITY);
		return;
	}

	if(severity == PLEX_ERROR_CATEGORY_NONE)
	{
		if(p_printErrors) printf("%s\r\n", PLEX_ERRORMESS_ERROR_NOCATEGORY);
		return;
	}

	toAdd.category = category;
	toAdd.type = type;
	toAdd.severity = severity;
	toAdd.message = message;
	toAdd.message2 = message2;
	toAdd.lineNumber = lineNumber;
	toAdd.fileloc = fileloc;

	thresholdAt = toAdd.severity;

	printed = PLEX_printError(toAdd, false);
	shown = PLEX_showError(toAdd, false);

	if(thresholdAt <= (size_t)p_fatalThreshold)
	{
		if(p_printErrors) printf("%s\r\n", PLEX_ERRORMESS_ERROR_FATALERROR);
		PLEX_terminate();
	}

	if(p_dumpErrors)
	{
		FILE* file = NULL;

		if(printed || shown)
		{
			if(p_dumpFile == NULL)
			{
				char dumpFilename[64];

				sprintf(dumpFilename, "error/error_log_%04u_%02u_%02u_%02u_%02u_%02u", (unsigned)p_year, (unsigned)p_month, (unsigned)p_day, (unsigned)p_hour, (unsigned)p_minute, (unsigned)p_second);

				p_dumpFile = fopen(dumpFilename, "w+");

				if(p_dumpFile == NULL) printf("%s\r\n", PLEX_ERRROMESS_ERROR_DUMPFILEFAIL);
			}

			file = p_dumpFile;
		}else{
			if(p_suppressFile == NULL)
			{
				char suppressFilename[64];

				sprintf(suppressFilename, "error/error_suppress_log_%04u_%02u_%02u_%02u_%02u_%02u", (unsigned)p_year, (unsigned)p_month, (unsigned)p_day, (unsigned)p_hour, (unsigned)p_minute, (unsigned)p_second);

				p_suppressFile = fopen((const char*)suppressFilename, "w+");

				if(p_suppressFile == NULL) printf("%s\r\n", PLEX_ERRROMESS_ERROR_SUPPRESSFILEFAIL);
			}

			file = p_suppressFile;
		}

		if(file != NULL) PLEX_dumpError(toAdd, file);
	}
}

bool PLEX_printError(const PLEX_ERROR toPrint, const bool ignoreThreshold)
{
	char messBuff[PLEX_LIMIT_ERROR_MESSLEN];

	size_t thresholdAt = 0;

	if(!p_printErrors) return false;

	thresholdAt = (size_t)toPrint.severity;

	if(!ignoreThreshold)
	{
		const size_t printThreshold = (size_t)p_printThreshold;

		if(thresholdAt > printThreshold) return false;
	}

	p_readErrorString(toPrint, (char*)messBuff);

	if(p_printErrors) printf("%s\r\n", messBuff);

	return true;
}

bool PLEX_showError(const PLEX_ERROR toShow, const bool ignoreThreshold)
{
	char messBuffTitle[PLEX_LIMIT_ERROR_MESSLEN];
	char messBuff[PLEX_LIMIT_ERROR_MESSLEN];

	size_t thresholdAt = 0;

	if(!p_sdlErrorFlag || p_shownErrorCount > PLEX_LIMIT_ERROR_SHOWLEN) return false;

	if(p_shownErrorCount == PLEX_LIMIT_ERROR_SHOWLEN)
	{
		PLEX_showErrorPopup(PLEX_ERRORMESS_SUBSYSTEM_ERROR, PLEX_ERRORMESS_ERROR_SHOWOVERFLOW);
		p_shownErrorCount = PLEX_LIMIT_ERROR_SHOWLEN + 1;
		PLEX_terminate();
		return false;
	}

	if(!p_showErrors) return false;

	thresholdAt = (size_t)toShow.severity;

	if(!ignoreThreshold)
	{
		const size_t showThreshold = (size_t)p_showThreshold;

		if(thresholdAt > showThreshold) return false;
	}

	p_readErrorPopupTitle(toShow, messBuffTitle);
	p_readErrorPopup(toShow, messBuff);

	PLEX_showErrorPopup((char*)messBuffTitle, (char*)messBuff);

	++p_shownErrorCount;

	return true;
}

bool PLEX_dumpError(const PLEX_ERROR toDump, FILE* file)
{
	const char* const breakStr = "\r\n\r\n~\r\n\r\n";

	char messBuff[PLEX_LIMIT_ERROR_MESSLEN];

	size_t len = 0;

	bool silent = false;

	if(file == NULL) return false;

	if(silent) return false;

	p_readErrorString(toDump, (char*)messBuff);

	len = strlen((char*)messBuff);

	if(!len) return false;

	if(!fwrite((char*)messBuff, len, 1, file)) return false;

	fwrite(breakStr, strlen(breakStr), 1, file);

	return true;
}

void PLEX_setErrorLogDateInt(const uint16_t year, const uint8_t month, const uint8_t day, const uint8_t hour, const uint8_t minute, const uint8_t second)
{
	p_year = year;
	p_month = month;
	p_day = day;
	p_hour = hour;
	p_minute = minute;
	p_second = second;
}

void PLEX_getErrorLogDateInt(uint16_t* year, uint8_t* month, uint8_t* day, uint8_t* hour, uint8_t* minute, uint8_t* second)
{
	*year = p_year;
	*month = p_month;
	*day = p_day;
	*hour = p_hour;
	*minute = p_minute;
	*second = p_second;
}

void PLEX_setErrorPrintingThreshold(const PLEX_ERROR_SEVERITY threshold)
{
	if(p_printThreshold == threshold) return;

	if(p_printErrors) printf("%s%s%s\r\n", PLEX_ERRORMESS_ERROR_PRINTTHRESHOLDCHANGED, PLEX_getSeverityString(threshold), PLEX_ERRORMESS_ERROR_PRINTTHRESHOLDCHANGED2);

	p_printThreshold = threshold;
}

void PLEX_setErrorShowingThreshold(const PLEX_ERROR_SEVERITY threshold)
{
	if(p_showThreshold == threshold) return;

	if(p_printErrors) printf("%s%s%s\r\n", PLEX_ERRORMESS_ERROR_SHOWNTHRESHOLDCHANGED, PLEX_getSeverityString(threshold), PLEX_ERRORMESS_ERROR_SHOWNTHRESHOLDCHANGED2);

	p_showThreshold = threshold;
}

void PLEX_setErrorFatalThreshold(const PLEX_ERROR_SEVERITY threshold)
{
	if(p_fatalThreshold == threshold) return;

	if(threshold < PLEX_ERROR_SEVERITY_FATAL)
	{
		if(p_printErrors) printf("%s\r\n", PLEX_ERRORMESS_ERROR_ATTEMPTIMPOSSIBLEERROR);
		p_fatalThreshold = PLEX_ERROR_SEVERITY_FATAL;
		return;
	}

	if(p_printErrors) printf("%s%s%s\r\n", PLEX_ERRORMESS_ERROR_TERMINATEDTHRESHOLDCHANGED, PLEX_getSeverityString(threshold), PLEX_ERRORMESS_ERROR_TERMINATEDTHRESHOLDCHANGED2);

	p_fatalThreshold = threshold;
}

void PLEX_printErrors(const bool set){ p_printErrors = set; }
void PLEX_showErrors(const bool set){ p_showErrors = set; }
void PLEX_dumpErrors(const bool set){ p_dumpErrors = set; }

void PLEX_flagSDLError(){ p_sdlErrorFlag = true; }
void PLEX_unflagSDLError(){ p_sdlErrorFlag = false; }

const char* PLEX_getCategoryString(const PLEX_ERROR_CATEGORY category)
{
	switch(category)
	{
		case PLEX_ERROR_CATEGORY_START: return PLEX_ERRORMESS_CATEGORY_START;
		case PLEX_ERROR_CATEGORY_UPDATE: return PLEX_ERRORMESS_CATEGORY_UPDATE;
		case PLEX_ERROR_CATEGORY_SDL: return PLEX_ERRORMESS_CATEGORY_SDL;
		case PLEX_ERROR_CATEGORY_PORTAUDIO: return PLEX_ERRORMESS_CATEGORY_PORTAUDIO;
		case PLEX_ERROR_CATEGORY_MATH: return PLEX_ERRORMESS_CATEGORY_MATH;
		case PLEX_ERROR_CATEGORY_DATETIME: return PLEX_ERRORMESS_CATEGORY_DATETIME;
		case PLEX_ERROR_CATEGORY_FAKESTRING: return PLEX_ERRORMESS_CATEGORY_FAKESTRING;
		case PLEX_ERROR_CATEGORY_DEFAULT: return PLEX_ERRORMESS_CATEGORY_DEFAULT;
		case PLEX_ERROR_CATEGORY_MEM: return PLEX_ERRORMESS_CATEGORY_MEM;
		case PLEX_ERROR_CATEGORY_ARRAY: return PLEX_ERRORMESS_CATEGORY_ARRAY;
		case PLEX_ERROR_CATEGORY_SOUND: return PLEX_ERRORMESS_CATEGORY_SOUND;
		case PLEX_ERROR_CATEGORY_TEXTURE: return PLEX_ERRORMESS_CATEGORY_TEXTURE;
		case PLEX_ERROR_CATEGORY_WINDOW: return PLEX_ERRORMESS_CATEGORY_WINDOW;
		case PLEX_ERROR_CATEGORY_PROMINENCEMACHINE: return PLEX_ERRORMESS_CATEGORY_PROMINENCEMACHINE;
		case PLEX_ERROR_CATEGORY_SCREENEFFECTS: return PLEX_ERRORMESS_CATEGORY_SCREENEFFECTS;
		case PLEX_ERROR_CATEGORY_LOWLEVELGRAPHICS_GL: return PLEX_ERRORMESS_CATEGORY_LOWLEVELGRAPHICS_GL;
		case PLEX_ERROR_CATEGORY_LOWLEVELGRAPHICS_SDL: return PLEX_ERRORMESS_CATEGORY_LOWLEVELGRAPHICS_SDL;
		case PLEX_ERROR_CATEGORY_FILEPARSE: return PLEX_ERRORMESS_CATEGORY_FILEPARSE;
		case PLEX_ERROR_CATEGORY_INPUT: return PLEX_ERRORMESS_CATEGORY_INPUT;
		case PLEX_ERROR_CATEGORY_GENERATOR: return PLEX_ERRORMESS_CATEGORY_GENEARTOR;
		case PLEX_ERROR_CATEGORY_SPRITETHEATER: return PLEX_ERRORMESS_CATEGORY_SPRITETHEATER;
		case PLEX_ERROR_CATEGORY_TIMING: return PLEX_ERRORMESS_CATEGORY_TIMING;
		case PLEX_ERROR_CATEGORY_GRAPHICS: return PLEX_ERRORMESS_CATEGORY_GRAPHICS;
		case PLEX_ERROR_CATEGORY_ANIMATION: return PLEX_ERRORMESS_CATEGORY_ANIMATION;
		case PLEX_ERROR_CATEGORY_DIRECTORY: return PLEX_ERRORMESS_CATEGORY_DIRECTORY;
		case PLEX_ERROR_CATEGORY_SYSTEMREAD: return PLEX_ERRORMESS_CATEGORY_SYSTEMREAD;
		case PLEX_ERROR_CATEGORY_INPROJECT: return PLEX_ERRORMESS_CATEGORY_INPROJECT;
		case PLEX_ERROR_CATEGORY_NONE: break;
	};

	return PLEX_ERRORMESS_CATEGORY_NONE;
}

const char* PLEX_getTypeString(const PLEX_ERROR_TYPE type)
{
	switch(type)
	{
		case PLEX_ERROR_START: return PLEX_ERRORMESS_TYPE_START;
		case PLEX_ERROR_LOADEXTERNALMODULE: return PLEX_ERRORMESS_TYPE_LOADEXTERNALMODULE;
		case PLEX_ERROR_EXTERNALMODULE: return PLEX_ERRORMESS_TYPE_EXTERNALMODULE;
		case PLEX_ERROR_CREATE: return PLEX_ERRORMESS_TYPE_CREATE;
		case PLEX_ERROR_UPDATE: return PLEX_ERRORMESS_TYPE_UPDATE;
		case PLEX_ERROR_MINMAXREVERSE: return PLEX_ERRORMESS_TYPE_MINMAXREVERSE;
		case PLEX_ERROR_SETNULL: return PLEX_ERRORMESS_TYPE_SETNULL;
		case PLEX_ERROR_GETNULL: return PLEX_ERRORMESS_TYPE_GETNULL;
		case PLEX_ERROR_COMPNULL: return PLEX_ERRORMESS_TYPE_COMPNULL;
		case PLEX_ERROR_DEFAULTPOP: return PLEX_ERRORMESS_TYPE_DEFAULTPOP;
		case PLEX_ERROR_ASSIGNDEFAULT: return PLEX_ERRORMESS_TYPE_ASSIGNDEFAULT;
		case PLEX_ERROR_FILELOAD: return PLEX_ERRORMESS_TYPE_FILELOAD;
		case PLEX_ERROR_FILESAVE: return PLEX_ERRORMESS_TYPE_FILESAVE;
		case PLEX_ERROR_DOUBLESTART: return PLEX_ERRORMESS_TYPE_DOUBLESTART;
		case PLEX_ERROR_ENDBEFORESTART: return PLEX_ERRORMESS_TYPE_ENDBEFORESTART;
		case PLEX_ERROR_ACTIONBEFORESTART: return PLEX_ERRORMESS_TYPE_ACTIONBEFORESTART;
		case PLEX_ERROR_FREENULL: return PLEX_ERRORMESS_TYPE_FREENULL;
		case PLEX_ERROR_ALLOCFAIL: return PLEX_ERRORMESS_TYPE_ALLOCFAIL;
		case PLEX_ERROR_CONTENTCORRUPTION: return PLEX_ERRORMESS_TYPE_CONTENTCORRUPTION;
		case PLEX_ERROR_EXPANDNULL: return PLEX_ERRORMESS_TYPE_EXPANDNULL;
		case PLEX_ERROR_EXPAND: return PLEX_ERRORMESS_TYPE_EXPAND;
		case PLEX_ERROR_INCREMENTNULL: return PLEX_ERRORMESS_TYPE_INCREMENTNULL;
		case PLEX_ERROR_INCREMENT: return PLEX_ERRORMESS_TYPE_INCREMENTNULL;
		case PLEX_ERROR_DECREMENTNULL: return PLEX_ERRORMESS_TYPE_DECREMENTNULL;
		case PLEX_ERROR_DECREMENT: return PLEX_ERRORMESS_TYPE_DECREMENT;
		case PLEX_ERROR_CLEAR: return PLEX_ERRORMESS_TYPE_CLEAR;
		case PLEX_ERROR_SWAP: return PLEX_ERRORMESS_TYPE_SWAP;
		case PLEX_ERROR_CUT: return PLEX_ERRORMESS_TYPE_CUT;
		case PLEX_ERROR_REARRANGE: return PLEX_ERRORMESS_TYPE_REARRANGE;
		case PLEX_ERROR_NOGETARRAY: return PLEX_ERRORMESS_TYPE_NOGETARRAY;
		case PLEX_ERROR_SOUNDLOCK: return PLEX_ERRORMESS_TYPE_SOUNDLOCK;
		case PLEX_ERROR_BADBUFFERSOUNDHOOK: return PLEX_ERRORMESS_TYPE_BADBUFFERSOUNDHOOK;
		case PLEX_ERROR_OVERLIMIT: return PLEX_ERRORMESS_TYPE_OVERLIMIT;
		case PLEX_ERROR_ISNULL: return PLEX_ERRORMESS_TYPE_ISNULL;
		case PLEX_ERROR_SAMEVALUE: return PLEX_ERRORMESS_TYPE_SAMEVALUE;
		case PLEX_ERROR_UNDERLIMIT: return PLEX_ERRORMESS_TYPE_UNDERLIMIT;
		case PLEX_ERROR_ZEROVALUE: return PLEX_ERRORMESS_TYPE_ZEROVALUE;
		case PLEX_ERROR_DISABLED: return PLEX_ERRORMESS_TYPE_DISABLED;
		case PLEX_ERROR_BADOPERATION: return PLEX_ERRORMESS_TYPE_BADOPERATION;
		case PLEX_ERROR_HOOKCORRUPTION: return PLEX_ERRORMESS_TYPE_HOOKCORRUPTION;
		case PLEX_ERROR_BEGINNINGOVER: return PLEX_ERRORMESS_TYPE_BEGINNINGOVER;
		case PLEX_ERROR_BEGINNINGUNDER: return PLEX_ERRORMESS_TYPE_BEGINNINGUNDER;
		case PLEX_ERROR_NOLEN: return PLEX_ERRORMESS_TYPE_NOLEN;
		case PLEX_ERROR_LENGTHOVER: return PLEX_ERRORMESS_TYPE_LENGTHOVER;
		case PLEX_ERROR_LENGTHUNDER: return PLEX_ERRORMESS_TYPE_LENGTHUNDER;
		case PLEX_ERROR_UPDATEBEFORESTART: return PLEX_ERRORMESS_TYPE_UPDATEBEFORESTART;
		case PLEX_ERROR_SUCCESSCHECKFAIL: return PLEX_ERRORMESS_TYPE_SUCCESSCHECKFAIL;
		case PLEX_ERROR_ISNONTEXTURE: return PLEX_ERRORMESS_TYPE_ISNONTEXTURE;
		case PLEX_ERROR_SOUNDHOOKINUSE: return PLEX_ERRORMESS_TYPE_SOUNDHOOKINUSE;
		case PLEX_ERROR_SOUNDCHANNELSTATE: return PLEX_ERRORMESS_TYPE_SOUNDCHANNELSTATE;
		case PLEX_ERROR_ACQUIRE: return PLEX_ERRORMESS_TYPE_ACQUIRE;
		case PLEX_ERROR_STREAMSTATE: return PLEX_ERRORMESS_TYPE_STREAMSTATE;
		case PLEX_ERROR_FILETYPE: return PLEX_ERRORMESS_TYPE_FILETYPE;
		case PLEX_ERROR_DOUBLEDEALLOC: return PLEX_ERRORMESS_TYPE_DOUBLEDEALLOC;
		case PLEX_ERROR_DIRECTORYOPENSTATE: return PLEX_ERRORMESS_TYPE_DIRECTORYOPENSTATE;
	};

	return PLEX_ERRORMESS_TYPE_NONETYPE;
}

const char* PLEX_getSeverityString(const PLEX_ERROR_SEVERITY severity)
{
	switch(severity)
	{
		case PLEX_ERROR_SEVERITY_IMPOSSIBLE: return PLEX_ERRORMESS_SEVERITY_IMPOSSIBLE;
		case PLEX_ERROR_SEVERITY_FATAL: return PLEX_ERRORMESS_SEVERITY_FATAL;
		case PLEX_ERROR_SEVERITY_MAJOR: return PLEX_ERRORMESS_SEVERITY_MAJOR;
		case PLEX_ERROR_SEVERITY_MINOR: return PLEX_ERRORMESS_SEVERITY_MINOR;
		case PLEX_ERROR_SEVERITY_BADSTYLE: return PLEX_ERRORMESS_SEVERITY_BADSTYLE;

		default: break;
	};

	return PLEX_ERRORMESS_SEVERITY_NONE;
}

const char* PLEX_getTypeStringLong(const PLEX_ERROR_TYPE type)
{
	switch(type)
	{
		case PLEX_ERROR_START: return PLEX_ERRORMESS_TYPE_START_LONG;
		case PLEX_ERROR_LOADEXTERNALMODULE: return PLEX_ERRORMESS_TYPE_LOADEXTERNALMODULE_LONG;
		case PLEX_ERROR_EXTERNALMODULE: return PLEX_ERRORMESS_TYPE_EXTERNALMODULE_LONG;
		case PLEX_ERROR_CREATE: return PLEX_ERRORMESS_TYPE_CREATE_LONG;
		case PLEX_ERROR_UPDATE: return PLEX_ERRORMESS_TYPE_UPDATE_LONG;
		case PLEX_ERROR_MINMAXREVERSE: return PLEX_ERRORMESS_TYPE_MINMAXREVERSE_LONG;
		case PLEX_ERROR_SETNULL: return PLEX_ERRORMESS_TYPE_SETNULL_LONG;
		case PLEX_ERROR_GETNULL: return PLEX_ERRORMESS_TYPE_GETNULL_LONG;
		case PLEX_ERROR_COMPNULL: return PLEX_ERRORMESS_TYPE_COMPNULL_LONG;
		case PLEX_ERROR_DEFAULTPOP: return PLEX_ERRORMESS_TYPE_DEFAULTPOP_LONG;
		case PLEX_ERROR_ASSIGNDEFAULT: return PLEX_ERRORMESS_TYPE_ASSIGNDEFAULT_LONG;
		case PLEX_ERROR_FILELOAD: return PLEX_ERRORMESS_TYPE_FILELOAD_LONG;
		case PLEX_ERROR_FILESAVE: return PLEX_ERRORMESS_TYPE_FILESAVE_LONG;
		case PLEX_ERROR_DOUBLESTART: return PLEX_ERRORMESS_TYPE_DOUBLESTART_LONG;
		case PLEX_ERROR_ENDBEFORESTART: return PLEX_ERRORMESS_TYPE_ENDBEFORESTART_LONG;
		case PLEX_ERROR_ACTIONBEFORESTART: return PLEX_ERRORMESS_TYPE_ACTIONBEFORESTART_LONG;
		case PLEX_ERROR_FREENULL: return PLEX_ERRORMESS_TYPE_FREENULL_LONG;
		case PLEX_ERROR_ALLOCFAIL: return PLEX_ERRORMESS_TYPE_ALLOCFAIL_LONG;
		case PLEX_ERROR_CONTENTCORRUPTION: return PLEX_ERRORMESS_TYPE_CONTENTCORRUPTION_LONG;
		case PLEX_ERROR_EXPANDNULL: return PLEX_ERRORMESS_TYPE_EXPANDNULL_LONG;
		case PLEX_ERROR_EXPAND: return PLEX_ERRORMESS_TYPE_EXPAND_LONG;
		case PLEX_ERROR_INCREMENTNULL: return PLEX_ERRORMESS_TYPE_INCREMENTNULL_LONG;
		case PLEX_ERROR_INCREMENT: return PLEX_ERRORMESS_TYPE_INCREMENTNULL_LONG;
		case PLEX_ERROR_DECREMENTNULL: return PLEX_ERRORMESS_TYPE_DECREMENTNULL_LONG;
		case PLEX_ERROR_DECREMENT: return PLEX_ERRORMESS_TYPE_DECREMENT_LONG;
		case PLEX_ERROR_CLEAR: return PLEX_ERRORMESS_TYPE_CLEAR_LONG;
		case PLEX_ERROR_SWAP: return PLEX_ERRORMESS_TYPE_SWAP_LONG;
		case PLEX_ERROR_CUT: return PLEX_ERRORMESS_TYPE_CUT_LONG;
		case PLEX_ERROR_REARRANGE: return PLEX_ERRORMESS_TYPE_REARRANGE_LONG;
		case PLEX_ERROR_NOGETARRAY: return PLEX_ERRORMESS_TYPE_NOGETARRAY_LONG;
		case PLEX_ERROR_SOUNDLOCK: return PLEX_ERRORMESS_TYPE_SOUNDLOCK_LONG;
		case PLEX_ERROR_BADBUFFERSOUNDHOOK: return PLEX_ERRORMESS_TYPE_BADBUFFERSOUNDHOOK_LONG;
		case PLEX_ERROR_OVERLIMIT: return PLEX_ERRORMESS_TYPE_OVERLIMIT_LONG;
		case PLEX_ERROR_ISNULL: return PLEX_ERRORMESS_TYPE_ISNULL_LONG;
		case PLEX_ERROR_SAMEVALUE: return PLEX_ERRORMESS_TYPE_SAMEVALUE_LONG;
		case PLEX_ERROR_UNDERLIMIT: return PLEX_ERRORMESS_TYPE_UNDERLIMIT_LONG;
		case PLEX_ERROR_ZEROVALUE: return PLEX_ERRORMESS_TYPE_ZEROVALUE_LONG;
		case PLEX_ERROR_DISABLED: return PLEX_ERRORMESS_TYPE_DISABLED_LONG;
		case PLEX_ERROR_BADOPERATION: return PLEX_ERRORMESS_TYPE_BADOPERATION_LONG;
		case PLEX_ERROR_HOOKCORRUPTION: return PLEX_ERRORMESS_TYPE_HOOKCORRUPTION_LONG;
		case PLEX_ERROR_BEGINNINGOVER: return PLEX_ERRORMESS_TYPE_BEGINNINGOVER_LONG;
		case PLEX_ERROR_BEGINNINGUNDER: return PLEX_ERRORMESS_TYPE_BEGINNINGUNDER_LONG;
		case PLEX_ERROR_NOLEN: return PLEX_ERRORMESS_TYPE_NOLEN_LONG;
		case PLEX_ERROR_LENGTHOVER: return PLEX_ERRORMESS_TYPE_LENGTHOVER_LONG;
		case PLEX_ERROR_LENGTHUNDER: return PLEX_ERRORMESS_TYPE_LENGTHUNDER_LONG;
		case PLEX_ERROR_UPDATEBEFORESTART: return PLEX_ERRORMESS_TYPE_UPDATEBEFORESTART_LONG;
		case PLEX_ERROR_SUCCESSCHECKFAIL: return PLEX_ERRORMESS_TYPE_SUCCESSCHECKFAIL_LONG;
		case PLEX_ERROR_ISNONTEXTURE: return PLEX_ERRORMESS_TYPE_ISNONTEXTURE_LONG;
		case PLEX_ERROR_SOUNDHOOKINUSE: return PLEX_ERRORMESS_TYPE_SOUNDHOOKINUSE_LONG;
		case PLEX_ERROR_SOUNDCHANNELSTATE: return PLEX_ERRORMESS_TYPE_SOUNDCHANNELSTATE_LONG;
		case PLEX_ERROR_ACQUIRE: return PLEX_ERRORMESS_TYPE_ACQUIRE_LONG;
		case PLEX_ERROR_STREAMSTATE: return PLEX_ERRORMESS_TYPE_STREAMSTATE_LONG;
		case PLEX_ERROR_FILETYPE: return PLEX_ERRORMESS_TYPE_FILETYPE_LONG;
		case PLEX_ERROR_DOUBLEDEALLOC: return PLEX_ERRORMESS_TYPE_DOUBLEDEALLOC_LONG;
		case PLEX_ERROR_DIRECTORYOPENSTATE: return PLEX_ERRORMESS_TYPE_DIRECTORYOPENSTATE_LONG;
	};

	return PLEX_ERRORMESS_TYPE_NONETYPE_LONG;
}

const char* PLEX_getSeverityStringLong(const PLEX_ERROR_SEVERITY severity)
{
	switch(severity)
	{
		case PLEX_ERROR_SEVERITY_IMPOSSIBLE: return PLEX_ERRORMESS_SEVERITY_IMPOSSIBLE_LONG;
		case PLEX_ERROR_SEVERITY_FATAL: return PLEX_ERRORMESS_SEVERITY_FATAL_LONG;
		case PLEX_ERROR_SEVERITY_MAJOR: return PLEX_ERRORMESS_SEVERITY_MAJOR_LONG;
		case PLEX_ERROR_SEVERITY_MINOR: return PLEX_ERRORMESS_SEVERITY_MINOR_LONG;
		case PLEX_ERROR_SEVERITY_BADSTYLE: return PLEX_ERRORMESS_SEVERITY_BADSTYLE_LONG;
		default: break;
	};

	return PLEX_ERRORMESS_SEVERITY_NONE;
}

// Checks for Non-PLEX errors:

bool PLEX_errorSDL(const PLEX_ERROR_SEVERITY severity)
{
	char error[1024];

	SDL_GetErrorMsg(error, 1024);

	if(error[0] == '\0') return false;

	PLEX_error(PLEX_ERROR_CATEGORY_SDL, PLEX_ERROR_EXTERNALMODULE, severity, error);
	SDL_ClearError();

	return true;
}

bool PLEX_errorSDLImage(const PLEX_ERROR_SEVERITY severity)
{
	const char* const error = IMG_GetError();

	if(error == NULL || error[0] == '\0') return false;

	PLEX_error(PLEX_ERROR_CATEGORY_SDL, PLEX_ERROR_EXTERNALMODULE, severity, IMG_GetError());
	SDL_ClearError();

	return true;
}

bool PLEX_errorPortAudio(const PLEX_ERROR_SEVERITY severity, const PaError error)
{
	if(error == paNoError) return false;

	PLEX_error(PLEX_ERROR_CATEGORY_PORTAUDIO, PLEX_ERROR_EXTERNALMODULE, severity, Pa_GetErrorText(error));

	return true;
}

bool PLEX_startErrorSystem()
{
	// config file sets these as something different
	p_showThreshold = PLEX_ERROR_SEVERITY_MAJOR;
	p_fatalThreshold = PLEX_ERROR_SEVERITY_FATAL;

	p_printErrors = true;
	p_showErrors = true;
	p_dumpErrors = false;

	p_sdlErrorFlag = false;

	p_year = 0;
	p_month = 0;
	p_day = 0;
	p_hour = 0;
	p_minute = 0;
	p_second = 0;

	p_shownErrorCount = 0;

	PLEX_setErrorLogDate();

	return true;
}

bool PLEX_updateErrorSystem()
{
	return true;
}

void PLEX_endErrorSystem()
{
	if(p_suppressFile != NULL) fclose(p_suppressFile);
	if(p_dumpFile != NULL) fclose(p_dumpFile);

	p_suppressFile = NULL;
	p_dumpFile = NULL;
}
