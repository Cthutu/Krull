//-----------------------------------------------------------------------------
// Apps/krull/Parser.h
//
// Krull parser
//-----------------------------------------------------------------------------

#pragma once

enum Token
{
	// Errors
	Token_ERROR_Unknown		= -100,
	Token_ERROR_Overflow,
	Token_ERROR_UnterminatedString,
	Token_ERROR_InvalidChar,

	Token_EOF				= 0,

	// Token types
	Token_Integer,
	Token_Name,
	Token_LiteralString,

	// Operators
	TOKEN_OPERATORS,
	Token_ListOpen,			// (
	Token_ListClose,		// )
	Token_Dot,				// .
	Token_Colon,			// :
	Token_Star,				// *

	// Keywords
	TOKEN_KEYWORDS,
	Token_Uses,				// uses
	Token_Table,			// table
	Token_Data,				// data
	Token_String,			// string
	Token_Int,				// int
	Token_Float,			// float
	Token_Bool,				// bool
	Token_True,				// true
	Token_False,			// false

	TOKEN_COUNT
};

const int kNumOperators = TOKEN_KEYWORDS - TOKEN_OPERATORS - 1;
const int kNumKeyWords = TOKEN_COUNT - TOKEN_KEYWORDS - 1;

//-----------------------------------------------------------------------------
// Parser
//-----------------------------------------------------------------------------

class Parser
{
public:
	Parser ();
	~Parser ();

	void				Start		(const string& fileName, const char* buffer, unsigned size);
	void				End			();

	// Operations
	Token				Next		();
	static unsigned 	Hash		(const char* buffer, unsigned size, unsigned seed);
	void				Describe	() const;
	string				ShortDesc	() const;
	static string		ShortDesc	(Token token);

	// Attributes
	Token				GetToken	() const				{ return mToken; }
	string				GetFileName	() const				{ return GetState()->mFileName; }
	unsigned int		GetLine		() const				{ return GetState()->mLine; }
	string				GetString	() const				{ return string(mTokenStart, mTokenEnd); }
	int					GetInteger	() const				{ return mInteger; }

	// Queries
	static bool			IsInteger	(Token token)			{ return token == Token_Integer; }
	static bool			IsEOF		(Token token)			{ return token == Token_EOF; }
	static bool			IsOperator	(Token token)			{ return (token > TOKEN_OPERATORS) && (token < TOKEN_KEYWORDS); }
	static bool			IsKeyword	(Token token)			{ return (token > TOKEN_KEYWORDS) && (token < TOKEN_COUNT); }
	static bool			IsName		(Token token)			{ return token == Token_Name; }
	static bool			IsString	(Token token)			{ return token == Token_LiteralString; }
	
	bool				HasState	() const				{ return GetState() != 0; }

protected:
	char				NextChar	();
	void				UngetChar	();

	struct ParseState;
	ParseState*			GetState	()						{ return mParseStack.size() ? mParseStack[mParseStack.size() - 1] : 0; }
	const ParseState*	GetState	() const				{ return mParseStack.size() ? mParseStack[mParseStack.size() - 1] : 0; }

	struct ParseState
	{
		// Filename
		string			mFileName;

		// Buffer information
		const char*		mBuffer;
		const char*		mEnd;

		// Parser internal state
		const char*		mLastScan;
		unsigned int	mLastLine;

		// Scan information
		const char*		mScan;
		unsigned int	mLine;

		void Init (const string& fileName, const char* buffer, unsigned int size);
	};

private:
	// Token information
	const char*		mTokenStart;
	const char*		mTokenEnd;
	Token			mToken;
	unsigned int	mHash;
	int				mInteger;

	vector<ParseState*>	mParseStack;
};