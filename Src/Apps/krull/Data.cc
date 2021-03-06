//-----------------------------------------------------------------------------
// Apps/krull/Data.cc
//
// Implements the Data class
//-----------------------------------------------------------------------------

#include "Krull.h"
#include "Data.h"
#include "Table.h"
#include "Value.h"

//-----------------------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------------------

Data::Data (Compiler& compiler, const KTable& table, const string& name)
	: mCompiler(compiler)
	, mTable(table)
	, mName(name)
{}

//-----------------------------------------------------------------------------
// Destructor
//-----------------------------------------------------------------------------

Data::~Data ()
{
	for (Entries::iterator entry = mEntries.begin(); entry != mEntries.end(); ++entry)
	{
		unsigned int fieldIndex = 0;
		for (Entry::iterator field = entry->begin(); field != entry->end(); ++field)
		{
			K_ASSERT(fieldIndex < mTable.GetNumFields());
			field->Clean(mTable.GetFieldType(fieldIndex++));
		}
	}
}

//-----------------------------------------------------------------------------
// Field entries
//-----------------------------------------------------------------------------

Data::Status Data::StartEntry (const string& name)
{
	// Check to see that the entry name doesn't already exist
	if (mEntryMap.find(name) != mEntryMap.end())
	{
		// Duplicate name!
		return Data_Error_DuplicateName;
	}

	Entry newEntry;
	size_t newIndex = mEntries.size();

	mEntries.push_back(newEntry);
	mEntryMap[name] = newIndex;
	
	Type rowNameType;
	rowNameType.SetType(TypeValue_RowName);
	AddField(rowNameType, Value(rowNameType, name));

	return Data_OK;
}

Data::Status Data::AddField (const Type& type, const Value& value)
{
	Entry& latestEntry = mEntries[mEntries.size()-1];

	if (latestEntry.size() > mTable.GetNumFields())
	{
		return Data_Error_TooMuchData;
	}

	if (type != mTable.GetFieldType(latestEntry.size()))
	{
		return Data_Error_InvalidType;
	}

	latestEntry.push_back(value);

	return Data_OK;
}

//-----------------------------------------------------------------------------
// Attributes
//-----------------------------------------------------------------------------

string Data::GetName () const
{
	return mName;
}

size_t Data::GetNumEntries () const
{
	return mEntries.size();
}

Value Data::GetField (unsigned int entryIndex, unsigned int fieldIndex) const
{
	K_ASSERT(entryIndex < mEntries.size());
	const Entry& entry = mEntries[entryIndex];
	K_ASSERT(fieldIndex < entry.size());
	return entry[fieldIndex];
}

size_t Data::GetCurrentFieldIndex () const
{
	K_ASSERT(mEntries.size() > 0);
	const Entry& entry = mEntries[mEntries.size()-1];

	return entry.size();
}

size_t Data::GetEntryRef (const string& name) const
{
	EntryMap::const_iterator entry = mEntryMap.find(name);
	if (entry == mEntryMap.end())
	{
		// Not found
		return (unsigned int)-1;
	}
	else
	{
		return entry->second;
	}
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
