//-----------------------------------------------------------------------------
// Krull Compiler
//-----------------------------------------------------------------------------

#pragma once

#include <string>

class Compiler
{
public:
	Compiler ();
	~Compiler ();

	bool		Process			(const std::string& filename);

	// State methods
	void		DebugParserOn	();

protected:
	bool		mDebugParser;
};

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
