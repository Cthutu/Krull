//-----------------------------------------------------------------------------
// Apps/krull/SqliteBackEnd.cc
//
// Implementation of the SqliteBackEnd class
//-----------------------------------------------------------------------------

#include "Krull.h"
#include "SqliteBackEnd.h"
extern "C"
{
#include "..\Libs\Sqlite\sqlite3.h"
};

//-----------------------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------------------

SqliteBackEnd::SqliteBackEnd ()
	: mSqlite(0)
{

}

//-----------------------------------------------------------------------------
// Destructor
//-----------------------------------------------------------------------------

SqliteBackEnd::~SqliteBackEnd ()
{

}

//-----------------------------------------------------------------------------
// Builder
//-----------------------------------------------------------------------------

bool SqliteBackEnd::Build (const string& fileName, const Compiler& compiler, const Project& project)
{
	// Create the Sqlite database file
	compiler.Status("[SQLITE]  Opening database '%s'", fileName.c_str());
	int result = sqlite3_open(fileName.c_str(), &mSqlite);
	if (result)
	{
		sqlite3_close(mSqlite);
		return compiler.Error(0, "Failed to open database '%s'", fileName.c_str());
	}

	// Clean up
	sqlite3_close(mSqlite);

	return true;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
