#include "../common/test_utils.h"
#include <string>
#include "../../include/io_operations/lbm_io.h"

/*************************************************************** 
 * Suite for testing LbmIO class for I/O operations 
***************************************************************/

// Supporting functions
bool write_read_test(LbmIO& io_obj);

// Tests
bool read_test();
bool custom_test();
bool default_test();

int main()
{
	tst_pass(read_test(), "LbmIO read only");	
	tst_pass(custom_test(), "LbmIO write and read custom properties");
	tst_pass(default_test(), "LbmIO default properties");
}

/**
 * \brief Tests reading a 2D string vector 
 * \details Reads contents of /test_data/A_int.txt 
 * 		which has a mixed whitespace 3x3 dataset and
 * 		compares it with the same dataset in the 
 * 		function. All data represented as strings.  
 */
bool read_test()
{
	// Unused options of the LbmIO object
	std::string delim(" ");
	bool one_file = true;
	std::vector<size_t> dims = {0,0,0};

	// Read new_vec and compare to orig_vec
	std::vector<std::vector<std::string>> orig_vec
			= {{"0","1","2"}, {"3","4","5"}, {"6","7","8"}};
	std::vector<std::vector<std::string>> new_vec;
	LbmIO io_int("./test_data/A_int.txt", delim, one_file, dims);
	new_vec = io_int.read_vector();

	// True if all inner vectors are identical
	for (int i=0; i<orig_vec.size(); i++)
		if (!(orig_vec.at(i) == new_vec.at(i)))
			return false;
	if (new_vec.size() != orig_vec.size())
		return false;
	return true;
}

/**
 * \brief Tests writing and reading 2D string vector 
 * \details Writes and then reads from file, compares
 * 		the read vector to expected (written)   
 */
bool custom_test()
{
	bool one_file = true;
	std::vector<size_t> dims = {0,0,0};
	LbmIO io_string("./test_data/B_string.txt", ",", one_file, dims);
	return write_read_test(io_string);
}

/**
 * \brief Tests default constructor settings and behavior  
 */
bool default_test()
{
	LbmIO io_dflt;
	return write_read_test(io_dflt);		
}

/** 
 * \brief Checks writing and reading under different LbmIO settings
 * \details Writes to and then reads from the same file, compares 
 * 		the read vector of vectors of strings to expected
 * @param [in] io_obj - LbmIO object
 * @returns true if both vectors are the same, false if not
 */ 
bool write_read_test(LbmIO& io_obj)
{
	std::vector<std::vector<std::string>> orig_vec 
			= {{"0","1","2"}, {"3","4","5"}};
	std::vector<std::vector<std::string>> new_vec;
	
	// Write then read and compare
	io_obj.write_vector<std::string>(orig_vec);
	new_vec = io_obj.read_vector();

	// True if vectors are identical
	for (int i=0; i<orig_vec.size(); i++)
		if (!(orig_vec.at(i) == new_vec.at(i)))
			return false;
	if (new_vec.size() != orig_vec.size())
		return false;
	return true;
}


