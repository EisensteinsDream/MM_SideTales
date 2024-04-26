#include "mem/mem.h"

typedef struct
{
	void* contents;
} P_MEMENTRY;

typedef struct
{
	bool used;
	size_t line, size;
	char filename[1024];
} P_MEMREPORTLINE;

static PLEX_PROTOEXPANDARRAY* p_contents = NULL;
static PLEX_PROTOEXPANDARRAY* p_reuse = NULL;
static PLEX_PROTOEXPANDARRAY* p_memReportLines = NULL;

static bool p_running = false;

static uint64_t p_totalAlloc = 0;
static uint64_t p_totalDealloc = 0;

static uint64_t p_currentAlloc = 0;
static uint64_t p_peakAlloc = 0;

static bool p_memReport = false;
static FILE* p_memReportFile = NULL;

void* p_suballoc(const size_t size)
{
	void* const ret = malloc(size);

	if(ret == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_MEM, PLEX_ERROR_ALLOCFAIL, PLEX_ERROR_SEVERITY_FATAL, PLEX_ERRORMESS_ONSUBALLOC, PLEX_ERRORMESS_ALLOCED);

		return NULL;
	}

	memset(ret, 0, size);

	return ret;
}

void p_subdealloc(void** target)
{
	if(target == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_MEM, PLEX_ERROR_FREENULL, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_ONSUBDEALLOC, PLEX_ERRORMESS_ALLOCEDPOINTER);
		return;
	}

	if(*target == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_MEM, PLEX_ERROR_FREENULL, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_ONSUBDEALLOC, PLEX_ERRORMESS_ALLOCED);
		return;
	}

	free(*target);

	*target = NULL;
}

static void p_printMemReportString(const char* const toPrint)
{
	printf("%s", toPrint);
	if(p_memReportFile != NULL) fprintf(p_memReportFile, "%s", toPrint);
}

static void p_printMemReportSizeT(const size_t toPrint)
{
	printf("%zu", toPrint);
	if(p_memReportFile != NULL) fprintf(p_memReportFile, "%zu", toPrint);
}

static void p_printPlexMemReport()
{
	p_printMemReportString("\r\n\r\n");
	p_printMemReportString(PLEX_ERRORMESS_MEMREPORTHEADING);
	p_printMemReportString("\r\n\r\n\t");
	p_printMemReportString(PLEX_ERRORMESS_PEAKMEMORY);
	p_printMemReportString(": ");
	p_printMemReportSizeT(p_peakAlloc);
	p_printMemReportString("\r\n\r\n\t");
	p_printMemReportString(PLEX_ERRORMESS_MEMALLOCTOTAL);
	p_printMemReportString(" | ");
	p_printMemReportSizeT(p_totalAlloc);
	p_printMemReportString(" |");

	p_printMemReportString("\r\n\t");
	p_printMemReportString(PLEX_ERRORMESS_MEMDEALLOCTOTAL);
	p_printMemReportString(" | ");
	p_printMemReportSizeT(p_totalDealloc);
	p_printMemReportString(" |");

	if(p_totalAlloc != p_totalDealloc)
	{
		const size_t difference = p_totalAlloc - p_totalDealloc;

		p_printMemReportString("\r\n\r\n");
		p_printMemReportString(PLEX_ERRORMESS_ALLOCDIFF);
		p_printMemReportString(" | ");
		p_printMemReportSizeT(difference);
		p_printMemReportString(" | ");
		p_printMemReportString(PLEX_ERRORMESS_ALLOCDIFF2);
	}else{
		p_printMemReportString("\r\n\r\n");
		p_printMemReportString(PLEX_ERRORMESS_ALLOCSAME);
	}

	p_printMemReportString("\r\n\r\n");
	p_printMemReportString(PLEX_ERRORMESS_FOOTER);
	p_printMemReportString("\r\n");
}

// ---------------------------- //
// PUBLIC FUNCTIONS:
// ---------------------------- //

bool PLEX_startMem()
{
	if(p_running)
	{
		PLEX_errorNoDesc(PLEX_ERROR_CATEGORY_MEM, PLEX_ERROR_DOUBLESTART, PLEX_ERROR_SEVERITY_BADSTYLE);
		return true;
	}

	p_contents = PLEX_genProtoExpandArray(PLEX_getMemProfilePreload(), sizeof(P_MEMENTRY), p_suballoc, p_subdealloc);

	if(p_contents == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_MEM, PLEX_ERROR_CREATE, PLEX_ERROR_SEVERITY_FATAL, PLEX_ERRORMESS_ONSTARTMEM, PLEX_ERRORMESS_PREALLOCSSTACK);
		return false;
	}

	p_reuse = PLEX_genProtoExpandArray(PLEX_getMemProfilePreload(), sizeof(size_t), p_suballoc, p_subdealloc);

	if(p_reuse == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_MEM, PLEX_ERROR_CREATE, PLEX_ERROR_SEVERITY_FATAL, PLEX_ERRORMESS_ONSTARTMEM, PLEX_ERRORMESS_MEMREUSESTACK);
		PLEX_destroyProtoExpandArray(&p_contents);
		return false;
	}

	p_totalAlloc = 0;
	p_totalDealloc = 0;
	p_currentAlloc = 0;
	p_peakAlloc = 0;

	p_memReport = false;
	p_running = true;

	return true;
}

bool PLEX_updateMem() // < I'm keeping this even though it's empty just in case I want to implement more sophisticated system. Or somebody can modify this code for that.
{
	if(!p_running)
	{
		PLEX_errorNoDesc(PLEX_ERROR_CATEGORY_MEM, PLEX_ERROR_UPDATEBEFORESTART, PLEX_ERROR_SEVERITY_MINOR);
		return false;
	}

	if(p_currentAlloc > p_peakAlloc) p_peakAlloc = p_currentAlloc;

	return true;
}

void PLEX_endMem()
{
	char errPrintBuffer[256];

	if(!p_running)
	{
		PLEX_errorNoDesc(PLEX_ERROR_CATEGORY_MEM, PLEX_ERROR_ENDBEFORESTART, PLEX_ERROR_SEVERITY_BADSTYLE);
		return;
	}

	if(p_contents == NULL) PLEX_error2(PLEX_ERROR_CATEGORY_MEM, PLEX_ERROR_ISNULL, PLEX_ERROR_SEVERITY_IMPOSSIBLE, PLEX_ERRORMESS_ONENDMEM, PLEX_ERRORMESS_PREALLOCSSTACK);

	if(p_memReport)
	{
		const size_t memReportLineCount = PLEX_protoExpandArrayCount(p_memReportLines);

		if(memReportLineCount)
		{
			p_printMemReportString("\r\n\r\n");
			p_printMemReportString("Spots where PLEX_alloc allocates memory but PLEX_dealloc is not applied");
			p_printMemReportString(": \r\n");

			size_t index = 0;

			for(size_t ze = 0; ze < memReportLineCount; ++ze)
			{
				const P_MEMREPORTLINE* const line = PLEX_getProtoExpandArrayEntry(ze, p_memReportLines);

				P_MEMENTRY* target = NULL;

				if(line->used)
				{
					target = (P_MEMENTRY*)PLEX_getProtoExpandArrayEntry(ze, p_contents);

					index = *(size_t*)target->contents;

					if(index != SIZE_MAX)
					{
						p_printMemReportString("\r\n\t");
						p_printMemReportString("Last Line Used @ Index");
						p_printMemReportString(": | ");
						p_printMemReportSizeT(ze);
						p_printMemReportString(" | This was allocated at line: ");
						p_printMemReportSizeT(line->line);
						p_printMemReportString(", In File: \"");
						p_printMemReportString(line->filename);
						p_printMemReportString("\" Amount of memory allocated here: ");
						p_printMemReportSizeT(line->size);
					}
				}
			}
		}
	}

	for(size_t ze = 0; ze < PLEX_protoExpandArrayCount(p_contents); ++ze)
	{
		P_MEMENTRY* target = (P_MEMENTRY*)PLEX_getProtoExpandArrayEntry(ze, p_contents);

		if(target == NULL)
		{
			sprintf((char*)errPrintBuffer, "%s %zu", PLEX_ERRORMESS_PREALLOCSSTACKENTRY, ze);

			PLEX_error2(PLEX_ERROR_CATEGORY_MEM, PLEX_ERROR_ISNULL, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONENDMEM, (char*)errPrintBuffer);

			continue;
		}

		if(target->contents == NULL)
		{
			sprintf((char*)errPrintBuffer, "%s %zu", PLEX_ERRORMESS_PREALLOCSSTACKENTRYCONTENTS, ze);

			PLEX_error2(PLEX_ERROR_CATEGORY_MEM, PLEX_ERROR_CONTENTCORRUPTION, PLEX_ERROR_SEVERITY_FATAL, PLEX_ERRORMESS_ONENDMEM, (char*)errPrintBuffer);

			continue;
		}

		p_subdealloc((void**)&target->contents);
	}

	if(p_memReport) p_printPlexMemReport();

	if(p_memReportFile != NULL)
	{
		fclose(p_memReportFile);
		p_memReportFile = NULL;
	}

	PLEX_destroyProtoExpandArray(&p_contents);
	PLEX_destroyProtoExpandArray(&p_reuse);
	if(p_memReportLines != NULL) PLEX_destroyProtoExpandArray(&p_memReportLines);
}

void* PLEX_allocInt(const size_t size, const size_t line, const char* const filename)
{
	const size_t fullsize = size + sizeof(size_t) * 2;

	P_MEMENTRY* target = NULL;

	void* ret = NULL;

	size_t index = 0;

	if(!p_running)
	{
		PLEX_error(PLEX_ERROR_CATEGORY_MEM, PLEX_ERROR_ACTIONBEFORESTART, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONMEMALLOC);
		return NULL;
	}

	if(p_contents == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_MEM, PLEX_ERROR_ISNULL, PLEX_ERROR_SEVERITY_FATAL, PLEX_ERRORMESS_ONMEMALLOC, PLEX_ERRORMESS_PREALLOCSSTACKCONTENTS);
		return NULL;
	}

	if(p_reuse == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_MEM, PLEX_ERROR_ISNULL, PLEX_ERROR_SEVERITY_FATAL, PLEX_ERRORMESS_ONMEMALLOC, PLEX_ERRORMESS_MEMREUSESTACK);
		return NULL;
	}

	if(PLEX_protoExpandArrayCount(p_reuse))
	{
		const size_t reuseIndex = ((size_t*)PLEX_getProtoExpandArrayLast(p_reuse))[0];

		target = (P_MEMENTRY*)PLEX_getProtoExpandArrayEntry(reuseIndex, p_contents);

		if(target == NULL)
		{
			PLEX_error2(PLEX_ERROR_CATEGORY_MEM, PLEX_ERROR_NOGETARRAY, PLEX_ERROR_SEVERITY_FATAL, PLEX_ERRORMESS_ONMEMALLOC, PLEX_ERRORMESS_MEMREUSESTACK);
			return NULL;
		}

		if(target->contents != NULL)
		{
			const size_t size2 = ((size_t*)target->contents)[1];

			if(size > size2 || size2 >= PLEX_getMemProfileMemReuse())
			{
				p_subdealloc((void**)&target->contents);
				target->contents = p_suballoc(fullsize + 1);
			}
		}else target->contents = p_suballoc(fullsize + 1);

		((size_t*)target->contents)[0] = reuseIndex;

		PLEX_decrementProtoExpandArray(p_reuse);

		index = reuseIndex;
	}else{
		if(!PLEX_incrementProtoExpandArray(p_contents))
		{
			PLEX_error2(PLEX_ERROR_CATEGORY_MEM, PLEX_ERROR_INCREMENT, PLEX_ERROR_SEVERITY_FATAL, PLEX_ERRORMESS_ONMEMALLOC, PLEX_ERRORMESS_PREALLOCSSTACK);
			return false;
		}

		target = (P_MEMENTRY*)PLEX_getProtoExpandArrayLast(p_contents);

		if(target == NULL)
		{
			PLEX_error2(PLEX_ERROR_CATEGORY_MEM, PLEX_ERROR_NOGETARRAY, PLEX_ERROR_SEVERITY_FATAL, PLEX_ERRORMESS_ONMEMALLOC, PLEX_ERRORMESS_PREALLOCSSTACK);
			return NULL;
		}

		target->contents = p_suballoc(fullsize + 1);

		index = PLEX_protoExpandArrayCount(p_contents) - 1;

		((size_t*)target->contents)[0] = index;
	}

	((size_t*)target->contents)[1] = size;

	ret = (void*)((size_t*)target->contents + 2);

	memset(ret, 0, size);

	if(p_memReportFile)
	{
		const size_t originalCount = PLEX_protoExpandArrayCount(p_memReportLines);

		P_MEMREPORTLINE* newLine = NULL;

		size_t copySize = 0;

		bool overSize = false;

		if(index >= originalCount)
		{
			if(!PLEX_multiIncrementProtoExpandArray(index - originalCount + 1, p_memReportLines))
			{
				PLEX_error2(PLEX_ERROR_CATEGORY_MEM, PLEX_ERROR_INCREMENT, PLEX_ERROR_SEVERITY_FATAL, PLEX_ERRORMESS_ONMEMALLOC, PLEX_ERRORMESS_MEMERRORREPORTSTACK);
				return ret;
			}

			if(index)
			{
				for(size_t ze = originalCount; ze < index - 1; ++ze)
				{
					P_MEMREPORTLINE* line = PLEX_getProtoExpandArrayEntry(ze, p_memReportLines);

					line->used = false;
				}
			}
		}

		newLine = PLEX_getProtoExpandArrayEntry(index, p_memReportLines);

		copySize = strlen(filename) + 1;

		if(copySize >= 1024)
		{
			copySize = 1019;
			overSize = true;

			PLEX_error2(PLEX_ERROR_CATEGORY_MEM, PLEX_ERROR_OVERLIMIT, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONMEMALLOC, PLEX_ERRORMESS_MEMERRORREPORTFILENAME);
		}

		memcpy(newLine->filename, filename, copySize);

		if(overSize)
		{
			newLine->filename[1020] = '.';
			newLine->filename[1021] = '.';
			newLine->filename[1022] = '.';
			newLine->filename[1023] = '\0';
		}

		newLine->line = line;
		newLine->size = size;
		newLine->used = true;
	}

	p_totalAlloc += size;
	p_currentAlloc += size;

	return ret;
}

void PLEX_deallocInt(void** toFree)
{
	size_t* reuseEntry = NULL;
	size_t reuseIndex = 0;

	size_t* sizePtr = NULL;
	size_t size = 0;

	size_t* indexPtr = NULL;
	size_t index = 0;

	void* toFreePtr = NULL;

	if(!p_running)
	{
		PLEX_error(PLEX_ERROR_CATEGORY_MEM, PLEX_ERROR_ACTIONBEFORESTART, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONMEMDEALLOC);
		return;
	}

	if(toFree == NULL)
	{
		PLEX_errorNoDesc(PLEX_ERROR_CATEGORY_MEM, PLEX_ERROR_FREENULL, PLEX_ERROR_SEVERITY_BADSTYLE);
		return;
	}

	toFreePtr = *toFree;
	*toFree = NULL;

	if(toFreePtr == NULL)
	{
		PLEX_errorNoDesc(PLEX_ERROR_CATEGORY_MEM, PLEX_ERROR_FREENULL, PLEX_ERROR_SEVERITY_BADSTYLE);
		return;
	}

	sizePtr = ((size_t*)toFreePtr - 1);
	size = *sizePtr;

	indexPtr = ((size_t*)toFreePtr - 2);
	index = *indexPtr;

	if(index == SIZE_MAX)
	{
		PLEX_errorNoDesc(PLEX_ERROR_CATEGORY_MEM, PLEX_ERROR_DOUBLEDEALLOC, PLEX_ERROR_SEVERITY_BADSTYLE);
		return;
	}

	reuseIndex = index;

	if(!PLEX_incrementProtoExpandArray(p_reuse))
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_MEM, PLEX_ERROR_INCREMENT, PLEX_ERROR_SEVERITY_FATAL, PLEX_ERRORMESS_ONMEMDEALLOC, PLEX_ERRORMESS_MEMREUSESTACK);
		return;
	}

	reuseEntry = (size_t*)PLEX_getProtoExpandArrayLast(p_reuse);

	if(reuseEntry == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_MEM, PLEX_ERROR_NOGETARRAY, PLEX_ERROR_SEVERITY_FATAL, PLEX_ERRORMESS_ONMEMDEALLOC, PLEX_ERRORMESS_MEMREUSESTACK);
		return;
	}

	*reuseEntry = reuseIndex;
	*indexPtr = SIZE_MAX;

	p_totalDealloc += size;
	p_currentAlloc -= size;
}

void PLEX_updateMemProfile()
{
	if(p_contents == NULL) PLEX_setGrowthUnitProtoExpandArray(PLEX_getMemProfilePreload(), p_contents);
	if(p_reuse == NULL) PLEX_setGrowthUnitProtoExpandArray(PLEX_getMemProfilePreload(), p_reuse);
	if(p_memReportLines == NULL) PLEX_setGrowthUnitProtoExpandArray(PLEX_getMemProfilePreload(), p_memReportLines);
}

void PLEX_flagMemReport(){ p_memReport = true; }

void PLEX_flagMemReportToFile()
{
	uint16_t year = 0;
	uint8_t month = 0;
	uint8_t day = 0;
	uint8_t hour = 0;
	uint8_t minute = 0;
	uint8_t second = 0;

	char* filename = NULL;

	if(!p_memReport || p_memReportFile != NULL || p_memReportLines != NULL) return;

	p_memReportLines = PLEX_genProtoExpandArray(PLEX_getMemProfilePreload(), sizeof(P_MEMREPORTLINE), p_suballoc, p_subdealloc);

	PLEX_getErrorLogDateInt(&year, &month, &day, &hour, &minute, &second);

	filename = malloc(PATH_MAX);

	sprintf(filename, "error/memreport_%04u_%02u_%02u_%02u_%02u_%02u", year, month, day, hour, minute, second);

	p_memReportFile = fopen(filename, "w+");

	free(filename);
}
