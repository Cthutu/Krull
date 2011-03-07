//-----------------------------------------------------------------------------
// Encapsulates a type qualifier
//-----------------------------------------------------------------------------

#pragma once

enum TypeValue
{
	TypeValue_Undefined,
	TypeValue_Integer,
	TypeValue_Float,
	TypeValue_Bool,
	TypeValue_String,
	TypeValue_TableRef,
};

class Type
{
public:
	Type ();

	// Set methods
	void	SetType			(TypeValue type);
	void	SetList			();
	void	SetTableName	(const string& tableName);

private:
	TypeValue		mType;
	bool			mIsList;
	string			mTableName;
};

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

