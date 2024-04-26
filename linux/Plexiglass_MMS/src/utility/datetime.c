#include "utility/datetime.h"

// NOTE: function descriptions in header.

static bool p_setDateTimeBase(const uint16_t year, const uint8_t month, const uint8_t day, const uint8_t hour, const uint8_t second, const uint8_t minute, const bool pm, PLEX_DATETIME* toSet, const bool is24)
{
	toSet->second = second;
	toSet->minute = minute;
	toSet->hour = hour;
	toSet->day = day;
	toSet->month = month;
	toSet->year = year;
	toSet->pm = pm;

	if(!is24)
	{
		toSet->hour %= 12;
		if(!hour) toSet->hour = 12;
	}

	return true;
}

static bool p_setCurrentDateTime(PLEX_DATETIME* toSet, const bool is24)
{
	time_t source;
	struct tm* values;

	uint16_t year = 0;
	uint8_t month = 0;
	uint8_t day = 0;
	uint8_t hour = 0;
	uint8_t second = 0;
	uint8_t minute = 0;
	bool pm = false;

	if(time(&source) == -1)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_DATETIME, PLEX_ERROR_BADOPERATION, PLEX_ERROR_SEVERITY_FATAL, PLEX_ERRORMESS_ONSETCURRENTDATETIMELOW, PLEX_ERRORMESS_OP_CTIME);
		return false;
	}

	values = localtime(&source);

	if(values == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_DATETIME, PLEX_ERROR_BADOPERATION, PLEX_ERROR_SEVERITY_FATAL, PLEX_ERRORMESS_ONSETCURRENTDATETIMELOW, PLEX_ERRORMESS_OP_CLOCALTIME);
		return false;
	}

	second = values->tm_sec;
	minute = values->tm_min;
	hour = values->tm_hour;
	day = values->tm_mday;
	month = values->tm_mon + 1;
	year = values->tm_year + 1900;
	pm = values->tm_hour >= 12;

	return p_setDateTimeBase(year, month, day, hour, second, minute, pm, toSet, is24);
}

static bool p_isLeapYearInt(const uint32_t year)
{
	return year % 4 && (!(year % 100) || year % 400);
}

static bool p_isLeapYear(const PLEX_DATETIME* from)
{
	return p_isLeapYearInt(from->year);
}

static uint64_t p_leapYearAdjust(const uint64_t onYear)
{
	return 365 + p_isLeapYearInt(onYear);
}

static uint64_t p_monthLen(const bool isLeapYear, const uint64_t month)
{
	const size_t aMonth = month % 12;

	switch(aMonth)
	{
		case 3:
		case 5:
		case 8:
		case 10:
			return 30;
		case 1:
			return 28 + isLeapYear;
		break;
	};

	return 31;
}

static uint64_t p_yearDays(const PLEX_DATETIME* from)
{
	return p_leapYearAdjust(from->year);
}

static uint64_t p_monthDays(const bool isLeapYear, const PLEX_DATETIME* from)
{
	uint64_t monthDays = 0;

	// month 0 is January and month 11 is December
	if(from->month > 0) monthDays += 31;
	if(from->month > 1) monthDays += 28 + isLeapYear;
	if(from->month > 2) monthDays += 31;
	if(from->month > 3) monthDays += 30;
	if(from->month > 4) monthDays += 31;
	if(from->month > 5) monthDays += 30;
	if(from->month > 6) monthDays += 31;
	if(from->month > 7) monthDays += 31;
	if(from->month > 8) monthDays += 30;
	if(from->month > 9) monthDays += 31;
	if(from->month > 10) monthDays += 30;

	return monthDays;
}

static uint64_t p_dayYears(const PLEX_DATETIME* from)
{
	uint64_t dayCount = from->day;
	uint64_t dayYears = 0;
	uint64_t onYear = from->year;
	uint64_t yearLen = p_leapYearAdjust(onYear);

	while(dayCount > yearLen)
	{
		dayCount -= yearLen;
		++onYear;
		++dayYears;
		yearLen = p_leapYearAdjust(onYear);
	}

	return dayYears;
}

static uint64_t p_dayMonths(const bool isLeapYear, const PLEX_DATETIME* from)
{
	uint64_t monthCount = 0;
	uint64_t monthDays = from->day;
	uint64_t onMonth = from->month;
	uint64_t monthLen = p_monthLen(isLeapYear, onMonth);

	while(monthDays > monthLen)
	{
		monthDays -= monthLen;
		++monthCount;

		if(onMonth) --onMonth;
		else onMonth = 11;

		monthLen = p_monthLen(isLeapYear, onMonth);
	}

	return monthCount;
}

static PLEX_DATETIME p_getDifferenceDateTimeBase(const PLEX_DATETIME* from, const PLEX_DATETIME* from2, const bool is24)
{
	PLEX_DATETIME lFrom, lFrom2;

	PLEX_DATETIME ret;

	uint8_t hour = 0;
	uint8_t hour2 = 0;

	bool swap = false;

	memset(&ret, 0, sizeof(ret));

	if(from2->year < from->year)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_DATETIME, PLEX_ERROR_MINMAXREVERSE, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_ONGETTINGDATETIMEDIFFERENCE, PLEX_ERRORMESS_TIMEYEAR);
		swap = true;
	}else{
		if(from2->year == from->year && from2->month < from->month)
		{
			PLEX_error2(PLEX_ERROR_CATEGORY_DATETIME, PLEX_ERROR_MINMAXREVERSE, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_ONGETTINGDATETIMEDIFFERENCE, PLEX_ERRORMESS_TIMEMONTH);
			swap = true;
		}else{
			if(from2->month == from->month && from2->day < from->day)
			{
				PLEX_error2(PLEX_ERROR_CATEGORY_DATETIME, PLEX_ERROR_MINMAXREVERSE, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_ONGETTINGDATETIMEDIFFERENCE, PLEX_ERRORMESS_TIMEDAY);
				swap = true;
			}else{
				if(from2->day == from->day && from2->hour < from->hour)
				{
					PLEX_error2(PLEX_ERROR_CATEGORY_DATETIME, PLEX_ERROR_MINMAXREVERSE, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_ONGETTINGDATETIMEDIFFERENCE, PLEX_ERRORMESS_TIMEHOUR);
					swap = true;
				}else{
					if(from2->hour == from->hour && from2->minute < from->minute)
					{
						PLEX_error2(PLEX_ERROR_CATEGORY_DATETIME, PLEX_ERROR_MINMAXREVERSE, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_ONGETTINGDATETIMEDIFFERENCE, PLEX_ERRORMESS_TIMEMINUTE);
						swap = true;
					}else{
						if(from2->minute == from->minute && from2->second < from->second)
						{
							PLEX_error2(PLEX_ERROR_CATEGORY_DATETIME, PLEX_ERROR_MINMAXREVERSE, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_ONGETTINGDATETIMEDIFFERENCE, PLEX_ERRORMESS_TIMESECOND);
							swap = true;
						}
					}
				}
			}
		}
	}

	if(swap)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_DATETIME, PLEX_ERROR_MINMAXREVERSE, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_ONGETTINGDATETIMEDIFFERENCE, PLEX_ERRORMESS_DATEDIFFSWAP);

		lFrom = *from2;
		lFrom2 = *from;
	}else{
		lFrom = *from;
		lFrom2 = *from2;
	}

	hour = lFrom.hour;
	hour2 = lFrom2.hour;

	if(!is24)
	{
		if(hour == 12 && lFrom.pm) hour += 12;
		if(hour2 == 12 && lFrom2.pm) hour2 += 12;
	}

	ret.year = lFrom2.year - lFrom.year;
	ret.month = lFrom2.month - lFrom.month;
	ret.day = lFrom2.day - lFrom.day;
	ret.hour = lFrom2.hour - lFrom.hour;
	ret.minute = lFrom2.minute - lFrom.minute;
	ret.second = lFrom2.second - lFrom.second;
	ret.pm = false;

	if(!is24)
	{
		if(ret.hour > 12)
		{
			ret.hour = ret.hour - 12;
			ret.pm = true;
		}

		if(hour == 0) hour = 12;
		if(hour2 == 0) hour2 = 12;
	}

	return ret;
}

// ---------------------------- //
// PUBLIC FUNCTIONS:
// ---------------------------- //


bool PLEX_setDateTime(const uint16_t year, const uint8_t month, const uint8_t day, const uint8_t hour, const uint8_t second, const uint8_t minute, const bool pm, PLEX_DATETIME* toSet)
{
	if(toSet == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_DATETIME, PLEX_ERROR_SETNULL, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_ONSETTIMEDATE, PLEX_ERRORMESS_DATETIME);
		return false;
	}

	return p_setDateTimeBase(year, month, day, hour, second, minute, pm, toSet, PLEX_DEF_24HourTime());
}

bool PLEX_setDateTime12(const uint16_t year, const uint8_t month, const uint8_t day, const uint8_t hour, const uint8_t second, const uint8_t minute, const bool pm, PLEX_DATETIME* toSet)
{
	if(toSet == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_DATETIME, PLEX_ERROR_SETNULL, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_ONSETTIMEDATE12, PLEX_ERRORMESS_DATETIME);
		return false;
	}

	return p_setDateTimeBase(year, month, day, hour, second, minute, pm, toSet, false);
}

bool PLEX_setDateTime24(const uint16_t year, const uint8_t month, const uint8_t day, const uint8_t hour, const uint8_t second, const uint8_t minute, const bool pm, PLEX_DATETIME* toSet)
{
	if(toSet == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_DATETIME, PLEX_ERROR_SETNULL, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_ONSETTIMEDATE24, PLEX_ERRORMESS_DATETIME);
		return false;
	}

	return p_setDateTimeBase(year, month, day, hour, second, minute, pm, toSet, true);
}

bool PLEX_setCurrentDateTime(PLEX_DATETIME* toSet)
{
	if(toSet == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_DATETIME, PLEX_ERROR_SETNULL, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_ONSETCURRENTDATETIME, PLEX_ERRORMESS_DATETIME);
		return false;
	}

	return p_setCurrentDateTime(toSet, PLEX_DEF_24HourTime());
}

bool PLEX_setCurrentDateTime12(PLEX_DATETIME* toSet)
{
	if(toSet == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_DATETIME, PLEX_ERROR_SETNULL, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_ONSETCURRENTDATETIME12, PLEX_ERRORMESS_DATETIME);
		return false;
	}

	return p_setCurrentDateTime(toSet, false);
}

bool PLEX_setCurrentDateTime24(PLEX_DATETIME* toSet)
{
	if(toSet == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_DATETIME, PLEX_ERROR_SETNULL, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_ONSETCURRENTDATETIME24, PLEX_ERRORMESS_DATETIME);
		return false;
	}

	return p_setCurrentDateTime(toSet, true);
}

void PLEX_setErrorLogDate()
{
	PLEX_DATETIME current;

	uint16_t year = 0;
	uint8_t month = 0, day = 0, hour = 0, minute = 0, second = 0;

	PLEX_setCurrentDateTime24(&current);

	year = PLEX_getYearDateTime(&current);
	month = PLEX_getMonthDateTime(&current);
	day = PLEX_getDayDateTime(&current);
	hour = PLEX_getHourDateTime(&current);
	minute = PLEX_getMinuteDateTime(&current);
	second = PLEX_getSecondDateTime(&current);

	PLEX_setErrorLogDateInt(year, month, day, hour % 24, minute, second);
}

bool PLEX_getDateTime(uint8_t* day, uint8_t* month, uint16_t* year, const PLEX_DATETIME* from)
{
	if(day == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_DATETIME, PLEX_ERROR_GETNULL, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_ONGETDATETIME, PLEX_ERRORMESS_TIMEDAY);
		return false;
	}

	if(month == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_DATETIME, PLEX_ERROR_GETNULL, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_ONGETDATETIME, PLEX_ERRORMESS_TIMEMONTH);
		return false;
	}

	if(year == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_DATETIME, PLEX_ERROR_GETNULL, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_ONGETDATETIME, PLEX_ERRORMESS_TIMEYEAR);
		return false;
	}

	if(from == NULL)
	{
		*day = 0;
		*month = 0;
		*year = 0;

		PLEX_error2(PLEX_ERROR_CATEGORY_DATETIME, PLEX_ERROR_GETNULL, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_ONGETTINGDATETIMEMYD, PLEX_ERRORMESS_DATETIME);

		return false;
	}

	*day = from->day;
	*month = from->month;
	*year = from->year;

	return true;
}

bool PLEX_getTimeDateTime(uint8_t* minute, uint8_t* second, uint8_t* hour, bool* pm, const PLEX_DATETIME* from)
{
	if(minute == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_DATETIME, PLEX_ERROR_GETNULL, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_ONGETDATETIME, PLEX_ERRORMESS_TIMEMINUTE);
		return false;
	}

	if(second == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_DATETIME, PLEX_ERROR_GETNULL, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_ONGETDATETIME, PLEX_ERRORMESS_TIMESECOND);
		return false;
	}

	if(hour == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_DATETIME, PLEX_ERROR_GETNULL, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_ONGETDATETIME, PLEX_ERRORMESS_TIMEHOUR);
		return false;
	}

	if(pm == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_DATETIME, PLEX_ERROR_GETNULL, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_ONGETDATETIME, PLEX_ERRORMESS_AMPM);
		return false;
	}

	if(from == NULL)
	{
		*hour = 12;
		*minute = 0;
		*second = 0;
		*pm = false;

		PLEX_error2(PLEX_ERROR_CATEGORY_DATETIME, PLEX_ERROR_GETNULL, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONGETTINGDATETIMEMSH, PLEX_ERRORMESS_DATETIMESMALL);

		return false;
	}

	*hour = from->hour;
	*minute = from->minute;
	*second = from->second;
	*pm = from->pm;
	return true;
}

uint8_t PLEX_getDayDateTime(const PLEX_DATETIME* from)
{ 
	if(from == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_DATETIME, PLEX_ERROR_GETNULL, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONGETTINGDATETIMEDAY, PLEX_ERRORMESS_TIMEDAY);

		return 0;
	}

	return from->day;
}

uint8_t PLEX_getMonthDateTime(const PLEX_DATETIME* from)
{
	if(from == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_DATETIME, PLEX_ERROR_GETNULL, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONGETTINGDATETIMEMONTH, PLEX_ERRORMESS_TIMEMONTH);
		return 0;
	}

	return from->month;
}

uint16_t PLEX_getYearDateTime(const PLEX_DATETIME* from)
{
	if(from == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_DATETIME, PLEX_ERROR_GETNULL, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONGETTINGDATETIMEYEAR, PLEX_ERRORMESS_TIMEYEAR);
		return 0;
	}

	return from->year;
}

uint8_t PLEX_getMinuteDateTime(const PLEX_DATETIME* from)
{
	if(from == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_DATETIME, PLEX_ERROR_GETNULL, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONGETTINGDATETIMEMINUTE, PLEX_ERRORMESS_TIMEMINUTE);
		return 0;
	}

	return from->minute;
}

uint8_t PLEX_getSecondDateTime(const PLEX_DATETIME* from)
{
	if(from == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_DATETIME, PLEX_ERROR_GETNULL, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONGETTINGDATETIMESECOND, PLEX_ERRORMESS_TIMESECOND);
		return 0;
	}

	return from->second;
}

uint8_t PLEX_getHourDateTime(const PLEX_DATETIME* from)
{
	if(from == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_DATETIME, PLEX_ERROR_GETNULL, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONGETTINGDATETIMEHOUR, PLEX_ERRORMESS_TIMEHOUR);
		return 0;
	}

	return from->hour;
}

uint8_t PLEX_getPMDateTime(const PLEX_DATETIME* from)
{
	if(from == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_DATETIME, PLEX_ERROR_GETNULL, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONGETTINGDATETIMEPM, PLEX_ERRORMESS_AMPM);
		return false;
	}

	return from->pm;
}

PLEX_DATETIME PLEX_getDifferenceDateTime(const PLEX_DATETIME* from, const PLEX_DATETIME* from2)
{
	if(from == NULL)
	{
		PLEX_DATETIME ret;

		memset(&ret, 0, sizeof(ret));

		PLEX_error2(PLEX_ERROR_CATEGORY_DATETIME, PLEX_ERROR_COMPNULL, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_ONGETTINGDATETIMEDIFFERENCE, PLEX_ERRORMESS_DATETIME);
		return ret;
	}

	if(from2 == NULL)
	{
		PLEX_DATETIME ret;

		memset(&ret, 0, sizeof(ret));

		PLEX_error2(PLEX_ERROR_CATEGORY_DATETIME, PLEX_ERROR_COMPNULL, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_ONGETTINGDATETIMEDIFFERENCE, PLEX_ERRORMESS_DATETIME2);
		return ret;
	}

	return p_getDifferenceDateTimeBase(from, from2, PLEX_DEF_24HourTime());
}

PLEX_DATETIME PLEX_getDifferenceDateTime12(const PLEX_DATETIME* from, const PLEX_DATETIME* from2)
{
	if(from == NULL)
	{
		PLEX_DATETIME ret;

		memset(&ret, 0, sizeof(ret));

		PLEX_error2(PLEX_ERROR_CATEGORY_DATETIME, PLEX_ERROR_COMPNULL, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_ONGETTINGDATETIMEDIFFERENCE12, PLEX_ERRORMESS_DATETIME);
		return ret;
	}

	if(from2 == NULL)
	{
		PLEX_DATETIME ret;

		memset(&ret, 0, sizeof(ret));

		PLEX_error2(PLEX_ERROR_CATEGORY_DATETIME, PLEX_ERROR_COMPNULL, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_ONGETTINGDATETIMEDIFFERENCE12, PLEX_ERRORMESS_DATETIME2);
		return ret;
	}

	return p_getDifferenceDateTimeBase(from, from2, false);
}

PLEX_DATETIME PLEX_getDifferenceDateTime24(const PLEX_DATETIME* from, const PLEX_DATETIME* from2)
{
	if(from == NULL)
	{
		PLEX_DATETIME ret;

		memset(&ret, 0, sizeof(ret));

		PLEX_error2(PLEX_ERROR_CATEGORY_DATETIME, PLEX_ERROR_COMPNULL, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_ONGETTINGDATETIMEDIFFERENCE24, PLEX_ERRORMESS_DATETIME);
		return ret;
	}

	if(from2 == NULL)
	{
		PLEX_DATETIME ret;

		memset(&ret, 0, sizeof(ret));

		PLEX_error2(PLEX_ERROR_CATEGORY_DATETIME, PLEX_ERROR_COMPNULL, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_ONGETTINGDATETIMEDIFFERENCE24, PLEX_ERRORMESS_DATETIME2);
		return ret;
	}

	return p_getDifferenceDateTimeBase(from, from2, true);
}

uint64_t PLEX_getTotalDays(const PLEX_DATETIME* from)
{
	bool isLeapYear = false;

	uint64_t yearDays = 0;
	uint64_t monthDays = 0;

	if(from == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_DATETIME, PLEX_ERROR_GETNULL, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONGETTINGDATETIMETOTALDAY, PLEX_ERRORMESS_TIMETOTALDAYS);
		return 0;
	}

	isLeapYear = p_isLeapYear(from);
	yearDays = p_yearDays(from);
	monthDays = p_monthDays(isLeapYear, from);

	return yearDays + monthDays + from->day;
}

uint64_t PLEX_getTotalMonthsDateTime(const PLEX_DATETIME* from)
{
	uint64_t ret = 0;

	bool isLeapYear = false;

	if(from == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_DATETIME, PLEX_ERROR_GETNULL, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_ONGETTINGDATETIMETOTALMONTH, PLEX_ERRORMESS_DATETIME);
		return 0;
	}

	isLeapYear = p_isLeapYear(from);

	ret += from->year * 12;
	ret += from->month;
	ret += p_dayMonths(isLeapYear, from);

	return ret;
}

uint64_t PLEX_getTotalYearsDateTime(const PLEX_DATETIME* from)
{
	uint64_t ret = 0;

	if(from == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_DATETIME, PLEX_ERROR_GETNULL, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_ONGETTINGDATETIMETOTALYEAR, PLEX_ERRORMESS_DATETIME);
		return 0;
	}

	ret += from->year;
	ret += from->month/12;
	ret += p_dayYears(from);

	return ret;
}

uint64_t PLEX_getTotalHoursDateTime(const PLEX_DATETIME* from)
{
	uint64_t ret = 0;

	if(from == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_DATETIME, PLEX_ERROR_GETNULL, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_ONGETTINGDATETIMETOTALHOUR, PLEX_ERRORMESS_DATETIME);
		return 0;
	}

	ret = PLEX_getTotalSecondsDateTime(from)/360;

	return ret;
}

uint64_t PLEX_getTotalMinutesDateTime(const PLEX_DATETIME* from)
{
	uint64_t ret = 0;

	if(from == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_DATETIME, PLEX_ERROR_GETNULL, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_ONGETTINGDATETIMETOTALMINUTE, PLEX_ERRORMESS_DATETIME);
		return 0;
	}

	ret = PLEX_getTotalSecondsDateTime(from)/60;

	return ret;
}

uint64_t PLEX_getTotalSecondsDateTime(const PLEX_DATETIME* from)
{
	uint64_t ret = 0;

	if(from == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_DATETIME, PLEX_ERROR_GETNULL, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_ONGETTINGDATETIMETOTALSECOND, PLEX_ERRORMESS_DATETIME);
		return 0;
	}

	ret += from->second;
	ret += from->minute * 60;
	ret += from->hour * 360;
	ret = PLEX_getTotalDays(from) * 86400;

	return ret;
}
