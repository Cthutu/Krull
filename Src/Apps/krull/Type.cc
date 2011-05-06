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
//-----------------------------------------------------------------------------
