#ifndef PLEX_DATETIME_H
#define PLEX_DATETIME_H

#include "utility/defaults.h"

// This unit stores realworld datetime information which can be created manually or taken from the system directly (ie. acquire the current date and time).

// NOTE: There isn't much date math in here but there will be a lot more of that. I think this may be useful for save files or for games that use real world time. Personally, I don't imagine I'm going to use this feature too extensively, so if I neglect to add to it then that's probably why. Still, it's a nice little thing to have. Also, I might add options to use military (ie. 24-hour) time.


// the struct which contains the date time info
typedef struct
{
	uint16_t year;
	uint8_t month, day, hour, second, minute;
	bool pm;
} PLEX_DATETIME;

#ifdef __cplusplus
extern "C"
{
#endif
	// functions to set a new datetime. NOTE: I may sometime add a genDateTime to be a little more consistent stylistically.

	// these functions return false if toSet is NULL
	extern bool PLEX_setDateTime(const uint16_t year, const uint8_t month, const uint8_t day, const uint8_t hour, const uint8_t second, const uint8_t minute, const bool pm, PLEX_DATETIME* toSet);
	extern bool PLEX_setDateTime12(const uint16_t year, const uint8_t month, const uint8_t day, const uint8_t hour, const uint8_t second, const uint8_t minute, const bool pm, PLEX_DATETIME* toSet);
	extern bool PLEX_setDateTime24(const uint16_t year, const uint8_t month, const uint8_t day, const uint8_t hour, const uint8_t second, const uint8_t minute, const bool pm, PLEX_DATETIME* toSet);

	extern bool PLEX_setCurrentDateTime(PLEX_DATETIME* toSet);
	extern bool PLEX_setCurrentDateTime12(PLEX_DATETIME* toSet);
	extern bool PLEX_setCurrentDateTime24(PLEX_DATETIME* toSet);

	// functions to get values for datetime

	// aquiring the day date or the time from the datetime
	extern bool PLEX_getDateTime(uint8_t* day, uint8_t* month, uint16_t* year, const PLEX_DATETIME* from);
	extern bool PLEX_getTimeDateTime(uint8_t* minute, uint8_t* second, uint8_t* hour, bool* pm, const PLEX_DATETIME* from);

	// get a day value individually
	extern uint8_t PLEX_getDayDateTime(const PLEX_DATETIME* from);
	extern uint8_t PLEX_getMonthDateTime(const PLEX_DATETIME* from);
	extern uint16_t PLEX_getYearDateTime(const PLEX_DATETIME* from);

	// get a time value individually
	extern uint8_t PLEX_getMinuteDateTime(const PLEX_DATETIME* from);
	extern uint8_t PLEX_getSecondDateTime(const PLEX_DATETIME* from);
	extern uint8_t PLEX_getHourDateTime(const PLEX_DATETIME* from);
	extern uint8_t PLEX_getPMDateTime(const PLEX_DATETIME* from);

	// return a datetime that's equiv to the difference between two other datetimes. Of course, since the numbers are differences days and months don't reperesent their calendar dates and months
	extern PLEX_DATETIME PLEX_getDifferenceDateTime(const PLEX_DATETIME* from, const PLEX_DATETIME* from2);
	extern PLEX_DATETIME PLEX_getDifferenceDateTime12(const PLEX_DATETIME* from, const PLEX_DATETIME* from2);
	extern PLEX_DATETIME PLEX_getDifferenceDateTime24(const PLEX_DATETIME* from, const PLEX_DATETIME* from2);


	// returns the grand total calculated increments of a datetime. Could be useful combined with the getDifference function to get the day difference between two.
	extern uint64_t PLEX_getTotalDaysDateTime(const PLEX_DATETIME* from);
	extern uint64_t PLEX_getTotalMonthsDateTime(const PLEX_DATETIME* from);
	extern uint64_t PLEX_getTotalYearsDateTime(const PLEX_DATETIME* from);
	extern uint64_t PLEX_getTotalHoursDateTime(const PLEX_DATETIME* from);
	extern uint64_t PLEX_getTotalMinutesDateTime(const PLEX_DATETIME* from);
	extern uint64_t PLEX_getTotalSecondsDateTime(const PLEX_DATETIME* from);
#ifdef __cplusplus
}
#endif

#endif
