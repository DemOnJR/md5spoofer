#include "main.h"
#include "utils.h"
#include "global.h"

bool __cdecl hkMD5_Hash_File(md5_hash_t *hash, char *name, bool useFOpen, bool useSeed, md5_hash_t *seed)
{
	if (cvar_md5_dump->value > 0.0f)
		Engine.Con_Printf("Consistency request: %s\n", name);

	char fileName[MAX_PATH];
	char procName[MAX_PATH];

	GetModuleFileName(GetModuleHandle(nullptr), procName, sizeof(procName));
	char *p = strrchr(procName, '\\');
	if (p != nullptr)
		*p = '\0';

	p = fileName;
	sprintf(p, "%s\\%s\\%s", procName, gameDir, name);
	FormatFilePath(p);

	for (int i = 0; i < spoofList.length; i++)
	{
		spoof_t *spoof = &((spoof_t *)spoofList.data)[i];

		if (!stricmp(p, spoof->name))
			if (spoof->sum == 0)
				return false;
			else
			{
				*(unsigned int *)hash = spoof->sum;
				return true;
			}
	}

	return orgMD5_Hash_File(hash, name, useFOpen, useSeed, seed);
}