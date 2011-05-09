//-----------------------------------------------------------------------------
// Apps/krull/Table.h
//
// Implements a table definition
//-----------------------------------------------------------------------------

#pragma once

#include "Type.h"

class Compiler;

class Table
{
public:
	Table (Compiler& compiler, const string& name);
	~Table ();

	bool			AddField		(const string& name, const Type& type);
	unsigned int	NumFields		() const;
	bool			HasField		(const string& name);

	string			GetName			() const		{ return mName; }

private:
	struct FieldDef
	{
		string	name;
		Type	type;
	};

	typedef		vector<FieldDef>		Fields;

	Compiler&	mCompiler;
	string		mName;
	Fields		mFields;		

};