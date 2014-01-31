
#include "stdafx.h"

void Bank::Alphabetize()
{
	for(size_t i = 0; i < this->Sections.size(); i++)
	{
		for(size_t j = 0; j < this->Sections[i].Keys.size(); j++)
			stable_sort(this->Sections[i].Keys[j].Values.begin(), this->Sections[i].Keys[j].Values.end());

		stable_sort(this->Sections[i].Keys.begin(), this->Sections[i].Keys.end());
	}
	stable_sort(this->Sections.begin(), this->Sections.end());
}

/*void Bank::Parse(string bank)
{
	size_t sectionStart = 0;
	size_t sectionEnd = 0;
	size_t keyStart = 0;
	size_t keyEnd = 0;
	size_t keyValueTypeStart = 0;
	size_t keyValueStart = 0;
	size_t sigStart = 0;
	string sectionName;
	string keyName;
	string keyValueType;
	string keyValue;
	string sig;

	while((sectionStart = bank.find("<Section name=\"", sectionEnd)) != string::npos)
	{
		sectionEnd = bank.find("</Section>", sectionStart);
		sectionName.clear();
		for(size_t i = sectionStart + 15; i < bank.size() && bank[i] != '\"'; i++)
			sectionName += bank[i];
		this->Sections.push_back(Section(sectionName));

		keyEnd = keyStart = sectionStart;
		while((keyStart = bank.find("<Key name=\"", keyEnd)) != string::npos && keyStart < sectionEnd)
		{
			keyEnd = bank.find("</Key>", keyStart);
			keyName.clear();
			keyValueType.clear();
			keyValue.clear();
			for(size_t i = keyStart + 11; i < bank.size() && bank[i] != '\"'; i++)
				keyName += bank[i];
			this->Sections.back().Keys.push_back(Key(keyName));

			keyValueTypeStart = bank.find("<Value ", keyStart);
			keyValueStart = bank.find("=\"", keyValueTypeStart);
			for(size_t i = keyValueTypeStart + 7; i < bank.size() && bank[i] != '='; i++)
				keyValueType += bank[i];
			for(size_t i = keyValueStart + 2; i < bank.size() && bank[i] != '"'; i++)
				keyValue += bank[i];

			this->Sections.back().Keys.back().ValueType = keyValueType;
			this->Sections.back().Keys.back().Value = keyValue;
		}
	}

	sigStart = bank.find("<Signature value=\"");
	if(sigStart != string::npos)
	{
		for(size_t i = sigStart + 18; i < bank.size() && bank[i] != '"'; i++)
			sig += bank[i];
		this->Signature = sig;
	}
}*/

void myReplace(std::string& str, const std::string& oldStr, const std::string& newStr)
{
  size_t pos = 0;
  while((pos = str.find(oldStr, pos)) != std::string::npos)
  {
     str.replace(pos, oldStr.length(), newStr);
     pos += newStr.length();
  }
}

void Bank::Parse(string bank)
{
	size_t sectionStart = 0;
	size_t sectionEnd = 0;
	size_t keyStart = 0;
	size_t keyEnd = 0;
	size_t keyValueNameStart = 0;
	size_t keyValueTypeStart = 0;
	size_t keyValueStart = 0;
	size_t sigStart = 0;
	string sectionName;
	string keyName;
	string keyValueName;
	string keyValueType;
	string keyValue;
	string sig;

	KeyValue kv;

	while((sectionStart = bank.find("<Section name=\"", sectionEnd)) != string::npos)
	{
		sectionEnd = bank.find("</Section>", sectionStart);
		sectionName.clear();
		for(size_t i = sectionStart + 15; i < bank.size() && bank[i] != '\"'; i++)
			sectionName += bank[i];
		this->Sections.push_back(Section(sectionName));

		keyEnd = keyStart = sectionStart;
		while((keyStart = bank.find("<Key name=\"", keyEnd)) != string::npos && keyStart < sectionEnd)
		{
			keyEnd = bank.find("</Key>", keyStart);
			keyValueNameStart = keyStart;
			keyName.clear();
			keyValueName.clear();
			keyValueType.clear();
			keyValue.clear();
			for(size_t i = keyStart + 11; i < bank.size() && bank[i] != '\"'; i++)
				keyName += bank[i];
			this->Sections.back().Keys.push_back(Key(keyName));

			do
			{
				keyValueNameStart = bank.find("<", keyValueNameStart + 1);
				if(keyValueNameStart < keyEnd)
				{
					keyValueTypeStart = bank.find(" ", keyValueNameStart);
					keyValueStart = bank.find("=\"", keyValueNameStart);

					keyValueName.clear();
					keyValueType.clear();
					keyValue.clear();

					for(size_t i = keyValueNameStart + 1; i < bank.size() && bank[i] != ' '; i++)
						keyValueName += bank[i];

					for(size_t i = keyValueTypeStart + 1; i < bank.size() && bank[i] != '='; i++)
						keyValueType += bank[i];

					for(size_t i = keyValueStart + 2; i < bank.size() && bank[i] != '"'; i++)
						keyValue += bank[i];
					
					myReplace(keyValue, "&quot;", "\"");
					myReplace(keyValue, "&apos;", "'");
					myReplace(keyValue, "&gt;", ">");
					myReplace(keyValue, "&lt;", "<");
					myReplace(keyValue, "&amp;", "&");

					kv.ValueName = keyValueName;
					kv.ValueType = keyValueType;
					kv.Value = keyValue;

					this->Sections.back().Keys.back().Values.push_back(kv);
				}
			} while(keyValueNameStart < keyEnd);
		}
	}

	sigStart = bank.find("<Signature value=\"");
	if(sigStart != string::npos)
	{
		for(size_t i = sigStart + 18; i < bank.size() && bank[i] != '"'; i++)
			sig += bank[i];
		this->Signature = sig;
	}
}

bool KeyValue::operator<(const KeyValue& keyValue2) const
{
	for(size_t i = 0; i < this->ValueName.size() && i < keyValue2.ValueName.size(); i++)
	{
		if(this->ValueName[i] < keyValue2.ValueName[i])
			return true;
		else if(this->ValueName[i] > keyValue2.ValueName[i])
			return false;
	}
	if(this->ValueName.size() < keyValue2.ValueName.size())
		return true;
	else
		return false;
}

bool Key::operator<(const Key& key2) const
{
	for(size_t i = 0; i < this->Name.size() && i < key2.Name.size(); i++)
	{
		if(this->Name[i] < key2.Name[i])
			return true;
		else if(this->Name[i] > key2.Name[i])
			return false;
	}
	if(this->Name.size() < key2.Name.size())
		return true;
	else
		return false;
}

bool Section::operator<(const Section& section2) const
{
	for(size_t i = 0; i < this->Name.size() && i < section2.Name.size(); i++)
	{
		if(this->Name[i] < section2.Name[i])
			return true;
		else if(this->Name[i] > section2.Name[i])
			return false;
	}
	if(this->Name.size() < section2.Name.size())
		return true;
	else
		return false;
}