#include "mmcommon.h"

bool MM_COMMON_start()
{
	MM_COMMON_startTheme();

	return true;
}

bool MM_COMMON_update()
{
	MM_COMMON_updateTheme();

	return true;
}

void MM_COMMON_end()
{
	MM_COMMON_endTheme();
}
