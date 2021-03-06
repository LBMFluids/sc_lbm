#ifndef UTILS_H
#define UTILS_H

#include "common.h"

/*************************************************************** 
 * Common utility functions 
***************************************************************/

/** 
 * \brief Convert from subscript (i,j) to linear index
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
 * \brief Convert a flat array of type T to nested vector of type T
 * \details Assumes all inner vectors have the same size. 
 * @param Nrow [in] - number of rows (inner vectors) 
 * @param Ncol [in] - number of columns (elements in the inner vectors)
 * @param flat_arr [in] - pointer to a flat array of type T
 * @return - vector of vectors of type T
*/
template<typename T>
std::vector<std::vector<T>> flat_array_2_vector_2D(const size_t Nrow, const size_t Ncol, const T* flat_arr);

/** 
 * \brief Convert a nested vector of type T to flat array of type T
 * \details Size of the array is #inner vectors x #elements in inner vectors
 * 		Number of elements in the inner vectors assumed the same across all
 * 		inner vectors and is determined based on the first vector.
 * @param vec2D - vector of vectors of type T
 * @param Nelem [in] - number of elements in flat_arr
 * @param flat_arr [out] - pointer to a flat array of type T and size Nelem
*/
template<typename T>
void vector_2D_2_flat_array(std::vector<std::vector<T>> vec2D, const size_t Nelem, T* flat_arr);

/**
 * \brief Convert a string to all lower case
 * \details From https://en.cppreference.com/w/cpp/string/byte/tolower
 * @param [in] s - string which will be converted to all lower case
 */
std::string str_to_lower(std::string s); 

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
std::vector<std::vector<T>> flat_array_2_vector_2D(const size_t Nrow, const size_t Ncol, const T* flat_arr)
{
	std::vector<std::vector<T>> vec2D(Nrow, std::vector<T>(Ncol));
	size_t ind = 0;
	for (size_t ir=0; ir < Nrow; ir++){
		for (size_t icl = 0; icl < Ncol; icl++){
			ind = Ncol*ir + icl;
			vec2D.at(ir).at(icl) = flat_arr[ind];
		}
	}
	return vec2D;
}

template<typename T>
void vector_2D_2_flat_array(std::vector<std::vector<T>> vec2D, const size_t Nelem, T* flat_arr)
{
	size_t Nrow = vec2D.size(), Ncol = vec2D.at(0).size(), ind = 0;
	if (Nelem != Nrow*Ncol)
		throw std::range_error("Flat array and 2D vector sizes mismatch");
	for (auto row : vec2D){
		std::copy(row.begin(), row.end(), flat_arr + ind*Ncol);
		ind++;
	}
}

#endif
