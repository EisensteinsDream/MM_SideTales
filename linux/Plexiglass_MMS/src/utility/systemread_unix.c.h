#ifndef ROR_SYSTEMREAD_UNIX_C_H
#define ROR_SYSTEMREAD_UNIX_C_H

static bool p_SystemReadInit = false;

static PLEX_STACK* p_CPUData = NULL;

static PLEX_FAKESTRING_50 p_SysStr_processor = NULL;
static PLEX_FAKESTRING_50 p_SysStr_vendor = NULL;
static PLEX_FAKESTRING_50 p_SysStr_model = NULL;
static PLEX_FAKESTRING_50 p_SysStr_mhz = NULL;
static PLEX_FAKESTRING_50 p_SysStr_cache = NULL;
static PLEX_FAKESTRING_50 p_SysStr_coreID = NULL;

static double p_MHzAvg = 0;

static PLEX_FILE* p_getProcFile(const char* const name)
{
	const size_t pid = getpid();

	PLEX_FILE* ret = NULL;

	PLEX_FAKESTRING_50 procString = PLEX_genString("/proc/");
	PLEX_FAKESTRING_50 pidString = PLEX_genStringFromNumber(pid, 0);
	PLEX_FAKESTRING_50 statusString = PLEX_genString(name);

	PLEX_concat(pidString, procString);
	PLEX_concat(statusString, procString);

	ret = PLEX_genFile((char*)procString->contents, true, false, false);

	PLEX_destroyString(&procString);
	PLEX_destroyString(&pidString);
	PLEX_destroyString(&statusString);

	return ret;
}

static PLEX_FILE* p_getProcStatusFile()
{
	return p_getProcFile("/status");
}

static PLEX_FILE* p_getProcStatFile()
{
	return p_getProcFile("/stat");
}

static bool p_procStatusIterate(PLEX_FILE* file, PLEX_FAKESTRING_50* heading, PLEX_FAKESTRING_50* contents)
{
	int32_t next = PLEX_readByteFromFile(file);

	bool isCont = false;

	while(next != EOF && next != '\n' && next != '\r')
	{
		if(next != ':')
		{
			if(isCont) PLEX_concatChar(next, *contents);
			else PLEX_concatChar(next, *heading);
		}

		if(next == ':') isCont = true;

		next = PLEX_readByteFromFile(file);
	}

	PLEX_trim(*contents);
	PLEX_trim(*heading);

	return next != EOF;
}

bool PLEX_startSystemRead()
{
	if(p_SystemReadInit)
	{
		// error
		return true;
	}

	p_SysStr_processor = PLEX_genString("processor");
	p_SysStr_vendor = PLEX_genString("vendor_id");
	p_SysStr_model = PLEX_genString("model name");
	p_SysStr_mhz = PLEX_genString("cpu MHz");
	p_SysStr_cache = PLEX_genString("cache size");
	p_SysStr_coreID = PLEX_genString("core id");

	p_CPUData = PLEX_genStack(5, 10);

	p_SystemReadInit = true;

	return true;
}

bool PLEX_updateSystemRead()
{
	bool find = true;

	size_t count = 0;

	PLEX_FILE* cpuinfo = PLEX_genFile("/proc/cpuinfo", true, false, false);

	PLEX_clearStack(p_CPUData);

	p_MHzAvg = 0;

	while(find)
	{
		PLEX_FAKESTRING_50 heading = PLEX_genString("");
		PLEX_FAKESTRING_50 contents = PLEX_genString("");

		find = p_procStatusIterate(cpuinfo, &heading, &contents);

		if(PLEX_compString(heading, p_SysStr_processor))
		{
			PLEX_CPU_Info newCpuInfo;

			memset(&newCpuInfo, 0, sizeof(newCpuInfo));

			newCpuInfo.processorNum = atoi((char*)contents->contents);

			PLEX_addStack(&newCpuInfo, sizeof(newCpuInfo), p_CPUData);
		}

		if(PLEX_compString(heading, p_SysStr_vendor))
		{
			if(!PLEX_countStack(p_CPUData))
			{
				// error
				break;
			}

			PLEX_CPU_Info* last = PLEX_getLastStack(p_CPUData);

			if(last->vendor != NULL)
			{
				// error
				PLEX_destroyString(&last->vendor);
			}

			last->vendor = PLEX_copyString(contents);
		}

		if(PLEX_compString(heading, p_SysStr_model))
		{
			if(!PLEX_countStack(p_CPUData))
			{
				// error
				break;
			}

			PLEX_CPU_Info* last = PLEX_getLastStack(p_CPUData);

			if(last->model != NULL)
			{
				// error
				PLEX_destroyString(&last->model);
			}

			last->model = PLEX_copyString(contents);
		}

		if(PLEX_compString(heading, p_SysStr_mhz))
		{
			if(!PLEX_countStack(p_CPUData))
			{
				// error
				break;
			}

			PLEX_CPU_Info* last = PLEX_getLastStack(p_CPUData);

			last->mhz = strtod((char*)contents->contents, NULL);

			p_MHzAvg += last->mhz;
		}

		if(PLEX_compString(heading, p_SysStr_cache))
		{
			size_t prec = 1;
			size_t numIt = 0;
			char numString[contents->len + 1];

			PLEX_CPU_Info* last = PLEX_getLastStack(p_CPUData);

			numString[contents->len] = '\0';

			for(size_t z = 0; z < contents->len; ++z)
			{
				const uint8_t next = contents->contents[z];

				numString[z] = '\0';

				if(next >= '0' && next <= '9')
				{
					numString[numIt] = next;
					numIt++;
				}

				switch(next)
				{
					case 'k':
					case 'K':
						prec = 1024;
					break;

					case 'm':
					case 'M':
						prec = 1048576;
					break;

					case 'g':
					case 'G':
						prec = 1073741824;
					break;
				};
			}

			last->cache = strtol(numString, NULL, 10) * prec;
		}

		if(PLEX_compString(heading, p_SysStr_coreID))
		{
			if(!PLEX_countStack(p_CPUData))
			{
				// error
				break;
			}

			PLEX_CPU_Info* last = PLEX_getLastStack(p_CPUData);

			last->coreID = atoi((char*)contents->contents);
		}

		PLEX_destroyString(&heading);
		PLEX_destroyString(&contents);
	}

	count = PLEX_cpuCount();

	if(count) p_MHzAvg /= count;

	PLEX_destroyFile(&cpuinfo);

	return true;
}

bool PLEX_endSystemRead()
{
	if(!p_SystemReadInit)
	{
		// error
		return true;
	}

	for(size_t z = 0; z < PLEX_countStack(p_CPUData); ++z)
	{
		PLEX_CPU_Info* next = PLEX_getStack(z, p_CPUData);

		PLEX_destroyString(&next->vendor);
		PLEX_destroyString(&next->model);
	}

	PLEX_destroyString(&p_SysStr_processor);
	PLEX_destroyString(&p_SysStr_vendor);
	PLEX_destroyString(&p_SysStr_model);
	PLEX_destroyString(&p_SysStr_mhz);
	PLEX_destroyString(&p_SysStr_cache);
	PLEX_destroyString(&p_SysStr_coreID);

	PLEX_destroyStack(&p_CPUData);

	p_SystemReadInit = false;

	return true;
}

PLEX_FAKESTRING_50 PLEX_readOSName()
{
	struct utsname name;

	uname(&name);

	return PLEX_genString(name.sysname);
}

PLEX_FAKESTRING_50 PLEX_readOSVersion()
{
	struct utsname name;

	uname(&name);

	return PLEX_genString(name.release);
}

PLEX_FAKESTRING_50 PLEX_readSystemName()
{
	struct utsname name;

	uname(&name);

	return PLEX_genString(name.nodename);
}

PLEX_FAKESTRING_50 PLEX_readAssemblyVersion()
{
	struct utsname name;

	uname(&name);

	return PLEX_genString(name.machine);
}

PLEX_FAKESTRING_50 PLEX_readCPUInfo(const size_t index)
{
	PLEX_CPU_Info* target = PLEX_getStack(index, p_CPUData);

	PLEX_FAKESTRING_50 ret = NULL;

	if(target == NULL)
	{
		// error
		return NULL;
	}

	PLEX_printBufferFormatted("%zu (Core ID: %zu), %s, %s, %zu cache (bytes)", target->processorNum, target->coreID, (char*)target->model->contents, (char*)target->vendor->contents, target->cache);

	ret = PLEX_genString(PLEX_getCurrentBuffer());

	return ret;
}

PLEX_FAKESTRING_50 PLEX_readCPUMHZ(const size_t index)
{
	PLEX_CPU_Info* target = PLEX_getStack(index, p_CPUData);

	PLEX_FAKESTRING_50 ret = NULL;

	if(target == NULL)
	{
		// error
		return NULL;
	}

	PLEX_printBufferFormatted("%zu (Core ID: %zu), %lf Mhz", target->processorNum, target->coreID, target->mhz);

	ret = PLEX_genString(PLEX_getCurrentBuffer());

	return ret;
}

size_t PLEX_readSystemRamUsage()
{
	size_t ret = 0;

	PLEX_FILE* file = p_getProcStatusFile();

	bool find = true;

	PLEX_FAKESTRING_50 vmSize = PLEX_genString("VmSize");

	while(find)
	{
		PLEX_FAKESTRING_50 heading = PLEX_genString("");
		PLEX_FAKESTRING_50 contents = PLEX_genString("");

		find = p_procStatusIterate(file, &heading, &contents);

		while(find)
		{
			if(PLEX_compString(heading, vmSize))
			{
				size_t prec = 1;
				size_t numIt = 0;
				char numString[contents->len + 1];

				find = false;

				numString[contents->len] = '\0';

				for(size_t z = 0; z < contents->len; ++z)
				{
					const uint8_t next = contents->contents[z];

					numString[z] = '\0';

					if(next >= '0' && next <= '9')
					{
						numString[numIt] = next;
						numIt++;
					}

					switch(next)
					{
						case 'k':
						case 'K':
							prec = 1024;
						break;

						case 'm':
						case 'M':
							prec = 1048576;
						break;

						case 'g':
						case 'G':
							prec = 1073741824;
						break;
					};
				}

				ret = strtol(numString, NULL, 10) * prec;

				break;
			}

			PLEX_destroyString(&heading);
			PLEX_destroyString(&contents);

			heading = PLEX_genString("");
			contents = PLEX_genString("");

			find = p_procStatusIterate(file, &heading, &contents);
		}

		PLEX_destroyString(&heading);
		PLEX_destroyString(&contents);
	}

	PLEX_destroyString(&vmSize);

	PLEX_destroyFile(&file);

	return ret;
}

size_t PLEX_cpuCount(){ return PLEX_countStack(p_CPUData); }

double PLEX_cpuAvg(){ return p_MHzAvg; }

double PLEX_cpuUsagePerc()
{
	PLEX_FILE* file = p_getProcStatFile();

	PLEX_FAKESTRING_50 token = NULL;

	size_t entNumber = 0;

	int32_t next = 0;

	double utime = 0;
	double ktime = 0;
	double cutime = 0;
	double cktime = 0;
	double startTime = 0;

	if(file == NULL)
	{
		// error
		return 0;
	}

	token = PLEX_genString("");

	next = PLEX_readByteFromFile(file);

	while(true)
	{
		if(next == ' ' || next == '\t' || next == '\r' || next == '\n' || next == EOF)
		{
			switch(entNumber)
			{
				case 14: // < user time
					utime = strtod((char*)token->contents, NULL);
				break;

				case 15: // < kernel time
					ktime = strtod((char*)token->contents, NULL);
				break;

				case 16: // < child process user time
					cutime = strtod((char*)token->contents, NULL);
				break;

				case 17: // < child process kernel time
					cktime = strtod((char*)token->contents, NULL);
				break;

				case 18: // < start time
					startTime = strtod((char*)token->contents, NULL);
				break;

				default: break;
			};

			PLEX_destroyString(&token);
			token = PLEX_genString("");
			entNumber++;

			if(next == EOF || entNumber == 45) break;
		}else PLEX_concatChar(next, token);

		next = PLEX_readByteFromFile(file);
	}

	PLEX_destroyString(&token);

	PLEX_destroyFile(&file);

	return 0;
}

#endif
