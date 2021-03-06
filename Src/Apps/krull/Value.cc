//-----------------------------------------------------------------------------
// Apps/krull/Value.cc
//
// Implementation of the Value class
//-----------------------------------------------------------------------------

#include "Krull.h"
#include "Value.h"
#include "Table.h"
#include "Data.h"

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
	K_ASSERT(type.GetType() == TypeValue_String || type.GetType() == TypeValue_RowName);
	mData.mString = new string(s);
}

Value::Value (const Type& type, const Data& data, size_t ref)
{
	K_ASSERT(type.GetType() == TypeValue_DataRef);
	(void)data;
	mData.mDataRef = ref;
}

Value::Value (const Type& type, const Data& data)
{
	K_ASSERT(type.GetType() == TypeValue_DataRefList);
	(void)data;
	mData.mDataRefList = new vector<size_t>();
}

Value::~Value ()
{
}

//-----------------------------------------------------------------------------
// Table references
//-----------------------------------------------------------------------------

void Value::AddDataRef (size_t ref)
{
	// We assume that the data holds a pointer to a vector of table references.
	// We have no way to protect against this as type information is not stored
	// with the data to save memory.  The calling code should do due diligence.
	K_ASSERT(mData.mDataRefList != 0);
	mData.mDataRefList->push_back(ref);
}

size_t Value::NumDataRefs () const
{
	K_ASSERT(mData.mDataRefList != 0);
	return mData.mDataRefList->size();
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
	case TypeValue_DataRef:
		mData.mInteger = 0;
		break;

	case TypeValue_Bool:
		mData.mBoolean = false;
		break;

	case TypeValue_String:
		delete mData.mString;
		mData.mString = 0;
		break;

	case TypeValue_DataRefList:
		delete mData.mDataRefList;
		mData.mDataRefList = 0;
		break;

	default:
		// We have an unknown type to clean up here
		K_ASSERT(0);
		break;
	}
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

