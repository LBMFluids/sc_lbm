#ifndef UTILS_H
#define UTILS_H

#include "common.h"

/*************************************************************** 
 * Common utility functions 
 ***************************************************************/

/** 
 * \brief Convert from subscript (i,j) to linear index
 *
 * @param subs [in] - vector of subscripts, each inner vector is a 
 * 		pair of x and y direction indices/subscripts
 * @param Nx [in] - total number of nodes in x direction 
 * @return - vector of linear indices 
 */
template<typename T>
std::vector<T> sub2ind(std::vector<std::vector<T>>&& subs, const size_t Nx);
/// \brief sub2ind overload for lvalue references
template<typename T>
std::vector<T> sub2ind(std::vector<std::vector<T>>& subs, const size_t Nx);

/** 
 * \brief Convert a flat vector of type T to nested vector of type T
 * \details Assumes all inner vectors have the same size.
 *		Size of a row is Ncols. 
 *
 * @param Nrow [in] - number of columns (elements in the inner vectors)  
 * @param Ncol [in] - number of rows (inner vectors)
 * @param flat_vec [in] - reference to a flat vector of type T
 * @return - vector of vectors of type T
*/
template<typename T>
std::vector<std::vector<T>> flat_vector_2_vector_2D(const size_t Ncol, 
								const size_t Nrow, const std::vector<T>& flat_vec);

/** 
 * \brief Convert a nested vector of type T to flat vector of type T
 * \details Size of the vector is #inner vectors x #elements in inner vectors
 * 		Number of elements in the inner vectors assumed the same across all
 * 		inner vectors and is determined based on the first vector.
 *		Final vector is: [inner_vec_1] ... [last inner vec]
 *
 * @param vec2D - reference to vector of vectors of type T
 * @preturn flat_vec - flat vector of type T and size Nelem
*/
template<typename T>
std::vector<T> vector_2D_2_flat_vector(const std::vector<std::vector<T>>& vec2D);

/**
 * \brief Convert a string to all lower case
 * \details From https://en.cppreference.com/w/cpp/string/byte/tolower
 *
 * @param [in] s - string which will be converted to all lower case
 */
std::string str_to_lower(std::string s); 

/**
 * \brief Prints optional string opstr followed by all elements from coll
   \details Prints in one line to stdout, separated by spaces; From Josuttis p. 216
 * 
 * @param coll - collection of elements
 * @param optstr - optional string proceeding the collection
 */
template<typename T>
inline void print_elements(const T& coll, const std::string& optstr="");

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
bool equal_floats(T, T, T);

//
// Implementation - templates
//

template<typename T>
std::vector<T> sub2ind(std::vector<std::vector<T>>&& subs, const size_t Nx)
{
	size_t i = 0;
	std::vector<T> lin(subs.size());
	for (const auto& sb : subs)
		lin.at(i++) = sb.at(1)*Nx + sb.at(0);	
	return lin;
}

template<typename T>
std::vector<T> sub2ind(std::vector<std::vector<T>>& subs, const size_t Nx)
{
	size_t i = 0;
	std::vector<T> lin(subs.size());
	for (const auto& sb : subs)
		lin.at(i++) = sb.at(1)*Nx + sb.at(0);	
	return lin;
}

template<typename T>
std::vector<std::vector<T>> flat_vector_2_vector_2D(const size_t Ncol, 
								const size_t Nrow, const std::vector<T>& flat_vec)
{
	std::vector<std::vector<T>> vec2D(Nrow, std::vector<T>(Ncol));
	size_t ind = 0;
	for (size_t ir=0; ir < Nrow; ++ir) {
		for (size_t icl = 0; icl < Ncol; ++icl) {
			ind = Ncol*ir + icl;
			vec2D.at(ir).at(icl) = flat_vec.at(ind);
		}
	}
	return vec2D;
}

template<typename T>
std::vector<T> vector_2D_2_flat_vector(const std::vector<std::vector<T>>& vec2D)
{
	size_t Nrow = vec2D.size(), Ncol = vec2D.at(0).size(), ind = 0;
	std::vector<T> flat_vec(Nrow*Ncol);
	for (auto row : vec2D) {
		auto it = flat_vec.begin() + ind*Ncol;
		std::copy(row.begin(), row.end(), it);
		++ind;
	}
	return flat_vec;
}

template<typename T>
inline void print_elements(const T& coll, const std::string& optstr)
{
	std::cout << optstr;
	for (const auto& elem : coll) {
		std::cout << elem << ' ';
	}
	std::cout << std::endl;
}

// Compares two floating point numbers for approximate equality
template <typename T>
bool equal_floats(T num1, T num2, T tol)
{
	T max_num_one = std::max({1.0, std::fabs(num1) , std::fabs(num2)});
    return std::fabs(num1 - num2) <= tol*max_num_one;
}

#endif
