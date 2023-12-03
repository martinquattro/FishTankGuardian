/*!****************************************************************************
 * @file utilities.cpp
 * @brief Implementation of utility functions
 * @author Quattrone Martin
 * @date Oct 2023
 *******************************************************************************/

#include "utilities.h"

#include <cstdio>
#include <cstdarg>
#include "mbed.h"

//----static-------------------------------------------------------------------
bool Utilities::IsNumeric(const std::string& str)
{
    return ((!str.empty()) && (std::all_of(str.begin(), str.end(), ::isdigit)));
}


//----static-------------------------------------------------------------------
std::string Utilities::FormatString(const char* format, ...) 
{
    va_list args;
    va_start(args, format);

    int size = std::vsnprintf(nullptr, 0, format, args);

    std::string result(size + 1, '\0');
    std::vsnprintf(&result[0], result.size(), format, args);

    va_end(args);

    result.pop_back();

    return result;
}
