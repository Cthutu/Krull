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
	K_ASSERT(mType == TypeValue_DataRef);
	mType = TypeValue_DataRefList;
}

void Type::SetDataName(const string& dataName)
{
	K_ASSERT(mType == TypeValue_DataRef);
	mDataName = dataName;
}

//-----------------------------------------------------------------------------
// Description of type
//-----------------------------------------------------------------------------

string Type::ShortDesc () const
{
	string result;

	switch(mType)
	{
	case TypeValue_Undefined:		result = "<undefined>";						break;
	case TypeValue_Integer:			result = "<integer>";						break;
	case TypeValue_Float:			result = "<float>";							break;
	case TypeValue_Bool:			result = "<boolean>";						break;
	case TypeValue_String:			result = "<string>";						break;
	case TypeValue_DataRef:			result = "<" + mDataName + ">";				break;
	case TypeValue_DataRefList:		result = "<List of " + mDataName + ">";		break;
	}

	return result;
}

//-----------------------------------------------------------------------------
// Operators
//-----------------------------------------------------------------------------

bool Type::operator == (const Type& type) const
{
	if (mType != type.mType) return false;

	if ((mType != TypeValue_DataRef) &&
		(mType != TypeValue_DataRefList))
	{
		return true;
	}

	return (mDataName == type.mDataName);
}

bool Type::operator != (const Type& type) const
{
	return !(*this == type);
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
