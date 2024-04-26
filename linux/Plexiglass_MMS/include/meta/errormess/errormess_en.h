 #ifndef PLEX_ERRORMESS_EN_H
#define PLEX_ERRORMESS_EN_H

// This is a header which contains all of the error macro strings for the English version of PLEX. This is basically the error message equivalent of "meta/language/language_en.h". More information can be found there or in "meta/language.h". As with all headers in the meta folder, there is no correlating source file.

// ERROR CONSTRUCTION STRINGS

#define PLEX_ERRORMESS_ERROR_FILENAMEMID " for file "
#define PLEX_ERRORMESS_ERROR_CATEGORY "\" in category \""
#define PLEX_ERRORMESS_ERROR_DUMPTHRESHOLDCHANGED "Error dumping threshold has been changed to \""
#define PLEX_ERRORMESS_ERROR_DUMPTHRESHOLDCHANGED2 "\". Any error with an equal or above severity will not be dumped to error file."
#define PLEX_ERRORMESS_ERROR_FILE " in file \""
#define PLEX_ERRORMESS_ERROR_FOOTER ") "
#define PLEX_ERRORMESS_ERROR_HEADER "PLEX Error! "
#define PLEX_ERRORMESS_ERROR_LINE "@ line "
#define PLEX_ERRORMESS_ERROR_LOGCREATED "An error log named \""
#define PLEX_ERRORMESS_ERROR_LOGCREATED2 "\" has been created."
#define PLEX_ERRORMESS_ERROR_MESSAGEEND "\")"
#define PLEX_ERRORMESS_ERROR_MESSAGESPLIT ": \""
#define PLEX_ERRORMESS_ERROR_MESSAGESPLIT2 "\" | \""
#define PLEX_ERRORMESS_ERROR_MESSAGESTART "\" ("
#define PLEX_ERRORMESS_ERROR_NOTERMINATE "Not terminated."
#define PLEX_ERRORMESS_ERROR_PRINTINGSILENCED "Error printing has been silenced here. Nothing will print until it's unsilenced."
#define PLEX_ERRORMESS_ERROR_PRINTINGUNSILENCED "Error printing has been unsilenced here. Anything within the error threshold will be printed. The threshold for printing is: "
#define PLEX_ERRORMESS_ERROR_PRINTTHRESHOLDCHANGED "Error printing threshold has been changed to \""
#define PLEX_ERRORMESS_ERROR_PRINTTHRESHOLDCHANGED2 "\". No error with a severity below this will be printed."
#define PLEX_ERRROMESS_ERROR_DUMPFILEFAIL "Failed to open a file to dump errors. If you want dump errors on, make sure to have a folder called 'error' in your main directory. If you want dumping off, set dump errors to false in the config."
#define PLEX_ERRROMESS_ERROR_SUPPRESSFILEFAIL "Failed to open a file to dump suppressed errors. If you want dump errors on, make sure to have a folder called 'error' in your main directory. If you want dumping off, set dump errors to false in the config."

#define PLEX_ERRORMESS_ERROR_SEVERITY ". Error Severity: "
#define PLEX_ERRORMESS_ERROR_SEVERITY_LONG " ("
#define PLEX_ERRORMESS_ERROR_SHOWINGSILENCED "Error showing has been deactivated here. No error pop-ups will until it's unsilenced."
#define PLEX_ERRORMESS_ERROR_SHOWINGUNSILENCED "Error showing has been activated here. Anything within the error threshold will be shown. The threshold for showing is: "
#define PLEX_ERRORMESS_ERROR_SHOWNTHRESHOLDCHANGED "Error showing threshold has been changed to \""
#define PLEX_ERRORMESS_ERROR_SHOWNTHRESHOLDCHANGED2 "\". No error with a severity below this will be shown in a popup."
#define PLEX_ERRORMESS_ERROR_SHOWOVERFLOW "Pop-up errors have reached their printable limit. This is probably something very bad. The program will terminate. Recompiling code from source is reccomended. If that doesn't work, please report bug."
#define PLEX_ERRORMESS_ERROR_STACKOVERFLOW "TOO MANY ERRORS SHOWN! WILL NOT SHOW ANY MORE!"
#define PLEX_ERRORMESS_ERROR_START "Error!"
#define PLEX_ERRORMESS_ERROR_SYSTEMFAIL_NOOS "No operating system detected! This means that PLEX somehow compiled incorrectly."
#define PLEX_ERRORMESS_ERROR_SYSTEMFAIL_INIT "The error system has failed on initialization!"
#define PLEX_ERRORMESS_ERROR_SYSTEMFAIL_UPDATE "The error system has failed to update. The program is terminating."
#define PLEX_ERRORMESS_ERROR_TERMINATE "TERMINATED!"
#define PLEX_ERRORMESS_ERROR_TERMINATEDTHRESHOLDCHANGED "Fatal error threshold has been changed to \""
#define PLEX_ERRORMESS_ERROR_TERMINATEDTHRESHOLDCHANGED2 "\". Any error with an equal or above severity will terminate the program."
#define PLEX_ERRORMESS_ERROR_MESSDIVIDE " | "
#define PLEX_ERRORMESS_ERROR_EXCLAMATION "! "
#define PLEX_ERRORMESS_ERROR_POPUPTITLE " Error! "
#define PLEX_ERRORMESS_ERROR_FATALERROR "Terminal Error! Program will be terminated!"
#define PLEX_ERRORMESS_ERROR_ATTEMPTIMPOSSIBLEERROR "You cannot set the error termination threshold above 'fatal'. Which means it's impossible to set it to 'impossible' or 'none'. This cannot happen. It will be set to 'fatal'."
#define PLEX_ERRORMESS_ERROR_ATTEMPTNOCATEGORY "You cannot create an error with the category of 'none'"
#define PLEX_ERRORMESS_ERROR_NOSEVERITY "You cannot create an error with severity of 'none'"
#define PLEX_ERRORMESS_ERROR_NOCATEGORY "You cannot create an error with category of 'none'"

// CATEGORY STRINGS

#define PLEX_ERRORMESS_CATEGORY_NONE "None (This should be impossible)"
#define PLEX_ERRORMESS_CATEGORY_START "Start"
#define PLEX_ERRORMESS_CATEGORY_UPDATE "Update"
#define PLEX_ERRORMESS_CATEGORY_SDL "SDL"
#define PLEX_ERRORMESS_CATEGORY_PORTAUDIO "PortAudio"
#define PLEX_ERRORMESS_CATEGORY_MATH "Math"
#define PLEX_ERRORMESS_CATEGORY_DATETIME "Datetime"
#define PLEX_ERRORMESS_CATEGORY_FAKESTRING "Fakestrings"
#define PLEX_ERRORMESS_CATEGORY_DEFAULT "Default"
#define PLEX_ERRORMESS_CATEGORY_MEM "Memory"
#define PLEX_ERRORMESS_CATEGORY_ARRAY "Array"
#define PLEX_ERRORMESS_CATEGORY_SOUND "Sound"
#define PLEX_ERRORMESS_CATEGORY_TEXTURE "Texture"
#define PLEX_ERRORMESS_CATEGORY_WINDOW "Window"
#define PLEX_ERRORMESS_CATEGORY_PROMINENCEMACHINE "Prominence Machine"
#define PLEX_ERRORMESS_CATEGORY_SCREENEFFECTS "Screen Effects"
#define PLEX_ERRORMESS_CATEGORY_LOWLEVELGRAPHICS_GL "Low Level Graphics - GL"
#define PLEX_ERRORMESS_CATEGORY_LOWLEVELGRAPHICS_SDL "Low Level Graphics - SDL"
#define PLEX_ERRORMESS_CATEGORY_FILEPARSE "File Parser"
#define PLEX_ERRORMESS_CATEGORY_INPUT "Input"
#define PLEX_ERRORMESS_CATEGORY_GENEARTOR "Generator"
#define PLEX_ERRORMESS_CATEGORY_SPRITETHEATER "Sprite Theater"
#define PLEX_ERRORMESS_CATEGORY_TIMING "Timing System"
#define PLEX_ERRORMESS_CATEGORY_GRAPHICS "Graphics"
#define PLEX_ERRORMESS_CATEGORY_ANIMATION "Animation"
#define PLEX_ERRORMESS_CATEGORY_DIRECTORY "Directory"
#define PLEX_ERRORMESS_CATEGORY_SYSTEMREAD "System Read"
#define PLEX_ERRORMESS_CATEGORY_INPROJECT "In Project Error"

// SEVERITY STRINGS

#define PLEX_ERRORMESS_SEVERITY_NONE "None"
#define PLEX_ERRORMESS_SEVERITY_NONE_LONG "It should be impossible to print an error of severity 'none'. You shouldn't see this message. I reccomend recompiling the source from the original download. Unless you modified it and are confident you know what you are doing. If that's the case, please keep it impossible to set error severities to 'none'. If this persists after rebuilding the source report it @ https://eisensteinsdream.github.io/"
#define PLEX_ERRORMESS_SEVERITY_IMPOSSIBLE "Severity"
#define PLEX_ERRORMESS_SEVERITY_IMPOSSIBLE_LONG "Impossible errors are the most serious errors. They only happen if something was corrupted by a bad modification of code. Every impossible error is fatal"
#define PLEX_ERRORMESS_SEVERITY_FATAL "Fatal"
#define PLEX_ERRORMESS_SEVERITY_FATAL_LONG "Fatal error indicates the program cannot safely continue after error is posted. This should always terminate the program"
#define PLEX_ERRORMESS_SEVERITY_MAJOR "Major"
#define PLEX_ERRORMESS_SEVERITY_MAJOR_LONG "A major error isn't quite as serious as a fatal error but it's something to be concerned about. It may terminate the program based on plex settings. There's a config file entry if you only want 'fatal' and 'impossible' errors to execute"
#define PLEX_ERRORMESS_SEVERITY_MINOR "Minor"
#define PLEX_ERRORMESS_SEVERITY_MINOR_LONG "Minor errors may be a problem but rarely ever are on their own. It's good for optimization and final unit checks to make this terminate. Otherwise, it really shouldn't. If this error causes to terminate otherwise, change the severity level in the config file"
#define PLEX_ERRORMESS_SEVERITY_BADSTYLE "Bad Style"
#define PLEX_ERRORMESS_SEVERITY_BADSTYLE_LONG "An error which is pretty much harmless but might be an indication of some sloppy code. Typically you don't even want this error to print let alone terminate unless you are developing something yourself. The print, show, and terminate levels are in the config file"

// TYPE STRINGS

#define PLEX_ERRORMESS_TYPE_NONETYPE "No Type - This shouldn't happen"
#define PLEX_ERRORMESS_TYPE_START "Start Failure"
#define PLEX_ERRORMESS_TYPE_LOADEXTERNALMODULE "Load External Module Failure"
#define PLEX_ERRORMESS_TYPE_EXTERNALMODULE "External Module Error"
#define PLEX_ERRORMESS_TYPE_CREATE "Creation Failure"
#define PLEX_ERRORMESS_TYPE_UPDATE "Update Failure"
#define PLEX_ERRORMESS_TYPE_MINMAXREVERSE "Min/Max Reverse"
#define PLEX_ERRORMESS_TYPE_SETNULL "Set NULL Attempt"
#define PLEX_ERRORMESS_TYPE_GETNULL "Get from NULL Attempt"
#define PLEX_ERRORMESS_TYPE_COMPNULL "NULL Compare Attempt"
#define PLEX_ERRORMESS_TYPE_DEFAULTPOP "Default Populate Fail"
#define PLEX_ERRORMESS_TYPE_ASSIGNDEFAULT "Assign Default Fail"
#define PLEX_ERRORMESS_TYPE_FILELOAD "File Load Fail"
#define PLEX_ERRORMESS_TYPE_FILESAVE "File Save Fail"
#define PLEX_ERRORMESS_TYPE_DOUBLESTART "Double Start"
#define PLEX_ERRORMESS_TYPE_ENDBEFORESTART "End Before Start"
#define PLEX_ERRORMESS_TYPE_ACTIONBEFORESTART "Action Execute Before Start"
#define PLEX_ERRORMESS_TYPE_FREENULL "Free Null Attempt"
#define PLEX_ERRORMESS_TYPE_ALLOCFAIL "Allocation Failure"
#define PLEX_ERRORMESS_TYPE_CONTENTCORRUPTION "Content Corruption"
#define PLEX_ERRORMESS_TYPE_EXPANDNULL "Expand NULL Failure"
#define PLEX_ERRORMESS_TYPE_EXPAND "Expand Failure"
#define PLEX_ERRORMESS_TYPE_INCREMENTNULL "Increment NULL Attempt"
#define PLEX_ERRORMESS_TYPE_INCREMENT "Increment Failure"
#define PLEX_ERRORMESS_TYPE_DECREMENTNULL "Decrement NULL Attempt"
#define PLEX_ERRORMESS_TYPE_DECREMENT "Decrement Failure"
#define PLEX_ERRORMESS_TYPE_CLEAR "Clear Failure"
#define PLEX_ERRORMESS_TYPE_SWAP "Swap Failure"
#define PLEX_ERRORMESS_TYPE_CUT "Cut Failure"
#define PLEX_ERRORMESS_TYPE_REARRANGE "Rearrange Failure"
#define PLEX_ERRORMESS_TYPE_NOGETARRAY "Array Return fail"
#define PLEX_ERRORMESS_TYPE_SOUNDLOCK "Sound Lock Breakout Failure"
#define PLEX_ERRORMESS_TYPE_BADBUFFERSOUNDHOOK "Bad Buffer in Sound Hook"
#define PLEX_ERRORMESS_TYPE_OVERLIMIT "Out of Range"
#define PLEX_ERRORMESS_TYPE_ISNULL "Is Null"
#define PLEX_ERRORMESS_TYPE_SAMEVALUE "Same Value"
#define PLEX_ERRORMESS_TYPE_UNDERLIMIT "Under Limit"
#define PLEX_ERRORMESS_TYPE_ZEROVALUE "Zero Value"
#define PLEX_ERRORMESS_TYPE_DISABLED "Disabled Feature"
#define PLEX_ERRORMESS_TYPE_BADOPERATION "Bad Operation"
#define PLEX_ERRORMESS_TYPE_HOOKCORRUPTION "Hook Corruption"
#define PLEX_ERRORMESS_TYPE_BEGINNINGOVER "Beginning Over"
#define PLEX_ERRORMESS_TYPE_BEGINNINGUNDER "Beginning Under"
#define PLEX_ERRORMESS_TYPE_NOLEN "No Length"
#define PLEX_ERRORMESS_TYPE_LENGTHOVER "Length Over"
#define PLEX_ERRORMESS_TYPE_LENGTHUNDER "Length Under"
#define PLEX_ERRORMESS_TYPE_UPDATEBEFORESTART "Update Before Start"
#define PLEX_ERRORMESS_TYPE_SUCCESSCHECKFAIL "Success Check Failed"
#define PLEX_ERRORMESS_TYPE_ISNONTEXTURE "Non-Texture"
#define PLEX_ERRORMESS_TYPE_SOUNDHOOKINUSE "Sound Hook In Use"
#define PLEX_ERRORMESS_TYPE_SOUNDCHANNELSTATE "Sound Channel State"
#define PLEX_ERRORMESS_TYPE_ACQUIRE "Acquire"
#define PLEX_ERRORMESS_TYPE_STREAMSTATE "Sound Stream State"
#define PLEX_ERRORMESS_TYPE_FILETYPE "File type"
#define PLEX_ERRORMESS_TYPE_DOUBLEDEALLOC "Double Dealloc"
#define PLEX_ERRORMESS_TYPE_DIRECTORYOPENSTATE "Directory State"

// LONG TYPE STRINGS

#define PLEX_ERRORMESS_TYPE_NONETYPE_LONG "No Type Description - this shouldn't happen"
#define PLEX_ERRORMESS_TYPE_START_LONG "Subsystem failed to start"
#define PLEX_ERRORMESS_TYPE_LOADEXTERNALMODULE_LONG "External module failed to load"
#define PLEX_ERRORMESS_TYPE_EXTERNALMODULE_LONG "Error related to external module function"
#define PLEX_ERRORMESS_TYPE_CREATE_LONG "failed to create something"
#define PLEX_ERRORMESS_TYPE_UPDATE_LONG "failed to update something"
#define PLEX_ERRORMESS_TYPE_MINMAXREVERSE_LONG "the minimum and maximum are reversed"
#define PLEX_ERRORMESS_TYPE_SETNULL_LONG "tried to set a NULL pointer to something"
#define PLEX_ERRORMESS_TYPE_GETNULL_LONG "tried to get a NULL pointer from something"
#define PLEX_ERRORMESS_TYPE_COMPNULL_LONG "tried to compare two pointers but one or both are NULL"
#define PLEX_ERRORMESS_TYPE_DEFAULTPOP_LONG "a default value failed to populate"
#define PLEX_ERRORMESS_TYPE_ASSIGNDEFAULT_LONG "failed to assign the value of a default"
#define PLEX_ERRORMESS_TYPE_FILELOAD_LONG "failed to load a file"
#define PLEX_ERRORMESS_TYPE_FILESAVE_LONG "failed to save a file"
#define PLEX_ERRORMESS_TYPE_DOUBLESTART_LONG "attempted to double start a system"
#define PLEX_ERRORMESS_TYPE_ENDBEFORESTART_LONG "attempted to end a system before starting it"
#define PLEX_ERRORMESS_TYPE_ACTIONBEFORESTART_LONG "attempted an action before a system set up"
#define PLEX_ERRORMESS_TYPE_FREENULL_LONG "attempted to free a null"
#define PLEX_ERRORMESS_TYPE_ALLOCFAIL_LONG "failed to allocate a piece of memory"
#define PLEX_ERRORMESS_TYPE_CONTENTCORRUPTION_LONG "contents of something have been corrupted"
#define PLEX_ERRORMESS_TYPE_EXPANDNULL_LONG "attempted to expand an array from null pointer or null contents"
#define PLEX_ERRORMESS_TYPE_EXPAND_LONG "failed to expand array contents"
#define PLEX_ERRORMESS_TYPE_INCREMENTNULL_LONG "attempted to increment an array from null pointer or null contents"
#define PLEX_ERRORMESS_TYPE_INCREMENT_LONG "failed to increment array contents"
#define PLEX_ERRORMESS_TYPE_DECREMENTNULL_LONG "attempted to decrement an array from null pointer or null contents"
#define PLEX_ERRORMESS_TYPE_DECREMENT_LONG "failed to decrement array contents"
#define PLEX_ERRORMESS_TYPE_CLEAR_LONG "failed to clear an array"
#define PLEX_ERRORMESS_TYPE_SWAP_LONG "failed to swap array"
#define PLEX_ERRORMESS_TYPE_CUT_LONG "failed to cut array"
#define PLEX_ERRORMESS_TYPE_REARRANGE_LONG "failed to rearrange array"
#define PLEX_ERRORMESS_TYPE_NOGETARRAY_LONG "A function intended to return a value on an array returned NULL instead"
#define PLEX_ERRORMESS_TYPE_SOUNDLOCK_LONG "failed to break out of locked sound channel"
#define PLEX_ERRORMESS_TYPE_BADBUFFERSOUNDHOOK_LONG "the buffer in the sound hook is bad for whatever reason"
#define PLEX_ERRORMESS_TYPE_OVERLIMIT_LONG "went out of bounds when accessing a value"
#define PLEX_ERRORMESS_TYPE_ISNULL_LONG "something is NULL that really shouldn't be"
#define PLEX_ERRORMESS_TYPE_SAMEVALUE_LONG "Two values that shouldn't be the same are the same"
#define PLEX_ERRORMESS_TYPE_UNDERLIMIT_LONG "Value is under the limit"
#define PLEX_ERRORMESS_TYPE_ZEROVALUE_LONG "Value is zero where it cannot be"
#define PLEX_ERRORMESS_TYPE_DISABLED_LONG "Tried to access a disabled feature. Usually, this is completely harmless. If not intended, then check the config file to see what's disabled."
#define PLEX_ERRORMESS_TYPE_BADOPERATION_LONG "This operation has been blocked for whatever reason."
#define PLEX_ERRORMESS_TYPE_HOOKCORRUPTION_LONG "A hook function has been corrupted somewhere. This is serious and likely demands the source to be recompiled."
#define PLEX_ERRORMESS_TYPE_BEGINNINGOVER_LONG "The beginning of a value has went over the limit"
#define PLEX_ERRORMESS_TYPE_BEGINNINGUNDER_LONG "The beginning of a value is under the limit"
#define PLEX_ERRORMESS_TYPE_NOLEN_LONG "Something created or accessed has no length value"
#define PLEX_ERRORMESS_TYPE_LENGTHOVER_LONG "The length of a value has went over the limit"
#define PLEX_ERRORMESS_TYPE_LENGTHUNDER_LONG "The length of a value is under the limit"
#define PLEX_ERRORMESS_TYPE_UPDATEBEFORESTART_LONG "Attempted to update a subsystem before starting it. This kind of shouldn't happen but it's not severe enough for an impossible error. However, typically, the update PLEX function should update all subsystems so you don't have to worry about it."
#define PLEX_ERRORMESS_TYPE_SUCCESSCHECKFAIL_LONG "A success check for a function has failed."
#define PLEX_ERRORMESS_TYPE_ISNONTEXTURE_LONG "an operation requiring a texture has been passed a non-texture (ie. null texture)"
#define PLEX_ERRORMESS_TYPE_SOUNDHOOKINUSE_LONG "A process was halted because the sound hook is already in use"
#define PLEX_ERRORMESS_TYPE_SOUNDCHANNELSTATE_LONG "A process was halted because the sound channel was either on/off and it couldn't have happened in that state."
#define PLEX_ERRORMESS_TYPE_ACQUIRE_LONG "An attempt to acquire something has failed"
#define PLEX_ERRORMESS_TYPE_STREAMSTATE_LONG "Attempted a function that required the stream state to either be active or deactivated but was not in the right state."
#define PLEX_ERRORMESS_TYPE_FILETYPE_LONG "Attempted a function accessing the wrong filetype for it's operation."
#define PLEX_ERRORMESS_TYPE_DOUBLEDEALLOC_LONG "Attempted to deallocate something that's already deallocated"
#define PLEX_ERRORMESS_TYPE_DIRECTORYOPENSTATE_LONG "Directory open state wrong for function"

// SUBSYSTEM STRINGS

#define PLEX_ERRORMESS_SUBSYSTEM_ERROR "Error Subsystem"
#define PLEX_ERRORMESS_SUBSYSTEM_MEMORY "Memory Subsystem"
#define PLEX_ERRORMESS_SUBSYSTEM_DEFAULT "Default Subsystem"
#define PLEX_ERRORMESS_SUBSYSTEM_TIME "Time Subsystem"
#define PLEX_ERRORMESS_SUBSYSTEM_WINDOW "Window Subsystem"
#define PLEX_ERRORMESS_SUBSYSTEM_TEXTURE "Texture Subsystem"
#define PLEX_ERRORMESS_SUBSYSTEM_SCREENEFFECTS "Screen Effects Subsystem"
#define PLEX_ERRORMESS_SUBSYSTEM_GRAPHICS "Graphics Subsystem"
#define PLEX_ERRORMESS_SUBSYSTEM_INPUT "Input Subsystem"
#define PLEX_ERRORMESS_SUBSYSTEM_SOUND "Sound Subsystem"

// BAD OPERATION STRINGS

#define PLEX_ERRORMESS_OP_BREAKOUTFAIL "Too many attempts to break out a sound from a locked state have failed"
#define PLEX_ERRORMESS_OP_DOUBLESAMPLERATE "Attempted to open a stream with a double (ie. 64-bit float) sample rate. Port Audio does not support this."

#define PLEX_ERRORMESS_OP_CTIME "Failed to acquire the time struct from C standard library function. This is likely a platform issue and not a problem with the execution with PLEX. Try recompiling source before moving forward."

#define PLEX_ERRORMESS_OP_CLOCALTIME "Failed to acquire the local time from the C standard library function. This is most likely a platform issue and not a problem with PLEX. Try recompiling source before moving forward."
#define PLEX_ERRORMESS_OP_CUTSOUNDBUFFER "Cutting sound buffer"
#define PLEX_ERRORMESS_OP_REVERSESOUNDBUFFER "Reversing sound buffer"
#define PLEX_ERRORMESS_OP_CLEARCLEAREDTEXTURE "Attempted to clear an already cleared texture"
#define PLEX_ERRORMESS_OP_UNCLEARUNCLEAREDTEXTURE "Attempted to clear an already cleared texture"
#define PLEX_ERRORMESS_OP_PLAYSOUNDCHANNEL "Attempted to play sound channel"
#define PLEX_ERRORMESS_OP_CLOSEDIRECTORY "OS specific directory close operation"

// EXTERNAL MODUlES

#define PLEX_ERRORMESS_SDLMODULE_CONTROLLER "SDL Gamepad Module"
#define PLEX_ERRORMESS_SDLMODULE_IMAGE "SDL Image Module"
#define PLEX_ERRORMESS_SDLMODULE_INPUT "SDL Input Module"
#define PLEX_ERRORMESS_SDLMODULE_VIDEO "SDL Video Module"

// ------------------------------------------------------------------------------

// GENERIC ACTION TARGETS  ------------------------------------------------------

#define PLEX_ERRORMESS_GENERIC_INDEX "Index"
#define PLEX_ERRORMESS_GENERIC_LIMIT "Limit"
#define PLEX_ERRORMESS_GENERIC_COUNT "Count"
#define PLEX_ERRORMESS_GENERIC_COUNT2 "The Second Count"
#define PLEX_ERRORMESS_GENERIC_CAPACITY "Capacity"
#define PLEX_ERRORMESS_GENERIC_CAPACITY2 "The Second Capacity"

#define PLEX_ERRORMESS_GENERIC_DIMENX "X Coordinate"
#define PLEX_ERRORMESS_GENERIC_DIMENY "Y Coordinate"
#define PLEX_ERRORMESS_GENERIC_DIMENW "Width"
#define PLEX_ERRORMESS_GENERIC_DIMENH "Height"

#define PLEX_ERRORMESS_GENERIC_RED "Red"
#define PLEX_ERRORMESS_GENERIC_GREEN "Green"
#define PLEX_ERRORMESS_GENERIC_BLUE "Blue"
#define PLEX_ERRORMESS_GENERIC_ALPHA "Alpha"

#define PLEX_ERRORMESS_GENERIC_ALPHASTRING "Alphastring"
#define PLEX_ERRORMESS_GENERIC_MESSAGE "Message"
#define PLEX_ERRORMESS_GENERIC_STRING "String"
#define PLEX_ERRORMESS_GENERIC_NAME "Name"
#define PLEX_ERRORMESS_GENERIC_VALUE "Value"

#define PLEX_ERRORMESS_GENERIC_FILE "File"
#define PLEX_ERRORMESS_GENERIC_FILEPTR "Pointer to File"
#define PLEX_ERRORMESS_GENERIC_FILEPATH "Filepath"
#define PLEX_ERRORMESS_GENERIC_FILEPATH2 "The Second Filepath"

#define PLEX_ERRORMESS_GENERIC_LINE "Line"
#define PLEX_ERRORMESS_GENERIC_LINES "The Lines"
#define PLEX_ERRORMESS_GENERIC_NEXTLINE "The Next Line"

#define PLEX_ERRORMESS_GENERIC_TOKEN "Token"
#define PLEX_ERRORMESS_GENERIC_TOKEN2 "The Second Token"
#define PLEX_ERRORMESS_GENERIC_TOKENS "The Tokens"
#define PLEX_ERRORMESS_GENERIC_NEXTTOKEN "The Next Token"

#define PLEX_ERRORMESS_GENERIC_BUFFER "Buffer"
#define PLEX_ERRORMESS_GENERIC_BUFFER2 "The Second Buffer"

#define PLEX_ERRORMESS_GENERIC_WIDTH "Width"
#define PLEX_ERRORMESS_GENERIC_HEIGHT "Height"

#define PLEX_ERRORMESS_GENERIC_WPERC "Width Percentage"
#define PLEX_ERRORMESS_GENERIC_HPERC "Height Percentage"

#define PLEX_ERRORMESS_GENERIC_SLIDES "Slides"

#define PLEX_ERRORMESS_GENERIC_OPENED "Opened"
#define PLEX_ERRORMESS_GENERIC_CLOSED "Closed"

// ANIMATION --------------------------------------------------------------------

// ACTIONS

#define PLEX_ERRORMESS_ANIMATION_ONADDANIMATIONFRAME "While adding animation frame"
#define PLEX_ERRORMESS_ANIMATION_ONADDINGANIMATIONTOSPRITE "While adding an animation to a sprite"
#define PLEX_ERRORMESS_ANIMATION_ONCREATEANIMATION "While creating an animation"
#define PLEX_ERRORMESS_ANIMATION_ONCYCLESPEEDCHANGENANIMATION "While changing the cycle speed of an animation"
#define PLEX_ERRORMESS_ANIMATION_ONCLEARINGANIMATION "While clearing an animation"
#define PLEX_ERRORMESS_ANIMATION_ONDESTROYINGANIMATION "While destroying an animation"
#define PLEX_ERRORMESS_ANIMATION_ONGETFRAMEANIMATION "While trying to get the frame of an animation"
#define PLEX_ERRORMESS_ANIMATION_ONGETTINGANIMATION "While getting an animation"
#define PLEX_ERRORMESS_ANIMATION_ONSPRITESYNCANIMATIONS "While syncing two animations"
#define PLEX_ERRORMESS_ANIMATION_ONSTOPANIMATION "While stopping an animation"
#define PLEX_ERRORMESS_ANIMATION_ONUNCLEARINGANIMATION "While unclearing an aniation"
#define PLEX_ERRORMESS_ANIMATION_ONCLEARSPRITE "While clearing sprite"
#define PLEX_ERRORMESS_ANIMATION_ONDRAWSPRITE "While drawing a sprite"
#define PLEX_ERRORMESS_ANIMATION_ONDRAWSPRITETINTED "While drawing a tinted sprite"
#define PLEX_ERRORMESS_ANIMATION_ONCREATESPRITE "While creating a sprite"
#define PLEX_ERRORMESS_ANIMATION_ONSPRITECYCLESPEEDCHANGE "While changing sprite cycle speed"
#define PLEX_ERRORMESS_ANIMATION_ONSTARTSPRITE "While starting sprite"
#define PLEX_ERRORMESS_ANIMATION_ONSTOPSPRITE "While stopping sprite"
#define PLEX_ERRORMESS_ANIMATION_ONSYNCINGSPRITES "While syncing two sprites"
#define PLEX_ERRORMESS_ANIMATION_ONUNCLEARSPRITE "While unclearing sprite"
#define PLEX_ERRORMESS_ANIMATION_ONDRAWFRAME "While drawing a frame of an animation"
#define PLEX_ERRORMESS_ANIMATION_ONDRAWFRAMETINTED "While drawing a tinted frame of an animation"
#define PLEX_ERRORMESS_ANIMATION_ONISANIMATIONSTOPPED "While checking whether or not an animation is stopped"
#define PLEX_ERRORMESS_ANIMATION_ONISANIMATIONCYCLE "While acquiring the animation cycle length"
#define PLEX_ERRORMESS_ANIMATION_ONDESTROYSPRITE "While destroying a sprite"
#define PLEX_ERRORMESS_ANIMATION_ONCOPYSPRITE "While attempting to copy a sprite"
#define PLEX_ERRORMESS_ANIMATION_ONGETSPRITESTOPPED "While getting whether or not a sprite is stopped"
#define PLEX_ERRORMESS_ANIMATION_ONGETSPRITECYCLELEN "While getting the cycle length from a sprite"
#define PLEX_ERRORMESS_ANIMATOIN_ONGETSPRITEWPERC "While getting the sprite width percentage"
#define PLEX_ERRORMESS_ANIMATOIN_ONGETSPRITEHPERC "While getting the sprite height percentage"

// ACTION TARGETS

#define PLEX_ERRORMESS_ANIMATION_ANIMATION "Animation"
#define PLEX_ERRORMESS_ANIMATION_ANIMATION2 "The Second Animation"
#define PLEX_ERRORMESS_ANIMATION_ANIMATIONS "Animations"
#define PLEX_ERRORMESS_ANIMATION_ANIMATIONCYCLE "Animation Cycle"
#define PLEX_ERRORMESS_ANIMATION_ANIMATIONFRAMES "Animation Frames"
#define PLEX_ERRORMESS_ANIMATION_ANIMATIONPLATE "Animation Plate"
#define PLEX_ERRORMESS_ANIMATION_SPRITE "Sprite"
#define PLEX_ERRORMESS_ANIMATION_FRAMES "Frames"

// PROMINENCE MACHINE ------------------------------------------------------------------

// ACTIONS

#define PLEX_ERRORMESS_PROMINENCEMACHINE_ONADDINGPROMINENCE "While adding a prominence to a prominence machine"
#define PLEX_ERRORMESS_PROMINENCEMACHINE_ONGETPROMINENCE "While getting a prominence from a prominece machine"
#define PLEX_ERRORMESS_PROMINENCEMACHINE_ONREMOVINGPROMINENCE "While removing a prominence from a prominence machine"

#define PLEX_ERRORMESS_PROMINENCEMACHINE_ONGENPROMINENCE "While generating a prominence machine"
#define PLEX_ERRORMESS_PROMINENCEMACHINE_ONDESTROYPROMINENCE "While destroying a prominence machine"
#define PLEX_ERRORMESS_PROMINENCEMACHINE_ONGETPROMINENCECOUNT "While acquiring the count from prominence machine"

// ACTION TARGETS

#define PLEX_ERRORMESS_PROMINENCEMACHINE_PROMINENCEMACHINE "Prominence Machine"
#define PLEX_ERRORMESS_PROMINENCEMACHINE_PROMINENCEMACHINES "The Prominences stored on the Prominence Machine"

// HIGH LEVEL GRAPHICS ------------------------------------------------------------------

// ACTIONS

#define PLEX_ERRORMESS_GRAPHICS_ONADDINGTODRAWLIST "While adding to drawlist"
#define PLEX_ERRORMESS_GRAPHICS_ONDRAWDRAWLIST "While drawing the drawlist"
#define PLEX_ERRORMESS_GRAPHICS_ONBUTTONDDRAWING "While drawing a button"
#define PLEX_ERRORMESS_GRAPHICS_ONLABELDRAWING "While drawing a label"
#define PLEX_ERRORMESS_GRAPHICS_ONLETTERPRINTING "While printing a letter"
#define PLEX_ERRORMESS_GRAPHICS_ONLETTERPRINTINGTINTED "While printing a tinted letter"
#define PLEX_ERRORMESS_GRAPHICS_ONGRAPHICSSTART "While starting the graphics"
#define PLEX_ERRORMESS_GRAPHICS_ONGRAPHICSUPDATE "While updating the graphics"
#define PLEX_ERRORMESS_GRAPHICS_ONPRINTINGTEXTURE "While printing texture"
#define PLEX_ERRORMESS_GRAPHICS_ONPRINTINGTEXTUREROTATED "While printing rotated texture"
#define PLEX_ERRORMESS_GRAPHICS_ONSTRINGPRINTING "While printing a strings"
#define PLEX_ERRORMESS_GRAPHICS_ONSTRINGPRINTINGTINTED "While printing a tinted string"
#define PLEX_ERRORMESS_GRAPHICS_ONTEXTBOXDRAWING "While drawing a text box"
#define PLEX_ERRORMESS_GRAPHICS_ONTEXTBOXTINTEDDRAWING "While drawing a tinted text box"
#define PLEX_ERRORMESS_GRAPHICS_ONTOOLTIPDRAWING "While checking to draw a tooltip"
#define PLEX_ERRORMESS_GRAPHICS_ONTWOIMAGEBUTTONDDRAWING "While drawing a two image button"
#define PLEX_ERRORMESS_GRAPHICS_ONPANORAMADRAWING "While drawing a panorama backdrop"
#define PLEX_ERRORMESS_GRAPHICS_ONSCROLLINGTEXTBOXDRAWING "While drawing scrolling text box"
#define PLEX_ERRORMESS_GRAPHICS_ONSCROLLINGTEXTBOXTINTEDDRAWING "While drawing scrolling tinted text box"

// ACTION TARGETS

#define PLEX_ERRORMESS_GRAPHICS_DRAWLIST "Drawlist"

// LOW LEVEL GRAPHICS ------------------------------------------------------------------

// ACTIONS

#define PLEX_ERRORMESS_LOWGRAPHICS_ONGETTINGBLENDMODE "While getting the blend mode"
#define PLEX_ERRORMESS_LOWGRAPHICS_ONGETTINGRENDERER "While getting renderer"

// ACTION TARGETS

#define PLEX_ERRORMESS_LOWGRAPHICS_SDLRENDERER "SDL Renderer"
#define PLEX_ERRORMESS_LOWGRAPHICS_SDLUPDATE "Low Level SDL Update"
#define PLEX_ERRORMESS_LOWGRAPHICS_SDLSURFACE "SDL Surface"

// HIGH LEVEL TEXTURE ------------------------------------------------------------------

// ACTIONS

#define PLEX_ERRORMESS_TEXTURE_ONDESTROYTEXTURE "While destroying a texture"
#define PLEX_ERRORMESS_TEXTURE_ONLOADTEXTUREFROMFILE "While loading a texture from a file"
#define PLEX_ERRORMESS_TEXTURE_ONCONVHEADERLESSIMAGE "While converting a headerless image"
#define PLEX_ERRORMESS_TEXTURE_LOADTEXTUREFROMHEADERLESS "While loading texture from headerless image"
#define PLEX_ERRORMESS_TEXTURE_LOADTEXTUREFROMFILEORHEADERLESS "While loading texture from headerless image OR file location"
#define PLEX_ERRORMESS_TEXTURE_ONCOPYTEXTURE "While copying texture"
#define PLEX_ERRORMESS_TEXTURE_ONCREATEGENTEXTURE "While creating a generated texture (high level)"
#define PLEX_ERRORMESS_TEXTURE_ONGETBLENDMODE "While getting the texture blend mode"
#define PLEX_ERRORMESS_TEXTURE_ONSTARTTEXTURE "While starting the texture system"
#define PLEX_ERRORMESS_TEXTURE_ONSETTEXTUREALPHA "While setting a texture's alpha"
#define PLEX_ERRORMESS_TEXTURE_ONTINTINGTEXTURE "While tinting a texture"
#define PLEX_ERRORMESS_TEXTURE_ONUNCLEARTEXTURE "While unclearing a texture"
#define PLEX_ERRORMESS_TEXTURE_ONUPDATETEXTURE "While updating texture."
#define PLEX_ERRORMESS_TEXTURE_ONENDTEXTURE "While ending the texture system."
#define PLEX_ERRORMESS_TEXTURE_ONNULLCHECK "While null checking texture."
#define PLEX_ERRORMESS_TEXTURE_ONGETWIDTHHEIGHT "While null checking texture."
#define PLEX_ERRORMESS_TEXTURE_ONGETINNERTEXTURE "While getting inner texture."
#define PLEX_ERRORMESS_TEXTURE_ONGETCLEAR "While checking if texture cleared."
#define PLEX_ERRORMESS_TEXTURE_ONCLEARTEXTURE "While clearing a texture"

// ACTION TARGETS

#define PLEX_ERRORMESS_TEXTURE_TEXTURE "Texture"
#define PLEX_ERRORMESS_TEXTURE_TEXTURE2 "The Second Texture"
#define PLEX_ERRORMESS_TEXTURE_TEXTUREBANK "Texture Bank"
#define PLEX_ERRORMESS_TEXTURE_TEXTURECOPY "Texture Copy"
#define PLEX_ERRORMESS_TEXTURE_TEXBANKDESTROYLIST "Texbank Destroy list"
#define PLEX_ERRORMESS_TEXTURE_TOTEXTURE "To Texture"
#define PLEX_ERRORMESS_TEXTURE_FROMTEXTURE "From Texture"

// LOW LEVEL TEXTURE ------------------------------------------------------------------

// ACTIONS

#define PLEX_ERRORMESS_LOWTEXTURE_ONLOWLEVELGRAPHICSUPDATE "While low level graphics update"
#define PLEX_ERRORMESS_LOWTEXTURE_ONGENTEXTURE "While generating texture (low level)"
#define PLEX_ERRORMESS_LOWTEXTURE_ONGETTINGTINT "While getting the tint of a texture (low level)"
#define PLEX_ERRORMESS_LOWTEXTURE_ONPROCESSGENTEXTURE "While processing generated texture (low level)"
#define PLEX_ERRORMESS_LOWTEXTURE_ONGETFROMSURFACE "While getting a texture from a surface"

// ACTION TARGETS

#define PLEX_ERRORMESS_LOWTEXTURE_INNERTEXTURE "Inner Texture"

// WINDOW ------------------------------------------------------------------

// ACTIONS

#define PLEX_ERRORMESS_WINDOW_ONCREATELOWLEVELWINDOW "While creating a window (low level)"
#define PLEX_ERRORMESS_WINDOW_ONGETWINDOWTITLE "While getting window title"
#define PLEX_ERRORMESS_WINDOW_ONDESTROYLOWLEVELWINDOW "While destroying a window (low level)"
#define PLEX_ERRORMESS_WINDOW_ONENDINGWINDOW "While ending window"
#define PLEX_ERRORMESS_WINDOW_ONSETWINDOWTITLE "While setting window title"
#define PLEX_ERRORMESS_WINDOW_ONREADJUSTWINDOW "While readjusting window"
#define PLEX_ERRORMESS_WINDOW_ONSTARTWINDOW "While starting window"

// ACTION TARGETS

#define PLEX_ERRORMESS_WINDOW_WINDOW "Window"
#define PLEX_ERRORMESS_WINDOW_WINDOWRATIO "Window Size Ratio"
#define PLEX_ERRORMESS_WINDOW_WINDOWTITLE "Window Title"
#define PLEX_ERRORMESS_WINDOW_CURSORGRAPHIC "Cursor Graphic"
#define PLEX_ERRORMESS_WINDOW_CURSORGRAPHICCLICK "Cursor Click Graphic"
#define PLEX_ERRORMESS_WINDOW_CURSORGRAPHICDOWN "Cursor Down Graphic"

// FILE PARSER ------------------------------------------------------------------

// ACTIONS

#define PLEX_ERRORMESS_FILEPARSE_ONADDLINE "While adding a new line"
#define PLEX_ERRORMESS_FILEPARSE_ONADDTOKEN "While adding a new token"
#define PLEX_ERRORMESS_FILEPARSE_ONGETPARSELINE "While getting parse line"
#define PLEX_ERRORMESS_FILEPARSE_ONGETPARSELINECOUNT "While getting the amount of parse lines"
#define PLEX_ERRORMESS_FILEPARSE_ONGETTOKEN "While the destruction of a parse file"
#define PLEX_ERRORMESS_FILEPARSE_ONGETTOKENCCOUNT "While getting the amount of tokens"
#define PLEX_ERRORMESS_FILEPARSE_ONPARSEFILE "While parsing a file"
#define PLEX_ERRORMESS_FILEPARSE_ONPARSEFILEDESTROY "While the destruction of a parse file"

// ACTION TARGETS

#define PLEX_ERRORMESS_FILEPARSE_NEXTLINESTOKEN "The token of the next line"

// SPRITE THEATER ---------------------------------------------------------------

// ACTIONS

#define PLEX_ERRORMESS_SPRITETHEATER_ONADDSPRITETHEATERSLIDE "While executing a slide change event for Sprite theater"
#define PLEX_ERRORMESS_SPRITETHEATER_ONCHANGESPRITETHEATERBACKGROUND "While changing sprite theater background"
#define PLEX_ERRORMESS_SPRITETHEATER_ONCHANGESPRITETHEATERMUSIC "While changing sprite theater music"
#define PLEX_ERRORMESS_SPRITETHEATER_ONNEXTSPRITETHEATERSLIDE "While selecting Sprite theater's next slide"
#define PLEX_ERRORMESS_SPRITETHEATER_ONSPRITETHEATERMUSICCHANGE "While changing the music in sprite theater"
#define PLEX_ERRORMESS_SPRITETHEATER_ONSPRITETHEATERMUSICSTOP "While stopping the music in sprite theater"
#define PLEX_ERRORMESS_SPRITETHEATER_ONSPRITETHEATERSOUNDEFFECT "While having sprite theater play sound effect"
#define PLEX_ERRORMESS_SPRITETHEATER_ONANIMATIONACTORCHANGE "While changing an actor's animation in sprite theater"
#define PLEX_ERRORMESS_SPRITETHEATER_ONANIMATIONACTORMOVE "While moving an actor in sprite theater"
#define PLEX_ERRORMESS_SPRITETHEATER_ONBACKGROUNDCHANGE "While changing the background in sprite theater"
#define PLEX_ERRORMESS_SPRITETHEATER_ONCHANGEACTORANIMATION "While changing actor animation"
#define PLEX_ERRORMESS_SPRITETHEATER_ONCREATEACTOR "While executing an actor creation event for Sprite theater"
#define PLEX_ERRORMESS_SPRITETHEATER_ONHIDEACTOR "While hiding actor"
#define PLEX_ERRORMESS_SPRITETHEATER_ONSETACTORCHANGE "While executing an actor change event for Sprite theater"
#define PLEX_ERRORMESS_SPRITETHEATER_ONSETHIDEACTORCHANGE "While hiding an actor in sprite theater"
#define PLEX_ERRORMESS_SPRITETHEATER_ONSETMUSICCHANGE "While executing a music change event for Sprite theater"
#define PLEX_ERRORMESS_SPRITETHEATER_ONUNHIDEACTOR "While unhiding actor"
#define PLEX_ERRORMESS_SPRITETHEATER_ONSETUNHIDEACTORCHANGE "While unhiding an actor in sprite theater"

#define PLEX_ERRORMESS_SPRITETHEATER_ONSPRITETHEATERCHECKEXPANDARRAYS "While sprite theater is expanding internal arrays"
#define PLEX_ERRORMESS_SPRITETHEATER_ONSPRITEETHEATERSETACTORCHANGE "While setitng an actor change"
#define PLEX_ERRORMESS_SPRITETHEATER_ONSPRITEETHEATERSETMUSICCHANGE "While setitng a music change"


// ACTION TARGETS

#define PLEX_ERRORMESS_SPRITETHEATER_STAGENAME "Stage name for sprite theater"
#define PLEX_ERRORMESS_SPRITETHEATER_MUSICCHANGEEVENT "Music change event"
#define PLEX_ERRORMESS_SPRITETHEATER_ACTORS "The Actors"
#define PLEX_ERRORMESS_SPRITETHEATER_ACTORCHANGEEVENT "Actor change event"
#define PLEX_ERRORMESS_SPRITETHEATER_SLIDES "Slides"
#define PLEX_ERRORMESS_SPRITETHEATER_CHANGES "Changes"
#define PLEX_ERRORMESS_SPRITETHEATER_MUSICCHANGES "Music Changes"

// DEFAULTS ---------------------------------------------------------------

// ACTIONS

#define PLEX_ERRORMESS_ONGETDEFAULTSTRING "While getting string from default"
#define PLEX_ERRORMESS_ONSTARTDEFAULTS "While starting default system"
#define PLEX_ERRORMESS_ONASSIGNDEFAULTBOOL "While assigning a default of bool value"
#define PLEX_ERRORMESS_ONASSIGNDEFAULTDOUBLE "While assigning a default of double value"
#define PLEX_ERRORMESS_ONASSIGNDEFAULTSTRING "While assigning a default of string value"
#define PLEX_ERRORMESS_ONASSIGNDEFAULTUINT32 "While assigning a default of uint32 value"
#define PLEX_ERRORMESS_ONASSIGNDEFAULTUINT8 "While assigning a default of uint8 value"
#define PLEX_ERRORMESS_ONADDTOSTRINGBANK "While adding value to stringbank"
#define PLEX_ERRORMESS_ONLOADCONFIG "While loading a config file"

// ACTION TARGETS

#define PLEX_ERRORMESS_DEFAULT_ENTRIES "Default entries stack"
#define PLEX_ERRORMESS_DEFAULT_NONAME "No default name passed"
#define PLEX_ERRORMESS_DEFAULT_STRINGBANK "Default String Bank"

#define PLEX_ERRORMESS_DEFAULT_TYPE24HRTIME "Set time 24/7 Default"
#define PLEX_ERRORMESS_DEFAULT_TYPEALLOWMAXIMIZE "Allow Maximize Default"
#define PLEX_ERRORMESS_DEFAULT_TYPEALLOWRESIZE "Allow Resize Default"
#define PLEX_ERRORMESS_DEFAULT_TYPECONFIGRELOADBUTTON "Allow Config Reload Button Default"
#define PLEX_ERRORMESS_DEFAULT_TYPECONTROLLERPAUSE "Allow Controller Pause Default"
#define PLEX_ERRORMESS_DEFAULT_TYPECURSORB "Custom Cursor Blue Default"
#define PLEX_ERRORMESS_DEFAULT_TYPECURSORG "Custom Cursor Green Default"
#define PLEX_ERRORMESS_DEFAULT_TYPECURSORR "Custom Cursor Red Default"
#define PLEX_ERRORMESS_DEFAULT_TYPEDISABLECONTROLLER "Disable Controller Default"
#define PLEX_ERRORMESS_DEFAULT_TYPEDISABLEJOYSTICK "Disable Joystick Default"
#define PLEX_ERRORMESS_DEFAULT_TYPEDISABLEKEYBOARD "Disable Keyboard Default"
#define PLEX_ERRORMESS_DEFAULT_TYPEDISABLEMOUSE "Disble Mouse Default"
#define PLEX_ERRORMESS_DEFAULT_TYPEDISABLESOUND "Disable Sound Default"
#define PLEX_ERRORMESS_DEFAULT_TYPEHEIGHT "Screen Height Default"
#define PLEX_ERRORMESS_DEFAULT_TYPEKILLONCLOSE "Kill on Close Default"
#define PLEX_ERRORMESS_DEFAULT_TYPEKILLONESCAPE "Kill on Escape Default"
#define PLEX_ERRORMESS_DEFAULT_TYPESTARTMAXIMIZE "Start Maximized Default"
#define PLEX_ERRORMESS_DEFAULT_TYPEWIDTH "Screen Width Default"

#define PLEX_ERRORMESS_DEFAULT_TEXTUREDIMENSLIMIT "Texture Dimensions Limit Default"
#define PLEX_ERRORMESS_DEFAULT_PRINTERRORS "Print Errors Default"
#define PLEX_ERRORMESS_DEFAULT_SHOWERRORS "Show Errors Default"

#define PLEX_ERRORMESS_DEFAULT_PRINTERRORTHRESHOLD "Print Error Threshold Default"
#define PLEX_ERRORMESS_DEFAULT_SHOWERRORTHRESHOLD "Show Error Threshold Default"
#define PLEX_ERRORMESS_DEFAULT_FATALERRORTHRESHOLD "Print Error Threshold Default"

#define PLEX_ERRORMESS_DEFAULT_HEADERLESSCOPY "Headerless Copy Default"

#define PLEX_ERRORMESS_DEFAULT_MEMORYPROFILE "Memory Profile Default"

#define PLEX_ERRORMESS_DEFAULT_MEMORYREPORT "Memory Report Default"
#define PLEX_ERRORMESS_DEFAULT_MEMORYREPORTTOFILE "Memory Report To File Default"

// ARRAYS ---------------------------------------------------------------

// ACTIONS

#define PLEX_ERRORMESS_ONARRAYCLEARING "While the clearing of an array."
#define PLEX_ERRORMESS_ONARRAYCREATE "While an array is being created"
#define PLEX_ERRORMESS_ONARRAYCUTTING "While the cutting of an array."
#define PLEX_ERRORMESS_ONARRAYCUTTINGPERC "While the cutting of an array by percentage."
#define PLEX_ERRORMESS_ONARRAYDECREMENTATION "While the decrementation of an array."
#define PLEX_ERRORMESS_ONARRAYDESTROY "While the destruction of an array."
#define PLEX_ERRORMESS_ONARRAYEXPANSION "While the expansion of an array."
#define PLEX_ERRORMESS_ONARRAYGECONTENT "While the receiving of the contents of array."
#define PLEX_ERRORMESS_ONARRAYGETCAPACITY "While the receiving of the capacity from array."
#define PLEX_ERRORMESS_ONARRAYGETCAPACITYSIZE "While the receiving of the capacity size (ie. capacity times unit size) from array."
#define PLEX_ERRORMESS_ONARRAYGETCOUNT "While the receiving of the count from array."
#define PLEX_ERRORMESS_ONARRAYGETENTRY "While the receiving of entry from array."
#define PLEX_ERRORMESS_ONARRAYGETENTRYLAST "While the receiving of the last entry from array."
#define PLEX_ERRORMESS_ONARRAYGETSIZE "While the receiving of the size from array."
#define PLEX_ERRORMESS_ONARRAYGETUNITSIZE "While the receiving of the unit size from array."
#define PLEX_ERRORMESS_ONARRAYGETGROWTHUNIT "While the receiving of the growth size from array."
#define PLEX_ERRORMESS_ONARRAYINCREMENTATION "While the incrementation of an array."
#define PLEX_ERRORMESS_ONARRAYMULTIINCREMENTATION "While the multiple incrementation of an array."
#define PLEX_ERRORMESS_ONARRAYREARRANGING "While the rearranging of an array."
#define PLEX_ERRORMESS_ONARRAYREARRANGINGPERC "While the rearranging of an array by percentage."
#define PLEX_ERRORMESS_ONARRAYREVERSING "While the reversing of an array."
#define PLEX_ERRORMESS_ONARRAYREVERSINGPERC "While the reversing of an array by percentage."
#define PLEX_ERRORMESS_ONARRAYSWAPPING "While the swapping of an array."
#define PLEX_ERRORMESS_ONARRAYSWAPPING2 "While the swapping of the second array."
#define PLEX_ERRORMESS_ONARRAYSWAPPINGPERC "While the swapping of an array by percentage."
#define PLEX_ERRORMESS_ONARRAYSWAPPINGPERC2 "While the swapping of the second array by percentage."
#define PLEX_ERRORMESS_ONARRAYREMOVAL "While removing an element from an array"
#define PLEX_ERRORMESS_ONGROWTHUNITSET "While setting the growth unit of an array"

// ACTION TARGETS

#define PLEX_ERRORMESS_ARRAY "Array"
#define PLEX_ERRORMESS_ARRAY2 "The Second Array"
#define PLEX_ERRORMESS_ARRAYBEG1 "Array beginning"
#define PLEX_ERRORMESS_ARRAYBEG2 "Second array beginning"
#define PLEX_ERRORMESS_ARRAYBUFFER1 "Array buffer"
#define PLEX_ERRORMESS_ARRAYBUFFER2 "Second array buffer"
#define PLEX_ERRORMESS_ARRAYCAPACITY "Array Capacity"
#define PLEX_ERRORMESS_ARRAYCONTENTS "Array Contents"
#define PLEX_ERRORMESS_ARRAYCONTENTS2 "Array Contents 2"
#define PLEX_ERRORMESS_ARRAYFREEHOOK "Array's Free Hook"
#define PLEX_ERRORMESS_ARRAYGROWTHUNIT "Growth Unit"
#define PLEX_ERRORMESS_ARRAYLAST "Array Last"
#define PLEX_ERRORMESS_ARRAYLASTCONTENTS "Array Last Contents"
#define PLEX_ERRORMESS_ARRAYMEMHOOK "Array's Mem Hook"
#define PLEX_ERRORMESS_ARRAYPERC "Array by Percentage"
#define PLEX_ERRORMESS_ARRAYPERC2 "Array by Percentage 2"
#define PLEX_ERRORMESS_ARRAYPTR "Pointer to Array"
#define PLEX_ERRORMESS_ARRAYUNITSIZE "Array Unit Size"
#define PLEX_ERRORMESS_ARRAYCOUNT "Array Count"
#define PLEX_ERRORMESS_ARRAYCOUNT2 "The second Array Count"
#define PLEX_ERRORMESS_NEWARRAYCONTENTS "New Array Contents"
#define PLEX_ERRORMESS_NEWBUFFER "New Buffer"
#define PLEX_ERRORMESS_NEWBUFFER2 "New Buffer 2"
#define PLEX_ERRORMESS_PRELOADBUFFER "Preloaded Buffer"
#define PLEX_ERRORMESS_PRELOADBUFFER2 "The Second Preloaded Buffer"

// SOUND ---------------------------------------------------------------

// ACTIONS

#define PLEX_ERRORMESS_ONASSIGNBUFFER "While assinging sound to buffer"
#define PLEX_ERRORMESS_ONCHECKSOUNDPLAYING "While checking if a sound is playing"
#define PLEX_ERRORMESS_ONGETTINGCHANNELITERATION "While getting a channel's iteration"
#define PLEX_ERRORMESS_ONGETTINGSOUNDBUFFER "While getting a sound buffer"
#define PLEX_ERRORMESS_ONGETTINGSOUNDDURATION "While getting a sound's duration"
#define PLEX_ERRORMESS_ONOPENSOUNDSTREAM "While opening a sound stream"
#define PLEX_ERRORMESS_ONLOADSOUNDFILEINTOBUFFER "While loading a sound file into a buffer"
#define PLEX_ERRORMESS_ONPRELOADINTOSPECIFICBUFFER "While preloading a sound file into a specific buffer"
#define PLEX_ERRORMESS_ONPRELOADINTOBUFFER "While preloading a sound file into a buffer"
#define PLEX_ERRORMESS_ONSETTINGCHANNELVOLUME "While setting a sound channel's volume"
#define PLEX_ERRORMESS_ONSETTINGSOUNDBUFFERLEN "While setting the sound buffer's length"
#define PLEX_ERRORMESS_ONSETTINGSOUNDCOMPLEXSEQUENCE "While setting a sound channel's complex sequence"
#define PLEX_ERRORMESS_ONSETTINGSOUNDENDINGITERATION "While setting a sound channel's ending iteration"
#define PLEX_ERRORMESS_ONSETTINGSOUNDSTARTINGITERATION "While setting a sound channel's starting iteration"
#define PLEX_ERRORMESS_ONSOUNDBUFFERCUT "While cutting a sound buffer"
#define PLEX_ERRORMESS_ONSOUNDBUFFERRANDOMSWAP "While randomly swapping a sound buffer"
#define PLEX_ERRORMESS_ONSOUNDBUFFERREARRANGE "While rearranging a sound buffer"
#define PLEX_ERRORMESS_ONSOUNDBUFFERREVERSE "While sound buffer reversing"
#define PLEX_ERRORMESS_ONSOUNDLOCKCHECK "While checking if a sound thread is locked"
#define PLEX_ERRORMESS_ONSOUNDSTART "While starting sound system"
#define PLEX_ERRORMESS_ONTURNINGSOUNDCHANNELOFF "While turning sound channel off"
#define PLEX_ERRORMESS_ONTURNINGSOUNDCHANNELON "While turning sound channel on"
#define PLEX_ERRORMESS_ONCLEARBUFFER "While trying to clear the buffer"
#define PLEX_ERRORMESS_ONCLEARBUFFERBYCHANNEL "While trying to clear the buffer by channel"
#define PLEX_ERRORMESS_ONSTOPSOUNDSTREAM "On attempting to stop sound stream"

// ACTION TARGETS

#define PLEX_ERRORMESS_SOUNDBUFFER "Sound Buffer"
#define PLEX_ERRORMESS_SOUNDBUFFER2 "The Second Sound Buffer"
#define PLEX_ERRORMESS_SOUNDBUFFERFILE "Sound Buffer File"
#define PLEX_ERRORMESS_SOUNDCHANNEL "Sound Channel"
#define PLEX_ERRORMESS_SOUNDCHANNELPLAYBUFFER "The play buffer of a sound channel"
#define PLEX_ERRORMESS_SOUNDCHANNELSEQUENCE "Sound Channel Sequence"
#define PLEX_ERRORMESS_FAILEDTOOPENSOUNDSTREAM "Failed to open sound stream"
#define PLEX_ERRORMESS_STREAMISINACTIVE "Sound Stream Inactive"
#define PLEX_ERRORMESS_SOUNDCHANNELON "Sound Channel is On"
#define PLEX_ERRORMESS_SOUNDCHANNELOFF "Sound Channel is Off"
#define PLEX_ERRORMESS_SOUNDTYPE "The Sound's Type (Sound type of 0 means unavailable type)"

// DATETIME ---------------------------------------------------------------

// ACTIONS

#define PLEX_ERRORMESS_ONGETDATETIME "While getting the datetime"
#define PLEX_ERRORMESS_ONGETTINGDATETIMEDAY "While getting a datetime's day"
#define PLEX_ERRORMESS_ONGETTINGDATETIMEDIFFERENCE "While getting the difference between two datetimes"
#define PLEX_ERRORMESS_ONGETTINGDATETIMEDIFFERENCE12 "While getting the difference between two datetimes (12 hour time)"
#define PLEX_ERRORMESS_ONGETTINGDATETIMEDIFFERENCE24 "While getting the difference between two datetimes (24 hour time)"

#define PLEX_ERRORMESS_ONGETTINGDATETIMEHOUR "While getting a datetime's hour"
#define PLEX_ERRORMESS_ONGETTINGDATETIMEMINUTE "While getting a datetime's minute"
#define PLEX_ERRORMESS_ONGETTINGDATETIMEMONTH "While getting a datetime's month"
#define PLEX_ERRORMESS_ONGETTINGDATETIMEMSH "While getting the minute, second, and hour of a datetime"
#define PLEX_ERRORMESS_ONGETTINGDATETIMEMYD "While getting the month, year, and day of a datetime"
#define PLEX_ERRORMESS_ONGETTINGDATETIMEPM "While getting the is PM from a datetime"
#define PLEX_ERRORMESS_ONGETTINGDATETIMESECOND "While getting a datetime's second"
#define PLEX_ERRORMESS_ONGETTINGDATETIMETOTALDAY "While getting the total days of a datetime"
#define PLEX_ERRORMESS_ONGETTINGDATETIMETOTALHOUR "While getting the total hours of a datetime"
#define PLEX_ERRORMESS_ONGETTINGDATETIMETOTALMINUTE "While getting the total minutes of a datetime"
#define PLEX_ERRORMESS_ONGETTINGDATETIMETOTALMONTH "While getting the total months of a datetime"
#define PLEX_ERRORMESS_ONGETTINGDATETIMETOTALSECOND "While getting the total seconds of a datetime"
#define PLEX_ERRORMESS_ONGETTINGDATETIMETOTALYEAR "While getting the total years of a datetime"
#define PLEX_ERRORMESS_ONGETTINGDATETIMEYEAR "While getting a datetime's year"
#define PLEX_ERRORMESS_ONSETCURRENTDATETIME "While setting a date to current datetime"
#define PLEX_ERRORMESS_ONSETCURRENTDATETIME12 "While setting a datetime to current datetime (12 hour time)"
#define PLEX_ERRORMESS_ONSETCURRENTDATETIME24 "While setting a datetime to current datetime (24 hour time)"
#define PLEX_ERRORMESS_ONSETCURRENTDATETIMELOW "In the static base function setting a datetime current datetime"
#define PLEX_ERRORMESS_ONSETTIMEBASE "While setting the timebase"
#define PLEX_ERRORMESS_ONSETTIMEDATE "While setting the datetime"
#define PLEX_ERRORMESS_ONSETTIMEDATE12 "While setting the datetime (12 hour time)"
#define PLEX_ERRORMESS_ONSETTIMEDATE24 "While setting the datetime (24 hour time)"

// ACTION TARGETS

#define PLEX_ERRORMESS_AMPM "AM/PM"
#define PLEX_ERRORMESS_TIMEBASE "Time Base"
#define PLEX_ERRORMESS_TIMEDAY "Day"
#define PLEX_ERRORMESS_TIMEHOUR "Hour"
#define PLEX_ERRORMESS_TIMEMINUTE "Minute"
#define PLEX_ERRORMESS_TIMEMONTH "Month"
#define PLEX_ERRORMESS_TIMESECOND "Second"
#define PLEX_ERRORMESS_TIMESTRING "Time string"
#define PLEX_ERRORMESS_TIMETOTALDAYS "Total Day"
#define PLEX_ERRORMESS_TIMEYEAR "Year"
#define PLEX_ERRORMESS_CURRENTDATE "Current Date"
#define PLEX_ERRORMESS_DATETIME "Datetime"
#define PLEX_ERRORMESS_DATETIME2 "The Second Datetime"
#define PLEX_ERRORMESS_DATETIMESMALL "Date Time - hours, minutes, seconds"
#define PLEX_ERRORMESS_DATEDIFFSWAP "A difference was attempted between two dates with the later date taken before the earlier date. Since this doesn't make logical sense, the dates will be switched before the value is returned."

// TIMER ---------------------------------------------------------------

// ACTIONS

#define PLEX_ERRORMESS_ONGETTINGGAMETIME "While getting game time"
#define PLEX_ERRORMESS_ONGETTINGGAMETIMESTRING "While gettimg game time string"
#define PLEX_ERRORMESS_ONGETTINGUNPAUSEGAMETIME "While getting game time paused"
#define PLEX_ERRORMESS_ONPAUSEGAMETIME "While pausing game time"
#define PLEX_ERRORMESS_ONGETTINGGAMETIMERSECONDS "While getting the seconds passed on the game timer"

#define PLEX_ERRORMESS_ONGETTINGGAMETIMENOPAUSE "While getting the game timer that ignores pausetime"
#define PLEX_ERRORMESS_ONGETTINGGAMETIMENOPAUSETIMESTRING "While getting the game timer string that ignores pausetime"
#define PLEX_ERRORMESS_ONGETTINGGAMETIMERSECONDSNOPAUSE "while getting the seconds passed on game timer that ignores pausetime"

#define PLEX_ERRORMESS_ONTIMEMULTIPLIER "While setting a timer multiplier"
#define PLEX_ERRORMESS_ONTIMERESET "While resetting of timer"
#define PLEX_ERRORMESS_ONTIMEREWIND "While rewinding of timer"
#define PLEX_ERRORMESS_ONTIMERFASTFORWARD "While fast forwarding of timer"
#define PLEX_ERRORMESS_ONTIMERPAUSE "While pausing of timer"
#define PLEX_ERRORMESS_ONTIMERSTART "While start of timer"
#define PLEX_ERRORMESS_ONTIMERUNPAUSE "While unpausing of timer"
#define PLEX_ERRORMESS_ONTIMERUPDDATE "While updating of timer"
#define PLEX_ERRORMESS_ONUNPAUSEGAMETIME "While unpausing game time"
#define PLEX_ERRORMESS_ONUPDATINGTIMER "While updating timer"
#define PLEX_ERRORMESS_ONPAUSINGTIMER "While pausing the timer"
#define PLEX_ERRORMESS_ONGETTINGTIMERSECONDS "While getting seconds from a timer"

// ACTION TARGETS

#define PLEX_ERRORMESS_TIME "Time"
#define PLEX_ERRORMESS_TIMER "Timer"
#define PLEX_ERRORMESS_TIMERMULTIPLIER "Timer Multiplier"
#define PLEX_ERRORMESS_COUNTDOWNTIME "countdown time"
#define PLEX_ERRORMESS_COUNTDOWNTIMESTRING "Countdown time string"

// SCREEN EFFECTS ---------------------------------------------------------------

// ACTIONS

#define PLEX_ERRORMESS_ONCEASINGSCREENEFFECT "While ceasing a screen effect"
#define PLEX_ERRORMESS_ONDOINGRANGEDSCREENEFFECT "While doing a ranged screen effect"
#define PLEX_ERRORMESS_ONDOINGSCREENEFFECT "While doing a screen effect"

// ACTION TARGETS

#define PLEX_ERRORMESS_SCREENEFFECTCEASE "Cease Screen Effect"
#define PLEX_ERRORMESS_SCREENEFFECTCHECKOVER "Screen Effect is over"
#define PLEX_ERRORMESS_SCREENEFFECTDO "Do Screen Effect"
#define PLEX_ERRORMESS_SCREENEFFECTDORANGED "Do ranged Screen Effect"
#define PLEX_ERRORMESS_SCREENEFFECTS "Screen Effects"
#define PLEX_ERRORMESS_SCREENEFFECTUPDATE "Updating Screen Effects"

#define PLEX_ERRORMESS_SCREENEFFECTSTACK "Screen Effect Stack"

// GENERATOR ---------------------------------------------------------------

// ACTIONS

#define PLEX_ERRORMESS_ONGENGENEARTOR "While generating a generator."
#define PLEX_ERRORMESS_ONDESTROYINGGENEARTOR "While destroying a generator."

#define PLEX_ERRORMESS_ONCHANGEGENERATORAREA "While changing the generator area"
#define PLEX_ERRORMESS_ONEXTENDINGGENERATORDURATION "While extending the generator's duration"
#define PLEX_ERRORMESS_ONGETTINGGENERATORAREA "While getting the generator's area"
#define PLEX_ERRORMESS_ONPULLGENERATED "While pulling a generated value from generator"
#define PLEX_ERRORMESS_ONSTARTGENERATOR "While starting a generator"
#define PLEX_ERRORMESS_ONUPDATEGENERATOR "While updating a generator"

// ACTION TARGETS

#define PLEX_ERRORMESS_GENERATED "Generated Values of Generator"
#define PLEX_ERRORMESS_GENERATOR "Generator"
#define PLEX_ERRORMESS_GENERATORDURATION "Generator Duration"
#define PLEX_ERRORMESS_GENMAXX "Generator Minimum X"
#define PLEX_ERRORMESS_GENMAXXEND "Generator Minimum End X"
#define PLEX_ERRORMESS_GENMAXY "Generator Minimum Y"
#define PLEX_ERRORMESS_GENMAXYEND "Generator Minimum End Y"
#define PLEX_ERRORMESS_GENMINX "Generator Minimum X"
#define PLEX_ERRORMESS_GENMINXEND "Generator Minimum End X"
#define PLEX_ERRORMESS_GENMINY "Generator Minimum Y"
#define PLEX_ERRORMESS_GENMINYEND "Generator Minimum End Y"
#define PLEX_ERRORMESS_GENXCOORDINATE "Generated X Coordinate"
#define PLEX_ERRORMESS_GENYCOORDINATE "Generated Y Coordinate"
#define PLEX_ERRORMESS_WAITS "Wait time stack for the generator"

// INPUT ---------------------------------------------------------------

// ACTIONS

#define PLEX_ERRORMESS_ONINPUTSTART "While start of input system"
#define PLEX_ERRORMESS_ONINPUTUPDATE "While update of input system"
#define PLEX_ERRORMESS_ONCONTROLLERAXIS "While checking controller axis (ie. analog stick)"
#define PLEX_ERRORMESS_ONCONTROLLERCOUNT "While returning the controller count"
#define PLEX_ERRORMESS_ONCONTROLLERPRESS "While checking pressing of controller button"
#define PLEX_ERRORMESS_ONGETCUSTOMCURSORDISPLACEMENT "While retrieving the custom cursor's displacement values"
#define PLEX_ERRORMESS_ONSETCUSTOMCURSOR "While setting a custom cursor"
#define PLEX_ERRORMESS_ONSETCUSTOMCURSORDOWN "While setting custom cursor down graphic"
#define PLEX_ERRORMESS_ONSETCUSTOMCURSORTINTED "While setting a tinted custom cursor"

// ACTION TARGETS

#define PLEX_ERRORMESS_AXISVALUE "Controller Axis Value"
#define PLEX_ERRORMESS_CONTROLLER "Controller"
#define PLEX_ERRORMESS_CUSTOMCURSOR "Custom Cursor"
#define PLEX_ERRORMESS_DISPLACEMENTX "Displacement X"
#define PLEX_ERRORMESS_DISPLACEMENTY "Displacement Y"
#define PLEX_ERRORMESS_CURSORTEX "Cursor Texture"

// FAKESTRINGS ---------------------------------------------------------------

// ACTIONS

#define PLEX_ERRORMESS_ONCREATESTRING "While create string"

// ACTION TARGETS

#define PLEX_ERRORMESS_CREATEABLEFAKESTRING10 "Fakestring 10"
#define PLEX_ERRORMESS_CREATEABLEFAKESTRING100 "Fakestring 100"
#define PLEX_ERRORMESS_CREATEABLEFAKESTRING25 "Fakestring 25"
#define PLEX_ERRORMESS_CREATEABLEFAKESTRING50 "Fakestring 50"
#define PLEX_ERRORMESS_CREATEABLEFAKESTRING500 "Fakestring 500"

// MATH ---------------------------------------------------------------

// ACTIONS

#define PLEX_ERRORMESS_ONGETRANDOMNUMBER "While getting a random number"
#define PLEX_ERRORMESS_ONPOINTTOPOLYGONCOLLISION "While calculating a point to polygon collision"
#define PLEX_ERRORMESS_ONPOLYGONTOPOLYGONCOLLISION "While calculating a collision of two polygons"

// ACTION TARGETS

#define PLEX_ERRORMESS_MINMAXRANDNUM "Random number generation"
#define PLEX_ERRORMESS_POLYGONPOINTS "Polygon points"
#define PLEX_ERRORMESS_POLYGONPOINTS2 "Polygon points on second polygon"

// MEMORY ---------------------------------------------------------------

// ACTIONS

#define PLEX_ERRORMESS_ONSUBALLOC "While allocating memory on the suballoc level"
#define PLEX_ERRORMESS_ONSUBDEALLOC "While deallocating memory on the suballoc level"
#define PLEX_ERRORMESS_ONEXPANDPRELOAD "While attempting to expand preallocated memory"
#define PLEX_ERRORMESS_ONSTARTMEM "While starting the memory subsystem"
#define PLEX_ERRORMESS_ONENDMEM "While ending the memory subsystem"
#define PLEX_ERRORMESS_ONMEMALLOC "While allocating memory"
#define PLEX_ERRORMESS_ONMEMDEALLOC "While deallocating memory"

// ACTION TARGETS

#define PLEX_ERRORMESS_ALLOCED "Allocated memory"
#define PLEX_ERRORMESS_ALLOCEDPOINTER "Pointer to Pointer of Allocated Memory"
#define PLEX_ERRORMESS_FREEHOOK "Free Hook"
#define PLEX_ERRORMESS_MEMHOOK "Memory Hook"
#define PLEX_ERRORMESS_MEMREUSESTACK "Memory reuse stack"
#define PLEX_ERRORMESS_MEMREUSESTACKCONTENTS "Contents of memory reuse stack"
#define PLEX_ERRORMESS_PREALLOCSSTACK "The preallocation stack"
#define PLEX_ERRORMESS_PREALLOCSSTACKCONTENTS "Contents of the preallocation stack"
#define PLEX_ERRORMESS_PREALLOCSSTACKENTRY "Preallocation stack entry #"
#define PLEX_ERRORMESS_PREALLOCSSTACKENTRYCONTENTS "Contents of preallocation stack entry #"

#define PLEX_ERRORMESS_MEMERRORREPORTSTACK "Stack carrying memory report info"
#define PLEX_ERRORMESS_MEMERRORREPORTFILENAME "Mem error report filename (the filename of the source an error is reported, not a filename it uses to save)"

// MEM REPORT

#define PLEX_ERRORMESS_MEMREPORTHEADING "PLEX memory usage report (not to be confused with total memory usage as some memory is not handled through PLEX smart memory. ie. memory managed by PLEX's dependecies, low level system tasks, and memory used to run the smart memory containers themeselves. If properly done, however, most heap memory should theoretically be smart memory)"

#define PLEX_ERRORMESS_PEAKMEMORY "Peak memory use (most allocated at one time)"

#define PLEX_ERRORMESS_MEMALLOCTOTAL "Total Memory Allocated with PLEX_alloc"
#define PLEX_ERRORMESS_MEMDEALLOCTOTAL "Total Memory Deallocated with PLEX_alloc"

#define PLEX_ERRORMESS_ALLOCDIFF "The difference between memory allocated by PLEX_alloc and PLEX_delloc is:"
#define PLEX_ERRORMESS_ALLOCDIFF2 "The larger this number is; the worse it is. Make sure that's PLEX_alloc'd is also PLEX_dealloc'd."
#define PLEX_ERRORMESS_ALLOCSAME "Good news! The amount of memory allocated with PLEX_alloc and PLEX_dealloc are the same!"
#define PLEX_ERRORMESS_FOOTER "All allocated memory will be cleared from the system. Since this smart memory management is neither a garbage collector nor a reference counter, forgetting to deallocate memory could result in memory hangs which risk overrun."

// DIRECTORY ---------------------------------------------------------------

// ACTIONS

#define PLEX_ERRORMESS_ONGETTINGDIRECTORYFROMCRAWLER "While getting a directory from a crawler"
#define PLEX_ERRORMESS_ONSTARTDIRECTORYCRAWLER "While starting the directory crawler"
#define PLEX_ERRORMESS_ONCRAWLDIRECTORY "While crawling directory"
#define PLEX_ERRORMESS_ONFREEDIRECTORYCRAWLER "While freeing direcctory crawler"
#define PLEX_ERRORMESS_ONGETDIRECTORY "While getting directory from path"
#define PLEX_ERRORMESS_REOPENDIRECTORY "While trying to reopen a directory"
#define PLEX_ERRORMESS_CLOSEDIRECTORY "While trying to close a directory"
#define PLEX_ERRORMESS_ONCOMPDIRECTORYPATHS "While comparing directory paths"

// ACTION TARGETS

#define PLEX_ERRORMESS_DIRECTORY "Directory"
#define PLEX_ERRORMESS_DIRECTORYCONTENTS "Directory contents"
#define PLEX_ERRORMESS_DIRECTORYCRAWLER "Directory Crawler"
#define PLEX_ERRORMESS_CRAWLERSDIRECTORY "The directory inside a Directory Crawler"

#endif
