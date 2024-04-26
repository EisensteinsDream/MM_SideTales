#ifndef ROR_SYSTEMREAD_WIN_C_H
#define ROR_SYSTEMREAD_WIN_C_H

bool RoR_startSystemRead()
{

}

bool RoR_updateSystemRead()
{

}

bool RoR_endSystemRead()
{

}

RoR_STRING* RoR_readOSName()
{
	REGTURN(RoR_genString("Wendoes!"));
}

RoR_STRING* RoR_readOSVersion()
{
	REGTURN(RoR_genString("666"));
}

RoR_STRING* RoR_readSystemName()
{
	REGTURN(RoR_genString("Super Dave Osbourne"));
}

RoR_STRING* RoR_readAssemblyVersion()
{
	REGTURN(RoR_genString("Hardcore Supercomputer"));
}

size_t RoR_readSystemRamUsage()
{

}

RoR_STRING* RoR_readCPUInfo(const size_t index)
{

}

RoR_STRING* RoR_readCPUMHZ(const size_t index)
{

}

size_t RoR_cpuCount(){ return 0; }

double RoR_cpuAvg(){ return 0; }

double RoR_cpuUsagePerc(){ return 0; }

#endif
