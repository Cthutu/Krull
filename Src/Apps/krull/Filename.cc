//-----------------------------------------------------------------------------
// Apps/krull/Filename.cc
//
// Implementation of the file name utilities
//-----------------------------------------------------------------------------

#include "Krull.h"
#include "Filename.h"

namespace FileName
{

	//-----------------------------------------------------------------------------
	// Win32 versions
	//-----------------------------------------------------------------------------

#if K_WIN32

	string NormaliseFileName (const string& filename)
	{
		char fullName [MAX_PATH+1];
		GetFullPathNameA(filename.c_str(), MAX_PATH, fullName, 0);

		return string(fullName);
	}

	string ExtractPath (const string& normalisedFileName)
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

	string ExtractExtension (const string& fileName)
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

	string ExtractRootName (const string& fileName)
	{
		unsigned len = fileName.length();
		unsigned i;

		for (i = len; i > 0; --i)
		{
			if (fileName[i-1] == '\\') break;
		}
		
		unsigned start = i;

		for (; fileName[i] != '.' && fileName[i] != 0; ++i) ;

		return fileName.substr(start, i - start);
	}

	string ReplaceExtension (const string& fileName, const string& extension)
	{
		unsigned len = fileName.length();
		unsigned i;

		for (i = len; i > 0; --i)
		{
			if (fileName[i-1] == '.') break;
		}
		string result;
		if (0 == i)
		{
			result = fileName + ".";
		}
		else
		{
			result = fileName.substr(0, i);
		}

		result += extension;
		return result;
	}

	bool IsFullPath (const string& fileName)
	{
		return (fileName.length() > 2) && (fileName[1] == ':');
	}

	bool Delete (const string& fileName)
	{
		HRESULT hr = S_OK;
		if (!DeleteFileA(fileName.c_str()))
		{
			hr = GetLastError();
		}
		return (hr == S_OK) || (hr == ERROR_FILE_NOT_FOUND);			// ERROR CODE 2: Cannot find the file specified
	}
	
	//-----------------------------------------------------------------------------
	// Apple platforms
	//-----------------------------------------------------------------------------
	
#elif K_APPLE
	
	static char* realpath2 (const char* path, char* buff)
	{
		char* home;
		
		if (*path == '~' && (home = getenv("HOME")))
		{
			char buffer [PATH_MAX];
			return realpath(strcat(strcpy(buffer, home), path+1), buff);
		}
		else
		{
			return realpath(path, buff);
		}
	}
	
	string NormaliseFileName (const string& filename)
	{
		char fullName [PATH_MAX+1];
		int error = 0;
		if (realpath2(filename.c_str(), fullName))
		{		
			return string(fullName);
		}
		else
		{
			error = errno;
			return filename;
		}
	}
	
	string ExtractPath (const string& normalisedFileName)
	{
		size_t len = normalisedFileName.length();
		size_t i;
		
		for (i = len; i > 0; --i)
		{
			if (normalisedFileName[i-1] == '/') break;
		}
		if (0 == i)
		{
			return string();
		}
		
		return normalisedFileName.substr(0, i);
	}
	
	string ExtractExtension (const string& fileName)
	{
		size_t len = fileName.length();
		size_t i;
		
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
	
	string ExtractRootName (const string& fileName)
	{
		size_t len = fileName.length();
		size_t i;
		
		for (i = len; i > 0; --i)
		{
			if (fileName[i-1] == '/') break;
		}
		
		size_t start = i;
		
		for (; fileName[i] != '.' && fileName[i] != 0; ++i) ;
		
		return fileName.substr(start, i - start);
	}
	
	string ReplaceExtension (const string& fileName, const string& extension)
	{
		size_t len = fileName.length();
		size_t i;
		
		for (i = len; i > 0; --i)
		{
			if (fileName[i-1] == '.') break;
		}
		string result;
		if (0 == i)
		{
			result = fileName + ".";
		}
		else
		{
			result = fileName.substr(0, i);
		}
		
		result += extension;
		return result;
	}
	
	bool IsFullPath (const string& fileName)
	{
		return fileName[0] == '/';
	}
	
	bool Delete (const string& fileName)
	{
		if (unlink(fileName.c_str()))
		{
			return errno == ENOENT;
		}
		else
		{
			return true;
		}
	}
	
	//-----------------------------------------------------------------------------
	// Unknown platform
	//-----------------------------------------------------------------------------

#else
#	error "Please implement the file name utilities for your platform
#endif

}