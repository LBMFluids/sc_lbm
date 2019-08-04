#include "test_utils.h"

// Prints message test name tname with color that depends on val being
// true or false
void tst_pass(bool val, const std::string tname)
{
	if (!val)
		std::cout << "\033[1;35m  - " << tname << " test failed\033[0;0m" <<std::endl;
	else
		std::cout << "\033[1;32m  - " << tname << " test passed\033[0;0m" <<std::endl;
}
