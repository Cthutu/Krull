//-----------------------------------------------------------------------------
// Apps/krull/Project.cc
//
// Implementation of Project class
//-----------------------------------------------------------------------------

#include "Krull.h"
#include "Project.h"
#include "Compiler.h"

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
	// Clean up the tables
	for (Tables::iterator it = mTables.begin(); it != mTables.end(); ++it)
	{
		delete it->second;
	}

	mTables.clear();
}

//-----------------------------------------------------------------------------
// New functions
//-----------------------------------------------------------------------------

Table& Project::NewTable (const string& name)
{
	K_ASSERT(!HasTable(name));
	Table* table = new Table (mCompiler, name);
	mTables[name] = table;

	mCompiler.Status("Creating table definition '%s'", name.c_str());

	return *table;
}

//-----------------------------------------------------------------------------
// Has functions
//-----------------------------------------------------------------------------

bool Project::HasTable (const string& name) const
{
	return mTables.find(name) != mTables.end();
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
