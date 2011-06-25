//-----------------------------------------------------------------------------
// Apps/krull/Type.h
//
// Encapsulates a type qualifier
//-----------------------------------------------------------------------------

#pragma once

enum TypeValue
{
	TypeValue_Undefined,
	TypeValue_RowName,			// row name
	TypeValue_Integer,			// int
	TypeValue_Float,			// float
	TypeValue_Bool,				// bool
	TypeValue_String,			// string
	TypeValue_DataRef,			// Data
	TypeValue_DataRefList,		// Data*
};

class Type
{
public:
	Type ();

	// Set methods
	void		SetType			(TypeValue type);
	void		SetList			();
	void		SetDataName		(const string& dataName);

	// Get methods
	string		ShortDesc		() const;
	TypeValue	GetType			() const		{ return mType; }
	bool		IsList			() const		{ return mType == TypeValue_DataRefList; }
	string		GetDataName		() const		{ return mDataName; }

	// Operators
	bool		operator ==		(const Type& type) const;
	bool		operator !=		(const Type& type) const;

private:
	TypeValue		mType;
	string			mDataName;
};

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

