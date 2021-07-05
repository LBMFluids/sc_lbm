#include "../common/test_utils.h"
#include <string>
#include "../../include/io_operations/FileHandler.h"

/*************************************************************** 
 * Suite for testing FileHandler class for I/O operations 
 ***************************************************************/

// Supporting functions
bool check_file_content(std::fstream&, std::vector<std::string>);
void open_file(const std::string, const std::ios_base::openmode);

// Tests
bool default_io_test();
bool custom_io_test();
bool fail_to_open_test();

int main()
{
	tst_pass(default_io_test(), "Read and write default mode");
	tst_pass(custom_io_test(), "Custom modes");
	tst_pass(fail_to_open_test(), "File opening error");
}

/**
 * \brief Testing correctness of FileHandler default mode
 * \details Uses FileHandler to write and then read
 * a line of text - compares both, true if identical
 */ 
bool default_io_test()
{
	// The default FileHandler is both in and out and
	// truncates
	std::string fname("./test_data/default_mode.txt");
	FileHandler file(fname);
	std::fstream &out = file.get_stream();
	
	// Expected input/output
	std::string orig_line("This is a test."), new_line;
	std::vector<std::string> exp_lines = {orig_line};

	// Write and truncate if exists
	out << orig_line << std::endl;

	// Now read and check content
	// Move the input stream to the begining of the file
	std::fstream &in = file.reset_input_stream();
	return check_file_content(in, exp_lines);
}

/**
 * \brief Testing correctness of FileHandler custom modes
 * \details Uses FileHandler to write, read, append, and read again
 * 	compares reads to expected strings, true if identical. Important -
 * 	ensure the test file is deleted before the test.
 */ 
bool custom_io_test()
{
	// Custom FileHandlers - in and out in append mode
	// appends
	std::string fname("./test_data/custom_mode.txt");
	FileHandler out_file(fname, std::ios_base::out | std::ios_base::app);
	FileHandler in_file(fname, std::ios_base::in);
	
	// Two lines, written one at a time
	std::string line_1("This is line 1"), line_2("This is line 2");
	// For checking of the first write, line_2 is appended later
	std::vector<std::string> exp_lines = {line_1};

	// Write first line and check
	std::fstream &out = out_file.get_stream();
	std::fstream &in = in_file.get_stream();
	out << line_1 << std::endl;
	if (!check_file_content(in, exp_lines))
		return false;
	
	// Append second line and check
	out << line_2 << std::endl;
	exp_lines.push_back(line_2);
	// Clear the eof bit set by getline and reset
	// to the beginning
	// https://stackoverflow.com/questions/5343173/returning-to-beginning-of-file-after-getline
	in_file.clear_stream();
	in_file.reset_input_stream();
	if (!check_file_content(in, exp_lines))
		return false;	
	return true;
}

/**
 * \brief Checks exception behavior when opening a non-existent file
 * \details Forces ios_base::failure exception by trying to read 
 * 		from a file that doesn't exist.
 * @return true if the program throws an ios_base::failure 
 * 		exception, false for lack of exception or a different one. 
 */
bool fail_to_open_test()
{
	bool verbose = true;
	const std::string fname("./test_data/not_there.txt"); 
	const std::ios_base::failure ios_err("File doesn't exist.");
	if (!exception_test(verbose, &ios_err, open_file, fname, std::ios_base::in))
		return false;
	return true;
}

/**
 * \brief Performs tests on file content given a stream 
 * \details Given stream, uses getline to fetch lines from 
 * 		the file. Compares the lines with contents of 
 * 		the vector which elements correspond to expected lines.
 *
 * 	@param in [in] - input file stream
 * 	@param exp_lines [in] - vector of strings, each string representing
 * 							one line of input without the newline character
 * 	@return true if all lines match and vector size equal number of lines in 
 * 		the file. false otherwise.
 */
bool check_file_content(std::fstream& in, std::vector<std::string> exp_lines)
{
	size_t count = 0;
	size_t num_exp = exp_lines.size();
	std::string new_line;
	while (std::getline(in, new_line)){
		// Quit if the number of input lines exceeded expected
		if (count > num_exp)
			return false;
		if (new_line != exp_lines[count++])
			return false;
	}
	// False if input lines number less than expected 
	if (count != num_exp)
		return false;
	return true;
}

/**
 * \brief Just opens a file for accessibility testing purposes
 * @param fname - file name
 * @param mode - ios mode of opening
 */
void open_file(const std::string fname, const std::ios_base::openmode mode)
{
	// Negative test if one wants to make sure
	// throw std::runtime_error("Surprise!!!");
	FileHandler fh(fname, mode);
	return;
}


