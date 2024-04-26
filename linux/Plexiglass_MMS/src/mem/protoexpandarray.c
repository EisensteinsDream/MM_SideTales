#include "mem/protoexpandarray.h"

PLEX_PROTOEXPANDARRAY* PLEX_genProtoExpandArray(const size_t growthUnit, const size_t unitSize, PLEX_F_MEMHOOK memHook, PLEX_F_FREEHOOK freeHook)
{
	const size_t size = growthUnit * unitSize;

	PLEX_PROTOEXPANDARRAY* ret = NULL;

	if(memHook == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ARRAY, PLEX_ERROR_ISNULL, PLEX_ERROR_SEVERITY_MAJOR, PLEX_ERRORMESS_ONARRAYCREATE, PLEX_ERRORMESS_MEMHOOK);
		return NULL;
	}

	if(freeHook == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ARRAY, PLEX_ERROR_ISNULL, PLEX_ERROR_SEVERITY_MAJOR, PLEX_ERRORMESS_ONARRAYCREATE, PLEX_ERRORMESS_FREEHOOK);
		return NULL;
	}

	if(!size)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ARRAY, PLEX_ERROR_ZEROVALUE, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONARRAYCREATE, PLEX_ERRORMESS_ARRAYCOUNT);
		// do not return NULL here. This being 0 either means the growth unit or the unit size is 0. A separate error will be returned for which one.
	}

	if(!growthUnit)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ARRAY, PLEX_ERROR_ZEROVALUE, PLEX_ERROR_SEVERITY_MAJOR, PLEX_ERRORMESS_ONARRAYCREATE, PLEX_ERRORMESS_ARRAYGROWTHUNIT);
		return NULL;
	}

	if(!unitSize)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ARRAY, PLEX_ERROR_ZEROVALUE, PLEX_ERROR_SEVERITY_MAJOR, PLEX_ERRORMESS_ONARRAYCREATE, PLEX_ERRORMESS_ARRAYUNITSIZE);
		return NULL;
	}

	// do not use alloc hook here
	ret = malloc(sizeof(PLEX_PROTOEXPANDARRAY));

	if(ret == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ARRAY, PLEX_ERROR_ALLOCFAIL, PLEX_ERROR_SEVERITY_FATAL, PLEX_ERRORMESS_ONARRAYCREATE, PLEX_ERRORMESS_ARRAY);
		return NULL;
	}

	ret->count = 0;
	ret->capacity = growthUnit;
	ret->growthUnit = growthUnit;
	ret->unitSize = unitSize;
	ret->contents = (char*)memHook(size);
	ret->memHook = memHook;
	ret->freeHook = freeHook;

	if(ret->contents == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ARRAY, PLEX_ERROR_ALLOCFAIL, PLEX_ERROR_SEVERITY_FATAL, PLEX_ERRORMESS_ONARRAYCREATE, PLEX_ERRORMESS_ARRAYCONTENTS);
		free(ret); // < do not use dealloc hook
		return NULL;
	}

	return ret;
}

void PLEX_destroyProtoExpandArray(PLEX_PROTOEXPANDARRAY** expandArray)
{
	PLEX_PROTOEXPANDARRAY* ptr = NULL;

	if(expandArray == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ARRAY, PLEX_ERROR_FREENULL, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_ONARRAYDESTROY, PLEX_ERRORMESS_ARRAY);
		return;
	}

	ptr = *expandArray;

	if(ptr == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ARRAY, PLEX_ERROR_FREENULL, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_ONARRAYDESTROY, PLEX_ERRORMESS_ARRAYPTR);
		return;
	}

	if(ptr->contents == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ARRAY, PLEX_ERROR_CONTENTCORRUPTION, PLEX_ERROR_SEVERITY_FATAL, PLEX_ERRORMESS_ONARRAYDESTROY, PLEX_ERRORMESS_ONARRAYDESTROY);
		free(ptr);
		return;
	}

	if(ptr->freeHook == NULL)
	{
		free(ptr);

		PLEX_error2(PLEX_ERROR_CATEGORY_ARRAY, PLEX_ERROR_HOOKCORRUPTION, PLEX_ERROR_SEVERITY_IMPOSSIBLE, PLEX_ERRORMESS_ONARRAYDESTROY, PLEX_ERRORMESS_ARRAYFREEHOOK);
		return;
	}

	ptr->freeHook((void**)&ptr->contents);

	free(ptr);

	*expandArray = NULL;
}

bool PLEX_expandProtoExpandArray(const size_t capacity, PLEX_PROTOEXPANDARRAY* expandArray)
{
	size_t newCapacity = 0;
	size_t oldCapacity = 0;

	size_t oldSize = 0;
	size_t newSize = 0;

	void* oldContents = NULL;

	if(expandArray == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ARRAY, PLEX_ERROR_EXPANDNULL, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_ONARRAYDESTROY, PLEX_ERRORMESS_ONARRAYEXPANSION);
		return false;
	}

	if(expandArray->contents == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ARRAY, PLEX_ERROR_CONTENTCORRUPTION, PLEX_ERROR_SEVERITY_FATAL, PLEX_ERRORMESS_ONARRAYDESTROY, PLEX_ERRORMESS_ONARRAYEXPANSION);
		return false;
	}

	if(expandArray->memHook == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ARRAY, PLEX_ERROR_HOOKCORRUPTION, PLEX_ERROR_SEVERITY_IMPOSSIBLE, PLEX_ERRORMESS_ONARRAYDESTROY, PLEX_ERRORMESS_MEMHOOK);
		return false;
	}

	if(expandArray->freeHook == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ARRAY, PLEX_ERROR_HOOKCORRUPTION, PLEX_ERROR_SEVERITY_IMPOSSIBLE, PLEX_ERRORMESS_ONARRAYDESTROY, PLEX_ERRORMESS_FREEHOOK);
		return false;
	}

	if(capacity >= expandArray->capacity)
	{
		oldCapacity = expandArray->capacity;
		newCapacity = capacity + expandArray->growthUnit;

		oldSize = oldCapacity * expandArray->unitSize;
		newSize = newCapacity * expandArray->unitSize;

		oldContents = expandArray->contents;

		expandArray->contents = expandArray->memHook(newSize);

		if(expandArray->contents == NULL)
		{
			PLEX_error2(PLEX_ERROR_CATEGORY_ARRAY, PLEX_ERROR_ALLOCFAIL, PLEX_ERROR_SEVERITY_FATAL, PLEX_ERRORMESS_ONARRAYDESTROY, PLEX_ERRORMESS_NEWARRAYCONTENTS);

			expandArray->contents = oldContents;
			return false;
		}

		memcpy(expandArray->contents, oldContents, oldSize);

		expandArray->freeHook((void**)&oldContents);

		expandArray->capacity = newCapacity;
	}

	return true;
}

bool PLEX_incrementProtoExpandArray(PLEX_PROTOEXPANDARRAY* expandArray)
{
	if(expandArray == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ARRAY, PLEX_ERROR_ISNULL, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_ONARRAYINCREMENTATION, PLEX_ERRORMESS_ARRAY);
		return false;
	}

	if(expandArray->contents == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ARRAY, PLEX_ERROR_CONTENTCORRUPTION, PLEX_ERROR_SEVERITY_FATAL, PLEX_ERRORMESS_ONARRAYINCREMENTATION, PLEX_ERRORMESS_ARRAYCONTENTS);
		return false;
	}

	return PLEX_multiIncrementProtoExpandArray(1, expandArray);
}

bool PLEX_multiIncrementProtoExpandArray(const size_t by, PLEX_PROTOEXPANDARRAY* expandArray)
{
	if(expandArray == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ARRAY, PLEX_ERROR_EXPANDNULL, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_ONARRAYMULTIINCREMENTATION, PLEX_ERRORMESS_ARRAY);
		return false;
	}

	if(!PLEX_expandProtoExpandArray(expandArray->count + by, expandArray))
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ARRAY, PLEX_ERROR_EXPAND, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONARRAYMULTIINCREMENTATION, PLEX_ERRORMESS_ARRAYCONTENTS);
		return false;
	}

	expandArray->count += by;

	return true;
}

void PLEX_decrementProtoExpandArray(PLEX_PROTOEXPANDARRAY* expandArray)
{
	if(expandArray == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ARRAY, PLEX_ERROR_DECREMENTNULL, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_ONARRAYDECREMENTATION, PLEX_ERRORMESS_ARRAY);
		return;
	}

	if(expandArray->contents == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ARRAY, PLEX_ERROR_CONTENTCORRUPTION, PLEX_ERROR_SEVERITY_FATAL, PLEX_ERRORMESS_ONARRAYDECREMENTATION, PLEX_ERRORMESS_ARRAYCONTENTS);
		return;
	}

	if(!expandArray->count)
	{
		PLEX_error(PLEX_ERROR_CATEGORY_ARRAY, PLEX_ERROR_NOLEN, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONARRAYDECREMENTATION);
		return;
	}

	--expandArray->count;
}

void PLEX_removeProtoExpandArray(const size_t index, PLEX_PROTOEXPANDARRAY* expandArray)
{
	if(expandArray == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ARRAY, PLEX_ERROR_ISNULL, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_ONARRAYREMOVAL, PLEX_ERRORMESS_ARRAY);
		return;
	}

	if(expandArray->contents == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ARRAY, PLEX_ERROR_CONTENTCORRUPTION, PLEX_ERROR_SEVERITY_FATAL, PLEX_ERRORMESS_ONARRAYREMOVAL, PLEX_ERRORMESS_ARRAYCONTENTS);
		return;
	}

	if(index >= expandArray->count)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ARRAY, PLEX_ERROR_OVERLIMIT, PLEX_ERROR_SEVERITY_FATAL, PLEX_ERRORMESS_ONARRAYREMOVAL, PLEX_ERRORMESS_ARRAYCOUNT);
		return;
	}

	if(index == expandArray->count - 1)
	{
		PLEX_decrementProtoExpandArray(expandArray);
		return;
	}

	memmove(expandArray->contents + (index * expandArray->unitSize), expandArray->contents + ((index + 1) * expandArray->unitSize), expandArray->unitSize * (expandArray->count - index - 1));

	--expandArray->count;
}

void PLEX_clearCountProtoExpandArray(PLEX_PROTOEXPANDARRAY* expandArray)
{
	if(expandArray == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ARRAY, PLEX_ERROR_ISNULL, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_ONARRAYCLEARING, PLEX_ERRORMESS_ARRAY);
		return;
	}

	if(expandArray->contents == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ARRAY, PLEX_ERROR_CONTENTCORRUPTION, PLEX_ERROR_SEVERITY_IMPOSSIBLE, PLEX_ERRORMESS_ONARRAYCLEARING, PLEX_ERRORMESS_ARRAYCONTENTS);
		return;
	}

	if(!expandArray->count) return;

	memset(expandArray->contents, 0, expandArray->capacity);

	expandArray->count = 0;
}

bool PLEX_rearrangeProtoExpandArray(const size_t beg, const size_t beg2, const size_t len, PLEX_PROTOEXPANDARRAY* expandArray)
{
	size_t alen = len;

	size_t fbeg = 0;
	size_t fbeg2 = 0;
	size_t flen = 0;

	void* buffer = NULL;

	if(expandArray == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ARRAY, PLEX_ERROR_ISNULL, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_ONARRAYREARRANGING, PLEX_ERRORMESS_ARRAY);
		return false;
	}

	if(expandArray->contents == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ARRAY, PLEX_ERROR_CONTENTCORRUPTION, PLEX_ERROR_SEVERITY_IMPOSSIBLE, PLEX_ERRORMESS_ONARRAYREARRANGING, PLEX_ERRORMESS_ARRAYCONTENTS);
		return false;
	}

	if(beg >= expandArray->count - 1)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ARRAY, PLEX_ERROR_BEGINNINGOVER, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_ONARRAYREARRANGING, PLEX_ERRORMESS_ARRAY);
		return false;
	}

	if(beg2 >= expandArray->count - 1)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ARRAY, PLEX_ERROR_BEGINNINGOVER, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_ONARRAYREARRANGING, PLEX_ERRORMESS_ARRAY);
		return false;
	}

	if(!len)
	{
		PLEX_error(PLEX_ERROR_CATEGORY_ARRAY, PLEX_ERROR_NOLEN, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_ONARRAYREARRANGING);
		return false;
	}

	if(expandArray->memHook == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ARRAY, PLEX_ERROR_HOOKCORRUPTION, PLEX_ERROR_SEVERITY_IMPOSSIBLE, PLEX_ERRORMESS_ONARRAYREARRANGING, PLEX_ERRORMESS_ARRAYMEMHOOK);
		return false;
	}

	if(expandArray->freeHook == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ARRAY, PLEX_ERROR_HOOKCORRUPTION, PLEX_ERROR_SEVERITY_IMPOSSIBLE, PLEX_ERRORMESS_ONARRAYREARRANGING, PLEX_ERRORMESS_ARRAYFREEHOOK);
		return false;
	}

	if(beg > beg2)
	{
		if(beg + alen > expandArray->count - 1) alen = expandArray->count - beg - 1;
	}else{
		if(beg2 + alen > expandArray->count - 1) alen = expandArray->count - beg2 - 1;
	}

	fbeg = beg * expandArray->unitSize;
	fbeg2 = beg2 * expandArray->unitSize;
	flen = alen * expandArray->unitSize;

	buffer = expandArray->memHook(flen);

	if(buffer == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ARRAY, PLEX_ERROR_ALLOCFAIL, PLEX_ERROR_SEVERITY_FATAL, PLEX_ERRORMESS_ONARRAYREARRANGING, PLEX_ERRORMESS_NEWBUFFER);
		return false;
	}

	memcpy(buffer, expandArray->contents + fbeg, flen);
	memmove(expandArray->contents + fbeg, expandArray->contents + fbeg2, flen);
	memcpy(expandArray->contents + fbeg2, buffer, flen);

	expandArray->freeHook((void**)&buffer);

	return true;
}

bool PLEX_swapProtoExpandArray(const size_t beg, const size_t beg2, const size_t len, PLEX_PROTOEXPANDARRAY* expandArray, PLEX_PROTOEXPANDARRAY* expandArray2)
{
	size_t alen = len;

	size_t flen = 0;
	size_t flen2 = 0;

	size_t fbeg = 0;
	size_t fbeg2 = 0;

	void* buffer = NULL;
	void* buffer2 = NULL;

	if(expandArray == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ARRAY, PLEX_ERROR_ISNULL, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_ONARRAYSWAPPING, PLEX_ERRORMESS_ARRAY);
		return false;
	}

	if(expandArray2 == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ARRAY, PLEX_ERROR_ISNULL, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_ONARRAYSWAPPING2, PLEX_ERRORMESS_ARRAY);
		return false;
	}

	if(expandArray->contents == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ARRAY, PLEX_ERROR_CONTENTCORRUPTION, PLEX_ERROR_SEVERITY_IMPOSSIBLE, PLEX_ERRORMESS_ONARRAYSWAPPING, PLEX_ERRORMESS_ARRAYCONTENTS);
		return false;
	}

	if(expandArray2->contents == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ARRAY, PLEX_ERROR_CONTENTCORRUPTION, PLEX_ERROR_SEVERITY_IMPOSSIBLE, PLEX_ERRORMESS_ONARRAYSWAPPING, PLEX_ERRORMESS_ARRAYCONTENTS2);
		return false;
	}

	if(!len)
	{
		PLEX_error(PLEX_ERROR_CATEGORY_ARRAY, PLEX_ERROR_NOLEN, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONARRAYSWAPPING);
		return false;
	}

	if(beg >= expandArray->count - 1)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ARRAY, PLEX_ERROR_BEGINNINGOVER, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONARRAYSWAPPING, PLEX_ERRORMESS_ARRAYBEG1);
		return false;
	}

	if(beg >= expandArray2->count - 1)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ARRAY, PLEX_ERROR_BEGINNINGOVER, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONARRAYSWAPPING2, PLEX_ERRORMESS_ARRAYBEG1);
		return false;
	}

	if(beg2 >= expandArray->count - 1)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ARRAY, PLEX_ERROR_BEGINNINGOVER, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONARRAYSWAPPING, PLEX_ERRORMESS_ARRAYBEG2);
		return false;
	}

	if(beg2 >= expandArray2->count - 1)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ARRAY, PLEX_ERROR_BEGINNINGOVER, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONARRAYSWAPPING2, PLEX_ERRORMESS_ARRAYBEG2);
		return false;
	}

	if(expandArray->memHook == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ARRAY, PLEX_ERROR_HOOKCORRUPTION, PLEX_ERROR_SEVERITY_IMPOSSIBLE, PLEX_ERRORMESS_ONARRAYSWAPPING, PLEX_ERRORMESS_ARRAYMEMHOOK);
		return false;
	}

	if(expandArray->freeHook == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ARRAY, PLEX_ERROR_HOOKCORRUPTION, PLEX_ERROR_SEVERITY_IMPOSSIBLE, PLEX_ERRORMESS_ONARRAYSWAPPING, PLEX_ERRORMESS_ARRAYFREEHOOK);
		return false;
	}

	if(expandArray->contents == expandArray2->contents) // < not an error
	{
		PLEX_rearrangeProtoExpandArray(beg, beg2, len, expandArray);
		return false;
	}

	if(beg + alen > expandArray->count - 1) alen = expandArray->count - beg - 1;
	if(beg2 + alen > expandArray->count - 1) alen = expandArray->count - beg2 - 1;
	if(beg + alen > expandArray2->count - 1) alen = expandArray2->count - beg - 1;
	if(beg2 + alen > expandArray2->count - 1) alen = expandArray2->count - beg2 - 1;

	flen = alen * expandArray->unitSize;
	flen2 = alen * expandArray2->unitSize;

	fbeg = beg * expandArray->unitSize;
	fbeg2 = beg2 * expandArray2->unitSize;

	buffer = expandArray->memHook(flen);

	if(buffer == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ARRAY, PLEX_ERROR_ALLOCFAIL, PLEX_ERROR_SEVERITY_FATAL, PLEX_ERRORMESS_ONARRAYSWAPPING, PLEX_ERRORMESS_ARRAYBUFFER1);
		return false;
	}

	buffer2 = expandArray2->memHook(flen2);

	if(buffer2 == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ARRAY, PLEX_ERROR_ALLOCFAIL, PLEX_ERROR_SEVERITY_FATAL, PLEX_ERRORMESS_ONARRAYSWAPPING, PLEX_ERRORMESS_ARRAYBUFFER2);
		expandArray->freeHook((void**)&buffer);
		return false;
	}

	memcpy(buffer, expandArray->contents + fbeg, flen);
	memcpy(buffer2, expandArray2->contents + fbeg2, flen2);

	if(flen2 > flen)
	{
		memcpy(expandArray->contents + fbeg, buffer2, flen);
		memcpy(expandArray2->contents + fbeg2, buffer, flen);
	}else{
		memcpy(expandArray->contents + fbeg, buffer2, flen2);
		memcpy(expandArray2->contents + fbeg2, buffer, flen2);
	}

	expandArray->freeHook((void**)&buffer);
	expandArray->freeHook((void**)&buffer2);

	return true;
}

bool PLEX_cutProtoExpandArray(const size_t beg, const size_t len, PLEX_PROTOEXPANDARRAY* expandArray)
{
	size_t alen = len;

	size_t fbeg = 0;
	size_t flen = 0;
	size_t flen2 = 0;

	if(expandArray == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ARRAY, PLEX_ERROR_ISNULL, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_ONARRAYCUTTING, PLEX_ERRORMESS_ARRAY);
		return false;
	}

	if(expandArray->contents == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ARRAY, PLEX_ERROR_CONTENTCORRUPTION, PLEX_ERROR_SEVERITY_IMPOSSIBLE, PLEX_ERRORMESS_ONARRAYCUTTING, PLEX_ERRORMESS_ARRAYCONTENTS);
		return false;
	}

	if(!alen)
	{
		PLEX_error(PLEX_ERROR_CATEGORY_ARRAY, PLEX_ERROR_NOLEN, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONARRAYCUTTING);
		return false;
	}

	if(beg >= expandArray->count - 1)
	{
		PLEX_error(PLEX_ERROR_CATEGORY_ARRAY, PLEX_ERROR_BEGINNINGOVER, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONARRAYCUTTING);
		return false;
	}

	if(beg + alen >= expandArray->count) alen = expandArray->count - beg - 1;

	fbeg = beg * expandArray->unitSize;
	flen = alen * expandArray->unitSize;
	flen2 = (expandArray->count - (beg + alen)) * expandArray->unitSize;

	memmove(expandArray->contents + fbeg, expandArray->contents + fbeg + flen, flen2);

	if(alen + 1 < expandArray->count) expandArray->count -= alen + 1;
	else expandArray->count = 0;

	return true;
}

bool PLEX_reverseProtoExpandArray(const size_t beg, const size_t len, PLEX_PROTOEXPANDARRAY* expandArray)
{
	size_t alen = 0;
	size_t count = 0;

	size_t fbeg = 0;
	size_t flen = 0;

	char* buffer = NULL;

	if(expandArray == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ARRAY, PLEX_ERROR_ISNULL, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_ONARRAYREVERSING, PLEX_ERRORMESS_ARRAY);
		return false;
	}

	if(expandArray->contents == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ARRAY, PLEX_ERROR_CONTENTCORRUPTION, PLEX_ERROR_SEVERITY_IMPOSSIBLE, PLEX_ERRORMESS_ONARRAYREVERSING, PLEX_ERRORMESS_ARRAYCONTENTS);
		return false;
	}

	if(beg >= expandArray->count - 1)
	{
		PLEX_error(PLEX_ERROR_CATEGORY_ARRAY, PLEX_ERROR_BEGINNINGOVER, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONARRAYREVERSING);
		return false;
	}

	if(!len)
	{
		PLEX_error(PLEX_ERROR_CATEGORY_ARRAY, PLEX_ERROR_NOLEN, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONARRAYREVERSING);
		return false;
	}

	if(expandArray->memHook == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ARRAY, PLEX_ERROR_HOOKCORRUPTION, PLEX_ERROR_SEVERITY_IMPOSSIBLE, PLEX_ERRORMESS_ONARRAYREVERSING, PLEX_ERRORMESS_ARRAYMEMHOOK);
		return false;
	}

	if(expandArray->freeHook == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ARRAY, PLEX_ERROR_HOOKCORRUPTION, PLEX_ERROR_SEVERITY_IMPOSSIBLE, PLEX_ERRORMESS_ONARRAYREVERSING, PLEX_ERRORMESS_ARRAYFREEHOOK);
		return false;
	}

	if(beg + alen > expandArray->count - 1) alen = expandArray->count - beg - 1;

	fbeg = beg * expandArray->unitSize;
	flen = alen * expandArray->unitSize;

	buffer = expandArray->memHook(flen);

	if(buffer == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ARRAY, PLEX_ERROR_ALLOCFAIL, PLEX_ERROR_SEVERITY_FATAL, PLEX_ERRORMESS_ONARRAYREVERSING, PLEX_ERRORMESS_ARRAYBUFFER1);
		return false;
	}

	memcpy(buffer, expandArray->contents + fbeg, flen);

	count = beg + alen;

	for(size_t ze = beg; ze < count; ++ze)
	{
		const size_t it = ze - beg;
		const size_t last = count - ze;

		const size_t fit = it * expandArray->unitSize;
		const size_t flast = last * expandArray->unitSize;

		memcpy((char*)expandArray->contents + flast, buffer + fit, expandArray->unitSize);
	}

	expandArray->freeHook((void**)&buffer);

	return true;
}

bool PLEX_rearrangeProtoExpandArrayPerc(const double begPerc, const double begPerc2, const double lenPerc, PLEX_PROTOEXPANDARRAY* expandArray)
{
	size_t count = 0;

	double aBegPerc = begPerc;
	double aBegPerc2 = begPerc2;

	double aLenPerc = lenPerc;

	if(expandArray == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ARRAY, PLEX_ERROR_ISNULL, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_ONARRAYREARRANGINGPERC, PLEX_ERRORMESS_ARRAY);
		return false;
	}

	if(expandArray->contents == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ARRAY, PLEX_ERROR_CONTENTCORRUPTION, PLEX_ERROR_SEVERITY_IMPOSSIBLE, PLEX_ERRORMESS_ONARRAYREARRANGINGPERC, PLEX_ERRORMESS_ARRAYCONTENTS);
		return false;
	}

	if(begPerc >= 1.0)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ARRAY, PLEX_ERROR_BEGINNINGOVER, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONARRAYREARRANGINGPERC, PLEX_ERRORMESS_ARRAYBEG1);
		return false;
	}

	if(begPerc2 >= 1.0)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ARRAY, PLEX_ERROR_BEGINNINGOVER, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONARRAYREARRANGINGPERC, PLEX_ERRORMESS_ARRAYBEG2);
		return false;
	}

	if(begPerc <= 0)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ARRAY, PLEX_ERROR_BEGINNINGUNDER, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONARRAYREARRANGINGPERC, PLEX_ERRORMESS_ARRAYBEG1);
		aBegPerc = 0;
	}

	if(begPerc2 <= 0)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ARRAY, PLEX_ERROR_BEGINNINGUNDER, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONARRAYREARRANGINGPERC, PLEX_ERRORMESS_ARRAYBEG2);
		aBegPerc2 = 0;
	}

	if(lenPerc <= 0)
	{
		PLEX_error(PLEX_ERROR_CATEGORY_ARRAY, PLEX_ERROR_LENGTHUNDER, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONARRAYREARRANGINGPERC);
		return false;
	}

	if(lenPerc >= 1.0)
	{
		PLEX_error(PLEX_ERROR_CATEGORY_ARRAY, PLEX_ERROR_LENGTHOVER, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONARRAYREARRANGINGPERC);
		aLenPerc = 1;
	}

	count = expandArray->count;

	PLEX_rearrangeProtoExpandArray(aBegPerc * count, aBegPerc2 * count, aLenPerc * count, expandArray);

	return true;
}

bool PLEX_swapProtoExpandArrayPerc(const double begPerc, const double begPerc2, const double lenPerc, PLEX_PROTOEXPANDARRAY* expandArray, PLEX_PROTOEXPANDARRAY* expandArray2)
{
	size_t count = 0;

	double aBegPerc = begPerc;
	double aBegPerc2 = begPerc2;

	double aLenPerc = lenPerc;

	if(expandArray == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ARRAY, PLEX_ERROR_ISNULL, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_ONARRAYSWAPPINGPERC, PLEX_ERRORMESS_ARRAY);
		return false;
	}

	if(expandArray->contents == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ARRAY, PLEX_ERROR_CONTENTCORRUPTION, PLEX_ERROR_SEVERITY_IMPOSSIBLE, PLEX_ERRORMESS_ONARRAYSWAPPINGPERC, PLEX_ERRORMESS_ARRAYCONTENTS);
		return false;
	}

	if(expandArray2 == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ARRAY, PLEX_ERROR_ISNULL, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONARRAYSWAPPINGPERC2, PLEX_ERRORMESS_ARRAY);
		return false;
	}

	if(begPerc >= 1.0)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ARRAY, PLEX_ERROR_BEGINNINGOVER, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONARRAYSWAPPINGPERC, PLEX_ERRORMESS_ARRAYBEG1);
		return false;
	}

	if(begPerc2 >= 1.0)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ARRAY, PLEX_ERROR_BEGINNINGOVER, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONARRAYSWAPPINGPERC, PLEX_ERRORMESS_ARRAYBEG2);
		return false;
	}

	if(lenPerc <= 0)
	{
		PLEX_error(PLEX_ERROR_CATEGORY_ARRAY, PLEX_ERROR_LENGTHUNDER, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONARRAYSWAPPINGPERC);
		return false;
	}

	if(begPerc <= 0)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ARRAY, PLEX_ERROR_BEGINNINGOVER, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONARRAYSWAPPINGPERC, PLEX_ERRORMESS_ARRAYBEG1);
		aBegPerc = 0;
	}

	if(begPerc2 <= 0)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ARRAY, PLEX_ERROR_BEGINNINGOVER, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONARRAYSWAPPINGPERC, PLEX_ERRORMESS_ARRAYBEG2);
		aBegPerc2 = 0;
	}

	if(lenPerc >= 1.0)
	{
		PLEX_error(PLEX_ERROR_CATEGORY_ARRAY, PLEX_ERROR_LENGTHOVER, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONARRAYSWAPPINGPERC);
		aLenPerc = 1;
	}

	count = expandArray->count;

	PLEX_swapProtoExpandArray(aBegPerc * count, aBegPerc2 * count, aLenPerc * count, expandArray, expandArray2);

	return true;
}

bool PLEX_cutProtoExpandArrayPerc(const double begPerc, const double lenPerc, PLEX_PROTOEXPANDARRAY* expandArray)
{
	size_t count = 0;

	double aBegPerc = begPerc;

	double aLenPerc = lenPerc;

	if(expandArray == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ARRAY, PLEX_ERROR_ISNULL, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_ONARRAYCUTTINGPERC, PLEX_ERRORMESS_ARRAY);
		return false;
	}

	if(expandArray->contents == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ARRAY, PLEX_ERROR_CONTENTCORRUPTION, PLEX_ERROR_SEVERITY_IMPOSSIBLE, PLEX_ERRORMESS_ONARRAYCUTTINGPERC, PLEX_ERRORMESS_ARRAYCONTENTS);
		return false;
	}

	if(begPerc >= 1.0)
	{
		PLEX_error(PLEX_ERROR_CATEGORY_ARRAY, PLEX_ERROR_BEGINNINGOVER, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONARRAYCUTTINGPERC);
		return false;
	}

	if(lenPerc <= 0)
	{
		PLEX_error(PLEX_ERROR_CATEGORY_ARRAY, PLEX_ERROR_LENGTHUNDER, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONARRAYCUTTINGPERC);
		return false;
	}

	if(lenPerc >= 1.0)
	{
		PLEX_error(PLEX_ERROR_CATEGORY_ARRAY, PLEX_ERROR_LENGTHOVER, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONARRAYCUTTINGPERC);
		aLenPerc = 1;
	}

	if(begPerc <= 0)
	{
		PLEX_error(PLEX_ERROR_CATEGORY_ARRAY, PLEX_ERROR_BEGINNINGUNDER, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONARRAYCUTTINGPERC);
		aBegPerc = 0;
	}

	count = expandArray->count;

	PLEX_cutProtoExpandArray(aBegPerc * count, aLenPerc * count, expandArray);

	return true;
}

bool PLEX_reverseProtoExpandArrayPerc(const double begPerc, const double lenPerc, PLEX_PROTOEXPANDARRAY* expandArray)
{
	size_t count = 0;

	double aBegPerc = begPerc;
	double aLenPerc = lenPerc;

	if(expandArray == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ARRAY, PLEX_ERROR_ISNULL, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_ONARRAYREVERSINGPERC, PLEX_ERRORMESS_ARRAY);
		return false;
	}

	if(expandArray->contents == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ARRAY, PLEX_ERROR_CONTENTCORRUPTION, PLEX_ERROR_SEVERITY_IMPOSSIBLE, PLEX_ERRORMESS_ONARRAYREVERSINGPERC, PLEX_ERRORMESS_ARRAYCONTENTS);
		return false;
	}

	if(begPerc >= 1.0)
	{
		PLEX_error(PLEX_ERROR_CATEGORY_ARRAY, PLEX_ERROR_BEGINNINGOVER, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONARRAYREVERSINGPERC);
		return false;
	}

	if(lenPerc <= 0)
	{
		PLEX_error(PLEX_ERROR_CATEGORY_ARRAY, PLEX_ERROR_LENGTHUNDER, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONARRAYREVERSINGPERC);
		return false;
	}

	if(lenPerc >= 1.0)
	{
		PLEX_error(PLEX_ERROR_CATEGORY_ARRAY, PLEX_ERROR_LENGTHOVER, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONARRAYREVERSINGPERC);
		aLenPerc = 1;
	}

	if(begPerc <= 0)
	{
		PLEX_error(PLEX_ERROR_CATEGORY_ARRAY, PLEX_ERROR_BEGINNINGUNDER, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONARRAYREVERSINGPERC);
		aBegPerc = 0;
	}

	count = expandArray->count;

	PLEX_reverseProtoExpandArray(aBegPerc * count, aLenPerc * count, expandArray);

	return true;
}

void PLEX_setGrowthUnitProtoExpandArray(const size_t growthUnit, PLEX_PROTOEXPANDARRAY* expandArray)
{
	if(expandArray == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ARRAY, PLEX_ERROR_ZEROVALUE, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_ONGROWTHUNITSET, PLEX_ERRORMESS_ARRAY);
		return;
	}

	if(!growthUnit)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ARRAY, PLEX_ERROR_ZEROVALUE, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONGROWTHUNITSET, PLEX_ERRORMESS_ARRAYGROWTHUNIT);
		return;
	}

	expandArray->growthUnit = growthUnit;
}

void* PLEX_getProtoExpandArrayEntry(const size_t index, PLEX_PROTOEXPANDARRAY* expandArray)
{
	if(expandArray == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ARRAY, PLEX_ERROR_GETNULL, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_ONARRAYGETENTRY, PLEX_ERRORMESS_ARRAY);
		return NULL;
	}

	if(expandArray->contents == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ARRAY, PLEX_ERROR_CONTENTCORRUPTION, PLEX_ERROR_SEVERITY_IMPOSSIBLE, PLEX_ERRORMESS_ONARRAYGETENTRY, PLEX_ERRORMESS_ARRAYCONTENTS);
		return NULL;
	}

	if(index >= expandArray->count)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ARRAY, PLEX_ERROR_OVERLIMIT, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONARRAYGETENTRY, PLEX_ERRORMESS_ARRAYCOUNT);
		return NULL;
	}

	return expandArray->contents + (index * expandArray->unitSize);
}

void* PLEX_getProtoExpandArrayLast(PLEX_PROTOEXPANDARRAY* expandArray)
{
	if(expandArray == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ARRAY, PLEX_ERROR_GETNULL, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_ONARRAYGETENTRYLAST, PLEX_ERRORMESS_ARRAY);
		return NULL;
	}

	if(expandArray->contents == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ARRAY, PLEX_ERROR_CONTENTCORRUPTION, PLEX_ERROR_SEVERITY_IMPOSSIBLE, PLEX_ERRORMESS_ONARRAYGETENTRYLAST, PLEX_ERRORMESS_ARRAYCONTENTS);
		return NULL;
	}

	return PLEX_getProtoExpandArrayEntry(expandArray->count - 1, expandArray);
}

size_t PLEX_protoExpandArrayCount(const PLEX_PROTOEXPANDARRAY* expandArray)
{
	if(expandArray == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ARRAY, PLEX_ERROR_GETNULL, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_ONARRAYGETCOUNT, PLEX_ERRORMESS_ARRAY);
		return 0;
	}

	if(expandArray->contents == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ARRAY, PLEX_ERROR_CONTENTCORRUPTION, PLEX_ERROR_SEVERITY_IMPOSSIBLE, PLEX_ERRORMESS_ONARRAYGETCOUNT, PLEX_ERRORMESS_ARRAYCONTENTS);
		return 0;
	}

	return expandArray->count;
}

size_t PLEX_protoExpandArrayCapacity(const PLEX_PROTOEXPANDARRAY* expandArray)
{
	if(expandArray == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ARRAY, PLEX_ERROR_GETNULL, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_ONARRAYGETCAPACITY, PLEX_ERRORMESS_ARRAY);
		return 0;
	}

	if(expandArray->contents == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ARRAY, PLEX_ERROR_CONTENTCORRUPTION, PLEX_ERROR_SEVERITY_IMPOSSIBLE, PLEX_ERRORMESS_ONARRAYGETCAPACITY, PLEX_ERRORMESS_ARRAYCONTENTS);
		return 0;
	}

	return expandArray->capacity;
}

size_t PLEX_protoExpandArrayUnitSize(const PLEX_PROTOEXPANDARRAY* expandArray)
{
	if(expandArray == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ARRAY, PLEX_ERROR_GETNULL, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_ONARRAYGETUNITSIZE, PLEX_ERRORMESS_ARRAY);
		return 0;
	}

	if(expandArray->contents == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ARRAY, PLEX_ERROR_CONTENTCORRUPTION, PLEX_ERROR_SEVERITY_IMPOSSIBLE, PLEX_ERRORMESS_ONARRAYGETUNITSIZE, PLEX_ERRORMESS_ARRAYCONTENTS);
		return 0;
	}

	return expandArray->unitSize;
}

size_t PLEX_protoExpandArrayGrowthUnit(const PLEX_PROTOEXPANDARRAY* expandArray)
{
	if(expandArray == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ARRAY, PLEX_ERROR_GETNULL, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_ONARRAYGETGROWTHUNIT, PLEX_ERRORMESS_ARRAY);
		return 0;
	}

	if(expandArray->contents == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ARRAY, PLEX_ERROR_CONTENTCORRUPTION, PLEX_ERROR_SEVERITY_IMPOSSIBLE, PLEX_ERRORMESS_ONARRAYGETGROWTHUNIT, PLEX_ERRORMESS_ARRAYCONTENTS);
		return 0;
	}

	return expandArray->growthUnit;
}

size_t PLEX_protoExpandArraySize(const PLEX_PROTOEXPANDARRAY* expandArray)
{
	if(expandArray == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ARRAY, PLEX_ERROR_GETNULL, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_ONARRAYGETSIZE, PLEX_ERRORMESS_ARRAY);
		return 0;
	}

	if(expandArray->contents == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ARRAY, PLEX_ERROR_CONTENTCORRUPTION, PLEX_ERROR_SEVERITY_IMPOSSIBLE, PLEX_ERRORMESS_ONARRAYGETSIZE, PLEX_ERRORMESS_ARRAYCONTENTS);
		return 0;
	}

	return expandArray->count * expandArray->unitSize;
}

size_t PLEX_protoExpandArrayCapacitySize(const PLEX_PROTOEXPANDARRAY* expandArray)
{
	if(expandArray == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ARRAY, PLEX_ERROR_ZEROVALUE, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONARRAYGETCAPACITYSIZE, PLEX_ERRORMESS_ARRAYGROWTHUNIT);
		return 0;
	}

	if(expandArray->contents == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ARRAY, PLEX_ERROR_CONTENTCORRUPTION, PLEX_ERROR_SEVERITY_FATAL, PLEX_ERRORMESS_ONARRAYGETCAPACITYSIZE, PLEX_ERRORMESS_ARRAYCONTENTS);
		return 0;
	}

	return expandArray->capacity * expandArray->unitSize;
}

void* PLEX_protoExpandArrayContents(const PLEX_PROTOEXPANDARRAY* expandArray)
{
	if(expandArray == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ARRAY, PLEX_ERROR_GETNULL, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_ONARRAYGECONTENT, PLEX_ERRORMESS_ARRAY);
		return NULL;
	}

	if(expandArray->contents == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_ARRAY, PLEX_ERROR_CONTENTCORRUPTION, PLEX_ERROR_SEVERITY_IMPOSSIBLE, PLEX_ERRORMESS_ONARRAYGECONTENT, PLEX_ERRORMESS_ARRAYCONTENTS);
		return NULL;
	}

	return expandArray->contents;
}
