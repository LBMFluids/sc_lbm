#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <ctime>
#include <iomanip>
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
	Logger() : sout(true), file_object("tmp.log", std::ios_base::app) { }
	/// Constructor that takes a filename
	Logger(const std::string& fname) : 
		sout(false), file_object(fname, std::ios_base::app)
		{ }

	//
	// Text colors
	//

	enum Color
	{
		fg_red = 31,
		fg_yellow = 33
	};

	const std::string get_color(Color);

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
	void print_error(const std::string& s) { print_color(get_color(fg_red), s); }
	/// Warnings
	void print_warning(const std::string& s) { print_color(get_color(fg_yellow), s); }

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

	/// Logs string s in color cl
	void print_color(const std::string& cl, const std::string& s);

	// Colors enum
	// Headers 

};


#endif
