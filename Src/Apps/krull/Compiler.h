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

	bool				Process				(const std::string& filename);

	// State methods
	void				DebugParserOn		();
	void				VerboseOn			();

protected:
	bool				OpenFile			(const std::string& filename, Parser& parser);
	bool				ErrorArgs			(const Parser& parser, const char* errMsg, va_list args);
	bool				Error				(const Parser& parser, const char* errMsg, ...);

	// Phrase compilation
	bool				ProcessUses			(Parser& parser);

protected:
	bool				mDebugParser;
	bool				mVerbose;
	vector<string>		mFilesParsed;
};

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
