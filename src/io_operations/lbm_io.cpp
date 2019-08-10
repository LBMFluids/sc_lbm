#include "../../include/io_operations/lbm_io.h"

//
// Reading functionality 
//

std::vector<std::vector<std::string>> LbmIO::read_vector() const
{
	FileHandler file(fname);
	std::fstream &in = file.get_stream();
	std::string line, elem;
	std::vector<std::vector<std::string>> output;

	while (std::getline(in, line)){
		std::istringstream data_row(line);
		std::vector<std::string> temp;
		while (data_row >> elem)
			temp.push_back(elem);
		output.push_back(temp);
	}
	return output;
}


