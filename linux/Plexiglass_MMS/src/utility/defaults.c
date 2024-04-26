#include "utility/defaults.h"

// default defaults (ie. what defaults are if they don't load from config)

static const uint32_t p_minWidth = 800;
static const uint32_t p_maxWidth = 4096;

static const uint32_t p_minHeight = 600;
static const uint32_t p_maxHeight = 2160;

static const uint32_t p_defWidth = 800;
static const uint32_t p_defHeight = 600;

static const uint8_t p_defCursorR = 255;
static const uint8_t p_defCursorG = 0;
static const uint8_t p_defCursorB = 0;

static const bool p_defKillOnEscape = true;
static const bool p_defKillOnClose = true;

static const bool p_defAllowResize = true;
static const bool p_defAllowMaximize = true;
static const bool p_defStartMaximize = false;

static const bool p_defDisableSound = false;

static const bool p_defDisableController = false;
// static const bool p_defDisableJoystick = false;
static const bool p_defDisableKeyboard = false;
static const bool p_defDisableMouse = false;

static const bool p_defControllerPause = true;
// static const bool p_defConfigReloadButton = false;

static const bool p_def42Hour = false;

static const uint32_t p_defTextureDimensLimit = 10000;

static const bool p_defPrintErrors = true;
static const bool p_defShowErrors = true;
static const bool p_defDumpErrors = false;

static const uint8_t p_defPrintErrorThreshhold = 4;
static const uint8_t p_defShowErrorThreshold = 3;
static const uint8_t p_defFatalErrorThreshold = 2;

static const bool p_defHeaderlessCopy = true;

static const uint8_t p_defMemoryProfile = 0;

static const bool p_defMemoryReport = false;
static const bool p_defMemoryReportToFile = true;

// the index values of the defaults

static int64_t p_width = -1;
static int64_t p_height = -1;

static int64_t p_killOnEscape = -1;
static int64_t p_killOnClose = -1;

static int64_t p_allowResize = -1;
static int64_t p_allowMaximize = -1;
static int64_t p_startMaximized = -1;

static int64_t p_disableSound = -1;

static int64_t p_disableController = -1;
static int64_t p_disableKeyboard = -1;
static int64_t p_disableMouse = -1;

static int64_t p_cursorR = -1;
static int64_t p_cursorG = -1;
static int64_t p_cursorB = -1;

static int64_t p_controllerPause = -1;

static int64_t p_24hourTime = -1;

static int64_t p_textureDimensLimit = -1;

static int64_t p_printErrors = -1;
static int64_t p_showErrors = -1;
static int64_t p_dumpErrors = -1;

static int64_t p_printErrorThreshold = -1;
static int64_t p_showErrorThreshold = -1;
static int64_t p_fatalErrorThreshold = -1;

static int64_t p_heaaderlessCopy = -1;

static int64_t p_memoryProfile = -1;

static int64_t p_memReport = -1;
static int64_t p_memReportToFile = -1;

// struct that contains all the information for an individual default entries

typedef struct
{
	size_t nameLen;
	const char* name;
	PLEX_DEF_ENTRYTYPE type;
	char comment[PLEX_DEFAULT_COMMENTBUFFERLIMIT];
	double min, max, def;
} P_DEFAULTENTRY;

// the internal collection of entries

static PLEX_EXPANDARRAY* p_entries = NULL;

// a flag to signal reload to all external functions

static bool p_reloadFlag = false;

// where the string entires are stored

static PLEX_EXPANDARRAY* p_stringBank = NULL;

// ensures that default is within a minimum and maximum

static double p_validateNumberDefault(const double min, const double max, const double def)
{
	if(def < min) return min;
	if(def > max) return max;

	return def;
}

// generates a default entry

static P_DEFAULTENTRY p_genDefault(const char* const name, const PLEX_DEF_ENTRYTYPE type, const double min, const double max, const double def, const char* comment)
{
	P_DEFAULTENTRY ret;

	size_t commentLen = 0;

	ret.nameLen = strlen(name);
	ret.name = name;
	ret.type = type;
	ret.min = min;
	ret.max = max;
	ret.def = def;

	memset(ret.comment, '\0', PLEX_DEFAULT_COMMENTBUFFERLIMIT);

	if(comment != NULL && commentLen)
	{
		commentLen = strlen(comment);
		if(commentLen >= PLEX_DEFAULT_COMMENTBUFFERLIMIT - 1) commentLen = PLEX_DEFAULT_COMMENTBUFFERLIMIT - 1;
		memcpy(ret.comment, comment, commentLen);
	}

	return ret;
}

// sets particular default by name

static int64_t p_assignDefault(const char* const name, const PLEX_DEF_ENTRYTYPE type, const double min, const double max, const double def, const char* const comment, const size_t line, const char* const filename)
{
	P_DEFAULTENTRY* entry = NULL;

	if(!PLEX_incrementExpandArrayInt(p_entries, line, filename))
	{
		// error
		return -1;
	}

	entry = (P_DEFAULTENTRY*)PLEX_getExpandArrayLast(p_entries);

	if(entry == NULL)
	{
		// error - fatal
		return -1;
	}

	*entry = p_genDefault(name, type, min, max, def, comment);

	return PLEX_expandArrayCount(p_entries) - 1;
}

// populates all the defaults

static void p_populateDefaults()
{
	PLEX_clearExpandArray(p_entries);

	p_width = PLEX_assignDefaultUInt32(PLEX_DEFNAME_MAINWINWIDTH, p_minWidth, p_maxWidth, p_defWidth, PLEX_DEFDESC_MAINWINWIDTH);
	p_height = PLEX_assignDefaultUInt32(PLEX_DEFNAME_MAINWINHEIGHT, p_minHeight, p_maxHeight, p_defHeight, PLEX_DEFDESC_MAINWINHEIGHT);

	p_killOnEscape = PLEX_assignDefaultBool(PLEX_DEFNAME_KILLONESCAPE, p_defKillOnEscape, PLEX_DEFDESC_KILLONESCAPE);
	p_killOnClose = PLEX_assignDefaultBool(PLEX_DEFNAME_KILLONCLOSE, p_defKillOnClose, PLEX_DEFDESC_KILLONCLOSE);

	p_allowResize = PLEX_assignDefaultBool(PLEX_DEFNAME_ALLOWRESIZE, p_defAllowResize, PLEX_DEFDESC_ALLOWRESIZE);
	p_allowMaximize = PLEX_assignDefaultBool(PLEX_DEFNAME_ALLOWMAXIMIZE, p_defAllowMaximize, PLEX_DEFDESC_ALLOWMAXIMIZE);
	p_startMaximized = PLEX_assignDefaultBool(PLEX_DEFNAME_STARTMAXIMIZED, p_defStartMaximize, PLEX_DEFDESC_STARTMAXIMIZED);

	p_disableSound = PLEX_assignDefaultBool(PLEX_DEFNAME_DISABLESOUND, p_defDisableSound, PLEX_DEFDESC_DISABLESOUND);

	p_cursorR = PLEX_assignDefaultUInt8(PLEX_DEFNAME_CURSOR_R, 0, 255, p_defCursorR, PLEX_DEFDESC_CURSOR_R);
	p_cursorG = PLEX_assignDefaultUInt8(PLEX_DEFNAME_CURSOR_G, 0, 255, p_defCursorG, PLEX_DEFDESC_CURSOR_G);
	p_cursorB = PLEX_assignDefaultUInt8(PLEX_DEFNAME_CURSOR_B, 0, 255, p_defCursorB, PLEX_DEFDESC_CURSOR_B);

	p_controllerPause = PLEX_assignDefaultBool(PLEX_DEFNAME_CONTROLLERPAUSE, p_defControllerPause, PLEX_DEFDESC_CONTROLLERPAUSE);

	p_disableController = PLEX_assignDefaultBool(PLEX_DEFNAME_DISABLECONTROLLER, p_defDisableController, PLEX_DEFDESC_DISABLECONTROLLER);
	p_disableKeyboard = PLEX_assignDefaultBool(PLEX_DEFNAME_DISABLEKEYBOARD, p_defDisableKeyboard, PLEX_DEFDESC_DISABLEKEYBOARD);
	p_disableMouse = PLEX_assignDefaultBool(PLEX_DEFNAME_DISABLEMOUSE, p_defDisableMouse, PLEX_DEFDESC_DISABLEMOUSE);

	p_24hourTime = PLEX_assignDefaultBool(PLEX_DEFNAME_24HOURTIME, p_def42Hour, PLEX_DEFDESC_24HOURTIME);

	p_textureDimensLimit = PLEX_assignDefaultUInt32(PLEX_DEFNAME_TEXTUREDIMENSLIMIT, 0, 99999, p_defTextureDimensLimit, PLEX_DEFDESC_TEXTUREDIMENSLIMIT);

	p_printErrors = PLEX_assignDefaultBool(PLEX_DEFNAME_PRINTERRORS, p_defPrintErrors, PLEX_DEFDESC_PRINTERRORS);
	p_showErrors = PLEX_assignDefaultBool(PLEX_DEFNAME_SHOWERRORS, p_defShowErrors, PLEX_DEFDESC_SHOWERRORS);
	p_dumpErrors = PLEX_assignDefaultBool(PLEX_DEFNAME_DUMPERRORS, p_defDumpErrors, PLEX_DEFDESC_DUMPERRORS);

	p_printErrorThreshold = PLEX_assignDefaultUInt8(PLEX_DEFNAME_PRINTTHRESHOLD, 1, 5, p_defPrintErrorThreshhold, PLEX_DEFDESC_PRINTTHRESHOLD);
	p_showErrorThreshold = PLEX_assignDefaultUInt8(PLEX_DEFNAME_SHOWTHRESHOLD, 1, 5, p_defShowErrorThreshold, PLEX_DEFDESC_SHOWTHRESHOLD);
	p_fatalErrorThreshold = PLEX_assignDefaultUInt8(PLEX_DEFNAME_FATALTHRESHOLD, 2, 5, p_defFatalErrorThreshold, PLEX_DEFDESC_FATALTHRESHOLD);

	p_heaaderlessCopy = PLEX_assignDefaultBool(PLEX_DEFNAME_HEADERLESSCOPY, p_defHeaderlessCopy, PLEX_DEFDESC_HEADERLESSCOPY);

	p_memoryProfile = PLEX_assignDefaultUInt8(PLEX_DEFNAME_MEMORYPROFILE, 0, 3, p_defMemoryProfile, PLEX_DEFDESC_MEMORYPROFILE);

	p_memReport = PLEX_assignDefaultBool(PLEX_DEFNAME_MEMORYREPORT, p_defMemoryReport, PLEX_DEFDESC_MEMORYREPORT);

	p_memReportToFile = PLEX_assignDefaultBool(PLEX_DEFNAME_MEMORYREPORTTOFILE, p_defMemoryReportToFile, PLEX_DEFDESC_MEMORYREPORTTOFILE);

	if(p_width == -1) PLEX_error(PLEX_ERROR_CATEGORY_DEFAULT, PLEX_ERROR_DEFAULTPOP, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_DEFAULT_TYPEWIDTH);
	if(p_height == -1) PLEX_error(PLEX_ERROR_CATEGORY_DEFAULT, PLEX_ERROR_DEFAULTPOP, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_DEFAULT_TYPEHEIGHT);
	if(p_killOnEscape == -1) PLEX_error(PLEX_ERROR_CATEGORY_DEFAULT, PLEX_ERROR_DEFAULTPOP, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_DEFAULT_TYPEKILLONESCAPE);
	if(p_killOnClose == -1) PLEX_error(PLEX_ERROR_CATEGORY_DEFAULT, PLEX_ERROR_DEFAULTPOP, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_DEFAULT_TYPEKILLONCLOSE);
	if(p_allowResize == -1) PLEX_error(PLEX_ERROR_CATEGORY_DEFAULT, PLEX_ERROR_DEFAULTPOP, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_DEFAULT_TYPEALLOWRESIZE);
	if(p_allowMaximize == -1) PLEX_error(PLEX_ERROR_CATEGORY_DEFAULT, PLEX_ERROR_DEFAULTPOP, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_DEFAULT_TYPEALLOWMAXIMIZE);
	if(p_startMaximized == -1) PLEX_error(PLEX_ERROR_CATEGORY_DEFAULT, PLEX_ERROR_DEFAULTPOP, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_DEFAULT_TYPESTARTMAXIMIZE);
	if(p_disableSound == -1) PLEX_error(PLEX_ERROR_CATEGORY_DEFAULT, PLEX_ERROR_DEFAULTPOP, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_DEFAULT_TYPEDISABLESOUND);
	if(p_cursorR == -1) PLEX_error(PLEX_ERROR_CATEGORY_DEFAULT, PLEX_ERROR_DEFAULTPOP, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_DEFAULT_TYPECURSORR);
	if(p_cursorG == -1) PLEX_error(PLEX_ERROR_CATEGORY_DEFAULT, PLEX_ERROR_DEFAULTPOP, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_DEFAULT_TYPECURSORG);
	if(p_cursorB == -1) PLEX_error(PLEX_ERROR_CATEGORY_DEFAULT, PLEX_ERROR_DEFAULTPOP, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_DEFAULT_TYPECURSORB);
	if(p_controllerPause == -1) PLEX_error(PLEX_ERROR_CATEGORY_DEFAULT, PLEX_ERROR_DEFAULTPOP, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_DEFAULT_TYPECONTROLLERPAUSE);
	if(p_disableController == -1) PLEX_error(PLEX_ERROR_CATEGORY_DEFAULT, PLEX_ERROR_DEFAULTPOP, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_DEFAULT_TYPEDISABLECONTROLLER);
	if(p_disableKeyboard == -1) PLEX_error(PLEX_ERROR_CATEGORY_DEFAULT, PLEX_ERROR_DEFAULTPOP, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_DEFAULT_TYPEDISABLEKEYBOARD);
	if(p_disableMouse == -1) PLEX_error(PLEX_ERROR_CATEGORY_DEFAULT, PLEX_ERROR_DEFAULTPOP, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_DEFAULT_TYPEDISABLEMOUSE);
	if(p_24hourTime == -1) PLEX_error(PLEX_ERROR_CATEGORY_DEFAULT, PLEX_ERROR_DEFAULTPOP, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_DEFAULT_TYPE24HRTIME);

	if(p_textureDimensLimit == -1) PLEX_error(PLEX_ERROR_CATEGORY_DEFAULT, PLEX_ERROR_DEFAULTPOP, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_DEFAULT_TEXTUREDIMENSLIMIT);

	if(p_printErrors == -1) PLEX_error(PLEX_ERROR_CATEGORY_DEFAULT, PLEX_ERROR_DEFAULTPOP, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_DEFAULT_PRINTERRORS);
	if(p_showErrors == -1) PLEX_error(PLEX_ERROR_CATEGORY_DEFAULT, PLEX_ERROR_DEFAULTPOP, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_DEFAULT_SHOWERRORS);

	if(p_printErrorThreshold == -1) PLEX_error(PLEX_ERROR_CATEGORY_DEFAULT, PLEX_ERROR_DEFAULTPOP, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_DEFAULT_PRINTERRORTHRESHOLD);
	if(p_showErrorThreshold == -1) PLEX_error(PLEX_ERROR_CATEGORY_DEFAULT, PLEX_ERROR_DEFAULTPOP, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_DEFAULT_SHOWERRORTHRESHOLD);
	if(p_fatalErrorThreshold == -1) PLEX_error(PLEX_ERROR_CATEGORY_DEFAULT, PLEX_ERROR_DEFAULTPOP, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_DEFAULT_FATALERRORTHRESHOLD);

	if(p_heaaderlessCopy == -1) PLEX_error(PLEX_ERROR_CATEGORY_DEFAULT, PLEX_ERROR_DEFAULTPOP, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_DEFAULT_HEADERLESSCOPY);

	if(p_memoryProfile == -1) PLEX_error(PLEX_ERROR_CATEGORY_DEFAULT, PLEX_ERROR_DEFAULTPOP, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_DEFAULT_MEMORYPROFILE);

	if(p_memReport == -1) PLEX_error(PLEX_ERROR_CATEGORY_DEFAULT, PLEX_ERROR_DEFAULTPOP, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_DEFAULT_MEMORYREPORT);
	if(p_memReportToFile == -1) PLEX_error(PLEX_ERROR_CATEGORY_DEFAULT, PLEX_ERROR_DEFAULTPOP, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_DEFAULT_MEMORYREPORTTOFILE);
}

// reads the subject of a default

static bool p_readSubject(char* buffer, char* comment, bool* read, size_t* len, FILE* config)
{
	bool valid = false;

	bool firstNonSpace = false;

	size_t buffIt = 0;

	size_t lastNonSpace = 0;

	int next = fgetc(config);

	while(next != '\r' && next != '\n')
	{
		if(next == '=')
		{
			valid = true;
			break;
		}else{
			if(next == EOF || next == '\0')
			{
				*read = false;
				break;
			}else{
				if(next != '\t' && next != ' ') firstNonSpace = true;

				if(firstNonSpace)
				{
					if(next != '\t' && next != ' ') lastNonSpace = buffIt;

					buffer[buffIt] = next;
					buffIt++;
				}
			}
		}

		next = fgetc(config);
	}

	if(!firstNonSpace)
	{
		valid = false;
		*read = false;
		buffer[0] = '\0';
		return false;
	}

	buffer[lastNonSpace + 1] = '\0';

	*len = lastNonSpace + 1;

	return valid;
}

// read predicate of a default

static void p_readPredicate(char* buffer, char* comment, bool* read, size_t* len, FILE* config, bool* commented, size_t* commentIt)
{
	bool firstNonSpace = false;

	size_t buffIt = 0;

	size_t lastNonSpace = 0;

	int next = fgetc(config);

	size_t lcommentIt = *commentIt;

	bool lcommented = *commented;

	int64_t firstCommentNonSpace = -1;
	size_t lastCommentNonSpace = 0;

	while(next != '\r' && next != '\n')
	{
		if(next == EOF || next == '\0')
		{
			*read = false;
			break;
		}else{
			if(!lcommented && next == '#')
			{
				lcommentIt = 0;
				lcommented = true;
				*commented = true;
			}

			if(lcommented)
			{
				next = fgetc(config);

				if(lcommentIt >= PLEX_DEFAULT_COMMENTBUFFERLIMIT - 1)
				{
					comment[PLEX_DEFAULT_COMMENTBUFFERLIMIT - 1] = '\0';
					continue;
				}

				if(next != ' ' && next != '\t' && next != '\n' && next != '\r')
				{
					if(firstCommentNonSpace == -1) firstCommentNonSpace = lcommentIt;
					lastCommentNonSpace = lcommentIt + 1;
				}

				comment[lcommentIt] = next;
				++lcommentIt;
				comment[lcommentIt] = '\0';

				continue;
			}

			if(next != '\t' && next != ' ') firstNonSpace = true;

			if(firstNonSpace)
			{
				if(next != '\t' && next != ' ') lastNonSpace = buffIt;

				buffer[buffIt] = next;
				buffIt++;
			}
		}

		next = fgetc(config);
	}

	*commentIt = lcommentIt;

	if(firstCommentNonSpace != -1 && lastCommentNonSpace > firstCommentNonSpace)
	{
		const size_t len = lastCommentNonSpace - firstCommentNonSpace;

		memmove(comment, comment + firstCommentNonSpace, len);
		comment[len] = '\0';
	}

	buffer[lastNonSpace + 1] = '\0';

	*len = lastNonSpace + 1;
}

// checks if a subject is the name of a default

static bool p_checkDefault(const char* const subject, size_t len, P_DEFAULTENTRY* entry)
{
	if(len != entry->nameLen) return false;

	return !memcmp(subject, entry->name, len); // < remember that 0 == success with this method
}

// sets the default's value from the predicate

static uint64_t p_addToStringBank(const char* const value, const size_t line, const char* const filename)
{
	uint64_t ret = -1;

	uint32_t len = 0; // < do not make size_t

	char* entry = NULL;

	len = strlen(value);
	ret = PLEX_expandArrayCount(p_stringBank);

	if(!PLEX_multiIncrementExpandArrayInt(sizeof(len) + len, p_stringBank, line, filename))
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_DEFAULT, PLEX_ERROR_INCREMENT, PLEX_ERROR_SEVERITY_MAJOR, PLEX_ERRORMESS_ONADDTOSTRINGBANK, PLEX_ERRORMESS_DEFAULT_STRINGBANK);
		return -1;
	}

	entry = (char*)PLEX_expandArrayContents(p_stringBank);

	if(entry == NULL)
	{
		// error
		return -1;
	}

	entry += ret;
	memcpy(entry, (char*)&len, sizeof(len));

	if(len)
	{
		entry += sizeof(len);
		memcpy(entry, value, len);
	}

	return ret;
}

static void p_setDefault(const char* const predicate, size_t len, P_DEFAULTENTRY* entry, const size_t line, const char* const filename)
{
	switch(entry->type)
	{
		case PLEX_DEF_UINT8:
		case PLEX_DEF_UINT32:
			entry->def = atoi(predicate);
		break;

		case PLEX_DEF_BOOL: // any value that isn't 'true' all lowercase will be considered false
			if(len == 4 && (!memcmp(predicate, "true", 5))) entry->def = 1;
			else entry->def = 0;
		break;

		case PLEX_DEF_DOUBLE:
			entry->def = strtod(predicate, NULL);
		break;

		case PLEX_DEF_STRING:
			entry->def = p_addToStringBank(predicate, line, filename);
		break;
	};

	entry->def = p_validateNumberDefault(entry->min, entry->max, entry->def);
}

// sets the comment on the entry

static void p_setComment(const char* const comment, size_t len, P_DEFAULTENTRY* entry)
{
	size_t alen = len;

	if(!len) return;

	if(alen >= PLEX_DEFAULT_COMMENTBUFFERLIMIT) alen = PLEX_DEFAULT_COMMENTBUFFERLIMIT - 1;

	memcpy(entry->comment, comment, alen);
	memset(entry->comment + alen, '\0', PLEX_DEFAULT_COMMENTBUFFERLIMIT - alen);
}

// saves the default as a string

static void p_saveDefault(const size_t index, FILE* config, const size_t line, const char* const filename)
{
	char* sbuffer = (char*)PLEX_allocInt(PLEX_LIMIT_DEFAULT_SBUFFERLEN, line, filename);

	size_t commentLen = 0;

	P_DEFAULTENTRY* entry = NULL;

	if(sbuffer == NULL)
	{
		// error
		return;
	}

	entry = (P_DEFAULTENTRY*)PLEX_getExpandArrayEntry(index, p_entries);

	if(entry == NULL)
	{
		PLEX_dealloc(&sbuffer);
		// error - fatal
		return;
	}

	for(size_t z = 0; z < entry->nameLen; ++z) fputc(entry->name[z], config);

	fputc(' ', config);
	fputc('=', config);
	fputc(' ', config);

	switch(entry->type)
	{
		case PLEX_DEF_UINT8:
		case PLEX_DEF_UINT32:
			sprintf(sbuffer, "%u", (unsigned)entry->def);
		break;

		case PLEX_DEF_BOOL:
			if(entry->def) memcpy(sbuffer, "true", 5);
			else memcpy(sbuffer, "false", 6);
		break;

		case PLEX_DEF_DOUBLE:
			sprintf(sbuffer, "%lf", (double)entry->def);
		break;

		case PLEX_DEF_STRING:
			PLEX_getDefaultString((size_t)entry->def, sbuffer, PLEX_LIMIT_DEFAULT_SBUFFERLEN);
		break;
	};

	fwrite(sbuffer, strlen(sbuffer), 1, config);

	commentLen = strlen(entry->comment);

	if(commentLen)
	{
		fwrite(" # ", 3, 1, config);
		fwrite(entry->comment, commentLen, 1, config);
	}

	fputc('\n', config);

	PLEX_dealloc(&sbuffer);
}

// returns the value at the index as a double

static double p_getDefaultValue(const size_t index)
{
	P_DEFAULTENTRY* entry = NULL;

	if(index >= PLEX_expandArrayCount(p_entries)) return 0;

	entry = (P_DEFAULTENTRY*)PLEX_getExpandArrayEntry(index, p_entries);

	return entry->def;
}

// ---------------------------- //
// PUBLIC FUNCTIONS:
// ---------------------------- //

bool PLEX_loadConfigInt(const char* fileloc, const size_t line, const char* const filename)
{
	char predicate[PLEX_DEFAULT_COMMENTBUFFERLIMIT];
	char subject[PLEX_DEFAULT_COMMENTBUFFERLIMIT];
	char comment[PLEX_DEFAULT_COMMENTBUFFERLIMIT];

	bool read = true;
	bool commented = false;

	size_t slen = 0;
	size_t plen = 0;
	size_t commentIt = 0;

	FILE* config = NULL;

	if(fileloc == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_DEFAULT, PLEX_ERROR_ISNULL, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_ONLOADCONFIG, PLEX_ERRORMESS_GENERIC_FILEPATH);
		return false;
	}

	config = fopen(fileloc, "r");

	if(p_entries != NULL) PLEX_destroyExpandArray(&p_entries);
	p_entries = PLEX_genExpandArrayInt(PLEX_getMemProfileDefaultPreload(), sizeof(P_DEFAULTENTRY), line, filename);

	if(p_entries == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_DEFAULT, PLEX_ERROR_INCREMENT, PLEX_ERROR_SEVERITY_FATAL, PLEX_ERRORMESS_ONLOADCONFIG, PLEX_ERRORMESS_DEFAULT_ENTRIES);
		return false;
	}

	if(p_stringBank != NULL) PLEX_destroyExpandArray(&p_stringBank);
	p_stringBank = PLEX_genExpandArrayInt(128, 1, line, filename);

	if(p_stringBank == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_DEFAULT, PLEX_ERROR_ALLOCFAIL, PLEX_ERROR_SEVERITY_FATAL, PLEX_ERRORMESS_ONLOADCONFIG, PLEX_ERRORMESS_DEFAULT_STRINGBANK);
		return false;
	}

	p_populateDefaults();

	if(config == NULL)
	{
		const PLEX_FAKESTRING_100 targMessage = PLEX_fileLoadErrorPath(fileloc, PLEX_ERRORMESS_GENERIC_FILE);

		PLEX_error2(PLEX_ERROR_CATEGORY_DEFAULT, PLEX_ERROR_FILELOAD, PLEX_ERROR_SEVERITY_MAJOR, PLEX_ERRORMESS_ONLOADCONFIG, PLEX_fts(targMessage));
		return false;
	}

	while(read)
	{
		if(p_readSubject((char*)subject, (char*)comment, &read, &slen, config))
		{
			p_readPredicate((char*)predicate, (char*)comment, &read, &plen, config, &commented, &commentIt);

			for(size_t z = 0; z < p_entries->count; ++z)
			{
				P_DEFAULTENTRY* entry = (P_DEFAULTENTRY*)PLEX_getExpandArrayEntry(z, p_entries);

				if(p_checkDefault((char*)subject, slen, entry))
				{
					p_setDefault((const char*)predicate, plen, entry, line, filename);
					p_setComment(comment, commentIt, entry);
				}
			}
		}

		commentIt = 0;
		commented = false;
		predicate[0] = '\0';
		subject[0] = '\0';
	}

	fclose(config);

	return true;
}

uint32_t PLEX_DEF_width()
{
	if(p_width == -1) return p_defWidth;

	return PLEX_getDefaultUint32(p_width);
}

uint32_t PLEX_DEF_height()
{
	if(p_height == -1) return p_defHeight;

	return PLEX_getDefaultUint32(p_height);
}

bool PLEX_DEF_killOnEscape()
{
	if(p_killOnEscape == -1) return p_defKillOnClose;

	return PLEX_getDefaultBool(p_killOnEscape);
}

bool PLEX_DEF_killOnClose()
{
	if(p_killOnClose == -1) return p_defKillOnClose;

	return PLEX_getDefaultBool(p_killOnClose);
}

bool PLEX_DEF_allowResize()
{
	if(p_allowResize == -1) return p_defAllowResize;

	return PLEX_getDefaultBool(p_allowResize);
}

bool PLEX_DEF_allowMaximize()
{
	if(p_allowMaximize == -1) return p_defAllowMaximize;

	return PLEX_getDefaultBool(p_allowMaximize);
}

bool PLEX_DEF_startMaximized()
{
	if(p_startMaximized == -1) return p_defStartMaximize;

	return PLEX_getDefaultBool(p_startMaximized);
}

bool PLEX_DEF_disableAudio()
{
	if(p_disableSound == -1) return p_defDisableSound;

	return PLEX_getDefaultBool(p_disableSound);
}

uint8_t PLEX_DEF_cursorR()
{
	if(p_cursorR == -1) return p_defCursorR;

	return PLEX_getDefaultUint8(p_cursorR);
}

uint8_t PLEX_DEF_cursorG()
{
	if(p_cursorG == -1) return p_defCursorG;

	return PLEX_getDefaultUint8(p_cursorG);
}

uint8_t PLEX_DEF_cursorB()
{
	if(p_cursorB == -1) return p_defCursorB;

	return PLEX_getDefaultUint8(p_cursorB);
}

bool PLEX_DEF_controllerPause()
{
	if(p_controllerPause == -1) return p_defControllerPause;

	return PLEX_getDefaultBool(p_controllerPause);
}

bool PLEX_DEF_disableController()
{
	if(p_disableController == -1) return p_defDisableController;

	return PLEX_getDefaultBool(p_disableController);
}

bool PLEX_DEF_disableKeyboard()
{
	if(p_disableKeyboard == -1) return p_defDisableKeyboard;

	return PLEX_getDefaultBool(p_disableController);
}

bool PLEX_DEF_disableMouse()
{
	if(p_disableMouse == -1) return p_defDisableMouse;

	return PLEX_getDefaultBool(p_disableMouse);
}

bool PLEX_DEF_24HourTime()
{
	if(p_24hourTime == -1) return p_def42Hour;

	return PLEX_getDefaultBool(p_24hourTime);
}

uint32_t PLEX_DEF_textureDimensLimit()
{
	if(p_textureDimensLimit == -1) return p_defTextureDimensLimit;

	return PLEX_getDefaultUint32(p_textureDimensLimit);
}

bool PLEX_DEF_printErrors()
{
	if(p_printErrors == -1) return p_defShowErrors;

	return PLEX_getDefaultBool(p_printErrors);
}

bool PLEX_DEF_showErrors()
{
	if(p_showErrors == -1) return p_defShowErrors;

	return PLEX_getDefaultBool(p_showErrors);
}

bool PLEX_DEF_dumpErrors()
{
	if(p_dumpErrors == -1) return p_defDumpErrors;

	return PLEX_getDefaultBool(p_dumpErrors);
}

uint8_t PLEX_DEF_printErrorThreshold()
{
	if(p_printErrorThreshold == -1) return p_defPrintErrorThreshhold;

	return PLEX_getDefaultUint8(p_printErrorThreshold);
}

uint8_t PLEX_DEF_showErrorThreshold()
{
	if(p_showErrorThreshold == -1) return p_defShowErrorThreshold;

	return PLEX_getDefaultUint8(p_showErrorThreshold);
}

uint8_t PLEX_DEF_fatalErrorThreshold()
{
	if(p_fatalErrorThreshold == -1) return p_defFatalErrorThreshold;

	return PLEX_getDefaultUint8(p_fatalErrorThreshold);
}

bool PLEX_DEF_heaaderlessCopy()
{
	if(p_heaaderlessCopy == -1) return p_defHeaderlessCopy;

	return PLEX_getDefaultBool(p_heaaderlessCopy);
}

uint8_t PLEX_DEF_memProfile()
{
	if(p_memoryProfile == -1) return p_defMemoryProfile;

	return PLEX_getDefaultUint8(p_memoryProfile);
}

bool PLEX_DEF_memoryReport()
{
	if(p_memReport == -1) return p_defMemoryReport;

	return PLEX_getDefaultBool(p_memReport);
}

bool PLEX_DEF_memoryReportToFile()
{
	if(p_memReportToFile == -1) return p_defMemoryReportToFile;

	return PLEX_getDefaultBool(p_memReportToFile);
}

// do not use getDefault for these!

uint32_t PLEX_DEF_minWindowW(){ return p_minWidth; }
uint32_t PLEX_DEF_minWindowH(){ return p_minHeight; }

uint32_t PLEX_DEF_maxWindowW(){ return p_maxWidth; }
uint32_t PLEX_DEF_maxWindowH(){ return p_maxHeight; }

// the maximum a texture dimension can be

int64_t PLEX_assignDefaultUInt8Int(const char* const name, const uint8_t min, const uint8_t max, const uint8_t def, const char* const comment, const size_t line, const char* const filename)
{
	double adef = 0;

	if(name == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_DEFAULT, PLEX_ERROR_ASSIGNDEFAULT, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_ONASSIGNDEFAULTUINT8, PLEX_ERRORMESS_DEFAULT_NONAME);
		return -1;
	}

	adef = p_validateNumberDefault(min, max, def);

	return p_assignDefault(name, PLEX_DEF_UINT8, min, max, adef, comment, line, filename);
}

int64_t PLEX_assignDefaultUInt32Int(const char* const name, const uint32_t min, const uint32_t max, const uint32_t def, const char* const comment, const size_t line, const char* const filename)
{
	double adef = 0;

	if(name == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_DEFAULT, PLEX_ERROR_ASSIGNDEFAULT, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_ONASSIGNDEFAULTUINT32, PLEX_ERRORMESS_DEFAULT_NONAME);
		return -1;
	}

	adef = p_validateNumberDefault(min, max, def);

	return p_assignDefault(name, PLEX_DEF_UINT32, min, max, adef, comment, line, filename);
}

int64_t PLEX_assignDefaultBoolInt(const char* const name, const bool def, const char* const comment, const size_t line, const char* const filename)
{
	if(name == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_DEFAULT, PLEX_ERROR_ASSIGNDEFAULT, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_ONASSIGNDEFAULTBOOL, PLEX_ERRORMESS_DEFAULT_NONAME);
		return -1;
	}

	return p_assignDefault(name, PLEX_DEF_BOOL, false, true, def, comment, line, filename);
}

int64_t PLEX_assignDefaultDoubleInt(const char* const name, const double min, const double max, const double def, const char* const comment, const size_t line, const char* const filename)
{
	double adef = 0;

	if(name == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_DEFAULT, PLEX_ERROR_ASSIGNDEFAULT, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONASSIGNDEFAULTDOUBLE, PLEX_ERRORMESS_DEFAULT_NONAME);
		return -1;
	}

	adef = p_validateNumberDefault(min, max, def);

	return p_assignDefault(name, PLEX_DEF_DOUBLE, false, true, adef, comment, line, filename);
}

int64_t PLEX_assignDefaultStringInt(const char* const name, const char* const value, const char* const comment, const size_t line, const char* const filename)
{
	uint64_t adef = 0;

	if(name == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_DEFAULT, PLEX_ERROR_ISNULL, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_ONASSIGNDEFAULTSTRING,  PLEX_ERRORMESS_GENERIC_NAME);
		return -1;
	}

	if(value == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_DEFAULT, PLEX_ERROR_ISNULL, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_ONASSIGNDEFAULTSTRING,  PLEX_ERRORMESS_GENERIC_VALUE);
		return -1;
	}

	adef = p_addToStringBank(value, line, filename);

	if(adef == -1)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_DEFAULT, PLEX_ERROR_CREATE, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONASSIGNDEFAULTSTRING, PLEX_ERRORMESS_GENERIC_STRING);
		return -1;
	}

	return p_assignDefault(name, PLEX_DEF_STRING, 0, adef + 1, adef, comment, line, filename);
}

bool PLEX_startDefaults()
{
	if(!PLEX_loadConfig(PLEX_DEFAULT_CONFIGPATH))
	{
		p_stringBank = PLEX_genExpandArrayInt(128, 1, __LINE__, __FILE__); // keep here, if the load config succeeds the string bank is allocated

		if(p_stringBank == NULL)
		{
			PLEX_error2(PLEX_ERROR_CATEGORY_DEFAULT, PLEX_ERROR_ALLOCFAIL, PLEX_ERROR_SEVERITY_FATAL, PLEX_ERRORMESS_ONSTARTDEFAULTS, PLEX_ERRORMESS_DEFAULT_STRINGBANK);
			return false;
		}

		p_populateDefaults(); // keep here, if the load config succeeds it populates defaults anyway
		// don't return here
	}

	// these must be set here because 'error subsystem' is initialized before 'default subsystem'

	PLEX_printErrors(PLEX_DEF_printErrors());
	PLEX_showErrors(PLEX_DEF_showErrors());
	PLEX_dumpErrors(PLEX_DEF_dumpErrors());

	PLEX_setErrorPrintingThreshold((PLEX_ERROR_SEVERITY)PLEX_DEF_printErrorThreshold());
	PLEX_setErrorShowingThreshold((PLEX_ERROR_SEVERITY)PLEX_DEF_showErrorThreshold());
	PLEX_setErrorFatalThreshold((PLEX_ERROR_SEVERITY)PLEX_DEF_fatalErrorThreshold());

	// these must be set here because 'memory subsystem' is initialized before 'default subsystem'

	if(PLEX_DEF_memoryReport()) PLEX_flagMemReport();
	if(PLEX_DEF_memoryReportToFile()) PLEX_flagMemReportToFile();

	PLEX_setMemProfile(PLEX_DEF_memProfile());

	PLEX_updateMemProfile();

	if(p_entries != NULL) PLEX_setGrowthUnitProtoExpandArray(PLEX_getMemProfileDefaultPreload(), p_entries);

	return true;
}

void PLEX_endDefaults()
{
	FILE* config = fopen(PLEX_DEFAULT_CONFIGPATH,"w+");

	if(config != NULL)
	{
		for(size_t z = 0; z < p_entries->count; ++z)
		{
			p_saveDefault(z, config, __LINE__, __FILE__);
		}

		fclose(config);
	}

	PLEX_destroyExpandArray(&p_stringBank);
	PLEX_destroyExpandArray(&p_entries);
}

uint8_t PLEX_getDefaultUint8(const size_t index){ return (uint8_t)p_getDefaultValue(index); }
uint32_t PLEX_getDefaultUint32(const size_t index){ return (uint32_t)p_getDefaultValue(index); }
bool PLEX_getDefaultBool(const size_t index){ return p_getDefaultValue(index); }
double PLEX_getDefaultDouble(const size_t index){ return p_getDefaultValue(index); }

bool PLEX_getDefaultString(const size_t index, char* buffer, const size_t limit)
{
	char* entry = NULL;

	uint32_t len = 0; // < do not make size_t

	if(p_stringBank == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_DEFAULT, PLEX_ERROR_GETNULL, PLEX_ERROR_SEVERITY_MAJOR, PLEX_ERRORMESS_ONGETDEFAULTSTRING, PLEX_ERRORMESS_DEFAULT_STRINGBANK);
		return false;
	}

	if(buffer == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_DEFAULT, PLEX_ERROR_GETNULL, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_ONGETDEFAULTSTRING, PLEX_ERRORMESS_GENERIC_BUFFER);
		return false;
	}

	if(index >= PLEX_expandArrayCount(p_stringBank))
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_DEFAULT, PLEX_ERROR_OVERLIMIT, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONGETDEFAULTSTRING, PLEX_ERRORMESS_GENERIC_INDEX);
		return false;
	}

	if(!limit)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_DEFAULT, PLEX_ERROR_ZEROVALUE, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONGETDEFAULTSTRING, PLEX_ERRORMESS_GENERIC_LIMIT);
		return false;
	}

	entry = (char*)PLEX_expandArrayContents(p_stringBank);
	entry += index;

	memcpy((char*)&len, entry, sizeof(len));
	entry += sizeof(len);

	if(!len) // < not an error, string entries can be blank
	{
		buffer[0] = '\0';
		return true;
	}

	if(len >= limit - 1) len = limit - 2;

	memcpy(buffer, entry, len);
	buffer[len + 1] = '\0';

	return true;
}

bool PLEX_defaultReloadFlag(){ return p_reloadFlag; }
