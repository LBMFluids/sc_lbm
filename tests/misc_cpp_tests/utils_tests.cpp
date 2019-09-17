#include "../common/test_utils.h"
#include "../../include/utils.h"
#include <numeric>

/*************************************************************** 
 * Suite for testing utility functions in utility.h 
***************************************************************/

// Supporting functions
template<typename T>
bool equal_vec2D_flat_array(const std::vector<std::vector<T>>, const T*, 
								const size_t, const size_t);

// Tests
bool sub2ind_test_suite();
bool sub2ind_test(const size_t Nx, const size_t Ny);
bool flat2vec_test_suite();
bool flat2vec_int(const size_t Nrow, const size_t Ncol);
bool vec2flat_test_suite();
bool vec2flat_int(const size_t Nrow, const size_t Ncol);

int main()
{
	tst_pass(sub2ind_test_suite(), "Subscript to linear index conversion");
	tst_pass(flat2vec_test_suite(), "Flat array to 2D vector conversion");
	tst_pass(vec2flat_test_suite(), "2D vector to flat array conversion");
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
bool sub2ind_test(const size_t Nx, const size_t Ny)
{
	std::vector<std::vector<size_t>> sub_vec(Nx*Ny, std::vector<size_t>(2));	
	std::vector<size_t> lin_vec;
	size_t ik = 0;
	for (size_t ii=0; ii<Ny; ii++){
		for (size_t ij=0; ij<Nx; ij++){
			sub_vec.at(ik).at(0) = ij; 
			sub_vec.at(ik).at(1) = ii;
			ik++;
		}
	}	
	lin_vec = sub2ind<size_t>(sub_vec, Nx);
	for (size_t ii=0; ii < Nx*Ny; ii++)
		if (lin_vec.at(ii) != ii)
			return false;
	return true;
}

/**
 * \brief Check conversion from 1D array to a nested vector
 */
bool flat2vec_test_suite()
{
	if (!flat2vec_int(3,7))
		return false;	
	if (!flat2vec_int(1,1))
		return false;
	if (!flat2vec_int(1,5))
		return false;
	if (!flat2vec_int(6,1))
		return false;
	return true;
}

/** 
 * \brief Actual flat to 2D vector conversion test
 * \details Creates a flat int array, converts it to vector,
 *		checks if what expected
 * @param Nrow - number of vector rows
 * @param Ncol - number of vector columns
 * @return true if the vector is as expected, false otherwise
 */ 
bool flat2vec_int(const size_t Nrow, const size_t Ncol)
{
	// Array has size Nrow x Ncol and will has all
	// elements consecutive ints
	int* arr_int = new int[Nrow*Ncol]();
	std::iota(arr_int, arr_int+Nrow*Ncol, 1);

	// Convert to 2D vector
	std::vector<std::vector<int>> vec2D_int;
	vec2D_int = flat_array_2_vector_2D<int>(Nrow, Ncol, arr_int);

	// Compare by contents and size
	bool equal = equal_vec2D_flat_array<int>(vec2D_int, arr_int, Nrow, Ncol);

	delete[] arr_int;
	return equal;
}

/**
 * \brief Check conversion from a nested vector to a 1D array
 */
bool vec2flat_test_suite()
{
	if (!vec2flat_int(3,7))
		return false;	
	if (!vec2flat_int(1,1))
		return false;
	if (!vec2flat_int(1,5))
		return false;
	if (!vec2flat_int(6,1))
		return false;
	return true;
}

/** 
 * \brief Actual 2D vector to 1D array conversion test
 * \details Creates a flat int array, converts it to vector,
 *		checks if what expected
 * @param Nrow - number of vector rows
 * @param Ncol - number of vector columns
 * @return true if the vector is as expected, false otherwise
 */ 
bool vec2flat_int(const size_t Nrow, const size_t Ncol)
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
	int* arr_int = new int[Nrow*Ncol]();

	// Convert to 2D vector to array arr_int
	vector_2D_2_flat_array<int>(vec2D_int, Nrow*Ncol, arr_int);

	// Compare by contents and size
	bool equal = equal_vec2D_flat_array<int>(vec2D_int, arr_int, Nrow, Ncol);

	delete[] arr_int;
	return equal;
}
/** 
 * \brief Compares content and sizes of a 2D vector and 1D array of types T
 * @param vec2D - nested vector of type T and dimensions Nrow x Ncol
 * @param flat_arr - 1D array of type T and number of elements Nrow*Ncol, 
 * 						stored with row-major order
 * @param Nrow - number of inner vectors i.e. number of rows
 * @param Ncol - number of elements in each inner vector i.e. number of columns
 * @return true if correct sizes and all elements equal
 */
template<typename T>
bool equal_vec2D_flat_array(const std::vector<std::vector<T>> vec2D, const T* flat_arr, 
							const size_t Nrow, const size_t Ncol)
{
	// Check if sizes match
	if (vec2D.size()*vec2D.at(0).size() != Nrow*Ncol)
		return false;
	// Check elements for equality (for floats - add a functor with custom comparison)
	size_t ind = 0;
	for (auto row : vec2D)
		for (auto elem : row)	
			if (elem != flat_arr[ind++])
				return false;
	return true;
}





