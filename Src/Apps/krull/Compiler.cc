//-----------------------------------------------------------------------------
// Apps/krull/Compiler.cc
//
// Krull Compiler
//-----------------------------------------------------------------------------

#include "Krull.h"
#include "Compiler.h"
#include "Filename.h"
#include "Project.h"

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
	Status("Compiling %s...", fullName.c_str());

	// Load file
	fstream f;
	f.open(fullName.c_str(), ios::in | ios::binary);
	if (!f)
	{
		Error(parser, "Cannot open '%s'", fullName.c_str());
		return false;
	}
	f.seekg(0, ios::end);
	unsigned int fileSize = f.tellg();
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

bool Compiler::ErrorArgs (const Parser& parser, const char* errMsg, va_list args)
{
	char errorBuffer [1024];
	_vsnprintf_s(errorBuffer, 1024, errMsg, args);

	cerr << parser.GetFileName() << " (" << parser.GetLine() << "): ";
	cerr << errorBuffer << endl;

	return false;
}

bool Compiler::Error (const Parser& parser, const char* errMsg, ...)
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

void Compiler::StatusArgs (const char* msg, va_list args)
{
	if (mVerbose)
	{
		char msgBuffer [1024];
		_vsnprintf_s(msgBuffer, 1024, msg, args);

		cout << "--> " << msgBuffer << endl;
	}
}

void Compiler::Status (const char* msg, ...)
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

void Compiler::InfoArgs (const char* msg, va_list args)
{
	char msgBuffer [1024];
	_vsnprintf_s(msgBuffer, 1024, msg, args);

	cout << "Info: " << msgBuffer << endl;
}

void Compiler::Info (const char* msg, ...)
{
	va_list args;
	va_start(args, msg);
	StatusArgs(msg, args);
	va_end(args);
}

//-----------------------------------------------------------------------------
// Main entry point for compiler
//-----------------------------------------------------------------------------

bool Compiler::Process(const string& filename)
{
	// Parse the buffer
	Parser parser;

	if (!OpenFile(filename, parser))
	{
		cout << "Cannot open '" << filename << "'" << endl;
		return false;
	}

	// Initialise the project
	string projectName = FileName::ExtractRootName(filename);
	Status("Initialising project '%s'", projectName.c_str());
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

		default:
			Error(parser, "Syntax error, '%s' found", parser.ShortDesc().c_str());
			errorFound = true;
			break;
		}
	}
	while(!errorFound && !Parser::IsEOF(token));

	//
	// Invoke the back-end
	//
	if (!errorFound)
	{

	}

	//
	// Clean up
	//
	delete mProject;
	mProject = 0;

	return true;
}

//-----------------------------------------------------------------------------
// Compiler utilties
//-----------------------------------------------------------------------------

bool Compiler::ExpectToken (Parser& parser, Token token)
{
	Token nextToken = NextToken(parser);

	if (nextToken != token)
	{
		return Error(parser, "Syntax error, %s expected but '%s' found", Parser::ShortDesc(token),
			parser.ShortDesc());
	}

	return true;
}

Token Compiler::NextToken (Parser& parser)
{
	Token token = parser.Next();
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
		return Error(parser, "Invalid uses syntax, '%s' found", parser.ShortDesc().c_str());
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
		Table& table = mProject->NewTable(parser.GetString());

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
					return Error(parser, "Table '%s' has no fields", table.GetName());
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
				default:
					return Error(parser, "Syntax error, field type expected, found '%s'", parser.ShortDesc());
				}

				// Expect field name
				token = NextToken(parser);
				if (Token_Name == token)
				{
					fieldName = parser.GetString();

					if (!table.AddField(fieldName, fieldType))
					{
						return Error(parser, "Duplicate field found.  There are more than one field called '%s'", parser.ShortDesc());
					}
				}
				else
				{
					return Error(parser, "Syntax error, field name expected, found '%s'", parser.ShortDesc());
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
