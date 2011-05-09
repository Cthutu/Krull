//-----------------------------------------------------------------------------
// Apps/krull/Project.h
//
// Project definition
//-----------------------------------------------------------------------------

#pragma once

#include "Table.h"

class Compiler;

//-----------------------------------------------------------------------------
// A project is a collection of tables and data that is used to generate
// whatever data (Sqlite, SQL or whatever backend you care to create).
//-----------------------------------------------------------------------------

class Project
{
public:
	Project (Compiler& compiler, const string& name, const string& path);
	~Project ();

	// New functions
	Table&		NewTable		(const string& name);

	// Has functions
	bool		HasTable		(const string& name) const;

private:
	Compiler&	mCompiler;
	string		mPath;
	string		mName;

	typedef		map<string, Table*>		Tables;

	Tables		mTables;
};

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------