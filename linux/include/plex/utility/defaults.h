#ifndef PLEX_DEFAULT_H
#define PLEX_DEFAULT_H

// This is the unit that manages the 'defaults'. Basically any value managed externally by the config_xx.pcfg file. This is a file that every PLEX based program should have and will even autogenerate every single time a PLEX based program is closed properly and one is not there. It's also always loaded at the beginning of execution. Also, this library also allows for new defaults to be added specific to the particular plex based program.

// NOTE: this is a very crucial yet very strange part of the PLEX ecosystem. Some small things like what the default of the default values will change. Tons will be added. The internals might change quite a bit to account for safety and efficiency.

#include "meta/cinclude.h"
#include "meta/defaultnames.h"
#include "meta/defaultlimits.h"

#include "mem/expandarray.h"

#include "mem/mem.h"

#include "utility/error.h"

// an enumerator that portrays a certain entry type. This will likely eventually also have a string type.

typedef enum
{
	PLEX_DEF_UINT8,
	PLEX_DEF_UINT32,
	PLEX_DEF_BOOL,
	PLEX_DEF_DOUBLE,
	PLEX_DEF_STRING
} PLEX_DEF_ENTRYTYPE;

#ifdef __cplusplus
extern "C"
{
#endif
	extern bool PLEX_loadConfig(const char* fileloc);

	// these functions return default values after they were determined from loading a config file or initialized

	// returns the default width and height of the screen. It's safe to say whenever the words 'width' or 'height' appear in code with no descriptors they signify the scren width and screen height.
	extern uint32_t PLEX_DEF_width();
	extern uint32_t PLEX_DEF_height();

	// whether or not you can kill the program by escaping or closing. One of these must be true in order to prevent and inescapable program.
	extern bool PLEX_DEF_killOnEscape();
	extern bool PLEX_DEF_killOnClose();

	// whether the program will allow window resizing or maximization.
	extern bool PLEX_DEF_allowResize();
	extern bool PLEX_DEF_allowMaximize();

	// whether the program will start maximized or not.
	extern bool PLEX_DEF_startMaximized();

	// whether the audio is disabled in the program or not.
	extern bool PLEX_DEF_disableAudio();

	// the default red, green, and blue of custom cursors
	extern uint8_t PLEX_DEF_cursorR();
	extern uint8_t PLEX_DEF_cursorG();
	extern uint8_t PLEX_DEF_cursorB();

	// whether or not the config can be reloaded by pressing ctrl+f1
	extern bool PLEX_DEF_configReloadButton();

	// determines whether or not the 'start' button on a controller will pause
	extern bool PLEX_DEF_controllerPause();

	// whether or not an individual input type is disabled. It will not allow all to be disabled (defaults to allowing the keyboard). None of these will effect exiting on escape or close.
	extern bool PLEX_DEF_disableController();
	extern bool PLEX_DEF_disableJoystick();
	extern bool PLEX_DEF_disableKeyboard();
	extern bool PLEX_DEF_disableMouse();

	// defaults based off datetime format
	extern bool PLEX_DEF_24HourTime();

	// returns the smallest the window minimum width and height can be brought to
	extern uint32_t PLEX_DEF_minWindowW();
	extern uint32_t PLEX_DEF_minWindowH();

	// returns the largest the window maximum width and height can be brought to
	extern uint32_t PLEX_DEF_maxWindowW();
	extern uint32_t PLEX_DEF_maxWindowH();

	// The next few functions assign new defaults based on type. These functions are for extending the system and adding new defaults that are program specific to the config.
	extern int64_t PLEX_assignDefaultUInt8(const char* const name, const uint8_t min, const uint8_t max, const uint8_t def, const char* const comment);
	extern int64_t PLEX_assignDefaultUInt32(const char* const name, const uint32_t min, const uint32_t max, const uint32_t def, const char* const comment);
	extern int64_t PLEX_assignDefaultBool(const char* const name, const bool def, const char* const comment);
	extern int64_t PLEX_assignDefaultDouble(const char* const name, const double min, const double max, const double def, const char* const comment);
	extern int64_t PLEX_assignDefaultString(const char* const name, const char* const value, const char* const comment); // < experimental/untested

	// The start and end default functions which are automatically executed during the start and end of PLEX.
	extern bool PLEX_startDefaults();
	extern void PLEX_endDefaults();

	// a function to indicate the config file needs to be reloaded. It's highly reccomended you never use this function. It's mainly just for reloading by ctrl+f1.
	extern void PLEX_reloadDefaults();

	// this is a universal flag that goes throughout the program to tell anything that might need to reload if the config reloads to reload. Again, this is highly reccomended never to be actually used outside of the preexisting ctrl+f1 reload system.
	extern void PLEX_uncheckDefaultReloadFlag();

	// this will return the value of a default from it's index
	extern uint8_t PLEX_getDefaultUint8(const size_t index);
	extern uint32_t PLEX_getDefaultUint32(const size_t index);
	extern bool PLEX_getDefaultBool(const size_t index);
	extern double PLEX_getDefaultDouble(const size_t index);
	extern bool PLEX_getDefaultString(const size_t index, char* buffer, const size_t limit); // < experimental/untested

	// now, this one is important for custom defaults. It returns whether or not the reload defautls flag has been checked this iteration. You can use this to gauge whether or not you need to reset aspects of a program reliant on defaults. Essentially, if PLEX is operating properly, it's true if the ctrl+f1 command is hit to reload the defaults and it's enabled. It's false if it's not.'
	extern bool PLEX_defaultReloadFlag();
#ifdef __cplusplus
}
#endif

#endif
