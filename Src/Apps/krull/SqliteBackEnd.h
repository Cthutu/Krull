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
	SqliteBackEnd (bool addSymbols);
	virtual ~SqliteBackEnd ();

	virtual string		GetExtension		() const		{ return string("kx"); }
	virtual bool		Build				(const string& fileName, const Compiler& compiler, const Project& project);

protected:
	bool				Execute				(const string& sql);
	string				FromInt				(int value) const;
	string				FromUInt			(size_t value) const;
	string				FromFloat			(float value) const;

private:
	sqlite3*			mSqlite;
	bool				mAddSymbols;
};

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
