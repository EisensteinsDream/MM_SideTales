#ifndef PLEX_DEFAULTNAMES_EN_H
#define PLEX_DEFAULTNAMES_EN_H

// This is a header which contains all of the default name macro strings for the English version of PLEX. This is basically the error message equivalent of "meta/language/language_en.h". More information can be found there or in "meta/language.h". As with all headers in the meta folder, there is no correlating source file.

// the name of the file itself

#define PLEX_DEFAULT_FILENAME "config_en.pcfg"

// NAMES:

// termination

#define PLEX_DEFNAME_KILLONESCAPE "kill on escape"
#define PLEX_DEFNAME_KILLONCLOSE "kill on close"

// windowing

#define PLEX_DEFNAME_MAINWINWIDTH "main window width"
#define PLEX_DEFNAME_MAINWINHEIGHT "main window height"

#define PLEX_DEFNAME_ALLOWRESIZE "allow resize"
#define PLEX_DEFNAME_ALLOWMAXIMIZE "allow maximize"
#define PLEX_DEFNAME_STARTMAXIMIZED "start maximized"

// input

#define PLEX_DEFNAME_CURSOR_R "cursor red"
#define PLEX_DEFNAME_CURSOR_G "cursor green"
#define PLEX_DEFNAME_CURSOR_B "cursor blue"

#define PLEX_DEFNAME_CONTROLLERPAUSE "allow controller pause"

#define PLEX_DEFNAME_DISABLECONTROLLER "disable controller"
#define PLEX_DEFNAME_DISABLEJOYSTICK "disable joystick"
#define PLEX_DEFNAME_DISABLEKEYBOARD "disable keyboard"
#define PLEX_DEFNAME_DISABLEMOUSE "disable mouse"

#define PLEX_DEFNAME_CONFIGRELOADBUTTON "config reload button"

// sound

#define PLEX_DEFNAME_DISABLESOUND "disable sound"

// date and time

#define PLEX_DEFNAME_24HOURTIME "24 hour time"

// DESCRIPTIONS:

// termination

#define PLEX_DEFDESC_KILLONESCAPE "Controls if the ESC key kills the program. Either this or the kill on close need to be true."
#define PLEX_DEFDESC_KILLONCLOSE "Controls if closing the window kills the program. Either this or the kill on close need to be true."

// windowing

#define PLEX_DEFDESC_MAINWINWIDTH "Width of the window must be between 800 and 4096"
#define PLEX_DEFDESC_MAINWINHEIGHT "Height of the window must be between 600 and 2160"

#define PLEX_DEFDESC_ALLOWRESIZE "Controls whether or not you can resize the game window."
#define PLEX_DEFDESC_ALLOWMAXIMIZE "Controls whether or not you can maximize the window. Window resize must be enabled for this to work"
#define PLEX_DEFDESC_STARTMAXIMIZED "Starts window maximized (must have both allow maximize and allow resize enabled)"

// input

#define PLEX_DEFDESC_CURSOR_R "Sets cursor to red (only works with custom cursors)"
#define PLEX_DEFDESC_CURSOR_G "Sets cursor to green (only works with custom cursors)"
#define PLEX_DEFDESC_CURSOR_B "Sets cursor to blue (only works with custom cursors)"

#define PLEX_DEFDESC_CONTROLLERPAUSE "Decides whether or not the 'pause' button on a controller pauses the game"

#define PLEX_DEFDESC_DISABLECONTROLLER "Disable controller input"
#define PLEX_DEFDESC_DISABLEJOYSTICK "Disable joystick input (currently does nothing)"
#define PLEX_DEFDESC_DISABLEKEYBOARD "Disable keyboard input (does not work if both controller and mouse are disabled, does not interfere with close on escape)"
#define PLEX_DEFDESC_DISABLEMOUSE "disable mouse input"

#define PLEX_DEFDESC_CONFIGRELOADBUTTON "Decides whether or not you are able to reload the config by pressing ctrl+F1 (experimental/dangerous)"

// sound

#define PLEX_DEFDESC_DISABLESOUND "Disables all sound if true"

// time

#define PLEX_DEFDESC_24HOURTIME "Sets the default time to 24-hours. Some times may still be 12, but those not specified as 12 in code will not."

#endif
