
#ifndef SHA1_H
#define SHA1_H

#include "stdafx.h"

void SHA1ProcessChunk(unsigned char* data, uint32_t* hash);
uint32_t ROL(uint32_t x, int count);

#endif
