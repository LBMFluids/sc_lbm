#ifndef TEST_UTILS_H
#define TEST_UTILS_H

#include <typeinfo>
#include <type_traits>
#include <functional>
#include "../../include/common.h"

/*************************************************************** 
 * General testing utility functions
***************************************************************/

/**
 * \brief Print a message 
 * @param [in] msg - content to print
 */
const auto print_msg = [](const std::string msg)
							{ std::cout << msg << std::endl; };
/**
 * \brief Print if the test passed or failed
 * \details Hardcoded color scheme, depending on val it
 * 		will print a pass/fail notification following the test name
 * @param val [in] - test outcome
 * @param tname [in] - test name
 */
void tst_pass(const bool val, const std::string msg);

/**
 * \brief Wrapper for testing if a function correctly raises exceptions
 * \details Executes given functions and returns true if the target exception happened and false 
 * 		if a different exception occurred. If nullptr specified instead of exception object, 
 * 		no exception will return true and exception will return false. All exceptions are
 * 		handled.
 * 	@param verbose [in] - prints exception messages
 * 	@param expected [in] - expected exception type or nullptr for no exception 	
 * 	@param fun [in] - function object (pointer, lambda) to be called by the wrapper
 * 	@param args [in] - arguments to the function as a parameter pack 
 * 	@return True or fals if test passed/failed under given conditions
*/
template<typename Functor, typename... Args>
bool exception_test(bool verbose, const std::exception* expected, 
				Functor fun, Args&&... args);
/// Overload for free functions and lambdas
/// This is used just for the calling part
template<typename Functor, typename... Args >
void exception_test_helper(std::false_type, Functor fun, Args&&... args);
/// Overload for member functions
/// More here: https://stackoverflow.com/a/22972231
template<typename Functor, typename Arg0, typename... Args >
void exception_test_helper(std::true_type, Functor fun, Arg0&& arg0, Args&&... args);

//
// Comparisons
//

/**
 * \brief Compares two 2D vectors of type appropriate for identical comparison
 * \details Suitable types include int, bool, std::string; will not work on float 
 * 		or double
 * @param [in] vec_1 - first vector
 * @param [in] vec_2 - second vector
 * @return true if all inner vectors are identical and the outer vectors
 * 		are the same size
*/
template <typename T>
bool is_equal_exact(const std::vector<std::vector<T>>, const std::vector<std::vector<T>>);

/**
 * \brief Compares two 2D vectors of type not appropriate for identical comparison
 * \details Suitable types include float and double; will work on other types too 
 * 		but when possible better use its exact counterpart
 * @param [in] vec_1 - first vector
 * @param [in] vec_2 - second vector
 * @param [in] tol - tolerance
 * @return true if all inner vectors are identical and the outer vectors
 * 		are the same size
*/
template <typename T>
bool is_equal_floats(const std::vector<std::vector<T>>, const std::vector<std::vector<T>>, 
						const T);

/**
 * \brief Compare num1 and num2 as inexact values
 * \details For floating point compoarisons, i.e. floats, doubles
 * Current version based on 
 * 		https://stackoverflow.com/a/15012792/2763915
 * @param num1 - first number
 * @param num2 - second number
 * @param tol - relative tolerance for comparison
 * @return true if equal within the tolerance
 */
template <typename T>
bool float_equality(T, T, T);

// True if correct exception behavior for a function call with arguments args
// Currently uses tag dispatch to accomodate both free and member function calls
template<typename Functor, typename... Args>
bool exception_test(bool verbose, const std::exception* expected, 
				Functor fun, Args&&... args)
{
	try {
		exception_test_helper(typename std::is_member_pointer<Functor>::type(), fun, std::forward<Args>(args)...);
		if (verbose)
			print_msg("No exception occurred");
		return false;
	} catch (const std::exception& e){
		if (verbose)
			print_msg(e.what());
		if (typeid(*expected) != typeid(e))
			return false;
	}
	return true;
}

template<typename Functor, typename... Args >
void exception_test_helper(std::false_type, Functor fun, Args&&... args)
{	
	fun(std::forward<Args>(args)...);
}
template <class Functor, class Arg0, class... Args>
void exception_test_helper(std::true_type, Functor fun, Arg0&& arg0, Args&&... args)
{
    (std::forward<Arg0>(arg0).*fun)(std::forward<Args>(args)...);
}

// Direct comparison of two 2D vectors
template <typename T>
bool is_equal_exact(const std::vector<std::vector<T>> vec1, const std::vector<std::vector<T>> vec2)
{
	if (vec1.size() != vec2.size())
		return false;
	for (int i=0; i<vec1.size(); i++)
		if (!(vec1.at(i) == vec2.at(i)))
			return false;
	return true;
}

// Indirect comparison 
template <typename T>
bool is_equal_floats(const std::vector<std::vector<T>> vec1, 
				const std::vector<std::vector<T>> vec2, const T tol)
{
	if (vec1.size() != vec2.size())
		return false;
	for (int i=0; i<vec1.size(); i++){
		if (vec1.at(i).size() != vec2.at(i).size())
			return false;
		for (int j=0; j<vec1.at(i).size(); j++)
			if (!(float_equality(vec1.at(i).at(j), vec2.at(i).at(j), tol)))
				return false;
	}
	return true;
}

// Compares two floating point numbers for approximate equality
template <typename T>
bool float_equality(T num1, T num2, T tol)
{
	T max_num_one = std::max({1.0, std::fabs(num1) , std::fabs(num2)});
    return std::fabs(num1 - num2) <= tol*max_num_one;
}

#endif
