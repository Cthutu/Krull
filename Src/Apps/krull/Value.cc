//-----------------------------------------------------------------------------
// Apps/krull/Value.cc
//
// Implementation of the Value class
//-----------------------------------------------------------------------------

#include "Krull.h"
#include "Value.h"
#include "Type.h"
#include "Table.h"

//-----------------------------------------------------------------------------
// Value constructors
//-----------------------------------------------------------------------------

Value::Value (const Type& type, int i)
{
	K_ASSERT(type.GetType() == TypeValue_Integer);
	mData.mInteger = i;
}

Value::Value (const Type& type, float f)
{
	K_ASSERT(type.GetType() == TypeValue_Float);
	mData.mFloat = f;
}

Value::Value (const Type& type, bool b)
{
	K_ASSERT(type.GetType() == TypeValue_Bool);
	mData.mBoolean = b;
}

Value::Value (const Type& type, const string& s)
{
	K_ASSERT(type.GetType() == TypeValue_String);
	mData.mString = new string(s);
}

Value::Value (const Type& type, const Table& table, unsigned int ref)
{
	K_ASSERT(type.GetType() == TypeValue_TableRef);
	(void)table;
	mData.mTableRef = ref;
}

Value::Value (const Type& type, const Table& table)
{
	K_ASSERT(type.GetType() == TypeValue_TableRefList);
	(void)table;
	mData.mTableRefList = new vector<unsigned int>();
}

Value::~Value ()
{
	K_ASSERT(mData.mInteger == 0);
}

//-----------------------------------------------------------------------------
// Table references
//-----------------------------------------------------------------------------

void Value::AddTableRef (unsigned int ref)
{
	// We assume that the data holds a pointer to a vector of table references.
	// We have no way to protect against this as type information is not stored
	// with the data to save memory.  The calling code should do due diligence.
	K_ASSERT(mData.mTableRefList != 0);
	mData.mTableRefList->push_back(ref);
}

unsigned int Value::NumTableRefs () const
{
	K_ASSERT(mData.mTableRefList != 0);
	return mData.mTableRefList->size();
}

//-----------------------------------------------------------------------------
// Clean up
//-----------------------------------------------------------------------------

void Value::Clean (const Type& type)
{
	switch(type.GetType())
	{
	case TypeValue_Integer:
	case TypeValue_Float:
	case TypeValue_TableRef:
		mData.mInteger = 0;
		break;

	case TypeValue_Bool:
		mData.mBoolean = false;
		break;

	case TypeValue_String:
		delete mData.mString;
		mData.mString = 0;
		break;

	case TypeValue_TableRefList:
		delete mData.mTableRefList;
		mData.mTableRefList = 0;
		break;

	default:
		// We have an unknown type to clean up here
		K_ASSERT(0);
		break;
	}
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

