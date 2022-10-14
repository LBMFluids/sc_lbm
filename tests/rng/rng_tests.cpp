#include <algorithm>
#include "../common/test_utils.h"
#include "../../include/rng.h"

/***************************************************** 
 *
 * Test suite for functionality of the RNG class
 *
 *****************************************************/

// Tests
bool uniform_real_test(double, double);

int main()
{
	test_pass(uniform_real_test(0.0, 2.5), "Uniform real distribution");
}

/// Test if the uniform distribution generation is correct
bool uniform_real_test(double a, double b)
{
	RNG rng;
	std::vector<double> rnum;

	for (int i=0; i<100000; ++i)
		rnum.push_back(rng.get_random(a, b));
	
	// Compare mean value
	double exp_mean = 0.5*(a + b);
	double rng_mean = std::accumulate(rnum.begin(), rnum.end(), 0.0)/static_cast<double>(rnum.size());
	
	if (!float_equality<double>(exp_mean, rng_mean, 0.01)){
		std::cout << exp_mean << " " << rng_mean << std::endl;
		return false;
	}
	
	return true;
}


