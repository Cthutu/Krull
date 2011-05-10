//-----------------------------------------------------------------------------
// Apps/krull/Project.h
//
// Project definition
//-----------------------------------------------------------------------------

#pragma once

class Compiler;
class Data;
class KTable;

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
	KTable&			NewTable		(const string& name);
	Data&			NewData			(const string& name, const KTable& table);

	// Has functions
	bool			HasTable		(const string& name) const;
	bool			HasData			(const string& name) const;

	// Get functions
	KTable&			GetTable		(const string& name);
	Data&			GetData			(const string& name);

	// Iteration functions
	const Data*		FirstData		() const;
	const Data*		NextData		() const;

private:
	Compiler&			mCompiler;
	string				mPath;
	string				mName;

	typedef		map<string, KTable*>	Tables;
	typedef		map<string, Data*>		DataDefs;

	Tables				mTables;
	DataDefs			mDataDefs;

	mutable DataDefs::const_iterator	mDataIt;
};

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
