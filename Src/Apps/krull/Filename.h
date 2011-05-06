//-----------------------------------------------------------------------------
// Apps/krull/Filename.h
//
// Utilities processing filenames
//-----------------------------------------------------------------------------

#pragma once

namespace FileName
{

	//-----------------------------------------------------------------------------
	// Operations
	//-----------------------------------------------------------------------------
	
	string		NormaliseFileName		(const string& fileName);
	string		ExtractPath				(const string& normalisedFileName);
	string		ExtractExtension		(const string& fileName);
	string		ExtractRootName			(const string& fileName);
	
	//-----------------------------------------------------------------------------
	// Attributes
	//-----------------------------------------------------------------------------
	
	bool			IsFullPath				(const string& fileName);

	//-----------------------------------------------------------------------------
	//-----------------------------------------------------------------------------

}