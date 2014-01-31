
#ifndef BANKPARSE_H
#define BANKPARSE_H

#include "stdafx.h"

class KeyValue
{
public:
	string ValueName;
	string ValueType;
	string Value;
	bool operator<(const KeyValue& keyValue2) const;
};

class Key
{
public:
	Key(string name)
	{
		Name = name;
	}
	bool operator<(const Key& key2) const;
	string Name;
	vector<KeyValue> Values;
};

class Section
{
public:
	Section(string name)
	{
		Name = name;
	}
	bool operator<(const Section& section2) const;
	string Name;
	vector<Key> Keys;
};

class Bank
{
public:
	string Name;
	vector<Section> Sections;
	string Signature;
	void Parse(string bank);
	void Alphabetize();
	void SetName(string name)
	{
		Name = name;
	}
};





#endif