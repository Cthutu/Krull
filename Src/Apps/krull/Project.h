//-----------------------------------------------------------------------------
// Apps/krull/Project.h
//
// Project definition
//-----------------------------------------------------------------------------

#pragma once

class Compiler;
class Data;
class Table;

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
	Data&		NewData			(const string& name, const Table& table);

	// Has functions
	bool		HasTable		(const string& name) const;
	bool		HasData			(const string& name) const;

	// Get functions
	Table&		GetTable		(const string& name);
	Data&		GetData			(const string& name);

private:
	Compiler&	mCompiler;
	string		mPath;
	string		mName;

	typedef		map<string, Table*>		Tables;
	typedef		map<string, Data*>		DataDefs;

	Tables		mTables;
	DataDefs	mDataDefs;
};

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
