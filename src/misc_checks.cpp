#include "../include/misc_checks.h"

/*************************************************************** 
 * Functions used for checking various conditions that are
 * used by multiple classes/functions from the main program
 * and not the tests.  
***************************************************************/

// True if number of nodes in given dimension is even
std::vector<bool> check_is_even_num_nodes(const size_t Nx, const size_t Ny)
{
	std::vector<bool> is_even = {false, false};
	if (!(Nx%2)){
		std::cout << "\033[1m\033[35m" << "*** WARNING ***\n"
			      << "Number of nodes in x direction is EVEN. Using it will result "
			  	  << "in an object that is not symmetric." 
				  << " Reducing number of nodes by 1." 
				  << "\033[0m" << std::endl;
  		is_even[0] = true;		
	}
	if (!(Ny%2)){
		std::cout << "\033[1m\033[35m" << "*** WARNING ***\n"
			      << "Number of nodes in y direction is EVEN. Using it will result "
			  	  << "in an object that is not symmetric." 
				  << " Reducing number of nodes by 1." 
				  << "\033[0m" << std::endl;
		is_even[1] = true;
	}
	return is_even;
}
