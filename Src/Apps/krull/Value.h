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
class Table;

class Value
{
public:
	Value (const Type& type, int i);									// For an int
	Value (const Type& type, float f);									// For a float
	Value (const Type& type, bool b);									// For a bool
	Value (const Type& type, const string& s);							// For a string
	Value (const Type& type, const Table& table, unsigned int ref);		// For a Table
	Value (const Type& type, const Table& table);						// For a Table*
	~Value ();

	void			AddTableRef			(unsigned int ref);						// For a Table*
	unsigned int	NumTableRefs		() const;
	void			Clean				(const Type& type);						// Deallocate memory usage

private:
	union Data
	{
		int						mInteger;			// For 'int' definitions
		float					mFloat;				// For 'float' definitions
		bool					mBoolean;			// For 'bool' definitions
		string*					mString;			// For 'string' definitions
		unsigned int			mTableRef;			// For 'Table' definitions
		vector<unsigned int>*	mTableRefList;		// For 'Table*' definitions
	};

	Data			mData;
};