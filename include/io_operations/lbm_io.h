#ifndef LBM_IO_H
#define LBM_IO_H

#include "FileHandler.h"
#include "../common.h"

/*************************************************************** 
 * class: LbmIO
 *
 * Interface for read and write operations 
 *
 * Manages IO operations from choice between single or multiple
 * files to file and data formats.
***************************************************************/

class LbmIO
{
public:

	//
	// Constructors
	//
	
	/**
	 * \brief Default constructor - sets file names and writing mode to single file
	 * \details File will be located in the current working directory
	 */ 
	LbmIO() = default;

	/** 
	 * \brief Creates a LbmIO object
	 * @param name - name of the file or begining of the file names for file sequences
	 * @param delim - data delimiter  
	 * @param sflag - true if IO to/from a single file (not a file sequence)
	 * @param dims - vector with #rows | # columns | #any higher dimensions
	 */
	LbmIO(const std::string& name, const std::string& delim, const bool sflag, 
			const std::vector<size_t>& dims) : 
			fname(name), single_file(sflag), dimensions(dims) { } 

	//
	// Reading functionality 
	//
	
	/**
	 * \brief Read a 1D or 2D vector from a file
	 * \details Currently data in a line needs to be separated by whitespace (tab, blank, ...)
	 * @returns std::vector of vectors of std::string with the data in each vector being
	 * 		one line of the file with elements as extracted based on the delimiter
	 */
	template<typename T>
	std::vector<std::vector<T>> read_vector() const;
	
	//
	// Writing functionality 
	//
	
	/**
	 * \brief Write a 2D vector to a file with delimiter delim
	 * \details Truncates if the file exists. 
	 * @param data - nested vector of type T with dimensions dims[0] x dims[1] 
	 */
	template<typename T>
	void write_vector(const std::vector<std::vector<T>>& data) const;

	//
	// Destructor
	//
	~LbmIO() = default;
private:
	std::string fname = {"dflt_lbm_io_file.txt"};
	std::string delim = {" "};
	bool single_file = true;
	std::vector<size_t> dimensions = {0,0,0};
};

//
// Reading functionality 
//

template<typename T>
std::vector<std::vector<T>> LbmIO::read_vector() const
{
	FileHandler file(fname);
	std::fstream &in = file.get_stream();
	std::string line;
   	T elem;
	std::vector<std::vector<T>> output;

	while (std::getline(in, line)) {
		std::istringstream data_row(line);
		std::vector<T> temp;
		while (data_row >> elem) {
			temp.push_back(elem);
		}
		output.push_back(temp);
	}
	return output;
}

//
// Writing functionality 
//

template<typename T>
void LbmIO::write_vector(const std::vector<std::vector<T>>& data) const
{
	FileHandler file(fname, std::ios_base::out | std::ios_base::trunc);
	std::fstream &out = file.get_stream();

	size_t nrows = data.size();
	size_t ncols = data.at(0).size();
	for (int i = 0; i<nrows; i++){
		std::copy(data.at(i).begin(), data.at(i).end(),	std::ostream_iterator<T>(out, delim.c_str()));
		out << '\n';
	}	
}		

#endif
