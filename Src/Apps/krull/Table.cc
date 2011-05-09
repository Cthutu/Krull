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

Table::Table (Compiler& compiler, const string& name)
	: mCompiler(compiler)
	, mName(name)
{
}

//-----------------------------------------------------------------------------
// Destructor
//-----------------------------------------------------------------------------

Table::~Table ()
{
	mFields.clear();
}

//-----------------------------------------------------------------------------
// Field management
//-----------------------------------------------------------------------------

bool Table::AddField (const string& name, const Type& type)
{
	if (HasField(name)) return false;

	FieldDef field;
	field.name = name;
	field.type = type;

	mFields.push_back(field);

	mCompiler.Status("Adding field '%s' to table '%s'", name.c_str(), mName.c_str());

	return true;
}

unsigned int Table::NumFields () const
{
	return mFields.size();
}

bool Table::HasField (const string& name)
{
	for (Fields::iterator it = mFields.begin(); it != mFields.end(); ++it)
	{
		if (it->name == name) return true;
	}

	return false;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
