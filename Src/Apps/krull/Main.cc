//-----------------------------------------------------------------------------
// Krull database generation language
//-----------------------------------------------------------------------------

#include "Krull.h"
#include "Compiler.h"

#include <stdio.h>
#include <conio.h>
#include <string>
#include <vector>

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

int main (int argc, char** argv)
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
			inputInfo.fileNames.insert(inputInfo.fileNames.end(), argv[i]);
		}
	}

	Compiler compiler;
	if (inputInfo.debugParser)
	{
		compiler.DebugParserOn();
	}

	for (vector<string>::const_iterator it = inputInfo.fileNames.begin();
		 it != inputInfo.fileNames.end();
		 ++it)
	{
		compiler.Process(*it);
	}

	K_DEBUG_PAUSE();
	return 0;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
