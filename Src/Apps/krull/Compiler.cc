//-----------------------------------------------------------------------------
// Apps/krull/Compiler.cc
//
// Krull Compiler
//-----------------------------------------------------------------------------

#include "Krull.h"
#include "Compiler.h"
#include "Filename.h"
#include "Project.h"
#include "Table.h"
#include "Data.h"
#include "Value.h"
#include "BackEnd.h"

#include <fstream>
#include <iostream>

using namespace std;

//-----------------------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------------------

Compiler::Compiler ()
: mDebugParser(false)
, mVerbose(false)
{

}

//-----------------------------------------------------------------------------
// Destructor
//-----------------------------------------------------------------------------

Compiler::~Compiler ()
{

}

//-----------------------------------------------------------------------------
// File opening
//-----------------------------------------------------------------------------

bool Compiler::OpenFile (const string& filename, Parser& parser)
{
	// Normalise the filename
	string fullName = FileName::NormaliseFileName(filename);
	if (find(mFilesParsed.begin(), mFilesParsed.end(), fullName) != mFilesParsed.end())
	{
		// We've already opened and parsed this file, so pretend that it's empty
		return true;
	}

	// Verbose message
	Status("[COMPILE] Compiling %s...", fullName.c_str());

	// Load file
	fstream f;
	f.open(fullName.c_str(), ios::in | ios::binary);
	if (!f)
	{
		Error(&parser, "Cannot open '%s'", fullName.c_str());
		return false;
	}
	f.seekg(0, ios::end);
	unsigned int fileSize = (unsigned int)f.tellg();
	f.seekg(0);
	char* buffer = new char [fileSize];
	f.read(buffer, fileSize);
	f.close();

	mFilesParsed.push_back(fullName);

	parser.Start(fullName, buffer, fileSize);

	return true;
}

//-----------------------------------------------------------------------------
// Error handling
//-----------------------------------------------------------------------------

bool Compiler::ErrorArgs (const Parser* parser, const char* errMsg, va_list args) const
{
	char errorBuffer [1024];
	_vsnprintf_s(errorBuffer, 1024, errMsg, args);

	if (parser && parser->HasState())
	{
		cerr << parser->GetFileName() << " (" << parser->GetLine() << "): ";
	}
	else
	{
		cerr << "Error: ";
	}
	cerr << errorBuffer << endl;

	return false;
}

bool Compiler::Error (const Parser* parser, const char* errMsg, ...) const
{
	va_list args;
	va_start(args, errMsg);
	ErrorArgs(parser, errMsg, args);
	va_end(args);
	return false;
}

//-----------------------------------------------------------------------------
// Status messages
// Only switched on if mVerbose is true
//-----------------------------------------------------------------------------

void Compiler::StatusArgs (const char* msg, va_list args) const
{
	if (mVerbose)
	{
		char msgBuffer [1024];
		_vsnprintf_s(msgBuffer, 1024, msg, args);

		cout << "--> " << msgBuffer << endl;
	}
}

void Compiler::Status (const char* msg, ...) const
{
	if (mVerbose)
	{
		va_list args;
		va_start(args, msg);
		StatusArgs(msg, args);
		va_end(args);
	}
}

//-----------------------------------------------------------------------------
// Info messages
//-----------------------------------------------------------------------------

void Compiler::InfoArgs (const char* msg, va_list args) const
{
	char msgBuffer [1024];
	_vsnprintf_s(msgBuffer, 1024, msg, args);

	cout << "Info: " << msgBuffer << endl;
}

void Compiler::Info (const char* msg, ...) const
{
	va_list args;
	va_start(args, msg);
	StatusArgs(msg, args);
	va_end(args);
}

//-----------------------------------------------------------------------------
// Main entry point for compiler
//-----------------------------------------------------------------------------

bool Compiler::Process(const string& filename, BackEnd& backEnd)
{
	// Parse the buffer
	Parser parser;

	if (!OpenFile(filename, parser))
	{
		return false;
	}

	// Initialise the project
	string projectName = FileName::ExtractRootName(filename);
	Status("[PROJECT] Initialising project '%s'", projectName.c_str());
	mProject = new Project (*this, projectName, FileName::ExtractPath(filename));

	Token token;
	bool errorFound = false;
	do
	{
		token = NextToken(parser);

		//
		// Main statement junction
		//
		switch(token)
		{
		case Token_Uses:
			if (!ProcessUses(parser)) errorFound = true;
			break;

		case Token_Table:
			if (!ProcessTable(parser)) errorFound = true;
			break;

		case Token_Data:
			if (!ProcessData(parser)) errorFound = true;
			break;

		default:
			if (token != Token_EOF)
			{
				Error(&parser, "Syntax error, '%s' found", parser.ShortDesc().c_str());
				errorFound = true;
			}
			break;
		}
	}
	while(!errorFound && !Parser::IsEOF(token));

	//
	// Invoke the back-end
	//
	if (!errorFound)
	{
		string outName = FileName::ReplaceExtension(filename, backEnd.GetExtension());
		Status("[SQLITE]  Building '%s'", outName.c_str());
		errorFound = !backEnd.Build(outName, *this, *mProject);
	}

	//
	// Clean up
	//
	delete mProject;
	mProject = 0;

	Status(errorFound
		? "[COMPILE] Error while compiling '%s'"
		: "[COMPILE] Finished compiling '%s'"
		, filename.c_str());
	return errorFound;
}

//-----------------------------------------------------------------------------
// Compiler utilties
//-----------------------------------------------------------------------------

bool Compiler::ExpectToken (Parser& parser, Token token)
{
	Token nextToken = NextToken(parser);

	if (nextToken != token)
	{
		return Error(&parser, "Syntax error, expected '%s' but found '%s'", Parser::ShortDesc(token).c_str(),
			parser.ShortDesc().c_str());
	}

	return true;
}

int Compiler::ExpectToken2 (Parser& parser, Token token1, Token token2)
{
	Token nextToken = NextToken(parser);
	
	if (nextToken != token1 && nextToken != token2)
	{
		return Error(&parser, "Syntax error, expected '%s' or '%s' but found '%s'", 
					 Parser::ShortDesc(token1).c_str(),
					 Parser::ShortDesc(token2).c_str(),
					 parser.ShortDesc().c_str());
	}
	
	return
		(nextToken == token1)
			? 1
			: (nextToken == token2)
				? 2
				: 0;
}

Token Compiler::NextToken (Parser& parser, bool detectEOF)
{
	Token token = parser.Next(detectEOF);
	if (mDebugParser) parser.Describe();
	return token;
}

//-----------------------------------------------------------------------------
// Uses clause
// Syntax:
//		uses name
//	or
//		uses "path"
//-----------------------------------------------------------------------------

bool Compiler::ProcessUses (Parser& parser)
{
	Token token = NextToken(parser);

	string usesFileName;

	if (Token_Name == token)
	{
		// Construct a path name from <current file path>\<name>.k
		usesFileName = FileName::ExtractPath(parser.GetFileName()) + parser.GetString() + ".k";
	}
	else if (Token_LiteralString == token)
	{
		// Construct a path from the actual string
		usesFileName = parser.GetString();
		if (FileName::ExtractExtension(usesFileName) == string())
		{
			usesFileName += ".k";
		}
	}
	else
	{
		return Error(&parser, "Invalid uses syntax, '%s' found", parser.ShortDesc().c_str());
	}

	if (!FileName::IsFullPath(usesFileName))
	{
		usesFileName = FileName::ExtractPath(parser.GetFileName()) + usesFileName;
		usesFileName = FileName::NormaliseFileName(usesFileName);
	}

	OpenFile(usesFileName, parser);

	return true;
}

//-----------------------------------------------------------------------------
// Table clause
// Syntax:
//		table <name> ( [<field type> <field name>]+ )
//-----------------------------------------------------------------------------

bool Compiler::ProcessTable (Parser& parser)
{
	if (ExpectToken(parser, Token_Name))
	{
		KTable& table = mProject->NewTable(parser.GetString());
		Type rowNameType;
		rowNameType.SetType(TypeValue_RowName);
		table.AddField("krull_id", rowNameType);

		if (!ExpectToken(parser, Token_ListOpen))
		{
			return false;
		}

		for(;;)
		{
			Token token = NextToken(parser);
			if (Token_ListClose == token)
			{
				if (table.NumFields() == 0)
				{
					return Error(&parser, "Table '%s' has no fields", table.GetName().c_str());
				}
				
				// Table definition complete
				return true;
			}
			else
			{
				// Process possible field
				Type fieldType;
				string fieldName;

				// Expect field type
				// Can be:
				//		string
				//		int
				//		float
				//		bool
				//
				switch(token)
				{
				case Token_String:		fieldType.SetType(TypeValue_String);			break;
				case Token_Int:			fieldType.SetType(TypeValue_Integer);			break;
				case Token_Float:		fieldType.SetType(TypeValue_Float);				break;
				case Token_Bool:		fieldType.SetType(TypeValue_Bool);				break;
				case Token_Name:
					{
						fieldType.SetType(TypeValue_DataRef);
						string dataName = parser.GetString();
						if (!mProject->HasData(dataName))
						{
							return Error(&parser, "Unknown data definition '%s'", dataName.c_str());
						}
						fieldType.SetDataName(dataName);
					}
					break;
										
				default:
					return Error(&parser, "Syntax error, field type expected, found '%s'", parser.ShortDesc().c_str());
				}

				// Expect field name or *
				for(;;)
				{
					token = NextToken(parser);
					if (Token_Name == token)
					{
						fieldName = parser.GetString();

						if (!table.AddField(fieldName, fieldType))
						{
							return Error(&parser, "Duplicate field found.  There are more than one field called '%s'", parser.ShortDesc().c_str());
						}

						break;
					}
					else if (Token_Star == token)
					{
						if (fieldType.GetType() != TypeValue_DataRef)
						{
							return Error(&parser, "Can only have list of data entries, found attempt to have list of %s", fieldType.ShortDesc().c_str());
						}
						fieldType.SetList();
					}
					else
					{
						return Error(&parser, "Syntax error, field name expected, found '%s'", parser.ShortDesc().c_str());
					}
				}
			}
		}
		
	}
	else
	{
		// No table name
		Info("Table definition has no name");
		return false;
	}
}

//-----------------------------------------------------------------------------
// Data clause
// Syntax:
//		data <name> : <table name> ( <entry>* )
//
// <entry>:
//		<name> ( <data>* )
//
// <data>:
//		<integer> ||
//		<float> ||
//		"<literal string>" ||
//		true || false ||
//		<name>
//		( <data>* )
//
//-----------------------------------------------------------------------------

bool Compiler::ProcessData (Parser& parser)
{
	if (ExpectToken(parser, Token_Name))
	{
		string dataName = parser.GetString();

		if (!ExpectToken(parser, Token_Colon)) return false;

		if (ExpectToken(parser, Token_Name))
		{
			string tableName = parser.GetString();
			if (!mProject->HasTable(tableName))
			{
				return Error(&parser, "Unknown table name '%s'", tableName.c_str());
			}
			KTable& table = mProject->GetTable(tableName);
			Data& data = mProject->NewData(dataName, table);

			if (!ExpectToken(parser, Token_ListOpen)) return false;

			Token token = NextToken(parser);
			while (token != Token_ListClose)
			{
				if (!ProcessEntry(parser, table, data)) return false;
				token = NextToken(parser);
			}

			return true;
		}
		else
		{
			Info("Data definition has no table type");
			return false;
		}
	}
	else
	{
		Info("Data definition has no name");
		return false;
	}
}

bool Compiler::ProcessEntry (Parser& parser, const KTable& table, Data& data)
{
	if (parser.GetToken() == Token_Name)
	{
		Data::Status status;
		if (Data::Data_OK != (status = data.StartEntry(parser.GetString())))
		{
			switch(status)
			{
			case Data::Data_Error_DuplicateName:
				return Error(&parser, "Data entry has duplicate name");
			default:
				K_ASSERT(0);
				break;
			}
			return false;
		}
		Status("[DATA]    Processing entry '%s'...", parser.GetString().c_str());
		
		int entryType = ExpectToken2(parser, Token_ListOpen, Token_Colon);
		if (!entryType) return false;

		Token token = NextToken(parser, (entryType == 2));
		while (token != (entryType == 1 ? Token_ListClose : Token_EOF))
		{
			if (!ProcessField(parser, table, data)) return false;
			token = NextToken(parser, (entryType == 2));
		}

		// Check that we have enough data given
		size_t numFieldsInTable = table.GetNumFields();
		size_t numFieldsInData = data.GetCurrentFieldIndex();
		if (numFieldsInData != numFieldsInTable)
		{
			return Error(&parser, "Incorrect number of fields for data entry '%s'.  Expected %d entries", data.GetName().c_str(), numFieldsInTable);
		}
	}
	return true;
}

bool Compiler::ProcessField (Parser& parser, const KTable& table, Data& data)
{
	Token token = parser.GetToken();

	// Work out the type defined in the table
	size_t fieldIndex = data.GetCurrentFieldIndex();
	Type currentType = table.GetFieldType(fieldIndex);

	switch(token)
	{
	case Token_Integer:
		if (currentType.GetType() == TypeValue_Integer)
		{
			Value intValue (currentType, parser.GetInteger());
			AddDataField(parser, data, currentType, intValue);
		}
		else
		{
			return Error(&parser, "Integer found, expected %s", currentType.ShortDesc().c_str());
		}
		break;

	case Token_Name:
		if (currentType.GetType() == TypeValue_DataRef)
		{
			string name = parser.GetString();
			Data& lookupData = mProject->GetData(currentType.GetDataName());
			size_t entryRef = lookupData.GetEntryRef(name);

			if (entryRef == (size_t)-1)
			{
				// We could not find the entry
				return Error(&parser, "Unknown data reference '%s' in data '%s'",
					name.c_str(), currentType.GetDataName().c_str());
			}

			Value dataRefValue (currentType, lookupData, entryRef);
			AddDataField(parser, data, currentType, dataRefValue);
		}
		else
		{
			return Error(&parser, "Name found, expected %s", currentType.ShortDesc().c_str());
		}
		break;

	case Token_LiteralString:
		if (currentType.GetType() == TypeValue_String)
		{
			string str = parser.GetString();
			Value strValue = Value(currentType, str);
			AddDataField(parser, data, currentType, strValue);
		}
		else
		{
			return Error(&parser, "String found, expected %s", currentType.ShortDesc().c_str());
		}
		break;

	case Token_True:
		if (currentType.GetType() == TypeValue_Bool)
		{
			Value boolValue = Value(currentType, true);
			AddDataField(parser, data, currentType, boolValue);
		}
		else
		{
			return Error(&parser, "Boolean found, expected %s", currentType.ShortDesc().c_str());
		}
		break;

	case Token_False:
		if (currentType.GetType() == TypeValue_Bool)
		{
			Value boolValue = Value(currentType, false);
			AddDataField(parser, data, currentType, boolValue);
		}
		else
		{
			return Error(&parser, "Boolean found, expected %s", currentType.ShortDesc().c_str());
		}
		break;

	case Token_ListOpen:
		if (currentType.GetType() == TypeValue_DataRefList)
		{
			// Process a list of data references
			Data& lookupData = mProject->GetData(currentType.GetDataName());
			Value value (currentType, lookupData);

			Token token = NextToken(parser);
			while (token != Token_ListClose)
			{
				if (Token_Name == token)
				{
					// This is a data reference.  Look it up and confirm it is valid
					string name = parser.GetString();
					size_t entryRef = lookupData.GetEntryRef(name);

					if (entryRef == (unsigned int)-1)
					{
						// We could not find the entry
						return Error(&parser, "Unknown data reference '%s' in data '%s'",
							name.c_str(), currentType.GetDataName().c_str());
					}

					// Add the entry reference to the list
					value.AddDataRef(entryRef);
				}
				else
				{
					return Error(&parser, "Syntax error, expecting data reference name or ')', found '%s'", parser.ShortDesc().c_str());
				}
				token = NextToken(parser);
			}

			AddDataField(parser, data, currentType, value);
		}
		else
		{
			return Error(&parser, "Data reference list found, expected %s", currentType.ShortDesc().c_str());
		}
		break;

	default:
		return Error(&parser, "Syntax error in data entry definition");
	}

	return true;
}

bool Compiler::AddDataField (Parser& parser, Data& data, const Type& type, Value& value)
{
	Data::Status status;

	status = data.AddField(type, value);
	if (Data::Data_OK != status)
	{
		switch(status)
		{
		case Data::Data_Error_InvalidType:
			return Error(&parser, "Entry parameter %d is an invalid type, expecting %s", data.GetCurrentFieldIndex(), type.ShortDesc().c_str());

		case Data::Data_Error_TooMuchData:
			return Error(&parser, "Too many parameters in data entry, should be only %d", data.GetTable().GetNumFields());
				
		default:
			// Should never reach here!  But we add this to stop compiler warnings
			return false;
		}
	}

	return true;
}

//-----------------------------------------------------------------------------
// State methods
//-----------------------------------------------------------------------------

void Compiler::DebugParserOn()
{
	mDebugParser = true;
}

void Compiler::VerboseOn ()
{
	mVerbose = true;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
