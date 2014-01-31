
#include "stdafx.h"

void SHA1ProcessChunk(BYTE* data, UINT32* hash)
{	
	/*this function will be very confusing if you don't know how to calculate an SHA-1 hash.
	/*therefore i suggest you read this: http://en.wikipedia.org/wiki/SHA-1 before trying to make sense of this.*/

	UINT32 A = hash[0];
	UINT32 B = hash[1];
	UINT32 C = hash[2];
	UINT32 D = hash[3];
	UINT32 E = hash[4];
	UINT32 F;
	UINT32 Temp;
	UINT32 W[80];

	UINT32 i = 0;
	while (i < 16)
	{	
		Temp = 0;
		Temp =               data[i * 4 + 0];
		Temp = (Temp << 8) | data[i * 4 + 1];
		Temp = (Temp << 8) | data[i * 4 + 2];
		Temp = (Temp << 8) | data[i * 4 + 3];
		W[i] = Temp;
		i++;
	}
	while (i < 80)
	{
		Temp  = W[i - 3];
		Temp ^= W[i - 8];
		Temp ^= W[i - 14];
		Temp ^= W[i - 16];
		W[i] = ROL(Temp, 1);
		i++;
	}
	
	i = 0;
	while (i < 20)
	{
		F = (B & C) | (~B & D);
		Temp = ROL(A, 5) + F + E + W[i] + 0x5A827999;
		E = D;
		D = C;
		C = ROL(B, 30);
		B = A;
		A = Temp;
		i++;
	}
	while (i < 40)
	{
		F = B ^ C ^ D;
		Temp = ROL(A, 5) + F + E + W[i] + 0x6ED9EBA1;
		E = D;
		D = C;
		C = ROL(B, 30);
		B = A;
		A = Temp;
		i++;
	}
	while (i < 60)
	{
		F = (B & C) | (B & D) | (C & D);
		Temp = ROL(A, 5) + F + E + W[i] + 0x8F1BBCDC;
		E = D;
		D = C;
		C = ROL(B, 30);
		B = A;
		A = Temp;
		i++;
	}
	while (i < 80)
	{
		F = B ^ C ^ D;
		Temp = ROL(A, 5) + F + E + W[i] + 0xCA62C1D6;
		E = D;
		D = C;
		C = ROL(B, 30);
		B = A;
		A = Temp;
		i++;
	}
	
	hash[0] += A;
	hash[1] += B;
	hash[2] += C;
	hash[3] += D;
	hash[4] += E;
}

UINT32 ROL(UINT32 x, int count)
{
	return ((x << count) | (x >> (0x20 - count)));
}
