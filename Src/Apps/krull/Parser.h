//-----------------------------------------------------------------------------
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
	Parser (const char* buffer, unsigned size);

	// Operations
	Token				Next		();
	static unsigned 	Hash		(const char* buffer, unsigned size, unsigned seed);
	void				Describe	();

	// Attributes

	// Queries
	static bool			IsInteger	(Token token)			{ return token == Token_Integer; }
	static bool			IsEOF		(Token token)			{ return token == Token_EOF; }
	static bool			IsOperator	(Token token)			{ return (token > TOKEN_OPERATORS) && (token < TOKEN_KEYWORDS); }
	static bool			IsKeyword	(Token token)			{ return (token > TOKEN_KEYWORDS) && (token < TOKEN_COUNT); }
	static bool			IsName		(Token token)			{ return token == Token_Name; }
	static bool			IsString	(Token token)			{ return token == Token_LiteralString; }

protected:
	char				NextChar	();
	void				UngetChar	();

private:
	// Buffer information
	const char*		mBuffer;
	const char*		mEnd;

	// Scan information
	const char*		mScan;
	unsigned int	mLine;

	// Token information
	const char*		mTokenStart;
	const char*		mTokenEnd;
	Token			mToken;
	unsigned int	mHash;
	int				mInteger;

	// Parser internal state
	const char*		mLastScan;
	unsigned int	mLastLine;

};