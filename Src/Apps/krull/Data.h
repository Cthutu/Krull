//-----------------------------------------------------------------------------
// Apps/krull/Data.h
//
// Defines the Data class that manages the data clause
//-----------------------------------------------------------------------------

#pragma once

class Compiler;
class Table;
class Value;
class Type;

class Data
{
public:
	Data (Compiler& compiler, const Table& table, const string& name);
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
	Status			StartEntry		(const string& name);
	Status			AddField		(const Type& type, const Value& value);

	//
	// Attributes
	//
	string			GetName			() const;
	unsigned int	GetNumEntries	() const;
	Value			GetField		(unsigned int entryIndex, unsigned int fieldIndex) const;

private:
	typedef vector<Value>				Entry;
	typedef map<string, unsigned int>	EntryMap;
	typedef vector<Entry>				Entries;

	Compiler&		mCompiler;
	const Table&	mTable;
	string			mName;
	Entries			mEntries;
	EntryMap		mEntryMap;
};