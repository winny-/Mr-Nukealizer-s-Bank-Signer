
#ifndef SHA1_H
#define SHA1_H

#include "stdafx.h"

void SHA1ProcessChunk(BYTE* data, UINT32* hash);
UINT32 ROL(UINT32 x, int count);

#endif