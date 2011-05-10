//-----------------------------------------------------------------------------
// Apps/krull/SqliteBackEnd.h
//
// Back-end for the SQLite database
//-----------------------------------------------------------------------------

#pragma once

#include "BackEnd.h"

class SqliteBackEnd
	: public BackEnd
{
public:
	SqliteBackEnd ();
	virtual ~SqliteBackEnd ();

	virtual string		GetExtension		() const		{ return string("kx"); }
	virtual bool		Build				(const string& fileName, const Project& project);

private:

};

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
