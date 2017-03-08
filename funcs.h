#pragma once

#include "global.h"

bool __cdecl hkMD5_Hash_File(md5_hash_t *hash, char *name, bool useFOpen, bool useSeed, md5_hash_t *seed);