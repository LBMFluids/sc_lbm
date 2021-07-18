
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

// Logs string s
void Logger::print(const std::string& s) 
{
	if (sout) { 
		std::cout << s << std::endl; 
	} else { 
		file_object.get_stream() << s << std::endl;
	}
}

void Logger::print(const std::vector<std::string>&& vs) 
{
	for (const auto& s : vs) {
		print(s);
	}
}

void Logger::print(const std::deque<std::string>& ds) 
{
	for (const auto& s : ds) {
		print(s);
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

// Logs text s in a header made of string sym
void Logger::print_header(const std::string& s, const std::string& sym)
{
	// Make text fit the line requirements
	std::vector<std::string> all_lines = split_lines(s);
	// Get the prepend/append header sequence
	std::string border;
	for (int i=0; i<hd_pre; ++i) {
		border += sym;
	}
	// Create bordered text lines
	std::deque<std::string> processed_lines;
	int max_line = 0;
	for (const auto& line : all_lines) {
		max_line = line.size() > max_line ? line.size() : max_line;
		processed_lines.push_back(border + " " + line + " " + border);
	}
	// Add top and bottom with a proper length
	int tot_len = max_line + 2*border.size() + 2;
	std::string top_bottom;
	for (int i=0; i<tot_len; ++i) {
		top_bottom += sym;
	}
	processed_lines.push_front(top_bottom);
	processed_lines.push_front(top_bottom);
	processed_lines.push_back(top_bottom);
	processed_lines.push_back(top_bottom);
	// Print
	print(processed_lines);	
}

// Splits text into strings/lines up to max_char characters
std::vector<std::string> Logger::split_lines(const std::string& s)
{
	int nchar = 0;
	std::vector<std::string> lines;
	std::string one_line;
	for (const auto& c : s) {
		if (nchar == max_char) {
			lines.push_back(one_line);
			one_line.clear();
			one_line += c;
			nchar = 0;
		} else {
			one_line += c;
		}
		++nchar;
	}
	if (nchar <= max_char) {
		lines.push_back(one_line);
	}
	return lines;
}

// Returns the color enum as a string
const std::string Logger::get_color(Color cl)
{
	switch(cl) {
		case fg_red : return "31";
		case fg_yellow : return "33";
		case bg_green : return "42";
	}
	return "0";
}

