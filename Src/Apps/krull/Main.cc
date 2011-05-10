//-----------------------------------------------------------------------------
// Apps/krull/Main.cc
//
// Main entry point for the Krull application
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Krull database generation language
//-----------------------------------------------------------------------------

#include "Krull.h"
#include "Compiler.h"
#include "Filename.h"
#include "SqliteBackEnd.h"

#if K_WIN32
#	include <crtdbg.h>
#endif

using namespace std;

//-----------------------------------------------------------------------------
// No arguments
//-----------------------------------------------------------------------------

void NoArgs ()
{
	printf("Krull: No arguments.\n");
}

//-----------------------------------------------------------------------------
// InputInfo struct
// Maintains information given at the command line
//-----------------------------------------------------------------------------

struct InputInfo
{
	vector<string>		fileNames;
	bool				verbose;
	bool				debugParser;

	InputInfo()
		: verbose(false)
		, debugParser(false)
	{}
};

//-----------------------------------------------------------------------------
// ProcessFlags

//-----------------------------------------------------------------------------

bool ProcessFlags (const char* flags, K_OUT InputInfo& info, K_OUT int& argIndex, char** argv)
{
	for (char flag = *flags; flag != 0; flag = *(++flags))
	{
		if (flag == 'v')
		{
			// Verbose
			info.verbose = true;
		}
		else
		{
			printf("Krull: Unknown flag '%c'.\n", flags[0]);
			return false;
		}
	}

	return true;
}

//-----------------------------------------------------------------------------
// ProcessNamedCommand
//-----------------------------------------------------------------------------

bool ProcessNamedCommand (const char* command, K_OUT InputInfo& info, K_OUT int& argIndex, char** argv)
{
	string cmd = command;

	if (cmd == "help")
	{
		printf("Krull database generator, copyright (C)2011 Matt Davies.\n\n");
		printf("Syntax: krull [flags] [files]\n\n");

		printf("--help    Display command line help.\n");
		printf("-v        Verbose mode");

		return false;
	}
	else if (cmd == "debugparser")
	{
		info.debugParser = true;
	}
	else
	{
		return false;
	}

	return true;
}

//-----------------------------------------------------------------------------
// Main entry point
//-----------------------------------------------------------------------------

int Start (int argc, char** argv)
{
	if (argc == 1)
	{
		NoArgs();
		K_DEBUG_PAUSE();
		return 0;
	}

	InputInfo inputInfo;

	for (int i = 1; i < argc; ++i)
	{
		if (argv[i][0] == '-')
		{
			if (argv[i][1] == '-')
			{
				// Named command
				if (!ProcessNamedCommand(&argv[i][2], inputInfo, i, argv))
				{
					K_DEBUG_PAUSE();
					return 1;
				}
			}
			else
			{
				// Flag
				if (!ProcessFlags(&argv[i][1], inputInfo, i, argv))
				{
					K_DEBUG_PAUSE();
					return 1;
				}
			}
		}
		else
		{
			inputInfo.fileNames.push_back(FileName::NormaliseFileName(argv[i]));
		}
	}

	// Initialise the compiler
	Compiler compiler;
	if (inputInfo.debugParser)
	{
		compiler.DebugParserOn();
	}
	if (inputInfo.verbose)
	{
		compiler.VerboseOn();
	}

	// Choose the back-end
	BackEnd* backEnd = 0;
	backEnd = new SqliteBackEnd();

	for (vector<string>::const_iterator it = inputInfo.fileNames.begin();
		 it != inputInfo.fileNames.end();
		 ++it)
	{
		compiler.Process(*it, *backEnd);
	}

	delete backEnd;

	K_DEBUG_PAUSE();
	return 0;
}

int main (int argc, char** argv)
{
#if K_WIN32
	_CrtSetBreakAlloc(0);
#endif

	int result = Start(argc, argv);

#if K_WIN32
	if (!_CrtDumpMemoryLeaks())
	{
		OutputDebugStringA("\n\nNo memory leaks found!\n\n");
	}
#endif

	return result;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
