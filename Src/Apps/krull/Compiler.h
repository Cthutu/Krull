//-----------------------------------------------------------------------------
// Apps/krull/Compiler.h
//
// Krull compiler class
//-----------------------------------------------------------------------------

#pragma once

#include "Parser.h"

class Project;
class KTable;
class Data;
class Type;
class Value;

class Compiler
{
public:
	Compiler ();
	~Compiler ();

	bool				Process				(const string& filename);

	// State methods
	void				DebugParserOn		();
	void				VerboseOn			();

	// Status messages - used by other classes (e.g. Project)
	void				StatusArgs			(const char* msg, va_list args);
	void				Status				(const char* msg, ...);

protected:
	bool				OpenFile			(const string& filename, Parser& parser);
	bool				ErrorArgs			(const Parser& parser, const char* errMsg, va_list args);
	bool				Error				(const Parser& parser, const char* errMsg, ...);
	void				InfoArgs			(const char* msg, va_list args);
	void				Info				(const char* msg, ...);

	// Compilation utilities
	bool				ExpectToken			(Parser& parser, Token token);
	Token				NextToken			(Parser& parser);

	// Phrase compilation
	bool				ProcessUses			(Parser& parser);
	bool				ProcessTable		(Parser& parser);
	bool				ProcessData			(Parser& parser);
	bool				ProcessEntry		(Parser& parser, const KTable& table, Data& data);
	bool				ProcessField		(Parser& parser, const KTable& table, Data& data);
	bool				AddDataField		(Parser& parser, Data& data, const Type& type, Value& value);

protected:
	bool				mDebugParser;
	bool				mVerbose;
	vector<string>		mFilesParsed;
	Project*			mProject;
};

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
