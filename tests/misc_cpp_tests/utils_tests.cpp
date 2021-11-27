#include "../common/test_utils.h"
#include "../../include/utils.h"
#include <numeric>

/*************************************************************** 
 * Suite for testing utility functions in utility.h 
 ***************************************************************/

// Supporting functions
template<typename T>
bool equal_vec2D_flat_vec(const std::vector<std::vector<T>>, 
								const std::vector<T>, const size_t, const size_t);

// Tests
bool sub2ind_test_suite();
bool sub2ind_test(const size_t Nx, const size_t Ny);
bool flat2nested_test_suite();
bool flat2nested_int(const size_t Nrow, const size_t Ncol);
bool nested2flat_test_suite();
bool nested2flat_int(const size_t Nrow, const size_t Ncol);

int main()
{
	test_pass(sub2ind_test_suite(), "Subscript to linear index conversion");
	test_pass(flat2nested_test_suite(), "Flat vector to 2D vector conversion");
	test_pass(nested2flat_test_suite(), "2D vector to flat vector conversion");
}

/// \brief Series of tests for sub to linear index conversion
bool sub2ind_test_suite()
{
	if (!sub2ind_test(5, 11)) {
		std::cout << "Test for " << 5 << "x" << 7 << "vector failed" << std::endl;
		return false;
	}
	if (!sub2ind_test(1, 101)) {
		std::cout << "Test for " << 1 << "x" << 101 << "vector failed" << std::endl;
		return false;
	}
	if (!sub2ind_test(509, 1)) {
		std::cout << "Test for " << 509 << "x" << 1 << "vector failed" << std::endl;
		return false;
	}
	return true;
}

/// \brief Tests subscript to linear index conversion
/// \details Creates a row-major vector of all subscripts, 
/// 	computes linear index for each using sub2ind,
///		and then compares the result to linearly iterated
///		loop variable (should be exactly the same)
bool sub2ind_test(const size_t Nx, const size_t Ny)
{
	std::vector<std::vector<size_t>> sub_vec(Nx*Ny, std::vector<size_t>(2));	
	std::vector<size_t> lin_vec;
	size_t ik = 0;
	for (size_t ii=0; ii<Ny; ii++) {
		for (size_t ij=0; ij<Nx; ij++) {
			sub_vec.at(ik).at(0) = ij; 
			sub_vec.at(ik).at(1) = ii;
			ik++;
		}
	}	
	lin_vec = sub2ind<size_t>(sub_vec, Nx);
	for (size_t ii=0; ii < Nx*Ny; ii++) {
		if (lin_vec.at(ii) != ii) {
			std::cout << lin_vec.at(ii) << " not matching expected " 
					  << ii << std::endl; 
			return false;
		}
	}
	return true;
}

/**
 * \brief Check conversion from a flat to a nested vector
 */
bool flat2nested_test_suite()
{
	if (!flat2nested_int(3,7)) {
		std::cout << "Test for " << 3 << "x" << 7 << "vector failed" << std::endl;
		return false;
	}	
	if (!flat2nested_int(1,1)) {
		std::cout << "Test for " << 1 << "x" << 1 << "vector failed" << std::endl;
		return false;
	}
	if (!flat2nested_int(1,5)) {
		std::cout << "Test for " << 1 << "x" << 5 << "vector failed" << std::endl;
		return false;
	}
	if (!flat2nested_int(6,1)) {
		std::cout << "Test for " << 6 << "x" << 1 << "vector failed" << std::endl;
		return false;
	}
	return true;
}

/** 
 * \brief Flat to 2D vector conversion test
 * \details Creates a flat int vector, 
 *		converts it to a nested vector,
 *		checks if what expected
 *
 * @param Nrow - number of vector rows
 * @param Ncol - number of vector columns
 * @return true if the vector is as expected, false otherwise
 */ 
bool flat2nested_int(const size_t Nrow, const size_t Ncol)
{
	// Vector has size Nrow x Ncol and will have all
	// elements consecutive ints
	std::vector<int> flat_vec(Nrow*Ncol);
	std::iota(flat_vec.begin(), flat_vec.end(), 1);

	// Convert to 2D vector
	std::vector<std::vector<int>> vec2D;
	vec2D = flat_vector_2_vector_2D<int>(Nrow, Ncol, flat_vec);

	// Compare by contents and size
	bool equal = equal_vec2D_flat_vec<int>(vec2D, flat_vec, Nrow, Ncol);

	return equal;
}

/**
 * \brief Check conversion from a nested vector to a 1D array
 */
bool nested2flat_test_suite()
{
	if (!nested2flat_int(3,7))
		return false;	
	if (!nested2flat_int(1,1))
		return false;
	if (!nested2flat_int(1,5))
		return false;
	if (!nested2flat_int(6,1))
		return false;
	return true;
}

/** 
 * \brief Actual 2D vector to 1D array conversion test
 * \details Creates a flat int array, converts it to vector,
 *		checks if what expected
 * 
 * @param Nrow - number of vector rows
 * @param Ncol - number of vector columns
 * @return true if the vector is as expected, false otherwise
 */ 
bool nested2flat_int(const size_t Nrow, const size_t Ncol)
{
	// Create a 2D vector with consecutive integers
	// as elements
	std::vector<std::vector<int>> vec2D_int(Nrow, std::vector<int>(Ncol));
	int iel = 1; 
	for (auto& row : vec2D_int){
		std::iota(row.begin(), row.end(), iel);
		iel += Ncol;
	}
	// 0 initialized array 
	std::vector<int> flat_vec(Nrow*Ncol);

	// Convert to 2D vector to a flat vector
	vector_2D_2_flat_vector<int>(vec2D_int, Nrow*Ncol, flat_vec);

	// Compare by contents and size
	bool equal = equal_vec2D_flat_vec<int>(vec2D_int, flat_vec, Nrow, Ncol);
	return equal;
}

/** 
 * \brief Compares content and sizes of a 2D vector and 1D array of types T
 *
 * @param vec2D - nested vector of type T and dimensions Nrow x Ncol
 * @param flat_arr - 1D array of type T and number of elements Nrow*Ncol, 
 * 						stored with row-major order
 * @param Nrow - number of inner vectors i.e. number of rows
 * @param Ncol - number of elements in each inner vector i.e. number of columns
 * @return true if correct sizes and all elements equal
 */
template<typename T>
bool equal_vec2D_flat_vec(const std::vector<std::vector<T>> vec2D, const std::vector<T> flat_vec, 
							const size_t Nrow, const size_t Ncol)
{
	// Check if sizes match
	if (vec2D.size()*vec2D.at(0).size() != flat_vec.size()) {
		std::cout << "Number of elements in the vectors is not the same" << std::endl;
		return false;
	}
	// Check elements for equality 
	size_t ind = 0;
	for (auto row : vec2D) {
		for (auto elem : row) {	
			if (elem != flat_vec.at(ind++)) {
				std::cout << "Values of elements don't match" << std::endl;
				return false;
			}
		}
	}
	// For an extra check
	for (const auto& row : vec2D) {
		for (const auto& el : row) {
			std::cout << el << " ";
		}
	}
	std::cout << std::endl;
	for (const auto& el : flat_vec) {
			std::cout << el << " ";
	}
	std::cout << std::endl;
	
	return true;
}





