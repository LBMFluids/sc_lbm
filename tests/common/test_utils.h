#ifndef TEST_UTILS_H
#define TEST_UTILS_H

#include <typeinfo>
#include "../../include/common.h"

/*************************************************************** 
 * General testing utility functions
***************************************************************/

/** \file */
/**
 * \brief Print message in specified color
 * @param [in] msg - content to print
 */
const auto print_msg = [](const std::string msg)
							{ std::cout << msg << std::endl; };
/** 
 * \brief Equality comparison with doubles
 * @param v1 [in] 
 * @param v2 [in] - two doubles to check for equality
 * @param eps [in] - relative tolerance, default 1e-5
*/
const auto equal_doubles = [](const double v1, const double v2, const double eps=1e-5)
							{ return !(std::abs((v1 - v2)/v1) > eps); };
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
 * 	@param function [in] - function to be called by the wrapper
 * 	@param args [in] - arguments to that function as a parameter pack 
 * 	@return True or fals if test passed/failed under given conditions
*/
template<typename R, typename... Args>
bool exception_test(bool verbose, const std::exception* expected, R(*function)(Args...), Args... args);

// True if correct exception behavior for a function call with arguments args
template<typename R, typename... Args>
bool exception_test(bool verbose, const std::exception* expected, R(*function)(Args...), Args... args)
{
	try {
		function(args...);
		if (verbose)
			print_msg("No exception occurred");
		if (expected != nullptr)
			return false;
	} catch (const std::exception& e){
		if (verbose)
			print_msg(e.what());
		if (typeid(*expected) != typeid(e))
			return false;
	}
	return true;
}

#endif
