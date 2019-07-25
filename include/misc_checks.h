#ifndef MISC_CHECKS_H
#define MISC_CHECKS_H

#include "common.h"

/*************************************************************** 
 * Functions used for checking various conditions that are
 * used by multiple classes/functions from the main program
 * and not the tests.  
***************************************************************/

/**
 * \brief Verify if the number of nodes in both directions is even. 
 * \details If the number of nodes in one of the directions is even
 *		throws a warning about that direction.
 * @param[in] Nx - number of nodes in x direction
 * @param[in] Ny - number of nodes in y direction
 * @return Vector indicating if the node number in x and y dimension respectively
 * 				is even. 
 */
std::vector<bool> check_is_even_num_nodes(const size_t Nx, const size_t Ny);

#endif
