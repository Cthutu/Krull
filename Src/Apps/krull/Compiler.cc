//-----------------------------------------------------------------------------
// Apps/krull/Compiler.cc
//
// Krull Compiler
//-----------------------------------------------------------------------------

#include "Krull.h"
#include "Compiler.h"
#include "Parser.h"

#include <fstream>

using namespace std;

//-----------------------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------------------

Compiler::Compiler ()
: mDebugParser(false)
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
	// Load file
	fstream f;
	f.open(filename.c_str(), ios::in | ios::binary);
	if (!f)
	{
		printf("Krull: ERROR: Cannot open '%s'", filename);
		return false;
	}
	f.seekg(0, ios::end);
	unsigned int fileSize = f.tellg();
	f.seekg(0);
	char* buffer = new char [fileSize];
	f.read(buffer, fileSize);
	f.close();

	parser.Start(buffer, fileSize);

	return true;
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
	}
	while(!Parser::IsEOF(token));

	return true;
}

//-----------------------------------------------------------------------------
// State methods
//-----------------------------------------------------------------------------

void Compiler::DebugParserOn()
{
	mDebugParser = true;
}