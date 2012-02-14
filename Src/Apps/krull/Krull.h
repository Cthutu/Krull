//-----------------------------------------------------------------------------
// Apps/krull/Krull.h
//
// Main header file for the Krull application
//-----------------------------------------------------------------------------

#pragma once

//-----------------------------------------------------------------------------
// Helper macros
//-----------------------------------------------------------------------------

#define K_UNUSED(var)	var=var
#define K_OUT
#define K_OVERRIDE

#define K_YES (1)
#define K_NO (0)

#define K_DEBUG_PAUSE()

//-----------------------------------------------------------------------------
// Build configuration
//-----------------------------------------------------------------------------

#ifdef _DEBUG
#	define K_DEBUG	K_YES
#else
#	define K_DEBUG	K_NO
#endif

//-----------------------------------------------------------------------------
// Platform
//-----------------------------------------------------------------------------

#ifdef WIN32
#	define K_WIN32 K_YES
#	define WIN32_LEAN_AND_MEAN
#	include <Windows.h>
#	include <conio.h>
#	undef K_DEBUG_PAUSE
#	define K_DEBUG_PAUSE() getch()
#elif defined(__APPLE__)
#	define K_APPLE K_YES
#endif

//-----------------------------------------------------------------------------
// Assertions
//-----------------------------------------------------------------------------

#include <assert.h>

#if K_DEBUG
#	define K_ASSERT(x) assert(x)
#else
#	define K_ASSERT(x)
#endif

//-----------------------------------------------------------------------------
// Common includes
//-----------------------------------------------------------------------------

// C includes
#include <stdlib.h>
#include <stdio.h>
#if K_APPLE
#	include <errno.h>
#endif

// C++ includes
#include <string>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

//-----------------------------------------------------------------------------
// Handling secure functions in non-windows
//-----------------------------------------------------------------------------

#if !K_WIN32
#	define _vsnprintf_s vsnprintf
#	define sprintf_s(buffer, bufferSize, format, args) sprintf(buffer, format, args)
#endif

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
