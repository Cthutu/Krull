//-----------------------------------------------------------------------------
// Apps/krull/Compiler.cc
//
// Krull Compiler
//-----------------------------------------------------------------------------

#include "Krull.h"
#include "Compiler.h"
#include "Parser.h"
#include "Filename.h"

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

bool Compiler::OpenFile (const std::string& filename, Parser& parser)
{
	// Normalise the filename
	string fullName = FileName::NormaliseFileName(filename);
	if (find(mFilesParsed.begin(), mFilesParsed.end(), fullName) != mFilesParsed.end())
	{
		// We've already opened and parsed this file, so pretend that it's empty
		return true;
	}

	// Verbose message
	if (mVerbose)
	{
		cout << "Compiling " << fullName << "..." << endl;
	}

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

	cerr << parser.GetFileName().c_str() << " (" << parser.GetLine() << "): ";
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
// Main entry point for compiler
//-----------------------------------------------------------------------------

bool Compiler::Process(const std::string& filename)
{
	// Parse the buffer
	Parser parser;

	if (!OpenFile(filename, parser))
	{
		return false;
	}

	Token token;
	do
	{
		token = parser.Next();
		if (mDebugParser) parser.Describe();

		//
		// Main statement junction
		//
		switch(token)
		{
		case Token_Uses:
			if (!ProcessUses(parser)) return false;
			break;

		default:
			Error(parser, "Syntax error, '%s' found", parser.ShortDesc().c_str());
			return false;
		}
	}
	while(!Parser::IsEOF(token));

	return true;
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
	Token token = parser.Next();
	if (mDebugParser) parser.Describe();

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
