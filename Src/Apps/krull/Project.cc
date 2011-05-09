//-----------------------------------------------------------------------------
// Apps/krull/Project.cc
//
// Implementation of Project class
//-----------------------------------------------------------------------------

#include "Krull.h"
#include "Project.h"
#include "Compiler.h"
#include "Table.h"
#include "Data.h"

//-----------------------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------------------

Project::Project (Compiler& compiler, const string& name, const string& path)
	: mCompiler(compiler)
	, mName(name)
	, mPath(path)
{

}

//-----------------------------------------------------------------------------
// Destructor
//-----------------------------------------------------------------------------

Project::~Project ()
{
	// Clean up the data definitions
	for (DataDefs::iterator it = mDataDefs.begin(); it != mDataDefs.end(); ++it)
	{
		delete it->second;
	}

	// Clean up the tables
	for (Tables::iterator it = mTables.begin(); it != mTables.end(); ++it)
	{
		delete it->second;
	}

	mDataDefs.clear();
	mTables.clear();
}

//-----------------------------------------------------------------------------
// New functions
//-----------------------------------------------------------------------------

KTable& Project::NewTable (const string& name)
{
	K_ASSERT(!HasTable(name));
	KTable* table = new KTable (mCompiler, name);
	mTables[name] = table;

	mCompiler.Status("Creating table definition '%s'", name.c_str());

	return *table;
}

Data& Project::NewData (const string& name, const KTable& table)
{
	K_ASSERT(!HasData(name));
	Data* data = new Data (mCompiler, table, name);
	mDataDefs[name] = data;

	mCompiler.Status("Creating data definition '%s' [%s]", name.c_str(), table.GetName().c_str());

	return *data;
}

//-----------------------------------------------------------------------------
// Has functions
//-----------------------------------------------------------------------------

bool Project::HasTable (const string& name) const
{
	return mTables.find(name) != mTables.end();
}

bool Project::HasData (const string& name) const
{
	return mDataDefs.find(name) != mDataDefs.end();
}

//-----------------------------------------------------------------------------
// Get functions
//-----------------------------------------------------------------------------

KTable& Project::GetTable (const string& name)
{
	K_ASSERT(HasTable(name));
	return *mTables[name];
}

Data& Project::GetData (const string& name)
{
	K_ASSERT(HasData(name));
	return *mDataDefs[name];
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
