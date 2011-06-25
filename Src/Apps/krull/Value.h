//-----------------------------------------------------------------------------
// Apps/krull/Value.h
//
// Holds a single value for a field in a data entry
//
// Note that Value doesn't hold the value type.  That is because it is used
// in the Data clause that stores a Table reference.  This keeps the actual
// size of a value to its minimum.
//-----------------------------------------------------------------------------

#pragma once

class Type;
class Data;

class Value
{
public:
	Value (const Type& type, int i);									// For an int
	Value (const Type& type, float f);									// For a float
	Value (const Type& type, bool b);									// For a bool
	Value (const Type& type, const string& s);							// For a string
	Value (const Type& type, const Data& data, size_t ref);				// For a Data
	Value (const Type& type, const Data& data);							// For a Data*
	~Value ();

	void					AddDataRef			(size_t ref);			// For a Data*
	size_t					NumDataRefs			() const;

	// Clean up.  Data instances will end up owning Values and as such will
	// call Clean() to deallocate any used memory.
	void					Clean				(const Type& type);						// Deallocate memory usage

	// Get functions
	int						GetInteger			() const		{ return mData.mInteger; }
	float					GetFloat			() const		{ return mData.mFloat;}
	bool					GetBool				() const		{ return mData.mBoolean; }
	const string&			GetString			() const		{ return *mData.mString; }
	size_t					GetDataRef			() const		{ return mData.mDataRef; }
	vector<size_t>&			GetDataRefList		() const		{ return *mData.mDataRefList; }

private:
	union Values
	{
		int						mInteger;			// For 'int' definitions
		float					mFloat;				// For 'float' definitions
		bool					mBoolean;			// For 'bool' definitions
		string*					mString;			// For 'string' definitions
		size_t					mDataRef;			// For 'Table' definitions
		vector<size_t>*			mDataRefList;		// For 'Table*' definitions
	};

	Values			mData;
};