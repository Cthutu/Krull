//-----------------------------------------------------------------------------
// Apps/krull/BackEnd.h
//
// Base class for all back-ends.
//-----------------------------------------------------------------------------

#pragma once

class Project;

class BackEnd
{
public:
	BackEnd () {}
	virtual ~BackEnd () {}

	virtual string	GetExtension		() const		= 0;
	virtual bool	Build				(const string& fileName, const Project& project) = 0;
};

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
