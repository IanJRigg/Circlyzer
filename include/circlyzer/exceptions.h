#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <exception>

namespace Circlyzer
{

class Non_Existant_UID_Exception : public std::exception
{
    const char * what() const throw()
    {
        return "Please provide a existing UID";
    }
};

class Non_Existant_Alias_Exception : public std::exception
{
    const char * what() const throw()
    {
        return "Please provide an existing alias";
    }
};


class Invalid_Alias_Exception : public std::exception
{
    const char * what() const throw()
    {
        return "Please provide an alias meeting the length requirements";
    }
};

class Duplicate_Alias_Exception : public std::exception
{
    const char * what() const throw()
    {
        return "Please provide a unique alias";
    }
};

class Wrong_Entity_Type_Exception : public std::exception
{
    const char * what() const throw()
    {
        return "Please request a type that matches the provided UID";
    }
};

class Null_Component_Exception : public std::exception
{
    const char * what() const throw()
    {
        return "Nullptr provided in place of component pointer";
    }
};

} // Namespace Circlyzer

#endif
