/*!****************************************************************************
 * @file    Utilities.h
 * @brief   Declaration of utility functions
 * @author  Quattrone Martin
 * @date    Oct 2023
 *******************************************************************************/

#ifndef UTILITIES_H
#define UTILITIES_H

#include <string>

class Utilities 
{ 
public:

    /**
     * @brief   Checks if all characters in the string are digits.
     * @param   str The input string.
     * @return  True if all characters are digits, false otherwise.
     */
    static bool IsNumeric(const std::string& str);

    /**
     * @brief   Formats a string according to the provided format.
     * @param   format The format string.
     * @param   ... Additional arguments for formatting.
     * @return  The formatted string.
     */
    static std::string FormatString(const char* format, ...);

};

#endif // UTILITIES_H