
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
	if (sout) { 
    	std::cout << std::asctime(std::localtime(&std::time(nullptr))) 
				  << std::endl;
	else {
		file_object.get_stream() << std::asctime(std::localtime(&std::time(nullptr))) 
								 << std::endl;
	}	
}

// Runtime info preceeded with a timestamp
void Logger::log_with_time(const std::string& s)
{	
	if (sout) { 
    	std::cout << "[" << std::asctime(std::localtime(&std::time(nullptr))) 
				  << "] " << s << std::endl;
	else {
		file_object.get_stream() << "[" << std::asctime(std::localtime(&std::time(nullptr))) 
				  << "] " << s << std::endl;
	}
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


