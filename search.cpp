#include "global.h"

bool Find_CL_SendConsistencyInfo()
{
	void *addr = imemory.FindPushOffset(hlBase, hlSize, "begin CL_RegisterResources()");

	if (addr == nullptr)
		return false;

	orgCL_SendConsistencyInfo = (pfnCL_SendConsistencyInfo)imemory.FindCallEx(addr, 3, false);
	return true;
}

bool Find_MD5_Hash_File()
{
	void *addr = imemory.FindLongPtr(orgCL_SendConsistencyInfo, 1024, 0x006A006A, 0, false);
	if (addr == nullptr)
		return false;

	MD5FileHashPos = imemory.FindCall(addr, 1, 0, false);
	orgMD5_Hash_File = (pfnMD5_Hash_File)imemory.FindCallEx(MD5FileHashPos, 1, false);
	return true;
}