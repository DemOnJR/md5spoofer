#include <Windows.h>
#include "sdk.h"
#include "plucoresdk.h"
#include "main.h"
#include "utils.h"
#include "cmds.h"
#include "search.h"
#include "funcs.h"
#include "global.h"

void WINAPI InjectMain(HINSTANCE inst, pfnFindInterface pfn, plugininfo_t &info)
{
	pcrInst = inst;
	FindInterface = pfn;
	info = pluginInfo;
}

void WINAPI InjectRecords(cl_enginefunc_t* pEngine, cl_clientfunc_t* pClient, engine_studio_api_t* pStudio, enginefuncs_t * pServer)
{
	memcpy(&Engine, pEngine, sizeof(Engine));
	memcpy(&Client, pClient, sizeof(Client));
	memcpy(&Studio, pStudio, sizeof(Studio));
	memcpy(&Server, pServer, sizeof(Server));

	memcpy(&irenderer, FindInterface("IRenderer"), sizeof(irenderer));
	memcpy(&imemory, FindInterface("IMemory"), sizeof(imemory));
	memcpy(&icvar, FindInterface("ICVar"), sizeof(icvar));
	memcpy(&icmd, FindInterface("ICmd"), sizeof(icmd));
	memcpy(&idynarr, FindInterface("IDynArr"), sizeof(idynarr));

	irenderer.GetRendererInfo(&hlBase, &hlSize, nullptr);

	if (!Find_CL_SendConsistencyInfo())
		return;

	if (!Find_MD5_Hash_File())
		return;

	gameDir = (char *)Engine.pfnGetGameDirectory();

	imemory.InsertCall(MD5FileHashPos, hkMD5_Hash_File);

	cvar_md5_dump = Engine.pfnRegisterVariable(szConsistencyLog, "0", 0);
	Engine.pfnAddCommand(szConsistency, Cmd_Consistency_f);
	Engine.pfnAddCommand(szMD5, Cmd_MD5_f);
}

void WINAPI InjectUnload()
{
	imemory.InsertCall(MD5FileHashPos, orgMD5_Hash_File);
	icvar.DeleteCVar(szConsistencyLog);
	icmd.DeleteCmd(szConsistency);
	icmd.DeleteCmd(szMD5);
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved)
{
	switch (fdwReason)
	{
	case DLL_PROCESS_ATTACH:

		char mutex[64];
		sprintf_s(mutex, "PlucorePlugin%i", hinstDLL);

		if (CreateMutex(nullptr, false, mutex) == (HANDLE)ERROR_ALREADY_EXISTS)
			return true;

		break;
	}

	return true;
}