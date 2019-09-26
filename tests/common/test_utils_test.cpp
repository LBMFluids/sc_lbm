#include "test_utils.h"

/*************************************************************** 
 * Tests of general testing utility functions
***************************************************************/

void tst_pass_test();
bool exception_wrapper_test(bool verbose=false);
int exception_helper_v1(double, double);
void exception_helper_v2(bool);
bool equal_doubles_test(bool verbose=false);

int main()
{
	tst_pass_test();
	tst_pass(exception_wrapper_test(true), "Exception wrapper");
	tst_pass(equal_doubles_test(true), "Equality of doubles");
	return 0;
}

// Verifies if printing right messages
void tst_pass_test()
{
	tst_pass(false, "Should fail");
	tst_pass(true, "Should work");
}

// Verifies correct behavior of exception test with
// exceptions and without them
bool exception_wrapper_test(bool verbose)
{
	const std::runtime_error rtime("Run time error");
	const std::range_error range("Run time - Range error");
	const std::bad_cast cast;

	std::function<int(double, double)> helper_functor_v1 = exception_helper_v1;
	std::function<void(bool)> helper_functor_v2 = exception_helper_v2;

	if (!exception_test(verbose, nullptr, helper_functor_v1, 0.5, 0.1))
		return false;
	if (!exception_test(verbose, &rtime, helper_functor_v1, 2000.0, 0.1))
		return false;
	if (!exception_test(verbose, &range, helper_functor_v1, 2.0, 0.1))
		return false;
	if (!exception_test(verbose, &cast, helper_functor_v2, false))
		return false;
	if (!exception_test(verbose, nullptr, helper_functor_v2, true))
		return false;
	return true;
}
// Functions to be called by exception wrapper
// The only meaning in choice of exception types here is to 
// test/demonstrate that the wrapper correctly captures their
// types and polymorphism
int exception_helper_v1(double d1, double d2)
{
	if (d1 > 1.0)
		if (d1 > 10.0)
			throw std::runtime_error("d1 larger than 1.0");
		else
			throw std::range_error("d1 larger than 1.0, smaller than 10.0");
 	else
		return (int)(d1+d2);	
}

void exception_helper_v2(bool b)
{
	if (!b)
		throw std::bad_cast();
}

// Test the double equality function
// Add capability as needed 
// Part of tests test for returning false i.e. unequal 
bool equal_doubles_test(bool verbose)
{
	double tol = 1.0e-5;
	if (verbose)
		print_msg("Comparing 1.0 and 1.0...");
	if (!float_equality(1.0, 1.0, tol))
		return false;
	//
	if (verbose)
		print_msg("Comparing 1.0 and 1.01 with tol 0.1...");
	if (!float_equality(1.0, 1.01, 1e-1))
		return false;
	//
	if (verbose)
		print_msg("Comparing 1.0 and 0.9999998 (should pass)...");
	if (!float_equality(1.0, 0.9999998, tol))
		return false;
	//
	if (verbose)
		print_msg("Comparing 1.0 and 0.99998...");
	if (float_equality(1.0, 0.99998, tol))
		return false;
	//
	if (verbose)
		print_msg("Comparing 1.0e10 and 1.001e10... - need to expand for large numbers");
	if (float_equality(1.0e10, 1.001e10, tol))
		return false;
	//
	if (verbose)
		print_msg("Comparing 1.0 and -1.0...");
	if (float_equality(1.0, -1.0, tol))
		return false;
	return true;
}
