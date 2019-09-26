#include "test_utils.h"

/*************************************************************** 
 * Tests of general testing utility functions
***************************************************************/

void tst_pass_test();
bool exception_wrapper_test(bool verbose=false);
bool exception_wrapper_member_functions_test(bool verbose=false);
int exception_helper_v1(double, double);
void exception_helper_v2(bool);
bool equal_doubles_test(bool verbose=false);

// For member function exception testing
class TestClass{
public:
	// Member function - two args, two types of error
	// one inherited from another
	bool throwing_f(double d, int i)
	{ 
		if ((d > 0.0) && (i > 0))
			throw std::runtime_error("d and i larger than 0");
		if (i == 0)
			throw std::range_error("i equal to 0");
		return true;
	}
	// Member function with differnt args and error type
	void throwing_bcast(bool b)
	{
		if (!b)
			throw std::bad_cast();
	}
};

int main()
{
	bool verbose = true;
	tst_pass_test();
	tst_pass(exception_wrapper_test(verbose), "Exception wrapper");
	tst_pass(exception_wrapper_member_functions_test(verbose), "Exception wrapper - member functions");
	tst_pass(equal_doubles_test(verbose), "Equality of doubles");
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

	// No exception
	if (exception_test(verbose, nullptr, exception_helper_v1, 0.5, 0.1))
		return false;
	// Correct exception
	if (!exception_test(verbose, &rtime, exception_helper_v1, 2000.0, 0.1))
		return false;
	// Correct exception different than above
	if (!exception_test(verbose, &range, exception_helper_v1, 2.0, 0.1))
		return false;
	// Correct exception, different function and arguments
	if (!exception_test(verbose, &cast, exception_helper_v2, false))
		return false;
	// No exception, different function and arguments
	if (exception_test(verbose, nullptr, exception_helper_v2, true))
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
			throw std::runtime_error("d1 larger than 10.0");
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

// Verifies correct behavior of exception test with
// exceptions and without them - version for classes,
// i.e. member functions
bool exception_wrapper_member_functions_test(bool verbose)
{
	const std::runtime_error rtime("Run time error");
	const std::range_error range("Run time - Range error");
	const std::bad_cast cast;
	TestClass tclass; 	

	// No exception
	if (exception_test(verbose, nullptr, &TestClass::throwing_f, tclass, -0.5, -1))
		return false;
	// Correct exception
	if (!exception_test(verbose, &rtime, &TestClass::throwing_f, tclass, 2000.0, 1))
		return false;
	// Correct exception different than above
	if (!exception_test(verbose, &range, &TestClass::throwing_f, tclass, 2.0, 0))
		return false;
	// Correct exception, different function and arguments
	if (!exception_test(verbose, &cast, &TestClass::throwing_bcast, tclass, false))
		return false;
	// No exception, different function and arguments
	if (exception_test(verbose, nullptr, &TestClass::throwing_bcast, tclass, true))
		return false;
	return true;
}

// Test the double equality function
// Add capability as needed 
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
		print_msg("Comparing 1.0 and 0.9999998...");
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
