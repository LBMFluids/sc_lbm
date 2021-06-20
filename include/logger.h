#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <ctime>
#include "io_operations/FileHandler.h"

/***************************************************** 
 * class: Logger 
 * 
 * Functionality for logging the runtime information   
 * 
 ******************************************************/

class Logger {
public:

	//
	// Constructors
	//

	/// Default constructor (outputs to std::out)
	Logger() : sout(true) { }
	/// Constructor that takes a filename
	Logger(const std::string& fname) : 
		sout(false), FileHandler(fname, std::ios_base::app)
		{ }

	//
	// Basic functionality
	//

	/// Unformatted runtime information
	void log(const std::string& s) { print(s); }
	/// Timestamp in a separate line
	void print_time();
	/// Runtime info preceeded with a timestamp
	void log_with_time(const std::string& s);	

	
	//
	// Errors and warnings
	//

	/// Errors
	void print_error(const std::string& s) { print(s); }
	/// Warnings
	void print_warning(const std::string& s) { print(s); }

	//
	// Detailed information
	//

	// Info what is being run
	// Heading/title etc
	// Basic description of the step
	
	//
	// Code sections
	// 

private:
	// True if outputing to standard output
	bool sout = false;
	// File handling object
	FileHandler file_object;

	/// Logs string s
	void print(const std::string& s);

	// Colors enum
	// Headers 

};


#endif
