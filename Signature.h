
#ifndef SIG_H
#define SIG_H

#include "stdafx.h"

struct Signature
{
	UINT64 length;
	UINT32 hash[5];
	vector<BYTE> data;
};

void Init(Signature* sig);
void AppendText(Signature* sig, BYTE* text, UINT32 length);
void Finalize(Signature* sig);
void Encrypt(Signature* sig);
string GetHash(Signature* sig);

#endif