#include "utility/generator.h"

// NOTE: function descriptions in header.

PLEX_GENERATOR* PLEX_genGeneratorInt(const double minX, const double maxX, const double minY, const double maxY, const double minEndX, const double maxEndX, const double minEndY, const double maxEndY, const uint64_t duration, const uint64_t minWait, const uint64_t maxWait, const uint64_t minGenDuration, const uint64_t maxGenDuration, const bool recycle, const size_t line, const char* const filename)
{
	PLEX_GENERATOR* ret = PLEX_allocInt(sizeof(PLEX_GENERATOR), line, filename);

	if(ret == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_GENERATOR, PLEX_ERROR_ALLOCFAIL, PLEX_ERROR_SEVERITY_FATAL, PLEX_ERRORMESS_ONGENGENEARTOR, PLEX_ERRORMESS_GENERATOR);
		return NULL;
	}

	ret->minX = minX;
	ret->maxX = maxX;
	ret->minY = minY;
	ret->maxY = maxY;

	ret->minEndX = minEndX;
	ret->maxEndX = maxEndX;
	ret->minEndY = minEndY;
	ret->maxEndY = maxEndY;

	ret->duration = duration;
	ret->minWait = minWait;
	ret->maxWait = maxWait;
	ret->maxGenDuration = maxGenDuration;
	ret->minGenDuration = minGenDuration;

	ret->start = 0;
	ret->recycle = recycle;

	ret->generated = NULL;

	ret->wait = PLEX_randomNumber(minWait, maxWait);

	return ret;
}

void PLEX_destroyGenerator(PLEX_GENERATOR** generator)
{
	PLEX_GENERATOR* generatorPtr = NULL;

	if(generator == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_GENERATOR, PLEX_ERROR_ISNULL, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_ONDESTROYINGGENEARTOR, PLEX_ERRORMESS_GENERATOR);
		return;
	}

	generatorPtr = *generator;

	if(generatorPtr == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_GENERATOR, PLEX_ERROR_ISNULL, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_ONDESTROYINGGENEARTOR, PLEX_ERRORMESS_GENERATOR);
		return;
	}

	if(generatorPtr->generated != NULL) PLEX_destroyExpandArray(&generatorPtr->generated);

	PLEX_dealloc(generator);
}

void PLEX_startGeneratorInt(PLEX_GENERATOR* generator, const uint64_t time, const size_t line, const char* const filename)
{
	if(generator == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_GENERATOR, PLEX_ERROR_ISNULL, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_ONSTARTGENERATOR, PLEX_ERRORMESS_GENERATOR);
		return;
	}

	generator->start = time;

	if(generator->generated != NULL) PLEX_destroyExpandArray(&generator->generated);

	generator->generated = PLEX_genExpandArrayInt(PLEX_getMemProfileGenStart(), sizeof(PLEX_GENERATED), line, filename);
}

void PLEX_updateGeneratorInt(PLEX_GENERATOR* generator, const uint64_t time, const size_t line, const char* const filename)
{
	uint64_t duration = 0;

	size_t genCount = 0;

	PLEX_GENERATED* generated = NULL;

	if(generator == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_GENERATOR, PLEX_ERROR_ISNULL, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_ONUPDATEGENERATOR, PLEX_ERRORMESS_GENERATOR);
		return;
	}

	if(generator->generated == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_GENERATOR, PLEX_ERROR_ISNULL, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONUPDATEGENERATOR, PLEX_ERRORMESS_GENERATED);
		return;
	}

	duration = time - generator->start;

	if(duration < generator->wait) return;

	genCount = PLEX_expandArrayCount(generator->generated);

	if(genCount) generated = PLEX_getExpandArrayLast(generator->generated);

	if(!genCount || (generated->phaseLen < duration && duration < generator->duration))
	{
		if(!PLEX_incrementExpandArrayInt(generator->generated, line, filename))
		{
			PLEX_error2(PLEX_ERROR_CATEGORY_GENERATOR, PLEX_ERROR_INCREMENT, PLEX_ERROR_SEVERITY_FATAL, PLEX_ERRORMESS_ONUPDATEGENERATOR, PLEX_ERRORMESS_GENERATED);
			return;
		}

		generated = PLEX_getExpandArrayLast(generator->generated);

		generated->startX = PLEX_randomNumber(generator->minX, generator->maxX);
		generated->startY = PLEX_randomNumber(generator->minY, generator->maxY);
		generated->endX = PLEX_randomNumber(generator->minEndX, generator->maxEndX);
		generated->endY = PLEX_randomNumber(generator->minEndY, generator->maxEndY);

		generated->phaseLen = duration + PLEX_randomNumber(generator->minGenDuration, generator->maxGenDuration);
		generated->phaseStart = duration;
	}
}

bool PLEX_pullGenerated(const size_t index, const uint64_t time, double* x, double* y, PLEX_GENERATOR* generator)
{
	PLEX_GENERATED* generated = NULL;

	uint64_t duration = 0;

	double perc = 0;

	size_t count = 0;

	if(x == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_GENERATOR, PLEX_ERROR_ISNULL, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONPULLGENERATED, PLEX_ERRORMESS_GENXCOORDINATE);
		return false;
	}

	if(y == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_GENERATOR, PLEX_ERROR_ISNULL, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONPULLGENERATED, PLEX_ERRORMESS_GENYCOORDINATE);
		return false;
	}

	if(generator == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_GENERATOR, PLEX_ERROR_GETNULL, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_ONPULLGENERATED, PLEX_ERRORMESS_GENERATOR);
		*x = 0;
		*y = 0;
		return false;
	}

	count = PLEX_expandArrayCount(generator->generated);

	if(index >= count)
	{
		// error
		return false;
	}

	duration = time - generator->start;

	generated = PLEX_getExpandArrayEntry(index, generator->generated);

	perc = generated->phaseLen ? (duration - generated->phaseStart)/(double)generated->phaseLen : 0;

	if(perc > 1) perc = 1;

	*x = generated->startX + (perc * (generated->endX - generated->startX));
	*y = generated->startY + (perc * (generated->endY - generated->startY));

	if(generator->recycle && perc >= 1) generated->phaseStart = duration;

	return true;
}

void PLEX_changeGeneratorArea(const double minX, const double maxX, const double minY, const double maxY, const double minEndX, const double maxEndX, const double minEndY, const double maxEndY, PLEX_GENERATOR* generator)
{
	if(generator == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_GENERATOR, PLEX_ERROR_UNDERLIMIT, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_ONCHANGEGENERATORAREA, PLEX_ERRORMESS_GENERATOR);
		return;
	}

	generator->minX = minX;
	generator->maxX = maxX;
	generator->minY = minY;
	generator->maxY = maxY;

	generator->minEndX = minEndX;
	generator->maxEndX = maxEndX;
	generator->minEndY = minEndY;
	generator->maxEndY = maxEndY;
}

void PLEX_extendGeneratorDuration(const uint64_t by, PLEX_GENERATOR* generator)
{
	if(generator == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_GENERATOR, PLEX_ERROR_UNDERLIMIT, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONEXTENDINGGENERATORDURATION, PLEX_ERRORMESS_GENERATOR);
		return;
	}

	generator->duration += by;
}

void PLEX_getGeneratorArea(double* minX, double* maxX, double* minY, double* maxY, double* minEndX, double* maxEndX, double* minEndY, double* maxEndY, PLEX_GENERATOR* generator)
{
	if(minX == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_GENERATOR, PLEX_ERROR_UNDERLIMIT, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONGETTINGGENERATORAREA, PLEX_ERRORMESS_GENMINX);
		return;
	}

	if(maxX == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_GENERATOR, PLEX_ERROR_UNDERLIMIT, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONGETTINGGENERATORAREA, PLEX_ERRORMESS_GENMAXX);
		return;
	}

	if(minY == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_GENERATOR, PLEX_ERROR_UNDERLIMIT, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONGETTINGGENERATORAREA, PLEX_ERRORMESS_GENMINY);
		return;
	}

	if(maxY == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_GENERATOR, PLEX_ERROR_UNDERLIMIT, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONGETTINGGENERATORAREA, PLEX_ERRORMESS_GENMAXY);
		return;
	}

	if(minEndX == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_GENERATOR, PLEX_ERROR_UNDERLIMIT, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONGETTINGGENERATORAREA, PLEX_ERRORMESS_GENMINXEND);
		return;
	}

	if(maxEndX == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_GENERATOR, PLEX_ERROR_UNDERLIMIT, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONGETTINGGENERATORAREA, PLEX_ERRORMESS_GENMAXXEND);
		return;
	}

	if(minEndY == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_GENERATOR, PLEX_ERROR_UNDERLIMIT, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONGETTINGGENERATORAREA, PLEX_ERRORMESS_GENMINYEND);
		return;
	}

	if(maxEndY == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_GENERATOR, PLEX_ERROR_UNDERLIMIT, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONGETTINGGENERATORAREA, PLEX_ERRORMESS_GENMAXYEND);
		return;
	}

	if(generator == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_GENERATOR, PLEX_ERROR_GETNULL, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONGETTINGGENERATORAREA, PLEX_ERRORMESS_GENERATOR);

		*minX = 0;
		*maxX = 0;

		*minY = 0;
		*maxY = 0;

		*minEndX = 0;
		*maxEndX = 0;

		*minEndY = 0;
		*maxEndY = 0;

		return;
	}

	*minX = generator->minX;
	*maxX = generator->maxX;

	*minY = generator->minY;
	*maxY = generator->maxY;

	*minEndX = generator->minEndX;
	*maxEndX = generator->maxEndX;

	*minEndY = generator->minEndY;
	*maxEndY = generator->maxEndY;
}

size_t PLEX_generatedCount(const PLEX_GENERATOR generator)
{
	if(generator.generated == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_GENERATOR, PLEX_ERROR_GETNULL, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONGETTINGGENERATORAREA, PLEX_ERRORMESS_GENERATED);
		return 0;
	}

	return PLEX_expandArrayCount(generator.generated);
}

bool PLEX_generationFinished(const PLEX_GENERATOR generator, const uint64_t time)
{
	if(generator.generated == NULL)
	{
		// error
		return false;
	}

	return time - generator.start >= generator.duration;
}
