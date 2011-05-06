//-----------------------------------------------------------------------------
// Apps/krull/Compiler.h
//
// Krull compiler class
//-----------------------------------------------------------------------------

#pragma once

class Parser;
class Project;

class Compiler
{
public:
	Compiler ();
	~Compiler ();

	bool				Process				(const string& filename);

	// State methods
	void				DebugParserOn		();
	void				VerboseOn			();

protected:
	bool				OpenFile			(const string& filename, Parser& parser);
	bool				ErrorArgs			(const Parser& parser, const char* errMsg, va_list args);
	bool				Error				(const Parser& parser, const char* errMsg, ...);
	void				StatusArgs			(const char* msg, va_list args);
	void				Status				(const char* msg, ...);

	// Phrase compilation
	bool				ProcessUses			(Parser& parser);
	bool				ProcessTable		(Parser& parser);

protected:
	bool				mDebugParser;
	bool				mVerbose;
	vector<string>		mFilesParsed;
	Project*			mProject;
};

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
