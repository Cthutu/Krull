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
class BackEnd;

class Compiler
{
public:
	Compiler ();
	~Compiler ();

	bool				Process				(const string& filename, BackEnd& backEnd);

	// State methods
	void				DebugParserOn		();
	void				VerboseOn			();

	// Status messages - used by other classes (e.g. Project)
	bool				ErrorArgs			(const Parser* parser, const char* errMsg, va_list args) const;
	bool				Error				(const Parser* parser, const char* errMsg, ...) const;
	void				InfoArgs			(const char* msg, va_list args) const;
	void				Info				(const char* msg, ...) const;
	void				StatusArgs			(const char* msg, va_list args) const;
	void				Status				(const char* msg, ...) const;

protected:
	bool				OpenFile			(const string& filename, Parser& parser);

	// Compilation utilities
	bool				ExpectToken			(Parser& parser, Token token);
	int					ExpectToken2		(Parser& parser, Token token1, Token token2);
	Token				NextToken			(Parser& parser, bool detectEOF = false);

	// Phrase compilation
	bool				ProcessUses			(Parser& parser);														// Process: uses ...
	bool				ProcessTable		(Parser& parser);														// Process: table <name> ( ... )
	bool				ProcessData			(Parser& parser);														// Process: data <name>: <name> ( ... )
	bool				ProcessEntry		(Parser& parser, const KTable& table, Data& data);						// Process: <name> ( ... ) within a data clause
	bool				ProcessField		(Parser& parser, const KTable& table, Data& data);						// Process: <value> within an data entry clause
	bool				AddDataField		(Parser& parser, Data& data, const Type& type, Value& value);

protected:
	bool				mDebugParser;
	bool				mVerbose;
	vector<string>		mFilesParsed;
	Project*			mProject;
};

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
