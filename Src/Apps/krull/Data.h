//-----------------------------------------------------------------------------
// Apps/krull/Data.h
//
// Defines the Data class that manages the data clause
//-----------------------------------------------------------------------------

#pragma once

#include "Table.h"

class Compiler;
class Value;

class Data
{
public:
	Data (Compiler& compiler, const KTable& table, const string& name);
	~Data ();

	enum Status
	{
		Data_OK,
		Data_Error_DuplicateName,
		Data_Error_TooMuchData,
		Data_Error_InvalidType,
	};

	//
	// Operations
	//

	// Field entry
	Status			StartEntry				(const string& name);
	Status			AddField				(const Type& type, const Value& value);

	//
	// Attributes
	//
	string			GetName					() const;
	size_t			GetNumEntries			() const;
	Value			GetField				(unsigned int entryIndex, unsigned int fieldIndex) const;
	size_t			GetCurrentFieldIndex	() const;
	size_t			GetEntryRef				(const string& name) const;
	const KTable&	GetTable				() const			{ return mTable; }
	
private:
	typedef vector<Value>				Entry;
	typedef map<string, size_t>			EntryMap;
	typedef vector<Entry>				Entries;

	Compiler&		mCompiler;
	const KTable&	mTable;
	string			mName;
	Entries			mEntries;
	EntryMap		mEntryMap;
};