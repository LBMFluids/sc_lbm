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
 * @param Ny [in] - total number of nodes in y direction 
 * @return - vector of linear indices 
*/
template<typename T>
std::vector<T> sub2ind(std::vector<std::vector<T>>&& subs, const size_t Ny);
/// \brief sub2ind overload for lvalue references
template<typename T>
std::vector<T> sub2ind(std::vector<std::vector<T>>& subs, const size_t Ny);

template<typename T>
std::vector<T> sub2ind(std::vector<std::vector<T>>&& subs, const size_t Ny)
{
	size_t i = 0;
	std::vector<T> lin(subs.size());
	for (const auto& sb : subs)
		lin.at(i++) = sb.at(0)*Ny + sb.at(1);	
	return lin;
}

template<typename T>
std::vector<T> sub2ind(std::vector<std::vector<T>>& subs, const size_t Ny)
{
	size_t i = 0;
	std::vector<T> lin(subs.size());
	for (const auto& sb : subs)
		lin.at(i++) = sb.at(0)*Ny + sb.at(1);	
	return lin;
}

#endif
