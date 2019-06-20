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

	if (!exception_test(verbose, nullptr, exception_helper_v1, 0.5, 0.1))
		return false;
	if (!exception_test(verbose, &rtime, exception_helper_v1, 2000.0, 0.1))
		return false;
	if (!exception_test(verbose, &range, exception_helper_v1, 2.0, 0.1))
		return false;
	if (!exception_test(verbose, &cast, exception_helper_v2, false))
		return false;
	if (!exception_test(verbose, nullptr, exception_helper_v2, true))
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

// Test the double eqaulity function
// Add capability as needed 
bool equal_doubles_test(bool verbose)
{
	if (verbose)
		print_msg("Comparing 1.0 and 1.0...");
	if (!equal_doubles(1.0, 1.0))
		return false;
	//
	if (verbose)
		print_msg("Comparing 1.0 and 1.01 with tol 0.1...");
	if (!equal_doubles(1.0, 1.01, 1e-1))
		return false;
	//
	if (verbose)
		print_msg("Comparing 1.0 and 0.9999998 (should pass)...");
	if (!equal_doubles(1.0, 0.9999998))
		return false;
	//
	if (verbose)
		print_msg("Comparing 1.0 and 0.99998 (should fail)...");
	if (equal_doubles(1.0, 0.99998))
		return false;
	//
	if (verbose)
		print_msg("Comparing 1.0e10 and 1.001e10 (should fail)... - need to expand for large numbers");
	//if (equal_doubles(1.0e10, 1.001e10))
		//return false;
	//
	if (verbose)
		print_msg("Comparing 1.0 and -1.0...");
	if (equal_doubles(1.0, -1.0))
		return false;
	return true;
}
