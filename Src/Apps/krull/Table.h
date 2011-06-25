//-----------------------------------------------------------------------------
// Apps/krull/Table.h
//
// Implements a table definition
//-----------------------------------------------------------------------------

#pragma once

#include "Type.h"

class Compiler;

class KTable
{
public:
	KTable (Compiler& compiler, const string& name);
	~KTable ();

	bool			AddField		(const string& name, const Type& type);
	size_t			NumFields		() const;
	bool			HasField		(const string& name);
	size_t			GetNumFields	() const		{ return mFields.size(); }
	const string&	GetFieldName	(size_t index) const;
	const Type&		GetFieldType	(size_t index) const;

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