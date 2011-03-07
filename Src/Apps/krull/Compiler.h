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
};

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
