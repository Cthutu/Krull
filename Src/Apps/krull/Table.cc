//-----------------------------------------------------------------------------
// Apps/krull/Table.cc
//
// Implementation of the Table class
//-----------------------------------------------------------------------------

#include "Krull.h"
#include "Table.h"
#include "Compiler.h"

//-----------------------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------------------

KTable::KTable (Compiler& compiler, const string& name)
	: mCompiler(compiler)
	, mName(name)
{
}

//-----------------------------------------------------------------------------
// Destructor
//-----------------------------------------------------------------------------

KTable::~KTable ()
{
	mFields.clear();
}

//-----------------------------------------------------------------------------
// Field management
//-----------------------------------------------------------------------------

bool KTable::AddField (const string& name, const Type& type)
{
	if (HasField(name)) return false;

	FieldDef field;
	field.name = name;
	field.type = type;

	mFields.push_back(field);

	mCompiler.Status("Adding field '%s' to table '%s'", name.c_str(), mName.c_str());

	return true;
}

unsigned int KTable::NumFields () const
{
	return mFields.size();
}

bool KTable::HasField (const string& name)
{
	for (Fields::iterator it = mFields.begin(); it != mFields.end(); ++it)
	{
		if (it->name == name) return true;
	}

	return false;
}

const string& KTable::GetFieldName (unsigned int index) const
{
	K_ASSERT(index < mFields.size());
	return mFields[index].name;
}

const Type& KTable::GetFieldType (unsigned int index) const
{
	K_ASSERT(index < mFields.size());
	return mFields[index].type;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
