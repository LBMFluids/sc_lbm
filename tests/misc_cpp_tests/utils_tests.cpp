#include "../common/test_utils.h"
#include "../../include/utils.h"

/*************************************************************** 
 * Suite for testing utility functions in utility.h 
***************************************************************/

// Tests
bool sub2ind_test_suite();
bool sub2ind_test(size_t Nx, size_t Ny);

int main()
{
	tst_pass(sub2ind_test_suite(), "Subscript to linear index conversion");
}

///\brief Series of tests for sub to linear index conversion
bool sub2ind_test_suite()
{
	// Test 1 - both dimensions != 1
	if (!sub2ind_test(5, 11))
		return false;
	if (!sub2ind_test(1, 101))
		return false;
	if (!sub2ind_test(509, 1))
		return false;
	return true;
}

///\brief Tests subscript to linear index conversion
/// Creates a row-major vector of all subscripts, 
/// 	computes linear index for each using sub2ind,
///		and then compares the result to linearly iterated
///		loop variable (should be exactly the same)
bool sub2ind_test(size_t Nx, size_t Ny)
{
	std::vector<std::vector<size_t>> sub_vec(Nx*Ny, std::vector<size_t>(2));	
	std::vector<size_t> lin_vec;
	size_t ik = 0;
	for (size_t ii=0; ii<Nx; ii++){
		for (size_t ij=0; ij<Ny; ij++){
			sub_vec.at(ik).at(0) = ii; 
			sub_vec.at(ik).at(1) = ij;
			ik++;
		}
	}	
	lin_vec = sub2ind<size_t>(sub_vec, Ny);
	for (size_t ii=0; ii < Nx*Ny; ii++)
		if (lin_vec.at(ii) != ii)
			return false;
	return true;
}


