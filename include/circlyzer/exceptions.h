#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <exception>

namespace Circlyzer
{
	class Invalid_UID_Exception : public std::exception
	{

	};

	class Invalid_Alias_Exception : public std::exception
	{

	};

	class Invalid_Type_Exception : public std::exception
	{

	};
}

#endif
