
#ifndef SIG_H
#define SIG_H

#include "stdafx.h"

struct Signature
{
	uint64_t length;
	uint32_t hash[5];
	vector<unsigned char> data;
};

void Init(Signature* sig);
void AppendText(Signature* sig, unsigned char* text, uint32_t length);
void Finalize(Signature* sig);
void Encrypt(Signature* sig);
string GetHash(Signature* sig);

#endif
