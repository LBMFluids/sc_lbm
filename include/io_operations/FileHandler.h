#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include "../common.h"
#include <ios>

/*************************************************************** 
 * class: FileHandler
 *
 * Creates and manages input and output file streams
 *
 * Opens and maintains a file stream, then closes it when
 * calling the destructor. 
***************************************************************/

class FileHandler
{
public:

	//
	// Constructors
	//
	FileHandler() = default;
	/** 
	 * \brief Creates a FileHandler object in a custom open mode
	 * @param name - name of the file
	 * @param opmode - open mode type (read, write, append, truncate, ...), https://en.cppreference.com/w/cpp/io/ios_base/openmode 
	*/
	FileHandler(const std::string name, std::ios_base::openmode opmode) : fname(name), mode(opmode) 
		{ open_file(); }
	/** 
	 * \brief Creates a FileHandler object in a default (read, write) open mode
	 * \details It is an error if the file doesn't exist
	 * @param name - name of the file
	*/
	FileHandler(const std::string name) : 
		FileHandler(name, std::ios_base::in | std::ios_base::out) { }	

	//
	// Getters/setters
	//
	
	/** 
	 * \brief Returns a file stream  
	*/
	std::fstream& get_stream() { return file; }
	/** 
	 * \brief Resets the input strem and returns a reset file stream 
	 * \details https://stackoverflow.com/a/26227809/2763915 
	*/
	std::fstream& reset_input_stream() { file.seekg(std::ios_base::beg); return file; }
	/** 
	 * \brief Clears stream for reuse 
	 * \details https://stackoverflow.com/questions/5343173/returning-to-beginning-of-file-after-getline 
	*/
	std::fstream& clear_stream() { file.clear(); return file; }

	//
	// Destructor
	//
	~FileHandler();
private:
	std::string fname;
	std::ios_base::openmode mode;		
	std::fstream file;
	void open_file();
};

#endif
