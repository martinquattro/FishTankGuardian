/*!****************************************************************************
 * @file Utilities.h
 * @brief TODO
 * @author Quattrone Martin
 * @date Oct 2023
 *******************************************************************************/

#ifndef UTILITIES_H
#define UTILITIES_H

#include <string>

class Utilities 
{ 
    public:

        //!
        static bool IsNumeric(const std::string& str);

        //!
        static std::string FormatString(const char* format, ...);

};

#endif // UTILITIES_H