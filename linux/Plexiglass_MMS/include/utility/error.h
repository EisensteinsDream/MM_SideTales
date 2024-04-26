#ifndef PLEX_ERROR_H
#define PLEX_ERROR_H

#include "terminate.h"

#define PLEX_error(category, type, severity, message) PLEX_addErrorInt(category, type, severity, message, "", __FILE__, __LINE__)

#define PLEX_error2(category, type, severity, message, message2) PLEX_addErrorInt(category, type, severity, message, message2, __FILE__, __LINE__)

#define PLEX_errorNoDesc(category, type, severity) PLEX_addErrorInt(category, type, severity, "", "", __FILE__, __LINE__)

typedef enum
{
	PLEX_ERROR_CATEGORY_NONE = 0, // < unassignable
	PLEX_ERROR_CATEGORY_START = 1,
	PLEX_ERROR_CATEGORY_UPDATE = 2,
	PLEX_ERROR_CATEGORY_SDL = 3,
	PLEX_ERROR_CATEGORY_PORTAUDIO = 4,
	PLEX_ERROR_CATEGORY_MATH = 5,
	PLEX_ERROR_CATEGORY_DATETIME = 6,
	PLEX_ERROR_CATEGORY_FAKESTRING = 7,
	PLEX_ERROR_CATEGORY_DEFAULT = 8,
	PLEX_ERROR_CATEGORY_MEM = 9,
	PLEX_ERROR_CATEGORY_ARRAY = 10,
	PLEX_ERROR_CATEGORY_SOUND = 11,
	PLEX_ERROR_CATEGORY_TEXTURE = 12,
	PLEX_ERROR_CATEGORY_WINDOW = 13,
	PLEX_ERROR_CATEGORY_PROMINENCEMACHINE = 14,
	PLEX_ERROR_CATEGORY_SCREENEFFECTS = 15,
	PLEX_ERROR_CATEGORY_LOWLEVELGRAPHICS_GL = 16, // < not yet implemented
	PLEX_ERROR_CATEGORY_LOWLEVELGRAPHICS_SDL = 17,
	PLEX_ERROR_CATEGORY_FILEPARSE = 18,
	PLEX_ERROR_CATEGORY_INPUT = 19,
	PLEX_ERROR_CATEGORY_GENERATOR = 20,
	PLEX_ERROR_CATEGORY_SPRITETHEATER = 21,
	PLEX_ERROR_CATEGORY_TIMING = 22,
	PLEX_ERROR_CATEGORY_GRAPHICS = 23,
	PLEX_ERROR_CATEGORY_ANIMATION = 24,
	PLEX_ERROR_CATEGORY_DIRECTORY = 25,
	PLEX_ERROR_CATEGORY_SYSTEMREAD = 26, // < nALLOCFAILot yet implemented
	PLEX_ERROR_CATEGORY_INPROJECT = 27 // < for errors in plex projects rather than plex itself
} PLEX_ERROR_CATEGORY;

typedef enum
{
	PLEX_ERROR_START = 0, // subsystem failed to start
	PLEX_ERROR_LOADEXTERNALMODULE = 1, // external module failed to load
	PLEX_ERROR_EXTERNALMODULE = 2, // an error related to the external module
	PLEX_ERROR_CREATE = 3, // failed to create something needed
	PLEX_ERROR_UPDATE = 4, // system failed to update
	PLEX_ERROR_MINMAXREVERSE = 5, // the minimum and maximum are reversed
	PLEX_ERROR_SETNULL = 6, // tried to set a null pointer to something
	PLEX_ERROR_GETNULL = 7, // tried to get a something from a null pointer
	PLEX_ERROR_COMPNULL = 8, // tried to compare the content of two pointers but one or both are null
	PLEX_ERROR_DEFAULTPOP = 9, // default failed to populate for whatever reason
	PLEX_ERROR_ASSIGNDEFAULT = 10, // failed to assign the value of a default
	PLEX_ERROR_FILELOAD = 11, // filed to load a crucial file
	PLEX_ERROR_FILESAVE = 12, // filed to load a crucial file
	PLEX_ERROR_DOUBLESTART = 13, // for whatever reason a system was started multiple times without ending
	PLEX_ERROR_ENDBEFORESTART = 14, // for whatevALLOCFAILer reason a system was started before it ended
	PLEX_ERROR_ACTIONBEFORESTART = 15, // some function inside a system was attempted but stopped because the system has not been started
	PLEX_ERROR_FREENULL = 16, // attempted to deallocate a null pointer
	PLEX_ERROR_ALLOCFAIL = 17, // failed to allocate something
	PLEX_ERROR_CONTENTCORRUPTION = 18, // any error happened because the contents of something are corrupted
	PLEX_ERROR_EXPANDNULL = 19, // something tried to expand from a null array
	PLEX_ERROR_EXPAND = 20, // something failed to expand somewhere
	PLEX_ERROR_INCREMENTNULL = 21, // something tried to increment from a NULL array
	PLEX_ERROR_INCREMENT = 22, // something failed to be incremented
	PLEX_ERROR_DECREMENTNULL = 23, // something tried to decrement from a NULL array
	PLEX_ERROR_DECREMENT = 24, // something failed to be decremented
	PLEX_ERROR_CLEAR = 25, // something failed to be cleared
	PLEX_ERROR_SWAP = 26, // something failed to be swapped
	PLEX_ERROR_CUT = 27, // something failed to be cut
	PLEX_ERROR_REARRANGE = 28, // something failed to be cut
	PLEX_ERROR_NOGETARRAY = 29, // a function failed to get a value from an array
	PLEX_ERROR_SOUNDLOCK = 30, // a function failed to get out of a reserved sound thread
	PLEX_ERROR_BADBUFFERSOUNDHOOK = 31, // for whatever reason the buffer in the soundhook is bad
	PLEX_ERROR_OVERLIMIT = 32, // tried to create something over the designated limit allowed
	PLEX_ERROR_ISNULL = 33, // something is NULL but probably shouldn't be
	PLEX_ERROR_SAMEVALUE = 34, // two values that should be different are the same
	PLEX_ERROR_UNDERLIMIT = 35, // the number is under the limit somewhere
	PLEX_ERROR_ZEROVALUE = 36, // a value is zero somewhere where it shouldn't be
	PLEX_ERROR_DISABLED = 37, // attempted to use a disabled feature
	PLEX_ERROR_BADOPERATION = 38, // an operation has been blocked for whatever reason
	PLEX_ERROR_HOOKCORRUPTION = 39, // the corruption of a hook that should not be accessed externally
	PLEX_ERROR_BEGINNINGOVER = 40, // the beginning of something is over what it should be
	PLEX_ERROR_BEGINNINGUNDER = 41, // the beginning of something is under what it should be
	PLEX_ERROR_NOLEN = 42, // something has no length value
	PLEX_ERROR_LENGTHOVER = 43, // the length of something is over what it should be
	PLEX_ERROR_LENGTHUNDER = 44, // the length of something is under what it should be
	PLEX_ERROR_UPDATEBEFORESTART = 45, // tried to update before starting
	PLEX_ERROR_SUCCESSCHECKFAIL = 46, // a success check has failed
	PLEX_ERROR_ISNONTEXTURE = 47, // an operation requiring a texture has been passed a non-texture (ie. null texture)
	PLEX_ERROR_SOUNDHOOKINUSE = 48, // a process was halted because a sound hook is being used
	PLEX_ERROR_SOUNDCHANNELSTATE = 49, // a process was halted because either the sound channel is already on or off
	PLEX_ERROR_ACQUIRE = 50, // failed to acquire something for whatever reason
	PLEX_ERROR_STREAMSTATE = 51, // a process was halted because the sound stream was either inactive when it should have been active or vice versa
	PLEX_ERROR_FILETYPE = 52, // is the wrong filetype for a particular operation
	PLEX_ERROR_DOUBLEDEALLOC = 53, // attempting to deallocating already deallocated memory
	PLEX_ERROR_DIRECTORYOPENSTATE = 54 // the wrong directory open state
} PLEX_ERROR_TYPE;

typedef enum
{
	PLEX_ERROR_SEVERITY_NONE = 0, // < unassignable to error
	PLEX_ERROR_SEVERITY_IMPOSSIBLE = 1,
	PLEX_ERROR_SEVERITY_FATAL = 2,
	PLEX_ERROR_SEVERITY_MAJOR = 3,
	PLEX_ERROR_SEVERITY_MINOR = 4,
	PLEX_ERROR_SEVERITY_BADSTYLE = 5
} PLEX_ERROR_SEVERITY;

typedef struct
{
	PLEX_ERROR_CATEGORY category;
	PLEX_ERROR_TYPE type;
	PLEX_ERROR_SEVERITY severity;
	uint32_t lineNumber;
	const char* message, *message2, *fileloc;
} PLEX_ERROR;

#ifdef __cplusplus
extern"C"
{
#endif
	// --- PLEX errors ---

	// PLEX_fileLoadErrorPath is in fakestrings.h

	extern void PLEX_showErrorPopup(const char* const title, const char* const message); // < defined in window.c

	extern void PLEX_addErrorInt(const PLEX_ERROR_CATEGORY category, const PLEX_ERROR_TYPE type, const PLEX_ERROR_SEVERITY severity, const char* const message, const char* const message2, const char* const fileloc, const size_t lineNumber);

	extern bool PLEX_printError(const PLEX_ERROR toPrint, const bool ignoreThreshold);
	extern bool PLEX_showError(const PLEX_ERROR toShow, const bool ignoreThreshold);
	extern bool PLEX_dumpError(const PLEX_ERROR toDump, FILE* file);

	extern void PLEX_setErrorLogDate(); // < defined in datetime.c
	extern void PLEX_setErrorLogDateInt(const uint16_t year, const uint8_t month, const uint8_t day, const uint8_t hour, const uint8_t minute, const uint8_t second); // < do not use
	extern void PLEX_getErrorLogDateInt(uint16_t* year, uint8_t* month, uint8_t* day, uint8_t* hour, uint8_t* minute, uint8_t* second);

	extern void PLEX_setErrorPrintingThreshold(const PLEX_ERROR_SEVERITY threshold);
	extern void PLEX_setErrorShowingThreshold(const PLEX_ERROR_SEVERITY threshold);

	extern void PLEX_setErrorFatalThreshold(const PLEX_ERROR_SEVERITY threshold);

	extern void PLEX_printErrors(const bool set);
	extern void PLEX_showErrors(const bool set);
	extern void PLEX_dumpErrors(const bool set);

	extern const char* PLEX_getCategoryString(const PLEX_ERROR_CATEGORY category);
	extern const char* PLEX_getTypeString(const PLEX_ERROR_TYPE type);
	extern const char* PLEX_getSeverityString(const PLEX_ERROR_SEVERITY severity);

	extern const char* PLEX_getTypeStringLong(const PLEX_ERROR_TYPE type);
	extern const char* PLEX_getSeverityStringLong(const PLEX_ERROR_SEVERITY severity);

	// these will be done automatically, no need to call them.
	extern void PLEX_flagSDLError();
	extern void PLEX_unflagSDLError();

	// --- Checks for Non-PLEX errors ---

	// checks for an SDL error and throws it if there is one present
	extern bool PLEX_errorSDL(const PLEX_ERROR_SEVERITY severity);

	// checks for an SDL image error and throws it if there is one present
	extern bool PLEX_errorSDLImage(const PLEX_ERROR_SEVERITY severity);

	// checks for a port audio error and shows it if there is one present
	extern bool PLEX_errorPortAudio(const PLEX_ERROR_SEVERITY severity, const PaError error);

	extern bool PLEX_startErrorSystem();

	extern bool PLEX_updateErrorSystem();

	extern void PLEX_endErrorSystem();
#ifdef __cplusplus
}
#endif

#endif
