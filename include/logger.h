#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <ctime>
#include <iomanip>
#include <string>
#include <vector>
#include <deque>
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
		sout(false), file_object(fname, std::ios_base::app)
		{ }

	//
	// Text colors
	//

	enum Color
	{
		fg_red = 31,
		fg_yellow = 33,
		bg_green = 42
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
	
	// Header/title etc
	void print_with_header(const std::string& s, const std::string& sym = "*") { print_header(s, sym); }
	// Info what is being run (in color)
	void name_current_step(const std::string& s) { print_color(get_color(bg_green), s); }
	// Basic description of the step
	void log_details(const std::string& s) { print(split_lines(s)); }
	
private:
	// True if outputing to standard output
	bool sout = false;
	// File handling object
	FileHandler file_object;
	// Maximum number of characters in a line
	int max_char = 75;
	// Number of symbols to prepend and append
	int hd_pre = 3;

	/// Logs string s
	void print(const std::string& s);
	void print(const std::vector<std::string>&& vs);
	void print(const std::deque<std::string>& ds);

	/// Logs string s in color cl
	void print_color(const std::string& cl, const std::string& s);

	/// Logs text s in a header made of string sym
	void print_header(const std::string& s, const std::string& sym);
	
	/// Splits text into strings/lines up to max_char characters
	std::vector<std::string> split_lines(const std::string& s);
	
};


#endif
