#ifndef PLEX_ERRORMESS_EN_H
#define PLEX_ERRORMESS_EN_H

// This is a header which contains all of the error macro strings for the English version of PLEX. This is basically the error message equivalent of "meta/language/language_en.h". More information can be found there or in "meta/language.h". As with all headers in the meta folder, there is no correlating source file.

// Messages for the error strings themselves

#define PLEX_ERRORMESS_ERRORLINE "@ line"
#define PLEX_ERRORMESS_ERRORFILE "in file"

#define PLEX_ERRORMESS_ERRORSTART "Error!"
#define PLEX_ERRORMESS_ERRORNOTERMINATE "Not terminated."
#define PLEX_ERRORMESS_ERRORTERMINATE "TERMINATED!"

// headers for types of errors

#define PLEX_ERRORMESS_START_STARTERR "Start error"
#define PLEX_ERRORMESS_UPDATE_UPDATEERR "Update error"
#define PLEX_ERRORMESS_UPDATE_SDLERR "SDL error"
#define PLEX_ERRORMESS_UPDATE_PORTERR "PortAudio error"
#define PLEX_ERRORMESS_MATH_MATHERR "Math error"
#define PLEX_ERRORMESS_DATETIME_DATETIMERR "Datetime error"
#define PLEX_ERRORMESS_FAKESTRING_FAKESTRINGERR "Fakestring error"
#define PLEX_ERRORMESS_DEFAULT_DEFAULTERR "Default error"
#define PLEX_ERRORMESS_MEM_MEMERR "Memory error"
#define PLEX_ERRORMESS_ARRAY_ARRAYERR "Array error"
#define PLEX_ERRORMESS_ARRAY_SOUNDERR "Sound error"

// various types of errors that can happen at start

#define PLEX_ERRORMESS_START_NOOS "No operating system detected!"


#define PLEX_ERRORMESS_START_NOMEM "Couldn't start mem!"
#define PLEX_ERRORMESS_START_NOTIME "Couldn't start time!"
#define PLEX_ERRORMESS_START_NOWINDOW "Couldn't start window!"
#define PLEX_ERRORMESS_START_NOGRAPHICS "Couldn't start graphics!"
#define PLEX_ERRORMESS_START_NOTEXTURE "Couldn't start texture machine!"
#define PLEX_ERRORMESS_START_NOINPUT "Couldn't start input!"
#define PLEX_ERRORMESS_START_NOSOUND "Couldn't start sound!"

// various types of errors that can happen at start when loading SDL (SDL is needed for PLEX to operate, even if on gl rendering mode)

#define PLEX_ERRORMESS_START_NOINPUTSDL "SDL input module couldn't be loaded."
#define PLEX_ERRORMESS_START_NOTEXSDL "SDL texture module couldn't be loaded."
#define PLEX_ERRORMESS_START_NOINITSDL "SDL could not be initialized."
#define PLEX_ERRORMESS_START_NOWINDOWSDL "SDL window not created."

// various types of errors that can happen while updating

#define PELX_ERRORMESS_UPDATE_NOUPDATE "Mem System failure!"
#define PLEX_ERRORMESS_UPDATE_NOTIME "Time System failure!"
#define PLEX_ERRORMESS_UPDATE_NOWINDOW "Window System failure!"
#define PLEX_ERRORMESS_UPDATE_NOGRAPHICS "Graphics System failure!"
#define PLEX_ERRORMESS_UPDATE_NOINPUT "Input System failure!"
#define PLEX_ERRORMESS_UPDATE_NOSOUND "Sound System failure!"

// errors based off of math

#define PLEX_ERRORMESS_MATH_MINMAXMIX "While prompting a random number the minimum number has been switched for the maximum number. This will typically not terminate and the min and max will simply be swapped."

// errors based off datetime

#define PLEX_ERRORMESS_DATETIME_NULLSET "Attempted to set a null datetime pointer."
#define PLEX_ERRORMESS_DATETIME_NULLGET "Attempted to get values from a null datetime pointer."
#define PLEX_ERRORMESS_DATETIME_NULLDIFF "Attempted to get the difference between two datetime pointers with one or both being NULL."
#define PLEX_ERRORMESS_DATETIME_NULLTOTD "Attempted to get the total dates of a datetime pointer that is null"

// errors for fakestrings

#define PLEX_ERRORMESS_FAKESTRING_NULLCREATE "Attempted to create a fake string from a NULL cstring. Will return blank."

// errors for defaults

#define PLEX_ERRORMESS_DEFAULT_NOPOPULATE_WIDTH "\'Width\' default did not populate for whatever reason. Will default as minimum."
#define PLEX_ERRORMESS_DEFAULT_NOPOPULATE_HEIGHT "\'Height\' default did not populate for whatever reason. Will default as minimum."
#define PLEX_ERRORMESS_DEFAULT_NOPOPULATE_KILLONESCAPE "\'Kill on Escape\' default did not populate for whatever reason.."
#define PLEX_ERRORMESS_DEFAULT_NOPOPULATE_KILLONCLOSE "\'Kill on Close\' default did not populate for whatever reason."
#define PLEX_ERRORMESS_DEFAULT_NOPOPULATE_ALLOWRESIZE "\'Allow Resize\' default did not populate for whatever reason."
#define PLEX_ERRORMESS_DEFAULT_NOPOPULATE_ALLOWMAXIMIZE "\'Allow Maximize\' default did not populate for whatever reason."
#define PLEX_ERRORMESS_DEFAULT_NOPOPULATE_STARTMAXIMIZE "\'Start Maximize\' default did not populate for whatever reason."
#define PLEX_ERRORMESS_DEFAULT_NOPOPULATE_DISABLEAUDIO "\'Disable Audio\' default did not populate for whatever reason."
#define PLEX_ERRORMESS_DEFAULT_NOPOPULATE_CURSORRED "\'Cursor Red\' default did not populate for whatever reason. Will default as minimum."
#define PLEX_ERRORMESS_DEFAULT_NOPOPULATE_CURSORGREEN "\'Cursor Green\' default did not populate for whatever reason. Will default as minimum."
#define PLEX_ERRORMESS_DEFAULT_NOPOPULATE_CURSORBLUE "\'Cursor Blue\' default did not populate for whatever reason. Will default as minimum."
#define PLEX_ERRORMESS_DEFAULT_NOPOPULATE_CONFIGRELOADBUTTON "\'Config Reload Button\' default did not populate for whatever reason."
#define PLEX_ERRORMESS_DEFAULT_NOPOPULATE_CONTROLLERPAUSE "\'Controller Pause\' default did not populate for whatever reason."
#define PLEX_ERRORMESS_DEFAULT_NOPOPULATE_DISABLECONTROLLER "\'Disable Controller\' default did not populate for whatever reason."
#define PLEX_ERRORMESS_DEFAULT_NOPOPULATE_DISABLEJOYSTICK "\'Disable Joystick\' default did not populate for whatever reason."
#define PLEX_ERRORMESS_DEFAULT_NOPOPULATE_DISABLEKEYBOARD "\'Disable Keyboard\' default did not populate for whatever reason."
#define PLEX_ERRORMESS_DEFAULT_NOPOPULATE_DISABLEMOUSE "\'Disable Mouse\' default did not populate for whatever reason."

#define PLEX_ERRORMESS_DEFUALT_NOASSIGN8 "Failed to assign uint8 default"
#define PLEX_ERRORMESS_DEFUALT_NOASSIGN32 "Failed to assign uint32 default"
#define PLEX_ERRORMESS_DEFUALT_NOASSIGNBOOL "Failed to assign bool default"
#define PLEX_ERRORMESS_DEFUALT_NOASSIGNDOUBLE "Failed to assign double default"

#define PLEX_ERRORMESS_DEFUALT_CONFIGNOLOAD "Failed to load config file. Everything will be restored to default. Likely a new config file will be generated on progrma termination."

// errors for memory system

#define PLEX_ERRORMESS_MEM_SUBALLOCFAIL "Memory allocation for the suballocation function returned NULL. This could be a serious problem. Probably memory overflow."
#define PLEX_ERRORMESS_MEM_PREALLOCNULL "Memory tried to expand even though preallocated memory is NULL. This is rare and likely serious. Make sure you have done the proper PLEX_start, PLEX_update, and PLEX_end setup. Otherwise preallocated memory will not initialize."
#define PLEX_ERRORMESS_MEM_EXPANDFAIL "Memory tried and failed to expand preallocated memory. This could be a serious problem. Probably memory overflow."
#define PLEX_ERRORMESS_MEM_ENTRYFAIL "Preallocated memory returned a NULL entry. There's a few reasons why this may happen but specifics are difficult to determine. This error will likely be coupled with some kind of protoexpandarray message that clarifies."
#define PLEX_ERRORMESS_MEM_CONTENTNULL "Somehow the contents of the preallocation array got corrupted. This should only be accessed in the memory unit. If you haven't messed with the memory unit, try rebuilding. If that doesn't work, redownload plex from the official github. If you have changed the code in mem.c, it is very reccomended you don't access the contents of the preallocation directly. If this doesn't make sense, it's reccomended to not mess around with the volitile mem.c unit."
#define PLEX_ERRORMESS_MEM_INCREMENTCONTENTFAIL "Failed to increment the preallocated memory to expand an entry. Likely due to memory buffer overflow."
#define PLEX_ERRORMESS_MEM_DOUBLESTART "Attempted to double start memory system. This is completely harmless, but it's typical to just let PLEX_start, PLEX_update, PLEX_end manage all the subsystems."
#define PLEX_ERRORMESS_MEM_PREALLOCINITFAIL "Failed to initialize preallocated memory. This could be a memory buffer issue, but this early in the program it might be something more serious."
#define PLEX_ERRORMESS_MEM_REUSEINITFAIL "Failed to initialize reusable preallocated memory. This could be a memory buffer issue, but this early in the program it might be something more serious."
#define PLEX_ERRORMESS_MEM_INITEXPANDPRELOADFAIL "Failed to expand initial preallocated memory. This could be due to a memory buffer overflow. Either PLEX_LIMIT_MEM_PRELOAD or PLEX_LIMIT_MEMENTRY_EXP are probably just set too large. Try recompiling PLEX with lower numbers for these entries."
#define PLEX_ERRORMESS_MEM_ENDWITHOUTSTART "Attempted to end the memory system while it's not started. This is completely harmless, but it's typical to just let PLEX_start, PLEX_update, PLEX_end manage all the subsystems."
#define PLEX_ERRORMESS_MEM_ENDNULLENTRY "For some bizarre reason there's a NULL entry on the preallocated memory during ending the memory subsystem. This could be a sign of corruption. This should only be accessed in the memory unit. If you haven't messed with the memory unit, try rebuilding. If that doesn't work, redownload plex from the official github. If you have changed the code in mem.c, it is very reccomended you don't access the contents of the preallocation directly. If this doesn't make sense, it's reccomended to not mess around with the volitile mem.c unit."
#define PLEX_ERRORMESS_MEM_ENDNULLCONTENTS "For whatever reason a memory entry's contents is null. This could be a sign of corruption. This should only be accessed in the memory unit. If you haven't messed with the memory unit, try rebuilding. If that doesn't work, redownload plex from the official github. If you have changed the code in mem.c, it is very reccomended you don't access the contents of the preallocation directly. If this doesn't make sense, it's reccomended to not mess around with the volitile mem.c unit."
#define PLEX_ERRORMESS_MEM_REUSEALLOCTARGETNULL "Memory allocation failure while trying to access reused entry. Could not expand the reuse list any further. Likely memory overflow."
#define PLEX_ERRORMESS_MEM_REUSEALLOCREGENFAIL "Memory allocation failure while trying to access reused entry. Could not regen the reuse list as needed. Possibly memory overflow."
#define PLEX_ERRORMESS_MEM_REUSEALLOCEXPANDFAIL "Memory allocation failure. Could not expand a reusable entry. Possibly memory overflow,"
#define PLEX_ERRORMESS_MEM_EXPANDALLOCFAIL "Memory allocation failure. Could not expand preallocate memory to get a new entry. Likely memory overflow"
#define PLEX_ERRORMESS_MEM_ENTRYALLOCNULL "Memory allocation failure. Failed to access an entry from the preallocated memory. Likely memory overflow or corruption."
#define PLEX_ERRORMESS_MEM_ENTRYEXPANDFAIL "Memory allocation failure. Failed to expand an entry to proper size. likely a memory overflow or corruption."
#define PLEX_ERRORMESS_MEM_FREENULL "tried to deallocate a NULL memory spot. No harm but could be the result of sloppy code."
#define PLEX_ERRORMESS_MEM_REUSEDESTROYINCREMENTFAIL "Failed to increment the reuse stack during deallocation. This will not terminate the program but it's not good."
#define PLEX_ERRORMESS_MEM_REUSEDESTROYENTRYFAIL "Failed to access the last entry of reuse stack during deallocation. This will not terminate the program but it's not good."

// errors for arrays

#define PLEX_ERRORMESS_ARRAY_NOHOOK "Attempted to create a protoexpandarray without a mem allocation hook of NULL. You cannot do this. Returning NULL."
#define PLEX_ERRORMESS_ARRAY_NOFREEHOOK "Attempted to create a protoexpandarray without a mem deaallocation hook of NULL. You cannot do this. Returning NULL."
#define PLEX_ERRORMESS_ARRAY_NOSIZE "Attempted to create a protoexpandarray with a size of zero. You cannot do this. Returning NULL."
#define PLEX_ERRORMESS_ARRAY_NOGROWTHUNIT "Attempted to create a protoexpandarray with a growth unit of zero. You cannot do this. Returning NULL."
#define PLEX_ERRORMESS_ARRAY_NOUNITSIZE "Attempted to create a protoexpandarray with a unit size of zero. You cannot do this. Returning NULL."
#define PLEX_ERRORMESS_ARRAY_BADALLOC "Failed to allocate protoexpandarray. May be paired with another memory failure error. Returning NULL."
#define PLEX_ERRORMESS_ARRAY_BADCONTENTALLOC "Failed to allocate protoexpandarray's content with free hook. May be paired with another memory failure error, or it may just be a bad allocation function. Returning NULL."
#define PLEX_ERRORMESS_ARRAY_DESTROYNULL "Attempting to free a protoexpandarray from a null pointer. Generally harmless but could be sign of sloppy code."
#define PLEX_ERRORMESS_ARRAY_DESTROYNULLPTR "Attempting to free a protoexpandarray from a pointer to a null pointer. Generally harmless but could be sign of sloppy code."
#define PLEX_ERRORMESS_ARRAY_EXPANDNULLARRAY "Attempted to expand a protoexpandarray but failed to generate the new contents. This should be linked to some kind of memory allocation error."
#define PLEX_ERRORMESS_ARRAY_MULTIINCNULL "Attempted to expand a protoexpandarray from a null pointer for incrementing multiple entries. Generally harmless but might be a sign of sloppy code."
#define PLEX_ERRORMESS_ARRAY_MULITIINCEXPANDFAIL "Failed to expand a protoexpandarray's contents. This could be for many reasons and will likely be paired with another error that givex context."
#define PLEX_ERRORMESS_ARRAY_DECREMENTNULL "Attempted to decrement a protoexpandarray from a null pointer. Generally harmless but might be a sign of sloppy code."
#define PLEX_ERRORMESS_ARRAY_DECREMENTEMPTY "Attempted to decrement a protoexpandarray that has no entries. Completely harmless but might be a sign of sloppy code."
#define PLEX_ERRORMESS_ARRAY_CLEARNULL "Attempted to clear a protoexpandarray from null. Completely harmless but might be a sign of sloppy code."
#define PLEX_ERRORMESS_ARRAY_SWAPNULL1 "Attempted to swap two expandarrays but the first is null. Generally harmless but might be a sign of sloppy code."
#define PLEX_ERRORMESS_ARRAY_SWAPNULL2 "Attempted to swap two expandarrays but the second is null. Generally harmless but might be a sign of sloppy code."
#define PLEX_ERRORMESS_ARRAY_SWAPNULL1CONTENT "Attempted to swap two expandarrays but the first's content is null. This can only happen if the contents of an expandarray are improperly tampered (ie. directly accessed in code rather than utilizing the built in functions. Most probably NOT maliciously but can be done on accident easily). Try rebuilding. If that doesn't work, try redownloading PLEX from github."
#define PLEX_ERRORMESS_ARRAY_SWAPNULL2CONTENT "Attempted to swap two expandarrays but the second's content is null. This can only happen if the contents of an expandarray are improperly tampered (ie. directly accessed in code rather than utilizing the built in functions. Most probably NOT maliciously but can be done on accident easily). Try rebuilding. If that doesn't work, try redownloading PLEX from github."
#define PLEX_ERRORMESS_ARRAY_SWAPNOLEN "Attempted to swap two expandarrays for the length of 0. This will be skipped because it does nothing."
#define PLEX_ERRORMESS_ARRAY_SWAPBEGIN1AFTER1 "Attempted to swap two expandarrays but the first beginning mark is larger than the size of the first expand array to be swapped. This will be skipped."
#define PLEX_ERRORMESS_ARRAY_SWAPBEGIN1AFTER2 "Attempted to swap two expandarrays but the first beginning mark is larger than the size of the second expand array to be swapped. This will be skipped."
#define PLEX_ERRORMESS_ARRAY_SWAPBEGIN2AFTER1 "Attempted to swap two expandarrays but the second beginning mark is larger than the size of the first expand array to be swapped. This will be skipped."
#define PLEX_ERRORMESS_ARRAY_SWAPBEGIN2AFTER2 "Attempted to swap two expandarrays but the second beginning mark is larger than the size of the second expand array to be swapped. This will be skipped."
#define PLEX_ERRORMESS_ARRAY_SWAPBUFFERFAIL "Attempting to swap two expandarrays but couldn't allocate buffer needed to swap data."
#define PLEX_ERRORMESS_ARRAY_SWAPBUFFERFAIL2 "Attempting to swap two expandarrays but couldn't allocate the second buffer needed to swap data."
#define PLEX_ERRORMESS_ARRAY_CUTARRAYNULL "Attempting to cut expandarray from null pointer. Generally harmless but might also be a sign of sloppy code."
#define PLEX_ERRORMESS_ARRAY_CUTARRAYNULLCONTENTS "Attempted to cut an expandarrays but the content is null. This can only happen if the contents of an expandarray are improperly tampered (ie. directly accessed in code rather than utilizing the built in functions. Most probably NOT maliciously but can be done on accident easily). Try rebuilding. If that doesn't work, try redownloading PLEX from github."
#define PLEX_ERRORMESS_ARRAY_CUTARRAYNOLEN "Attempted to cut a protoexpandarray of no length. This will be skipped as that does nothing."
#define PLEX_ERRORMESS_ARRAY_CUTARRAYBEGINAFTER "Attempted to cut a protoexpandaray with a beginning larger than it's size. This will be skipped as it does nothing."
#define PLEX_ERRORMESS_ARRAY_REARRANGEARRAYNULL "Attempted to rearrange protoexpandarray from null pointer. This is harmless but may be a sign of sloppy code."
#define PLEX_ERRORMESS_ARRAY_REARRANGEARRAYNULLCONTENT "Attempted to cut a protoexpandaray with null contents. This can only happen if the contents of an expandarray are improperly tampered (ie. directly accessed in code rather than utilizing the built in functions. Most probably NOT maliciously but can be done on accident easily). Try rebuilding. If that doesn't work, try redownloading PLEX from github."
#define PLEX_ERRORMESS_ARRAY_REARRANGEBEGIN1AFTER "Attempted to rearrange protoexpandarray with the first beginning mark larger than it's size."
#define PLEX_ERRORMESS_ARRAY_REARRANGEBEGIN2AFTER "Attempted to rearrange protoexpandarray with the first beginning mark larger than it's size."
#define PLEX_ERRORMESS_ARRAY_REARRANGENOLEN "Attempted to rearrange protoexpandarary but with a length of 0. The process will be skipped."
#define PLEX_ERRORMESS_ARRAY_REARRANGEBUFFERFAIL "Attempted to rearrange protoexpandarray but failed to allocate buffer needed to do so. There's probably a paired error explaining why."

#define PLEX_ERRORMESS_ARRAY_SWAPPERCNULL1 "Attempted to swap two protoexpandarrays by percentage from a null pointer on the first protoexpandarray. Completely harmless but could be a sign of sloppy code."
#define PLEX_ERRORMESS_ARRAY_SWAPPERCNULL2 "Attempted to swap two protoexpandarrays by percentage from a null pointer on the second protoexpandarary. Completely harmless but could be a sign of sloppy code."
#define PLEX_ERRORMESS_ARRAY_SWAPPERCBEGIN1AFTER "Attempted to swap two protoexpandarrays but the first beginning mark is larger than 100%. This process will be ignored."
#define PLEX_ERRORMESS_ARRAY_SWAPPERCBEGIN2AFTER "Attempted to swap two protoexpandarrays but the second beginning mark is larger than 100%. This process will be ignored."
#define PLEX_ERRORMESS_ARRAY_SWAPPERCNOLENGTH "Attempted to rearrange two protoexpandarrays but the length is zero percent or less. This process will be ignored."
#define PLEX_ERRORMESS_ARRAY_SWAPPERCBEGIN1BEFORE "Attempted to rearrange two protoexpandarrays but from the first beginning mark zero percent or less. The mark will be set to zero before the function proceeds."
#define PLEX_ERRORMESS_ARRAY_SWAPPERCBEGIN2BEFORE "Attempted to rearrange two protoexpandarrays but from the second beginning mark zero percent or less. The mark will be set to zero before the function proceeds."
#define PLEX_ERRORMESS_ARRAY_SWAPPERCLENGTHAFTER "Attempted to rearrange two protoexpandarrays but the length is over 100%. The length will be set to 100% before the function proceeds."

#define PLEX_ERRORMESS_ARRAY_REARRANGEPERCNULL "Attempted to rearrange protoexpandarray by percentage from a null pointer on. Completely harmless but could be a sign of sloppy code."
#define PLEX_ERRORMESS_ARRAY_REARRANGEPERCBEGIN1AFTER "Attempted to rearrange protoexpandarary but the first beginning mark is larger than 100%. This process will be ignored."
#define PLEX_ERRORMESS_ARRAY_REARRANGEPERCBEGIN2AFTER "Attempted to rearrange protoexpandarary but the second beginning mark is larger than 100%. This process will be ignored."
#define PLEX_ERRORMESS_ARRAY_REARRANGEPERCNOLENGTH "Attempted to rearrange protoexpandarray but the length is zero percent or less. This process will be ignored."
#define PLEX_ERRORMESS_ARRAY_REARRANGEPERCBEGIN1BEFORE "Attempted to rearrange protoexpandarray but from the first beginning mark zero percent or less. The mark will be set to zero before the function proceeds."
#define PLEX_ERRORMESS_ARRAY_REARRANGEPERCBEGIN2BEFORE "Attempted to rearrange protoexpandarray but from the second beginning mark zero percent or less. The mark will be set to zero before the function proceeds."
#define PLEX_ERRORMESS_ARRAY_REARRANGEPERCLENGTHAFTER "Attempted to rearrange protoexpandarray but the length is over 100%. The length will be set to 100% before the function proceeds."
#define PLEX_ERRORMESS_ARRAY_CUTPERCNULL "Attempted to cut protoexpandarray by percentage from a null pointer. Completely harmless but could be a sign of sloppy code."
#define PLEX_ERRORMESS_ARRAY_CUTPERCBEGINAFTER "Attempted to cut protoexpandarary but the beginning mark is larger than 100%. This process will be ignored."
#define PLEX_ERRORMESS_ARRAY_CUTPERCNOLENGTH "Attempted to cut protoexpandarray but the length is zero percent or less. This process will be ignored."
#define PLEX_ERRORMESS_ARRAY_CUTPERCLENGTHAFTER "Attempted to cut protoexpandarray but the length is over 100%. The length will be set to 100% before the function proceeds."
#define PLEX_ERRORMESS_ARRAY_CUTPERCBEGINBEFORE "Attempted to cut protoexpandarray but from the beginning mark zero percent or less. The mark will be set to zero before the function proceeds."

#define PLEX_ERRORMESS_ARRAY_GETENTRYNULL "Attempted to get an entry from a null protoexpandarray. This is harmless but might be a sign of sloppy code."
#define PLEX_ERRORMESS_ARRAY_GETENTRYOVERCOUNT "Attempted to get an entry from a protoexpandarray beyond it's count. This could mean nothing but could also be a sign of sloppy code."

#define PLEX_ERRORMESS_ARRAY_ENTRYLASTNULL "Attempted to get the last entry from a protoexpandarray that's a null pointer. This is most likely harmless. Will return a NULL pointer. May be a sign of sloppy code."

#define PLEX_ERRORMESS_ARRAY_GETCOUNTNULL "Attempted to get the count from a protoexpandarary that's a null pointer. This is most likely harmless. Will return a 0. May be a sign of sloppy code."

#define PLEX_ERRORMESS_ARRAY_GETCAPACITYNULL "Attempted to get the capacity from a protoexpandarary that's a null pointer. This is most likely harmless. Will return 0. May be a sign of sloppy code."

#define PLEX_ERRORMESS_ARRAY_GETUNITSIZENULL "Attempted to get the unit size from a protoexpandarary that's a null pointer. This is most likely harmless. Will return 0. May be a sign of sloppy code."

#define PLEX_ERRORMESS_ARRAY_GETGROWTHSIZENULL "Attempted to get the growth size from a protoexpandarary that's a null pointer. This is most likely harmless. Will return 0. May be a sign of sloppy code."

#define PLEX_ERRORMESS_ARRAY_GETSIZENULL "Attempted to get the size from a protoexpandarary that's a null pointer. This is most likely harmless. Will return 0. May be a sign of sloppy code."

#define PLEX_ERRORMESS_ARRAY_GETCONTENTSNULL "Attempted to get the contents from a protoexpandarary that's a null pointer. This is most likely harmless. Will return a NULL pointer. May be a sign of sloppy code."

// errors for sound

#define PLEX_ERRORMESS_SOUND_BREAKOUTFAIL "Failed to cease the sound channel. This means a sound thread is stuck. The program will terminate, if this continues it could be serious. Reasons may be complicated. Best this be reported."
#define PLEX_ERRORMESS_SOUND_PLAYNOBUFFER "The sound thread ceased because it had no assigned buffer. This likely is not a serious issue. Sometimes it's the result of a buffer not loading properly."
#define PLEX_ERRORMESS_SOUND_PLAYBUFFEROVERCOUNT "For whatever reason the buffer in a soundthread is above the limit. This will prevent a sound thread from crashing but is really bizarre and should not happen. If you haven't modified sound.c or are using a modified sound.c this should be impossible. If it continues, you may want to look into downloading the vanilla source and rebuilding."
#define PLEX_ERRORMESS_SOUND_PRELOADNORUN "Tried to preload a buffer but the sound system is not running. It probably means you either tried before the PLEX_start or after PLEX_end."
#define PLEX_ERRORMESS_SOUND_PRELOADOVERLIMIT "Tried to load a buffer that is larger than the limited number of buffers. It will simply not be loaded."
#define PLEX_ERRORMESS_SOUND_PRELOADBADFILE "Failed to load a file into a sound buffer. It simply will not be loaded."
#define PLEX_ERRORMESS_SOUND_PRELOADBUFFERFAIL "Failed to allocate a new sound buffer. This is will likely be paired with a mem error or multiple mem errors."
#define PLEX_ERRORMESS_SOUND_ASSIGNBUFFERNORUN "Tried to assign a buffer but the sound system is not running. It probably means you either tried before the PLEX_start or after PLEX_end."
#define PLEX_ERRORMESS_SOUND_ASSIGNCHANNELOVERLIMIT "Tried to assign a buffer to a channel that is larger than the limited number of buffers. It will simply not be assigned."
#define PLEX_ERRORMESS_SOUND_ASSIGNCHANNELNOBUFFER "Tried to assign a buffer from a negative value. This is entirely harmless, it just won't be assigned. Howeer, this could mean a buffer was loaded from a bad file."
#define PLEX_ERRORMESS_SOUND_ASSIGNBUFFEROVERLIMIT "Tried to assign a buffer that is larger than the limited number of buffers. It will simply not be assigned."
#define PLEX_ERRORMESS_SOUND_ASSIGNBUFFERSOUNDLOCK "Tried to assign a buffer to a unlockable channel. This means the thread is being stalled and won't break out. There's many technical reasons why this might happen. If it continues, it should be reported."

#define PLEX_ERRORMESS_SOUND_LOADSOUNDFILENOBUFFER "Tried to load a sound file into a buffer of negative value. -1 typically treats buffers as non-buffers, and considers anything below 0 a negative. This is completely harmless but will result in the buffer not being loaded, ultimately."

#define PLEX_ERRORMESS_SOUND_SETBUFFERLENNORUN "Tried to set the length of a buffer before the sound system has run. This probably means the function was executed before PLEX_start or after PLEX_end."
#define PLEX_ERRORMESS_SOUND_SETBUFFERLENOVERLIMIT "Tried to set the length of a buffer that is larger than the limited number of buffers. It will simply not be changed."
#define PLEX_ERRORMESS_SOUND_SETBUFFERLENSOUNDLOCK "Tried to set the buffer length to a unlockable channel. This means the thread is being stalled and won't break out. There's many technical reasons why this might happen. If it continues, it should be reported."

#define PLEX_ERRORMESS_SOUND_SETSTARTITNORUN "Tried to set the length of a buffer before the sound system has run. This probably means the function was executed before PLEX_start or after PLEX_end."
#define PLEX_ERRORMESS_SOUND_SETSTARTITOVERLIMIT "Tried to set the length of a buffer that is larger than the limited number of buffers. It will simply not be changed."
#define PLEX_ERRORMESS_SOUND_SETSTARTITSOUNDLOCK "Tried to set the buffer length to a unlockable channel. This means the thread is being stalled and won't break out. There's many technical reasons why this might happen. If it continues, it should be reported."

#define PLEX_ERRORMESS_SOUND_SETENDITNORUN "Tried to set the length of a buffer before the sound system has run. This probably means the function was executed before PLEX_start or after PLEX_end."
#define PLEX_ERRORMESS_SOUND_SETENDITOVERLIMIT "Tried to set the length of a buffer that is larger than the limited number of buffers. It will simply not be changed."
#define PLEX_ERRORMESS_SOUND_SETENDITSOUNDLOCK "Tried to set the buffer length to a unlockable channel. This means the thread is being stalled and won't break out. There's many technical reasons why this might happen. If it continues, it should be reported."

#define PLEX_ERRORMESS_SOUND_SETSOUNDSEQNORUN "Tried to set the a complex sound sequence to a channel before the sound system has run. This probably means the function was executed before PLEX_start or after PLEX_end."
#define PLEX_ERRORMESS_SOUND_SETSOUNDSEQNOVERLIMIT "Tried to set the a complex sound sequence to a channel than the limited number of buffers. It will simply not be changed."
#define PLEX_ERRORMESS_SOUND_SETSOUNDSEQNOCOUNT "Tried to set a complex sound sequence to a channel with a count of 0. This will do nothing. Probably harmless, possibly the result of sloppy code."
#define PLEX_ERRORMESS_SOUND_SETSOUNDSEQOVERCOUNT "Tried to set up a complex sound sequence with over 8 parts. You can't do this. The limit - for the time being - is 8. This may change in future versions."
#define PLEX_ERRORMESS_SOUND_SETSOUNDSEQNSOUNDLOCK "Tried to set the a complex sound sequence to a channel to a unlockable channel. This means the thread is being stalled and won't break out. There's many technical reasons why this might happen. If it continues, it should be reported."

#define PLEX_ERRORMESS_SOUND_SWAPNORUN "Tried to swap a channel before the sound system has run. This probably means the function was executed before PLEX_start or after PLEX_end."
#define PLEX_ERRORMESS_SOUND_SWAPBUFFER1OVERLIMIT "Tried to swap a buffer but the first buffer was over the buffer limit."
#define PLEX_ERRORMESS_SOUND_SWAPBUFFER2OVERLIMIT "Tried to swap a buffer but the second buffer was over the buffer limit."
#define PLEX_ERRORMESS_SOUND_SWAPBUFFER1NOBUFFER "Tried to swap a buffer with the first buffer being a negative value. This is entirely harmless, it just won't be assigned. Howeer, this could mean a buffer was loaded from a bad file."
#define PLEX_ERRORMESS_SOUND_SWAPBUFFER2NOBUFFER "Tried to swap a buffer with the second buffer being a negative value. This is entirely harmless, it just won't be assigned. Howeer, this could mean a buffer was loaded from a bad file."

#define PLEX_ERRORMESS_SOUND_REARRANGENORUN "Tried to rearrange a buffer before the sound system has run. This probably means the function was executed before PLEX_start or after PLEX_end."
#define PLEX_ERRORMESS_SOUND_REARRANGEBUFFEROVERLIMIT "Tried to rearrange a buffer but the buffer was over the buffer limit."
#define PLEX_ERRORMESS_SOUND_REARRANGEBUFFERNOBUFFER "Tried to rearrange a buffer with the buffer being a negative value. This is entirely harmless, it just won't be assigned. Howeer, this could mean a buffer was loaded from a bad file."



#endif
