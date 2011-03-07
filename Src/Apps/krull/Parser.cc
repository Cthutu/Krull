//-----------------------------------------------------------------------------
// Krull parser implementation
//-----------------------------------------------------------------------------

#include "Krull.h"
#include "Parser.h"

#include <string.h>
#include <stdio.h>

//-----------------------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------------------

Parser::Parser (const char* buffer, unsigned size)
: mBuffer(buffer)
, mEnd(buffer + size)
, mScan(buffer)
, mLine(1)
, mTokenStart(0)
, mTokenEnd(0)
, mToken(Token_ERROR_Unknown)
, mHash(0)
, mInteger(0)
{

}

//-----------------------------------------------------------------------------
// Internal routines
//-----------------------------------------------------------------------------

char Parser::NextChar()
{
	char ch;

	mLastScan = mScan;
	mLastLine = mLastLine;

	// Check for end of file
	if (mScan == mEnd)
	{
		return 0;
	}

	ch = *mScan++;

	// Check for end of line:
	//		Windows:	\r\n
	//		Unix:		\n
	//		Mac:		\r
	//
	// Whichever version, only return \n
	if ((ch == '\r') || (ch == '\n'))
	{
		++mLine;
		if (ch == '\r')
		{
			// Check for possible following \n and skip it
			if ((mScan != mEnd) && (*mScan == '\n'))
			{
				++mScan;
			}

			ch = '\n';
		}
	}

	return ch;
}

void Parser::UngetChar()
{
	mScan = mLastScan;
	mLine = mLastLine;
}

//-----------------------------------------------------------------------------
// Parser tables
//-----------------------------------------------------------------------------

static const unsigned int gKeyWordHashes [] =
{
	/* 0 */		0,
	/* 1 */		0,
	/* 2 */		0,
	/* 3 */		Token_Table,
	/* 4 */		Token_String,
	/* 5 */		0,
	/* 6 */		0,
	/* 7 */		0,
	/* 8 */		0,
	/* 9 */		0,
	/* A */		0,
	/* B */		Token_Data,
	/* C */		0,
	/* D */		0,
	/* E */		0,
	/* F */		Token_Uses,
};

// Must be the same order as the KParserToken enum definition
static const char* gKeywords [kNumKeyWords + 1] =
{
	0,
	"4uses",
	"5table",
	"4data",
	"6string",
};

//-----------------------------------------------------------------------------
// Parser
//-----------------------------------------------------------------------------

Token Parser::Next ()
{
	char ch;

	for(;;)
	{
		// Grab the first non-whitespace character
		do
		{
			ch = NextChar();
			if (ch == 0) return Token_EOF;
		}
		while ((ch <= ' ') || (ch > 126));

		// Check for possible comments.
		// Comments start with '#' and end with an EOL
		if (ch == '#')
		{
			while ((ch != '\n') && (ch != 0))
			{
				ch = NextChar();
			}
			UngetChar();
		}
		else
		{
			break;
		}
	}

	mTokenStart = mScan - 1;

	//-----------------------------------------------------------------------------
	// Integer
	//-----------------------------------------------------------------------------
	
	if ((ch >= '0') && (ch <= '9'))
	{
		// Parse an integer
		int t = 0;

		mInteger = 0;
		while ((ch >= '0') && (ch <= '9'))
		{
			ch = NextChar();
			t *= 10;
			t += (ch - '0');
			if (t < mInteger)
			{
				// We've overflown the size of an int
				UngetChar();
				mInteger = 0;
				return (mToken = Token_ERROR_Overflow);
			}
			mInteger = t;
		}

		UngetChar();
		mTokenEnd = mScan;
		return (mToken = Token_Integer);
	}

	//-----------------------------------------------------------------------------
	// Names & keywords
	//-----------------------------------------------------------------------------

	else if (((ch >= 'a') && (ch <= 'z')) ||
		     ((ch >= 'A') && (ch <= 'Z')) ||
			 ((ch == '_')))
	{
		while (((ch >= 'a') && (ch <= 'z')) ||
			   ((ch >= 'A') && (ch <= 'Z')) ||
			   ((ch >= '0') && (ch <= '9')) ||
			   (ch == '_'))
		{
			ch = NextChar();
		}
		UngetChar();
		mTokenEnd = mScan;
		mHash = Hash(mTokenStart, mTokenEnd - mTokenStart, 0x12345678);

		// Determine if this is a keyword or not
		unsigned int tokens = gKeyWordHashes[mHash & 0x0f];
		unsigned int sizeToken = mTokenEnd - mTokenStart;
		while (tokens != 0)
		{
			int index = (tokens & 0xff) - TOKEN_KEYWORDS;
			tokens >>= 8;

			if ((*gKeywords[index] - '0') == sizeToken)
			{
				// The length of the tokens match
				if (strncmp(mTokenStart, gKeywords[index]+1, sizeToken) == 0)
				{
					// It is a keyword
					return (mToken = (Token)(TOKEN_KEYWORDS + index));
				}
			}
		}

		return (mToken = Token_Name);
	}

	//-----------------------------------------------------------------------------
	// Literal strings
	//-----------------------------------------------------------------------------

	else if (ch == '"')
	{
		mTokenStart = mScan;
		do
		{
			ch = NextChar();
		}
		while (ch != '"' && ch != '\n' && ch != 0);

		if (ch == '\n' || ch == 0)
		{
			// Unterminated string detected
			if (ch == '\n')
			{
				UngetChar();
			}
			mTokenEnd = mScan;
			return (mToken = Token_ERROR_UnterminatedString);
		}

		mTokenEnd = mScan - 1;		// Ignore the trailing quote
		mHash = Hash(mTokenStart, mTokenEnd - mTokenStart, 0x12345678);
		return (mToken = Token_LiteralString);
	}

	//-----------------------------------------------------------------------------
	// Operators
	//-----------------------------------------------------------------------------
	
	else if (ch == '.')
	{
		mTokenEnd = mScan;
		return (mToken = Token_Dot);
	}
	else if (ch == '(')
	{
		mTokenEnd = mScan;
		return (mToken = Token_ListOpen);
	}
	else if (ch == ')')
	{
		mTokenEnd = mScan;
		return (mToken = Token_ListClose);
	}
	else if (ch == ':')
	{
		mTokenEnd = mScan;
		return (mToken = Token_Colon);
	}
	else if (ch == '*')
	{
		mTokenEnd = mScan;
		return (mToken = Token_Star);
	}

	//-----------------------------------------------------------------------------
	// Error
	//-----------------------------------------------------------------------------

	else
	{
		mTokenEnd = mScan;
		return (mToken = Token_ERROR_InvalidChar);
	}
}

//-----------------------------------------------------------------------------
// Hash function
//-----------------------------------------------------------------------------

unsigned int Parser::Hash (const char* buffer, unsigned size, unsigned seed)
{
	// Based on the Murmur Hash function by Austin Appleby
	const unsigned int m = 0x5bd1e995;
	const int r = 24;
	unsigned int h = seed ^ size;
	const unsigned char* data = (const unsigned char*)buffer;

	while (size >= 4)
	{
		unsigned int k;

		k = data[0];
		k |= data[1] << 8;
		k |= data[2] << 16;
		k |= data[3] << 24;

		k *= m;
		k ^= k >> r;
		k *= m;

		h *= m;
		h ^= k;

		data += 4;
		size -= 4;
	}

	switch(size)
	{
	case 3:		h ^= data[2] << 16;
	case 2:		h ^= data[1] << 8;
	case 1:		h ^= data[0];
		h *= m;
	}

	h ^= h >> 13;
	h *= m;
	h ^= h >> 15;

	return h;
}

//-----------------------------------------------------------------------------
// Describe function
//-----------------------------------------------------------------------------

void Parser::Describe()
{
	printf("Krull: Parser: ");

	char* buffer = new char [mTokenEnd - mTokenStart + 1];
	strncpy(buffer, mTokenStart, mTokenEnd - mTokenStart);
	buffer[mTokenEnd - mTokenStart] = 0;

	printf("Token found [%s]: ", buffer);

	if (mToken < 0)
	{
		printf("ERROR");
	}
	else if (mToken == Token_EOF)
	{
		printf("End of file");
	}
	else if (mToken == Token_Integer)
	{
		printf("Integer: %d", mInteger);
	}
	else if (mToken == Token_Name)
	{
		printf("Name: (%08x)", mHash);
	}
	else if (mToken == Token_LiteralString)
	{
		printf("String: (%08x)", mHash);
	}
	else if (IsOperator(mToken))
	{
		const char* ops = "().:*";
		printf("Operator: '%c'", ops[mToken - TOKEN_OPERATORS - 1]);
	}
	else if (IsKeyword(mToken))
	{
		printf("Keyword: '%s'", gKeywords[mToken - TOKEN_KEYWORDS]+1);
	}
	else
	{
		printf("Unknown");
	}

	printf("\n");
	delete [] buffer;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
