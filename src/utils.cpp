#include "../include/utils.h"

// Convert a string to all lowercase
std::string str_to_lower(std::string s) 
{
	std::transform(s.begin(), s.end(), s.begin(), 
                  [](unsigned char c){ return std::tolower(c); } );
    return s;
}
