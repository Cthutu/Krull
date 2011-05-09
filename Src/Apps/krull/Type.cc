//-----------------------------------------------------------------------------
// Apps/krull/Type.cc
//
// Type class implementation
//-----------------------------------------------------------------------------

#include "Krull.h"
#include "Type.h"

//-----------------------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------------------

Type::Type ()
: mType(TypeValue_Undefined)
, mIsList(false)
{}

//-----------------------------------------------------------------------------
// Set methods
//-----------------------------------------------------------------------------

void Type::SetType (TypeValue type)
{
	K_ASSERT(mType == TypeValue_Undefined);
	mType = type;
}

void Type::SetList ()
{
	K_ASSERT(mType != TypeValue_Undefined);
	mIsList = true;
}

void Type::SetTableName(const string& tableName)
{
	K_ASSERT(mType == TypeValue_TableRef);
	mTableName = tableName;
}

//-----------------------------------------------------------------------------
// Operators
//-----------------------------------------------------------------------------

bool Type::operator == (const Type& type) const
{
	if (mType != type.mType) return false;

	if ((mType != TypeValue_TableRef) &&
		(mType != TypeValue_TableRefList))
	{
		return true;
	}

	if (mTableName != type.mTableName) return false;
	return mIsList == type.mIsList;
}

bool Type::operator != (const Type& type) const
{
	return !(*this == type);
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
