#pragma once

#include <Windows.h>
#include "sdk.h"

#define FASTCALL __fastcall

typedef struct plugininfo_s{
	/* Plugin name visible to Plucore */
	char *name;
	/* Plugin Developer */
	char *author;
	/* Plugin Version */
	char *version;
	/* Plugin Description */
	char *description;
} *pplugininfo_s, plugininfo_t;

typedef struct plugininfoex_s{
	/* Plugin name visible to Plucore */
	char *name;
	/* Plugin Developer */
	char *author;
	/* Plugin Version */
	char *version;
	/* Plugin Description */
	char *description;
	/* Plugin Handle */
	HMODULE handle;
} *pplugininfoex_s, plugininfoex_t;

typedef struct dynarr_s
{
	int cellsize;
	int length;
	void *data;
} dynarr_t;

/** Working with interfaces **/
/*
/* Plucore suggests interfaces for its users. Interfaces are
/* sets of functions, which plugin can use for its work.
/* This way IMemory module lets use memory effectively, and module
/* IRenderer outputs different information about renderer (hw.dll, sw.dll) of game engine, particularly the address
/* of library which is loaded into memory, its size and build version of the game engine.
/*
/* To use interface you need to get it. For that we use
/* FindInterface, which is retrieved with InjectMain. We transfer interface name
/* there, which we want to get. List of interfaces will be attached to structures,
/* which are described below.
/*
/** **/

/* IMemory */
typedef struct interface_memory_s{
	/* Structure version. Every functions addition or reduction will cause
	this number's increment. */
	int version;

	/** Find closest function call (or jump) position, beginning from start address **/

	/* Functions with the ending of Ex are doing the same, but pass
	not the nearest function call, and the address of the function itself as a result */
	PVOID(WINAPI *FindCall)(PVOID start, DWORD number, int offset, bool back);
	PVOID(WINAPI *FindCallEx)(PVOID start, DWORD number, bool back);
	PVOID(WINAPI *FindJump)(PVOID start, DWORD number, int offset, bool back);
	PVOID(WINAPI *FindJumpEx)(PVOID start, DWORD number, bool back);
	/** **/

	/* Record function call to address */
	void(WINAPI *InsertCall)(PVOID addr, PVOID call);
	/* Record jump on function to address */
	void(WINAPI *InsertJump)(PVOID addr, PVOID jmp);

	/* Record data to adress */
	PVOID(WINAPI *WriteByte)(PVOID addr, BYTE value);
	PVOID(WINAPI *WriteWord)(PVOID addr, WORD value);
	PVOID(WINAPI *WriteLong)(PVOID addr, DWORD value);
	PVOID(WINAPI *WriteFloat)(PVOID addr, float value);
	PVOID(WINAPI *WriteDouble)(PVOID addr, double value);
	PVOID(WINAPI *WriteBuffer)(PVOID addr, PVOID buffer, DWORD size);
	PVOID(WINAPI *WriteCStr)(PVOID addr, char *value);

	/** Find data, beginning from "start" address **/

	/* back - search direction (true - backward, false - forward) */
	/* offset - offset the retrieved address on the specified number of bytes */
	PVOID(WINAPI *FindBytePtr)(PVOID start, DWORD size, BYTE value, int offset, bool back);
	PVOID(WINAPI *FindWordPtr)(PVOID start, DWORD size, WORD value, int offset, bool back);
	PVOID(WINAPI *FindLongPtr)(PVOID start, DWORD size, DWORD value, int offset, bool back);
	PVOID(WINAPI *FindPattern)(PVOID start, DWORD len, PVOID pattern, DWORD patternSize, int offset);
	PVOID(WINAPI *FindRefJump)(PVOID start, DWORD size, PVOID addr);
	PVOID(WINAPI *FindRefCall)(PVOID start, DWORD size, PVOID addr);
	/* Find a pointer to a code section, where szStr string is used */
	PVOID(WINAPI *FindPushOffset)(PVOID start, DWORD size, char *szStr);

	/** **/

	/** Intercept all calls or jumps to functions **/

	/* oldFunc - a pointer to a function that will be intercepted */
	/* newFunc - a pointer to a function, which will be installed instead of the intercepted function */
	DWORD(WINAPI *HookRefCalls)(PVOID start, DWORD size, PVOID oldFunc, PVOID newFunc);
	DWORD(WINAPI *HookRefJumps)(PVOID start, DWORD size, PVOID oldFunc, PVOID newFunc);

	/** **/

	DWORD(WINAPI *GetModuleSize)(PVOID addr);

	bool(WINAPI *CheckByte)(PVOID addr, BYTE value, int offset);
	bool(WINAPI *CheckWord)(PVOID addr, WORD value, int offset);
	bool(WINAPI *CheckLong)(PVOID addr, DWORD value, int offset);

	PVOID(WINAPI *Absolute)(PVOID addr);
	PVOID(WINAPI *Relative)(PVOID to, PVOID from);
	PVOID(WINAPI *Transpose)(PVOID addr, int offset);

	PIMAGE_SECTION_HEADER(WINAPI *GetSectionPtr)(HMODULE module, char *name);
	DWORD(WINAPI *GetSectionSize)(HMODULE module, char *name);

	bool(WINAPI *Bounds)(PVOID start, PVOID end, PVOID addr);
} *pinterface_memory_s, interface_memory_t;

/* ICmd */
typedef struct interface_cmd_s{
	int version;

	cmd_t*(WINAPI *FindCmd)(char *name);
	pfnCallback(WINAPI *HookCmd)(char *name, pfnCallback callback);
	void(WINAPI *DeleteCmd)(char *name);
} *pinterface_cmd_s, interface_cmd_t;

/* ICVar */
typedef struct interface_cvar_s{
	int version;

	cvar_t*(WINAPI *FindCVar)(char *name);
	void(WINAPI *DeleteCVar)(char *name);
} *pinterface_cvar_s, interface_cvar_t;

/* ISVCMsg */
typedef struct interface_svc_s{
	int version;

	server_msg_t*(WINAPI *FindSVCMsg)(char *msg);
	pfnCallback(WINAPI *HookSVCMsg)(char *msg, pfnCallback callback);
} *pinterface_svc_s, interface_svc_t;

/* ICLCMsg */
typedef struct interface_clc_s{
	int version;

	server_msg_t*(WINAPI *FindCLCMsg)(char *msg);
	pfnCallback(WINAPI *HookCLCMsg)(char *msg, pfnCallback callback);
} *pinterface_clc_s, interface_clc_t;

/* IEventMsg */
typedef struct interface_event_s{
	int version;

	event_msg_s*(WINAPI *FindEventMsg)(char *msg);
	event_msg_s(WINAPI *HookEventMsg)(char *msg, event_msg_s callback);
} *pinterface_event_s, interface_event_t;

/* IUserMsg */
typedef struct interface_usermsg_s{
	int version;

	user_msg_t*(WINAPI *FindUserMsg)(char *name);
	pfnUserMsgHook(WINAPI *HookUserMsg)(char *name, pfnUserMsgHook callback);
	int(WINAPI *CallUserMsg)(char *name, int size, PVOID buffer);
} *pinterface_usermsg_s, interface_usermsg_t;

/* IRenderer */
typedef struct interface_renderer_s{
	int version;

	/** Get render and build information **/

	/* base - render library address (hw.dll, sw.dll or hl.exe on old engine versions) */
	/* size - base library size, which is located in memory */
	/* build - engine build */
	/* If transfer 0, parameter will not be retrieved */
	void(WINAPI *GetRendererInfo)(PVOID *base, PDWORD size, PDWORD build);

	/* Get various engine structures */
	void(WINAPI *GetBases)(user_msg_t **userMsg, event_msg_t **eventMsg, server_msg_t **svc, server_msg_t **clc);
	/* Get info about Studio */
	void(WINAPI *GetStudio)(engine_studio_api_t **studioAPI, r_studio_interface_s **studioInterface);
} *pinterface_renderer_s, interface_renderer_t;

typedef struct interface_dynarr_s{
	int version;

	void(FASTCALL *DynArrCreate)(dynarr_t &arr, int cellSize);
	void(FASTCALL *DynArrPush)(dynarr_t &arr, PVOID data);
	void(FASTCALL *DynArrPop)(dynarr_t &arr);
	void(FASTCALL *DynArrClear)(dynarr_t &arr);
	void(FASTCALL *DynArrSetLength)(dynarr_t &arr, int size);
	void(FASTCALL *DynArrDelCell)(dynarr_t &arr, int id);
	PVOID(FASTCALL *DynArrGetCell)(dynarr_t &arr, int id);
	int(FASTCALL *DynArrLength)(dynarr_t &arr);
} *pinterface_dynarr_t, interface_dynarr_t;

typedef struct interface_console_s{
	int version;

	void(__cdecl *ColorPrint)(color24 color, const char *fmt, ...);
	void(__cdecl *ColorPrintDev)(color24 color, const char *fmt, ...);
} *pinterface_console_t, interface_console_t;

typedef struct interface_const_s{
	int version;

	char *(WINAPI *GetPlucoreDir)();
	char *(WINAPI *GetPluginsDir)();
	char *(WINAPI *GetModulesDir)();
} pinterface_const_t, interface_const_t;

/* Plucore functions declaration */

typedef PVOID(WINAPI *pfnFindInterface)(const char *name);

typedef void(WINAPI *pfnInjectMain)(HINSTANCE hinst, pfnFindInterface pfn, plugininfo_t &plugininfo);
typedef bool(WINAPI *pfnInjectRecords)(cl_enginefunc_t* engine, cl_clientfunc_t* client, engine_studio_api_t* studio, enginefuncs_t* server);
typedef void(WINAPI *pfnInjectUnload)();