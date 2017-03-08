#include "sdk.h"
#include "plucoresdk.h"
#include "global.h"

cl_enginefunc_t Engine;
cl_clientfunc_t Client;
engine_studio_api_t Studio;
enginefuncs_t Server;

PVOID hlBase;
DWORD hlSize;

HINSTANCE pcrInst;
pfnFindInterface FindInterface;

interface_renderer_t irenderer;
interface_memory_t imemory;
interface_cvar_t icvar;
interface_dynarr_t idynarr;
interface_cmd_t icmd;

plugininfo_t pluginInfo = {
	"MD5Spoofer",
	"2010kohtep",
	"1.0.0",
	"Plugin, which grants ability to make files hidden from server."
};

pfnCL_SendConsistencyInfo orgCL_SendConsistencyInfo = nullptr;
void *MD5FileHashPos = nullptr;
pfnMD5_Hash_File orgMD5_Hash_File = nullptr;
cvar_s *cvar_md5_dump = nullptr;
char *gameDir;
dynarr_t spoofList = { sizeof(spoof_t) };

char *szConsistency = "consistency";
char *szConsistencyLog = "consistency_log";
char *szMD5 = "md5";