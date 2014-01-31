#ifndef BANKSIGN_DLL_H
#define BANKSIGN_DLL_H

// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the BANKSIGN_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// BANKSIGN_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
// #ifdef BANKSIGN_EXPORTS
// #define BANKSIGN_API __declspec(dllexport)
// #else
// #define BANKSIGN_API __declspec(dllimport)
// #endif

void Sign(const char* authorAccountNumber, const char* userAccountNumber, const char* bankName, const char* bankData, char* destination, int destinationSize);
void SignFromString(const char* sigInputString, const char* bankData, char* destination, int destinationSize);

void GetSigInputString(const char* authorAccountNumber, const char* userAccountNumber, const char* bankName, const char* bankData, char* stringBuffer, int destinationSize);

void GetNewSigFromString(const char* sigInputString, char* sigBuffer, int destinationSize);
void GetNewSig(const char* authorAccountNumber, const char* userAccountNumber, const char* bankName, const char* bankData, char* sigBuffer, int destinationSize);
void GetCurrentSig(const char* bankData, char* sigBuffer, int destinationSize);

#endif
