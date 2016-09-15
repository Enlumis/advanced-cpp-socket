#pragma once

# include	<string>
# include	<iostream>
#include "ClassException.hh"

class ILibLoader
{
public:
	virtual ~ILibLoader() {};
	virtual void	open(std::string const &libPath) throw(ClassException) = 0;

	virtual void	*findSymbol(std::string const &symbol) throw(ClassException) = 0;

	virtual void	close() = 0;

};
