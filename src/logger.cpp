
#include "../include/logger.h"

/***************************************************** 
 * class: Logger 
 * 
 * Functionality for logging the runtime information   
 * 
 ******************************************************/

// Timestamp in a separate line
void Logger::print_time()
{
	std::time_t t = std::time(nullptr);
	std::tm tm = *std::localtime(&t);
	if (sout) { 
    	std::cout << std::put_time(&tm, "[%a %Y-%b-%d %H:%M:%S]") 
				  << std::endl;
	} else {
		file_object.get_stream() << std::put_time(&tm, "[%a %Y-%b-%d %H:%M:%S]") 
								 << std::endl;
	}	
}

// Runtime info preceeded with a timestamp
void Logger::log_with_time(const std::string& s)
{
	std::time_t t = std::time(nullptr);
	std::tm tm = *std::localtime(&t);
	if (sout) { 
    	std::cout << std::put_time(&tm, "[%a %Y-%b-%d %H:%M:%S] ") 
				  << s << std::endl;
	} else {
		file_object.get_stream() << std::put_time(&tm, "[%a %Y-%b-%d %H:%M:%S] ") 
				  << s << std::endl;
	}
}

// Returns the color enum as a string
const std::string Logger::get_color(Color cl)
{
	switch(cl) {
		case fg_red : return "31";
		case fg_yellow : return "33";
	}
	return "0";
}

// Logs string s
void Logger::print(const std::string& s) 
{
	if (sout) { 
		std::cout << s << std::endl; 
	} else { 
		file_object.get_stream() << s << std::endl;
	}
}

// Logs string s
void Logger::print_color(const std::string& cl, const std::string& s) 
{
	if (sout) { 
		std::cout << "\033[" << cl << "m "<< s << "\033[0m" << std::endl; 
	} else { 
		file_object.get_stream() << "\033[" << cl << "m "<< s << "\033[0m" << std::endl;
	}
}


