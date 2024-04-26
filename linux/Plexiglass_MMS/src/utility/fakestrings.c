#include "utility/fakestrings.h"

// NOTE: function descriptions can be found in header

// NOTE: the following functions convert fakestrings to uint8_t* pointers. which, tbf, is pretty unsafe in a lot of situations. These functions are static for a reason and work pretty well in thier specific context. It is not reccomended to make these public or run them in any other situation.

#define PLEX_MAXDIGIT 18

static const uint64_t p_tenPows[PLEX_MAXDIGIT] = { 1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000, 1000000000, 10000000000, 100000000000, 1000000000000, 10000000000000, 100000000000000, 1000000000000000, 10000000000000000, 100000000000000000 };

static size_t p_setString(const char* const from, uint8_t* to, const size_t limit)
{
	size_t next = from[0];
	size_t index = 0;

	while(next != '\0')
	{
		if(index >= limit) break;
		to[index] = next;
		++index;
		next = from[index];
	}

	to[index] = '\0';

	return index;
}

static size_t p_setStringFromNumber(const double from, uint8_t* to, const size_t limit)
{
	double adapt = from;
	size_t digitExcess = 0;

	size_t index = 0;
	size_t digitCount = 1;
	size_t digitBase = 9;

	memset(to, '\0', limit + 1); // limit + 1 is the 'nt' variable in fakestrings.

	if(from < 0) // adds the '-' for negative numbers, taking up a space
	{
		to[index] = '-';
		index++;
		adapt *= -1;
	}

	while(adapt > digitBase) // counts the number of digits above 0
	{
		++digitCount;
		digitBase = (digitBase * 10) + 9; // ensures the sequence 9, 99, 999, 9999, etc. for this number.
	}

	if(digitCount >= PLEX_MAXDIGIT) digitCount = PLEX_MAXDIGIT - 1;

	// prints each digit from left to right. If hitting less than 3 characters beyond the limit it ends in a '...' like if you had a PLEX_FAKESTRING_10 and you had a number like 123456789012 the string would be something like "1234567..."
	for(size_t ze = 0; ze < digitCount; ++ze)
	{
		const size_t digitOn = digitCount - ze;
		const uint64_t digitMask = p_tenPows[digitOn - 1];
		const size_t divider = digitMask ? adapt/digitMask : 0;

		uint8_t digit = '0' + divider;

		adapt -= divider * digitMask; // < do not use digit here

		if(index >= limit)
		{
			to[index - 3] = '.';
			to[index - 2] = '.';
			to[index - 1] = '.';
			to[index] = '\0';
			adapt = 0;
			break;
		}

		to[index] = digit;
		++index;
	}

	// the +1 preserves leading 0's past the decimal
	if(adapt > 0) digitExcess = (1 + adapt) * PLEX_STANDNUM_NUMCONVPREC; // sets a limit for the digits based off the limit which is described in hardlimits

	if(digitExcess) // all of this is ignored if the digitExcess is 0, which means no decimal value.
	{
		size_t digitExcessCount = 0;
		size_t digitExcessIterator = 0;
		size_t lastNonZero = 0;
		size_t zeroDiff = 0;

		digitBase = 9;

		while(digitExcess > digitBase) // counts the number of digits
		{
			++digitExcessCount;
			digitBase = (digitBase * 10) + 9; // ensures the sequence 9, 99, 999, 9999, etc. for this number.
		}

		// cuts off the 1 added for adapting. This preserves leading 0's past the decimal.
		digitExcess -= PLEX_STANDNUM_NUMCONVPREC;
		--digitExcessCount;
		//

		to[index] = '.'; // the '.' symbol is added at the beginning of string of decimal values
		++index;

		while(digitExcess) // reads and adds digits one by one by multiplying the number by a related amount. ie. if you have 0.33 it would determine the digit by multiplying that by 10 and making it 3.3 then converting it to an integer making it 3. Then for the next digit it would multiply by 100 to make it 33 and so on. It's an odd process but it works.
		{
			const size_t digitOn = digitExcessCount - digitExcessIterator;
			const size_t digitMask = p_tenPows[digitOn]; // does not need to subtract one for this one
			const size_t divider = digitMask ? digitExcess/digitMask : 0;

			uint8_t digit = '0' + divider;

			if(digit != '0') lastNonZero = digitExcessIterator;

			digitExcess -= divider * digitMask; // < do not use digit here

			if(index >= limit) break;

			to[index] = digit;
			++index;
			++digitExcessIterator;
		}

		if(lastNonZero < digitExcessIterator) // makes sure to add a null terminator at the end if it ends on a bunch of zeros so 0.3300000 is printed as "0.33"
		{
			zeroDiff = digitExcessIterator - lastNonZero - 1;
			memset(to + lastNonZero + 1, '\0', zeroDiff);
			index -= zeroDiff;
		}
	}

	return index;
}

static void p_concatString(uint8_t* to, const size_t limit, size_t* len, const uint64_t character, const size_t clen)
{
	size_t llen = *len;

	if(llen + clen >= limit) return;

	memcpy(to + llen, &character, clen);

	to[llen + clen] = '\0';

	*len = llen + clen;
}

static size_t p_mergeString(uint8_t* to, const uint8_t* from, const size_t tlen, const size_t flen, const size_t max)
{
	size_t totalLen = 0;

	size_t copyLen = flen;

	if(tlen >= max || !copyLen) return tlen;

	if(tlen + copyLen > max) copyLen = max - tlen;

	if(!copyLen) return tlen;

	memcpy(to + tlen, from, copyLen);

	totalLen = tlen + copyLen;

	to[totalLen] = '\0';

	return totalLen;
}

static void p_setLower(uint8_t* to, const size_t len, const size_t lowerStart, const size_t upperStart, const size_t checkLen)
{
	if(!len) return;

	for(size_t ze = 0; ze < len; ++ze)
	{
		uint8_t* next = to + ze;

		uint8_t lNext = *next;

		if(lNext >= upperStart && lNext < upperStart + checkLen) *next = lowerStart + (lNext - upperStart);
	}
}

static bool p_compare(const uint8_t* a, const uint8_t* b, const size_t aLen, const size_t bLen, const bool ignoreCase, const size_t line, const char* filename)
{
	bool ret = false;

	uint8_t* ac = NULL;
	uint8_t* bc = NULL;

	if(aLen != bLen) return false;

	ac = PLEX_allocInt(aLen + 1, line, filename);
	bc = PLEX_allocInt(bLen + 1, line, filename);

	memcpy(ac, a, aLen);
	memcpy(bc, b, bLen);

	if(ignoreCase)
	{
	#if (PLEX_LANGUAGE == PLEX_LANGUAGE_ENGLISH)
		p_setLower(ac, aLen, 'a', 'A', 26);
		p_setLower(bc, bLen, 'a', 'A', 26);
	#endif
	}

	ret = !memcmp(ac, bc, aLen); // remember 0 == success in memcmp

	PLEX_dealloc(&ac);
	PLEX_dealloc(&bc);

	return ret;
}

static bool p_startsWith(uint8_t* a, uint8_t* b, const size_t aLen, const size_t bLen, const bool ignoreCase, const size_t line, const char* filename)
{
	uint8_t* ac = NULL;
	uint8_t* bc = NULL;

	if(bLen > aLen) return false;

	ac = PLEX_allocInt(aLen + 1, line, filename);
	bc = PLEX_allocInt(bLen + 1, line, filename);

	memcpy(ac, a, aLen);
	memcpy(bc, b, bLen);

	if(ignoreCase)
	{
	#if (PLEX_LANGUAGE == PLEX_LANGUAGE_ENGLISH)
		p_setLower(ac, aLen, 'a', 'A', 26);
		p_setLower(bc, bLen, 'a', 'A', 26);
	#endif
	}

	for(size_t ze = 0; ze < bLen; ++ze)
	{
		const uint8_t aChar = ac[ze];
		const uint8_t bChar = bc[ze];

		if(aChar != bChar)
		{
			PLEX_dealloc(&ac);
			PLEX_dealloc(&bc);
			return false;
		}
	}

	PLEX_dealloc(&ac);
	PLEX_dealloc(&bc);

	return true;
}

// the basic operation to check if a string is a valid number

static bool p_numerical(const uint8_t* string, const size_t len)
{
	size_t it = 0;

	uint8_t next = string[it];
	uint8_t last = 0;

	bool hasDecimal = false;

	while(next != '\0' && it >= len)
	{
		if((next < '0' && next > '9') && (next == '-' && it) && (last != '-' && next == ' ') && (hasDecimal && next == '.'))
		{
			return false;
		}

		if(next == '.') hasDecimal = true;

		++it;
		if(it < len)
		{
			last = next;
			next = string[it];
		}
	}

	return true;
}

static bool p_integer(const uint8_t* string, const size_t len)
{
	size_t it = 0;

	uint8_t next = string[it];
	uint8_t last = 0;

	while(next != '\0' && it >= len)
	{
		if((next < '0' && next > '9') && (next == '-' && it) && (last != '-' && next == ' '))
		{
			return false;
		}

		++it;
		if(it < len)
		{
			last = next;
			next = string[it];
		}
	}

	return true;
}

// ---------------------------- //
// PUBLIC FUNCTIONS:
// ---------------------------- //

// NOTE: these are very reliant on static functions. It's probably more useful to look at the static functions they reference rather than what they contain.

PLEX_FAKESTRING_10 PLEX_createString10(const char* const from)
{
	PLEX_FAKESTRING_10 ret;

	if(from == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_FAKESTRING, PLEX_ERROR_GETNULL, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_ONCREATESTRING, PLEX_ERRORMESS_CREATEABLEFAKESTRING10);
		memset(&ret, 0, sizeof(ret));
		return ret;
	}

	ret.len = p_setString(from, (uint8_t*)&ret, 10);

	return ret;
}

PLEX_FAKESTRING_25 PLEX_createString25(const char* const from)
{
	PLEX_FAKESTRING_25 ret;

	if(from == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_FAKESTRING, PLEX_ERROR_GETNULL, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_ONCREATESTRING, PLEX_ERRORMESS_CREATEABLEFAKESTRING25);
		memset(&ret, 0, sizeof(ret));
		return ret;
	}

	ret.len = p_setString(from, (uint8_t*)&ret, 25);

	return ret;
}

PLEX_FAKESTRING_50 PLEX_createString50(const char* const from)
{
	PLEX_FAKESTRING_50 ret;

	if(from == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_FAKESTRING, PLEX_ERROR_GETNULL, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_ONCREATESTRING, PLEX_ERRORMESS_CREATEABLEFAKESTRING50);
		memset(&ret, 0, sizeof(ret));
		return ret;
	}

	ret.len = p_setString(from, (uint8_t*)&ret, 50);

	return ret;
}

PLEX_FAKESTRING_100 PLEX_createString100(const char* const from)
{
	PLEX_FAKESTRING_100 ret;

	if(from == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_FAKESTRING, PLEX_ERROR_GETNULL, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_ONCREATESTRING, PLEX_ERRORMESS_CREATEABLEFAKESTRING100);
		memset(&ret, 0, sizeof(ret));
		return ret;
	}

	ret.len = p_setString(from, (uint8_t*)&ret, 100);

	return ret;
}

PLEX_FAKESTRING_500 PLEX_createString500(const char* const from)
{
	PLEX_FAKESTRING_500 ret;

	if(from == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_FAKESTRING, PLEX_ERROR_GETNULL, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_ONCREATESTRING, PLEX_ERRORMESS_CREATEABLEFAKESTRING500);
		memset(&ret, 0, sizeof(ret));
		return ret;
	}

	ret.len = p_setString(from, (uint8_t*)&ret, 500);

	return ret;
}

size_t PLEX_strLen10(const PLEX_FAKESTRING_10 string){ return string.len; }
size_t PLEX_strLen25(const PLEX_FAKESTRING_25 string){ return string.len; }
size_t PLEX_strLen50(const PLEX_FAKESTRING_50 string){ return string.len; }
size_t PLEX_strLen100(const PLEX_FAKESTRING_100 string){ return string.len; }
size_t PLEX_strLen500(const PLEX_FAKESTRING_500 string){ return string.len; }

PLEX_FAKESTRING_10 PLEX_createStringFromNumber10(const double number){ PLEX_FAKESTRING_10 ret; ret.len = p_setStringFromNumber(number, (uint8_t*)&ret, 10); return ret; }
PLEX_FAKESTRING_25 PLEX_createStringFromNumber25(const double number){ PLEX_FAKESTRING_25 ret; ret.len = p_setStringFromNumber(number, (uint8_t*)&ret, 25); return ret; }
PLEX_FAKESTRING_50 PLEX_createStringFromNumber50(const double number){ PLEX_FAKESTRING_50 ret; ret.len = p_setStringFromNumber(number, (uint8_t*)&ret, 50); return ret; }
PLEX_FAKESTRING_100 PLEX_createStringFromNumber100(const double number){ PLEX_FAKESTRING_100 ret; ret.len = p_setStringFromNumber(number, (uint8_t*)&ret, 100); return ret; }
PLEX_FAKESTRING_500 PLEX_createStringFromNumber500(const double number){ PLEX_FAKESTRING_500 ret; ret.len = p_setStringFromNumber(number, (uint8_t*)&ret, 500); return ret; }

PLEX_FAKESTRING_10 PLEX_blankString10(){ return PLEX_createString10(""); }
PLEX_FAKESTRING_25 PLEX_blankString25(){ return PLEX_createString25(""); }
PLEX_FAKESTRING_50 PLEX_blankString50(){ return PLEX_createString50(""); }
PLEX_FAKESTRING_100 PLEX_blankString100(){ return PLEX_createString100(""); }
PLEX_FAKESTRING_500 PLEX_blankString500(){ return PLEX_createString500(""); }

PLEX_FAKESTRING_10 PLEX_concatString10(const PLEX_FAKESTRING_10 string, const uint64_t character, const size_t clen)
{
	PLEX_FAKESTRING_10 ret = string;

	p_concatString((uint8_t*)&ret, 10, &ret.len, character, clen);

	return ret;
}

PLEX_FAKESTRING_25 PLEX_concatString25(const PLEX_FAKESTRING_25 string, const uint64_t character, const size_t clen)
{
	PLEX_FAKESTRING_25 ret = string;

	p_concatString((uint8_t*)&ret, 25, &ret.len, character, clen);

	return ret;
}

PLEX_FAKESTRING_50 PLEX_concatString50(const PLEX_FAKESTRING_50 string, const uint64_t character, const size_t clen)
{
	PLEX_FAKESTRING_50 ret = string;

	p_concatString((uint8_t*)&ret, 50, &ret.len, character, clen);

	return ret;
}

PLEX_FAKESTRING_100 PLEX_concatString100(const PLEX_FAKESTRING_100 string, const uint64_t character, const size_t clen)
{
	PLEX_FAKESTRING_100 ret = string;

	p_concatString((uint8_t*)&ret, 100, &ret.len, character, clen);

	return ret;
}

PLEX_FAKESTRING_500 PLEX_concatString500(const PLEX_FAKESTRING_500 string, const uint64_t character, const size_t clen)
{
	PLEX_FAKESTRING_500 ret = string;

	p_concatString((uint8_t*)&ret, 500, &ret.len, character, clen);

	return ret;
}

PLEX_FAKESTRING_10 PLEX_mergeString10(const PLEX_FAKESTRING_10 string, const PLEX_FAKESTRING_10 string2)
{
	PLEX_FAKESTRING_10 ret = string;

	ret.len = p_mergeString((uint8_t*)&ret, (const uint8_t*)&string2, ret.len, string2.len, 10);

	return ret;
}

PLEX_FAKESTRING_25 PLEX_mergeString25(const PLEX_FAKESTRING_25 string, const PLEX_FAKESTRING_25 string2)
{
	PLEX_FAKESTRING_25 ret = string;

	ret.len = p_mergeString((uint8_t*)&ret, (const uint8_t*)&string2, ret.len, string2.len, 25);

	return ret;
}

PLEX_FAKESTRING_50 PLEX_mergeString50(const PLEX_FAKESTRING_50 string, const PLEX_FAKESTRING_50 string2)
{
	PLEX_FAKESTRING_50 ret = string;

	ret.len = p_mergeString((uint8_t*)&ret, (const uint8_t*)&string2, ret.len, string2.len, 50);

	return ret;
}

PLEX_FAKESTRING_100 PLEX_mergeString100(const PLEX_FAKESTRING_100 string, const PLEX_FAKESTRING_100 string2)
{
	PLEX_FAKESTRING_100 ret = string;

	ret.len = p_mergeString((uint8_t*)&ret, (const uint8_t*)&string2, ret.len, string2.len, 100);

	return ret;
}

PLEX_FAKESTRING_500 PLEX_mergeString500(const PLEX_FAKESTRING_500 string, const PLEX_FAKESTRING_500 string2)
{
	PLEX_FAKESTRING_500 ret = string;

	ret.len = p_mergeString((uint8_t*)&ret, (const uint8_t*)&string2, ret.len, string2.len, 500);

	return ret;
}

PLEX_FAKESTRING_10 PLEX_toLowerString10(const PLEX_FAKESTRING_10 string)
{
	PLEX_FAKESTRING_10 ret = string;

	#if (PLEX_LANGUAGE == PLEX_LANGUAGE_ENGLISH)
		p_setLower((uint8_t*)&ret, ret.len, 'a', 'A', 26);
	#endif

	return ret;
}

PLEX_FAKESTRING_25 PLEX_toLowerString25(const PLEX_FAKESTRING_25 string)
{
	PLEX_FAKESTRING_25 ret = string;

	#if (PLEX_LANGUAGE == PLEX_LANGUAGE_ENGLISH)
		p_setLower((uint8_t*)&ret, ret.len, 'a', 'A', 26);
	#endif

	return ret;
}

PLEX_FAKESTRING_50 PLEX_toLowerString50(const PLEX_FAKESTRING_50 string)
{
	PLEX_FAKESTRING_50 ret = string;

	#if (PLEX_LANGUAGE == PLEX_LANGUAGE_ENGLISH)
		p_setLower((uint8_t*)&ret, ret.len, 'a', 'A', 26);
	#endif

	return ret;
}

PLEX_FAKESTRING_100 PLEX_toLowerString100(const PLEX_FAKESTRING_100 string)
{
	PLEX_FAKESTRING_100 ret = string;

	#if (PLEX_LANGUAGE == PLEX_LANGUAGE_ENGLISH)
		p_setLower((uint8_t*)&ret, ret.len, 'a', 'A', 26);
	#endif

	return ret;
}

PLEX_FAKESTRING_500 PLEX_toLowerString500(const PLEX_FAKESTRING_500 string)
{
	PLEX_FAKESTRING_500 ret = string;

	#if (PLEX_LANGUAGE == PLEX_LANGUAGE_ENGLISH)
		p_setLower((uint8_t*)&ret, ret.len, 'a', 'A', 26);
	#endif

	return ret;
}

bool PLEX_compStrings10Int(const PLEX_FAKESTRING_10 a, const PLEX_FAKESTRING_10 b, const bool ignoreCase, const size_t line, const char* filename)
{
	return p_compare((uint8_t*)&a, (uint8_t*)&b, a.len, b.len, ignoreCase, line, filename);
}

bool PLEX_compStrings25Int(const PLEX_FAKESTRING_25 a, const PLEX_FAKESTRING_25 b, const bool ignoreCase, const size_t line, const char* filename)
{
	return p_compare((uint8_t*)&a, (uint8_t*)&b, a.len, b.len, ignoreCase, line, filename);
}

bool PLEX_compStrings50Int(const PLEX_FAKESTRING_50 a, const PLEX_FAKESTRING_50 b, const bool ignoreCase, const size_t line, const char* filename)
{
	return p_compare((uint8_t*)&a, (uint8_t*)&b, a.len, b.len, ignoreCase, line, filename);
}

bool PLEX_compStrings100Int(const PLEX_FAKESTRING_100 a, const PLEX_FAKESTRING_100 b, const bool ignoreCase, const size_t line, const char* filename)
{
	return p_compare((uint8_t*)&a, (uint8_t*)&b, a.len, b.len, ignoreCase, line, filename);
}

bool PLEX_compStrings500Int(const PLEX_FAKESTRING_500 a, const PLEX_FAKESTRING_500 b, const bool ignoreCase, const size_t line, const char* filename)
{
	return p_compare((uint8_t*)&a, (uint8_t*)&b, a.len, b.len, ignoreCase, line, filename);
}

bool PLEX_startsWithString10Int(const PLEX_FAKESTRING_10 a, const PLEX_FAKESTRING_10 b, const bool ignoreCase, const size_t line, const char* filename)
{
	return p_startsWith((uint8_t*)&a, (uint8_t*)&b, a.len, b.len, ignoreCase, line, filename);
}

bool PLEX_startsWithString25Int(const PLEX_FAKESTRING_25 a, const PLEX_FAKESTRING_25 b, const bool ignoreCase, const size_t line, const char* filename)
{
	return p_startsWith((uint8_t*)&a, (uint8_t*)&b, a.len, b.len, ignoreCase, line, filename);
}

bool PLEX_startsWithString50Int(const PLEX_FAKESTRING_50 a, const PLEX_FAKESTRING_50 b, const bool ignoreCase, const size_t line, const char* filename)
{
	return p_startsWith((uint8_t*)&a, (uint8_t*)&b, a.len, b.len, ignoreCase, line, filename);
}

bool PLEX_startsWithString100Int(const PLEX_FAKESTRING_100 a, const PLEX_FAKESTRING_100 b, const bool ignoreCase, const size_t line, const char* filename)
{
	return p_startsWith((uint8_t*)&a, (uint8_t*)&b, a.len, b.len, ignoreCase, line, filename);
}

bool PLEX_startsWithString500Int(const PLEX_FAKESTRING_500 a, const PLEX_FAKESTRING_500 b, const bool ignoreCase, const size_t line, const char* filename)
{
	return p_startsWith((uint8_t*)&a, (uint8_t*)&b, a.len, b.len, ignoreCase, line, filename);
}

bool PLEX_isStringNumber10(const PLEX_FAKESTRING_10 string)
{
	return p_numerical((uint8_t*)&string, string.len);
}

bool PLEX_isStringNumber25(const PLEX_FAKESTRING_25 string)
{
	return p_numerical((uint8_t*)&string, string.len);
}

bool PLEX_isStringNumber50(const PLEX_FAKESTRING_50 string)
{
	return p_numerical((uint8_t*)&string, string.len);
}

bool PLEX_isStringNumber100(const PLEX_FAKESTRING_100 string)
{
	return p_numerical((uint8_t*)&string, string.len);
}

bool PLEX_isStringNumber500(const PLEX_FAKESTRING_500 string)
{
	return p_numerical((uint8_t*)&string, string.len);
}

bool PLEX_isStringInteger10(const PLEX_FAKESTRING_10 string)
{
	return p_integer((uint8_t*)&string, string.len);
}

bool PLEX_isStringInteger25(const PLEX_FAKESTRING_25 string)
{
	return p_integer((uint8_t*)&string, string.len);
}

bool PLEX_isStringInteger50(const PLEX_FAKESTRING_50 string)
{
	return p_integer((uint8_t*)&string, string.len);
}

bool PLEX_isStringInteger100(const PLEX_FAKESTRING_100 string)
{
	return p_integer((uint8_t*)&string, string.len);
}

bool PLEX_isStringInteger500(const PLEX_FAKESTRING_500 string)
{
	return p_integer((uint8_t*)&string, string.len);
}

PLEX_FAKESTRING_100 PLEX_fileLoadErrorPath(const char* const fileloc, const char* const errorTarget)
{
	PLEX_FAKESTRING_100 ret = PLEX_blankString100();
	PLEX_FAKESTRING_100 lFilename = PLEX_blankString100();
	PLEX_FAKESTRING_100 lErrorTarget = PLEX_blankString100();
	PLEX_FAKESTRING_100 mid = PLEX_blankString100();

	if(fileloc == NULL || errorTarget == NULL) return ret;

	lFilename = PLEX_createString100(fileloc);
	lErrorTarget = PLEX_createString100(errorTarget);
	mid = PLEX_createString100(PLEX_ERRORMESS_ERROR_FILENAMEMID);

	ret = PLEX_mergeString100(ret, lErrorTarget);
	ret = PLEX_mergeString100(ret, mid);
	ret = PLEX_concatString100(ret, '\"', 1);
	ret = PLEX_mergeString100(ret, lFilename);
	ret = PLEX_concatString100(ret, '\"', 1);

	return ret;
}
