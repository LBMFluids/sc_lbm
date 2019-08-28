#include "../common/test_utils.h"
#include <string>
#include <functional>
#include "../../include/io_operations/lbm_io.h"

/*************************************************************** 
 * Suite for testing LbmIO class for I/O operations 
***************************************************************/

// Aliases
template <typename T>
using vec2D = std::vector<std::vector<T>>; 

// Supporting functions
bool write_read_test(LbmIO& io_obj);

template <typename T>
bool read_test(const vec2D<T>, const std::string, const std::function<bool(vec2D<T>, vec2D<T>)>);

template <typename T> 
bool write_read_test(const vec2D<T>, const LbmIO&, const std::function<bool(vec2D<T>, vec2D<T>)>);

// Tests
bool read_test_suite();
bool custom_test_suite();
bool default_test_suite();

// Files required to be present
// ./test_data/r_bool.txt
// ./test_data/r_int.txt
// ./test_data/r_string.txt
// ./test_data/r_double.txt

// Files required to be deleted
// No error if not but may bias the test
// ./test_data/wr_bool.txt
// ./test_data/wr_int.txt
// ./test_data/wr_string.txt
// ./test_data/wr_double.txt

int main()
{
	tst_pass(read_test_suite(), "LbmIO read only");	
	tst_pass(custom_test_suite(), "LbmIO write and read custom properties");
	tst_pass(default_test_suite(), "LbmIO default properties");
}

/**
 * \brief Series of read tests with various data types
 */
bool read_test_suite()
{
	using namespace std::placeholders;	
	//
	// Types that can be compared directly/exactly
	
	// bool
	std::function<bool(vec2D<bool>, vec2D<bool>)> is_equal_bool = is_equal_exact<bool>;
	vec2D<bool> bool_exp = {{0, 0}, {1, 1}, {0, 1}, {0, 0}, {0, 1}};
	if (!read_test(bool_exp, "./test_data/r_bool.txt", is_equal_bool))
		return false;

	// int
 	std::function<bool(vec2D<int>, vec2D<int>)>is_equal_int = is_equal_exact<int>;
	vec2D<int> int_exp = {{2, 3, 2}, {8, 5, 10}, {4, 1, 10}};
	if (!read_test(int_exp, "./test_data/r_int.txt", is_equal_int))
		return false;
	
	// std::string
 	std::function<bool(vec2D<std::string>, vec2D<std::string>)>is_equal_str = is_equal_exact<std::string>;
	vec2D<std::string> str_exp = {{"int", "bool"}, {"float", "double"}};
	if (!read_test(str_exp, "./test_data/r_string.txt", is_equal_str))
		return false;

	//
	// Types that need indirect comparison
	
	// double
	// We need one more argument to call is_equal_floats so to maintain function signature in 
	// the test we will wrap the call in a lambda
	double tol = 1e-5;
	// Has access to tol and calls is_equal_floats with it
	auto is_equal_wrap = [tol](vec2D<double> orig_v, vec2D<double> new_v)
		{ return is_equal_floats<double>(orig_v, new_v, tol); };	
	std::function<bool(vec2D<double>, vec2D<double>)>is_equal_double = is_equal_wrap;
	//
	vec2D<double> dbl_exp = {{0.6477, 0.2963, 6.868e5, 0.6256}, {0.4509e10, 0.7447, 0.1835, 0.7802},
		   					 {0.5470, 0.1890e-7, 0.3685, 0.0811}};
	if (!read_test(dbl_exp, "./test_data/r_double.txt", is_equal_double))
		return false;
	
	// All passed
	return true;
}

/**
 * \brief Tests reading a 2D vector of type T
 * @param expected - 2D vector of expected values, each inner vector 
 * 						is a single line of file 
 * @param fname - name of the file to read from
 * @param is_equal - function object for comparing expected with 
 * 						read values of 2D vectors of type T - needs complete comparison
 * 						of the 2D vector; returns true if equal
 * @return true if is_equal returned true, false otherwise
 */
template <typename T>
bool read_test(const vec2D<T> expected, const std::string fname, 
	const std::function<bool(vec2D<T>, vec2D<T>)> is_equal)
{
	// Unused options of the LbmIO object
	std::string delim(" ");
	bool one_file = true;
	std::vector<size_t> dims = {0,0,0};

	// Read new_vec 
	vec2D<T> new_vec;
	LbmIO io_int(fname, delim, one_file, dims);
	new_vec = io_int.read_vector<T>();

	// Return the result of comparison
	return is_equal(new_vec, expected);
}

/**
 * \brief Tests writing and reading 2D string vector 
 * \details Writes and then reads from file, compares
 * 		the read vector to expected (written)   
 */
bool custom_test_suite()
{
	// Common and unused LbmIO properties
	bool one_file = true;
	std::vector<size_t> dims = {0,0,0};

	//
	// Types that can be compared directly/exactly
	
	// bool
	LbmIO io_bool("./test_data/wr_bool.txt", ",", one_file, dims);
	std::function<bool(vec2D<bool>, vec2D<bool>)> is_equal_bool = is_equal_exact<bool>;
	vec2D<bool> bool_exp = {{1, 0}, {1, 0}, {0, 1}, {1, 1}, {0, 1}};
	if (!write_read_test(bool_exp, io_bool, is_equal_bool))
		return false;

	// int
 	LbmIO io_int("./test_data/wr_int.txt", ",", one_file, dims);
	std::function<bool(vec2D<int>, vec2D<int>)>is_equal_int = is_equal_exact<int>;
	vec2D<int> int_exp = {{1, 5, 2}, {8, 9, 10}, {4, 6, 10}};
	if (!write_read_test(int_exp, io_int, is_equal_int))
		return false;
	
	// std::string
 	LbmIO io_str("./test_data/wr_string.txt", ",", one_file, dims);
	std::function<bool(vec2D<std::string>, vec2D<std::string>)>is_equal_str = is_equal_exact<std::string>;
	vec2D<std::string> str_exp = {{"long", "short"}, {"unsigned", "double"}};
	if (!write_read_test(str_exp, io_str, is_equal_str))
		return false;
	
	//
	// Types that need indirect comparison
	
	// double
	// We need one more argument to call is_equal_floats so to maintain function signature in 
	// the test we will wrap the call in a lambda
	double tol = 1e-5;
	// Has access to tol and calls is_equal_floats with it
	auto is_equal_wrap = [tol](vec2D<double> orig_v, vec2D<double> new_v)
		{ return is_equal_floats<double>(orig_v, new_v, tol); };	
	std::function<bool(vec2D<double>, vec2D<double>)>is_equal_double = is_equal_wrap;
	//
 	LbmIO io_dbl("./test_data/wr_double.txt", ",", one_file, dims);
	vec2D<double> dbl_exp = {{0.64677, 2.963, 6.868e5, 0.6256}, {0.4509e10, -0.75447, 0.1835, 0.7802},
		   					 {0.54701, 0.01890e-7, 0.3685, -0.0811}};
	if (!write_read_test(dbl_exp, io_dbl, is_equal_double))
		return false;
	
	// All passed
	return true;
}

/**
 * \brief Tests default constructor settings and behavior  
 */
bool default_test_suite()
{
	// Common default LbmIO object
	LbmIO io_dflt;
	
	//
	// Types that can be compared directly/exactly
	
	// bool
	std::function<bool(vec2D<bool>, vec2D<bool>)> is_equal_bool = is_equal_exact<bool>;
	vec2D<bool> bool_exp = {{1, 0}, {1, 0}, {0, 1}, {1, 1}, {0, 1}};
	if (!write_read_test(bool_exp, io_dflt, is_equal_bool))
		return false;

	// int
	std::function<bool(vec2D<int>, vec2D<int>)>is_equal_int = is_equal_exact<int>;
	vec2D<int> int_exp = {{1, 5, 2}, {8, 9, 10}, {4, 6, 10}};
	if (!write_read_test(int_exp, io_dflt, is_equal_int))
		return false;
	
	// std::string
 	std::function<bool(vec2D<std::string>, vec2D<std::string>)>is_equal_str = is_equal_exact<std::string>;
	vec2D<std::string> str_exp = {{"long", "short"}, {"unsigned", "double"}};
	if (!write_read_test(str_exp, io_dflt, is_equal_str))
		return false;

	//
	// Types that need indirect comparison
	
	// double
	// We need one more argument to call is_equal_floats so to maintain function signature in 
	// the test we will wrap the call in a lambda
	double tol = 1e-5;
	// Has access to tol and calls is_equal_floats with it
	auto is_equal_wrap = [tol](vec2D<double> orig_v, vec2D<double> new_v)
		{ return is_equal_floats<double>(orig_v, new_v, tol); };	
	std::function<bool(vec2D<double>, vec2D<double>)>is_equal_double = is_equal_wrap;
	//
 	vec2D<double> dbl_exp = {{0.64677, 2.963, 6.868e5, 0.6256}, {0.4509e10, -0.75447, 0.1835, 0.7802},
		   					 {0.54701, 0.01890e-7, 0.3685, -0.0811}};
	if (!write_read_test(dbl_exp, io_dflt, is_equal_double))
		return false;

	// All passed
	return true;
}

/** 
 * \brief Checks writing and reading under different LbmIO settings
 * \details Writes to and then reads from the same file, compares 
 * 		the read 2D vector of type T to expected
 * @param expected - 2D vector of expected values, each inner vector 
 * 						is a single line of file 
 * @param io_obj - LbmIO object 
 * @param is_equal - function object for comparing expected with 
 * 						read values of 2D vectors of type T - needs complete comparison
 * 						of the 2D vector; returns true if equal
 * @return true if is_equal returned true, false otherwise
 */
template <typename T> 
bool write_read_test(const vec2D<T> orig_vec, const LbmIO& io_obj, 
	const std::function<bool(vec2D<T>, vec2D<T>)> is_equal)
{
	// Write then read
	vec2D<T> new_vec;
	io_obj.write_vector<T>(orig_vec);
	new_vec = io_obj.read_vector<T>();

	// True if vectors are identical
	return is_equal(orig_vec, new_vec);
}

