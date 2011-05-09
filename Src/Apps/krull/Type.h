//-----------------------------------------------------------------------------
// Apps/krull/Type.h
//
// Encapsulates a type qualifier
//-----------------------------------------------------------------------------

#pragma once

enum TypeValue
{
	TypeValue_Undefined,
	TypeValue_Integer,			// int
	TypeValue_Float,			// float
	TypeValue_Bool,				// bool
	TypeValue_String,			// string
	TypeValue_TableRef,			// Table
	TypeValue_TableRefList,		// Table*
};

class Type
{
public:
	Type ();

	// Set methods
	void		SetType			(TypeValue type);
	void		SetList			();
	void		SetTableName	(const string& tableName);

	// Get methods
	TypeValue	GetType			() const		{ return mType; }

	// Operators
	bool		operator ==		(const Type& type) const;
	bool		operator !=		(const Type& type) const;

private:
	TypeValue		mType;
	bool			mIsList;
	string			mTableName;
};

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

