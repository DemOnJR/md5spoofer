#include <Windows.h>
#include "main.h"
#include "plucoresdk.h"
#include "global.h"
#include "utils.h"

void Cmd_MD5_f()
{
	int argCount = Engine.Cmd_Argc();
	if (argCount < 2)
		return Engine.Con_Printf("Syntax: %s <FileName>\n", Engine.Cmd_Argv(0));

	char *szfileName = Engine.Cmd_Argv(1);
	char fileName[MAX_PATH];

	sprintf(fileName, "..\\%s", szfileName);

	md5_hash_t hash;
	memset(hash, 0, sizeof(hash));
	orgMD5_Hash_File(&hash, fileName, false, false, nullptr);

	char *calcStatus;

	if (*(int *)hash == 0)
		calcStatus = "not calculated";
	else
		calcStatus = "calculated";
	
	Engine.Con_Printf("-- MD5 for \"%s\" is %s. --\n", szfileName, calcStatus);
	Engine.Con_Printf("%08X %08X %08X %08X\n", _byteswap_ulong(hash[0]), _byteswap_ulong(hash[1]), _byteswap_ulong(hash[2]), _byteswap_ulong(hash[3]));
	Engine.Con_Printf("%s \"%s\" \"%i\"\n", szConsistency, fileName, _byteswap_ulong(hash[0]));
}

void Cmd_Consistency_f()
{
	int argCount = Engine.Cmd_Argc();
	if (argCount < 2)
		return Engine.Con_Printf("Syntax: %s <FileName> <Checksum>\nUse \"list\" filename to get current consistecy list.\n", Engine.Cmd_Argv(0));

	char *szFileName = Engine.Cmd_Argv(1);
	if (!stricmp(szFileName, "list"))
	{
		if (spoofList.length == 0)
			return Engine.Con_Printf("Consistency list is empty.\n");;

		Engine.Con_Printf("-- Consistency list --\n");
		for (int i = 0; i < spoofList.length; i++)
		{
			spoof_t *spoof = &((spoof_t *)spoofList.data)[i];
			Engine.Con_Printf("%i. %s - %i\n", i + 1, spoof->name, spoof->sum);
		}

		return;
	}

	if (argCount < 3)
		return Engine.Con_Printf("Please, specify \"Checksum\" argument.\n");

	char *szSum = Engine.Cmd_Argv(2);

	int sum;
	if (!TryStrToInt(szSum, sum))
		return Engine.Con_Printf("Invalid \"Checksum\" value: %s.\n", szSum);

	char fileName[MAX_PATH];
	char procName[MAX_PATH];

	GetModuleFileName(GetModuleHandle(nullptr), procName, sizeof(procName));
	char *p = strrchr(procName, '\\');
	if (p != nullptr)
		*p = '\0';

	sprintf_s(fileName, "%s\\%s\\%s", procName, gameDir, szFileName);
	FormatFilePath(fileName);

	for (int i = 0; i < spoofList.length; i++)
	{
		spoof_t *spoof = &((spoof_t *)spoofList.data)[i];

		if (!stricmp(fileName, spoof->name))
		{
			free(spoof->name);

			spoof->name = _strdup(fileName);
			spoof->sum = sum;
			return;
		}
	}

	spoof_t spoof;
	spoof.name = _strdup(fileName);
	spoof.sum = sum;

	idynarr.DynArrPush(spoofList, &spoof);
}