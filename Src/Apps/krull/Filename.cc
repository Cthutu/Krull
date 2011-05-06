//-----------------------------------------------------------------------------
// Apps/krull/Filename.cc
//
// Implementation of the file name utilities
//-----------------------------------------------------------------------------

#include "Krull.h"

namespace FileName
{

	//-----------------------------------------------------------------------------
	// Win32 versions
	//-----------------------------------------------------------------------------

#if K_WIN32

	string NormaliseFileName (const std::string& filename)
	{
		char fullName [MAX_PATH+1];
		GetFullPathNameA(filename.c_str(), MAX_PATH, fullName, 0);

		return string(fullName);
	}

	string ExtractPath (const std::string& normalisedFileName)
	{
		unsigned len = normalisedFileName.length();
		unsigned i;

		for (i = len; i > 0; --i)
		{
			if (normalisedFileName[i-1] == '\\') break;
		}
		if (0 == i)
		{
			return string();
		}

		return normalisedFileName.substr(0, i);
	}

	string ExtractExtension (const std::string& fileName)
	{
		unsigned len = fileName.length();
		unsigned i;

		for (i = len; i > 0; --i)
		{
			if (fileName[i-1] == '.') break;
		}
		if (0 == i)
		{
			return string();
		}

		return fileName.substr(i, len - i);
	}

	bool IsFullPath (const std::string& fileName)
	{
		return (fileName.length() > 2) && (fileName[1] == ':');
	}

	//-----------------------------------------------------------------------------
	// Unknown platform
	//-----------------------------------------------------------------------------

#else
#	error "Please implement the file name utilities for your platform
#endif

}