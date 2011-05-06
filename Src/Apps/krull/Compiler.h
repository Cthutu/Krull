//-----------------------------------------------------------------------------
// Apps/krull/Compiler.h
//
// Krull compiler class
//-----------------------------------------------------------------------------

#pragma once

class Parser;

class Compiler
{
public:
	Compiler ();
	~Compiler ();

	bool		Process			(const std::string& filename);

	// State methods
	void		DebugParserOn	();

protected:
	bool		OpenFile		(const std::string& filename, Parser& parser);

protected:
	bool			mDebugParser;
	vector<string>	mFilesParsed;
};

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
