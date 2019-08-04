#include "../../include/io_operations/FileHandler.h"

/*************************************************************** 
 * class: FileHandler
 *
 * Creates and manages input and output file streams
 *
 * Opens and maintains a file stream, then closes it when
 * calling the destructor. Checks if file properly opened and
 * if there were error during reading or writing. 
***************************************************************/

void FileHandler::open_file()
{
	file.open(fname, mode);
	if (!file.is_open()){
		std::cerr << "Error opening file " << fname << std::endl;
		throw std::ios_base::failure(std::strerror(errno));
	}
}

FileHandler::~FileHandler()
{
	// This is all that is being checked since std::getline sets failbit on EOF
	// https://stackoverflow.com/a/7855777/2763915
	// Leaving it like this for now - maybe extend in the future
	if (file.bad())
		std::cerr << "Error reading or writing to file " << fname 
				  << "\nDetected badbit set i.e. broken stream." << std::endl;
	file.close();
}		

