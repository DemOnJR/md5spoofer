#pragma once

#include "sdk.h"
#include "plucoresdk.h"

extern cl_enginefunc_t Engine;
extern cl_clientfunc_t Client;
extern engine_studio_api_t Studio;
extern enginefuncs_t Server;

extern PVOID hlBase;
extern DWORD hlSize;

extern HINSTANCE pcrInst;
extern pfnFindInterface FindInterface;

extern interface_renderer_t irenderer;
extern interface_memory_t imemory;
extern interface_cvar_t icvar;
extern interface_dynarr_t idynarr;
extern interface_cmd_t icmd;

extern plugininfo_t pluginInfo;
typedef int md5_hash_t[4];

typedef void(__cdecl *pfnCL_SendConsistencyInfo)(sizebuf_t *buf);
typedef bool(__cdecl *pfnMD5_Hash_File)(md5_hash_t *hash, char *name, bool useFOpen, bool useSeed, md5_hash_t *seed);

extern pfnCL_SendConsistencyInfo orgCL_SendConsistencyInfo;
extern pfnMD5_Hash_File orgMD5_Hash_File;

bool Find_CL_SendConsistencyInfo();
bool Find_MD5_Hash_File();

typedef struct spoof_s
{
	char *name;
	int sum;
} spoof_t;

extern void *MD5FileHashPos;
extern pfnMD5_Hash_File orgMD5_Hash_File;
extern cvar_s *cvar_md5_dump;
extern char *gameDir;
extern dynarr_t spoofList;

extern char *szConsistency;
extern char *szConsistencyLog;
extern char *szMD5;