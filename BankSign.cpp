// BankSign.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "BankSign.h"

BANKSIGN_API void GetSigInputString(const char* authorAccountNumber, const char* userAccountNumber, const char* bankName, const char* bankData, char* stringBuffer, int destinationSize)
{
	string AANumber = authorAccountNumber;
	string UANumber = userAccountNumber;
	string BankName = bankName;
	string BankData = bankData;
	string SigInputString;
	string Temp;
	Bank ThisBank;

	ThisBank.Parse(bankData);
	ThisBank.Alphabetize();
	ThisBank.SetName(BankName);
	
	SigInputString += AANumber;
	SigInputString += UANumber;
	SigInputString += ThisBank.Name;
	for(size_t i = 0; i < ThisBank.Sections.size(); i++)
	{
		SigInputString += ThisBank.Sections[i].Name;
		for(size_t j = 0; j < ThisBank.Sections[i].Keys.size(); j++)
		{
			SigInputString += ThisBank.Sections[i].Keys[j].Name;
			for(size_t k = 0; k < ThisBank.Sections[i].Keys[j].Values.size(); k++)
			{		
				SigInputString += ThisBank.Sections[i].Keys[j].Values[k].ValueName;
				SigInputString += ThisBank.Sections[i].Keys[j].Values[k].ValueType;
				if(ThisBank.Sections[i].Keys[j].Values[k].ValueType != "text")
					SigInputString += ThisBank.Sections[i].Keys[j].Values[k].Value;
			}
		}
	}	

	strcpy_s(stringBuffer, destinationSize, SigInputString.c_str());
}

BANKSIGN_API void GetCurrentSig(const char* bankData, char* sigBuffer, int destinationSize)
{
	Bank ThisBank;
	ThisBank.Parse(bankData);
	strcpy_s(sigBuffer, destinationSize, ThisBank.Signature.c_str());
}

BANKSIGN_API void GetNewSig(const char* authorAccountNumber, const char* userAccountNumber, const char* bankName, const char* bankData, char* sigBuffer, int destinationSize)
{
	size_t BufferSize = strlen(bankData);
	char* Buffer = new char[BufferSize];

	GetSigInputString(authorAccountNumber, userAccountNumber, bankName, bankData, Buffer, BufferSize);
	GetNewSigFromString(Buffer, sigBuffer, destinationSize);

	delete[] Buffer;
}

/*BANKSIGN_API void GetNewSig(const char* authorAccountNumber, const char* userAccountNumber, const char* bankName, const char* bankData, char* sigBuffer, int destinationSize)
{
	string AANumber = authorAccountNumber;
	string UANumber = userAccountNumber;
	string BankName = bankName;
	string BankData = bankData;
	string Sig;
	string Temp;
	Bank ThisBank;
	Signature sig;

	Init(&sig);
	ThisBank.Parse(bankData);
	ThisBank.Alphabetize();
	ThisBank.SetName(BankName);
	
	AppendText(&sig, (BYTE*) (AANumber.c_str()), AANumber.size());
	AppendText(&sig, (BYTE*) (UANumber.c_str()), UANumber.size());
	AppendText(&sig, (BYTE*) (ThisBank.Name.c_str()), ThisBank.Name.size());
	for(size_t i = 0; i < ThisBank.Sections.size(); i++)
	{
		AppendText(&sig, (BYTE*) (ThisBank.Sections[i].Name.c_str()), ThisBank.Sections[i].Name.size());
		for(size_t j = 0; j < ThisBank.Sections[i].Keys.size(); j++)
		{
			Temp = "Value";
			AppendText(&sig, (BYTE*) (ThisBank.Sections[i].Keys[j].Name.c_str()), ThisBank.Sections[i].Keys[j].Name.size());
			AppendText(&sig, (BYTE*) (Temp.c_str()), Temp.size());
			AppendText(&sig, (BYTE*) (ThisBank.Sections[i].Keys[j].ValueType.c_str()), ThisBank.Sections[i].Keys[j].ValueType.size());
			if(ThisBank.Sections[i].Keys[j].ValueType != "text")
				AppendText(&sig, (BYTE*) (ThisBank.Sections[i].Keys[j].Value.c_str()), ThisBank.Sections[i].Keys[j].Value.size());
		}
	}	
	Finalize(&sig);
	Sig = GetHash(&sig);

	strcpy_s(sigBuffer, destinationSize, Sig.c_str());
}*/

BANKSIGN_API void GetNewSigFromString(const char* sigInputString, char* sigBuffer, int destinationSize)
{
	string Sig;
	string SigInputString = sigInputString;
	Signature sig;

	Init(&sig);	
	AppendText(&sig, (BYTE*) (SigInputString.c_str()), SigInputString.size());
	Finalize(&sig);
	Sig = GetHash(&sig);

	strcpy_s(sigBuffer, destinationSize, Sig.c_str());
}

BANKSIGN_API void Sign(const char* authorAccountNumber, const char* userAccountNumber, const char* bankName, const char* bankData, char* destination, int destinationSize)
{
	string BankData = bankData;
	string Sig;
	string Temp;
	char buffer[82];
	
	GetNewSig(authorAccountNumber,  userAccountNumber, bankName, bankData, buffer, 82);
	Sig = buffer;

	size_t sigStart = BankData.find("<Signature value=");
	size_t sigEnd = BankData.find("/>", sigStart);
	if(sigStart != string::npos && sigEnd != string::npos)
	{
		if(sigEnd > sigStart)
			BankData.erase(sigStart, (sigEnd - sigStart) + 2);

		Temp = Sig;
		Sig = "<Signature value=\"";
		Sig += Temp;
		Sig += "\"/>";
	}
	else
	{
		sigStart = BankData.find("</Bank>");
		if(sigStart != string::npos)
			sigStart -= 2;

		Temp = Sig;
		Sig = "\x0d\x0a    <Signature value=\"";
		Sig += Temp;
		Sig += "\"/>";
	}

	if(sigStart != string::npos)
		BankData.insert(sigStart, Sig);

	strcpy_s(destination, destinationSize, BankData.c_str());
}

BANKSIGN_API void SignFromString(const char* sigInputString, const char* bankData, char* destination, int destinationSize)
{
	string BankData = bankData;
	string SigInputString = sigInputString;
	string Sig;
	string Temp;
	char buffer[82];
	
	GetNewSigFromString(sigInputString, buffer, 82);
	Sig = buffer;

	size_t sigStart = BankData.find("<Signature value=");
	size_t sigEnd = BankData.find("/>", sigStart);
	if(sigStart != string::npos && sigEnd != string::npos)
	{
		if(sigEnd > sigStart)
			BankData.erase(sigStart, (sigEnd - sigStart) + 2);

		Temp = Sig;
		Sig = "<Signature value=\"";
		Sig += Temp;
		Sig += "\"/>";
	}
	else
	{
		sigStart = BankData.find("</Bank>");
		if(sigStart != string::npos)
			sigStart -= 2;

		Temp = Sig;
		Sig = "\x0d\x0a    <Signature value=\"";
		Sig += Temp;
		Sig += "\"/>";
	}

	if(sigStart != string::npos)
		BankData.insert(sigStart, Sig);

	strcpy_s(destination, destinationSize, BankData.c_str());
}