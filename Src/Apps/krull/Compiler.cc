//-----------------------------------------------------------------------------
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
// Main entry point for compiler
//-----------------------------------------------------------------------------

bool Compiler::Process(const std::string& filename)
{
	// Load file
	fstream f;
	f.open(filename.c_str(), ios::in);
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
	
	// Parse the buffer
	Parser parser (buffer, fileSize);

	Token token;
	do
	{
		token = parser.Next();
		if (mDebugParser) parser.Describe();
	}
	while(!Parser::IsEOF(token));

	delete [] buffer;

	return true;
}

//-----------------------------------------------------------------------------
// State methods
//-----------------------------------------------------------------------------

void Compiler::DebugParserOn()
{
	mDebugParser = true;
}