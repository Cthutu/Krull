//-----------------------------------------------------------------------------
// Apps/krull/SqliteBackEnd.h
//
// Back-end for the SQLite database
//-----------------------------------------------------------------------------

#pragma once

extern "C"
{
	struct sqlite3;
};

#include "BackEnd.h"

class Data;



class SqliteBackEnd
	: public BackEnd
{
public:
	SqliteBackEnd ();
	virtual ~SqliteBackEnd ();

	virtual string		GetExtension		() const		{ return string("kx"); }
	virtual bool		Build				(const string& fileName, const Compiler& compiler, const Project& project);

protected:
	bool				GenerateTable		(const Compiler& compiler, const Project& project, const Data& data);

private:
	sqlite3*			mSqlite;
};

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
