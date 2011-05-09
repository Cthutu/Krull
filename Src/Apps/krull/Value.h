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
	Value (const Type& type, const Data& data, unsigned int ref);		// For a Data
	Value (const Type& type, const Data& data);							// For a Data*
	~Value ();

	void			AddDataRef			(unsigned int ref);						// For a Data*
	unsigned int	NumDataRefs			() const;

	// Clean up.  Data instances will end up owning Values and as such will
	// call Clean() to deallocate any used memory.  However, creation of
	// Values will be outside the Data class and when ownership is passed to
	// a Data instance, Release() should be called.  This will not deallocate
	// any memory but will avoid the Value destructor from failing an assertion.
	void			Clean				(const Type& type);						// Deallocate memory usage
	void			Release				();

private:
	union Values
	{
		int						mInteger;			// For 'int' definitions
		float					mFloat;				// For 'float' definitions
		bool					mBoolean;			// For 'bool' definitions
		string*					mString;			// For 'string' definitions
		unsigned int			mTableRef;			// For 'Table' definitions
		vector<unsigned int>*	mTableRefList;		// For 'Table*' definitions
	};

	Values			mData;
};