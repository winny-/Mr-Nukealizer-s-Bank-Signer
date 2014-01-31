
#include "stdafx.h"

void Init(Signature* sig)
{
	sig->length = 0;
	sig->hash[0] = 0x67452301;
	sig->hash[1] = 0xEFCDAB89;
	sig->hash[2] = 0x98BADCFE;
	sig->hash[3] = 0x10325476;
	sig->hash[4] = 0xC3D2E1F0;
}
	
string GetHash(Signature* sig)
{
	char hex[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
	string returnVal;
	for (int i = 0; i < 5; i++) //for each 32-bit value in the hash array
	{
		for (int j = 3; j >= 0; j--) //start with the last byte since the data is stored little-endian and we need big-endian
		{
			returnVal += hex[(((char*) &(sig->hash[i]))[j] & 0xF0) >> 4]; //get character representation of high 4 bits
			returnVal += hex[(((char*) &(sig->hash[i]))[j] & 0x0F)];      //get character representation of low 4 bits
		}
	}	
	return returnVal;
}
	
void Finalize(Signature* sig)
{
	uint32_t PaddingLength = ((0xfffffff7 - (sig->length / 8)) % 64) + 1; //the number of bytes that need to be added before the length data
	                                                                    //to make the total length a multiple of 64 bytes
	unsigned char* Padding = new unsigned char[PaddingLength];
	unsigned char LengthInBits[8];

	Padding[0] = 128; //add a 1 bit to the beginning of padding
	for (uint32_t i = 1; i < PaddingLength; i++)
	{
		Padding[i] = 0; //zero the rest
	}
	
	for (int i = 0; i < 8; i++) //sig->length is stored little-endian, but we need big-endian
		LengthInBits[i] = ((unsigned char*)&(sig->length))[7 - i];
	
	AppendText(sig, Padding, PaddingLength); //add padding
	AppendText(sig, LengthInBits, 8); //add length data
	Encrypt(sig); //process the complete data

	delete[] Padding; //no memory leaks allowed!
}

void AppendText(Signature* sig, unsigned char* text, uint32_t length)
{
	sig->length += ((uint64_t) length) * 8; //add the length of the new data to the old length

	for (uint32_t i = 0; i < length; i++)
		sig->data.push_back(text[i]); //add the new text one character at a time
}

void Encrypt(Signature* sig)
{
	int NumBlocks = (int) (sig->length / 512); //the number of 64 byte blocks of data
	unsigned char MessageBlock[64]; //temporary storage for the current block
	                       //because i don't know if it is safe to use a vector the way we need to use the data

	for (int i = 0; i < NumBlocks; i++) //for each block
	{
		for (int j = 0; j < 64; j++) //for each character in the current block
			MessageBlock[j] = sig->data[i * 64 + j]; //copy it to temporary storage

		SHA1ProcessChunk(MessageBlock, sig->hash); //process the current block
	}
}
