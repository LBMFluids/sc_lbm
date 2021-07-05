#include "../common/test_utils.h"
#include "../../include/logger.h"
#include <string>

// Tests
bool basic_screen_test();
bool basic_file_test();

int main()
{
	tst_pass(basic_screen_test(), "Basic functionality, standard output");
	tst_pass(basic_file_test(), "Basic functionality, file");
}

bool basic_screen_test()
{
	Logger logger;

	logger.print_time();
	logger.log("Cukni vo drvo");
	logger.log_with_time("Cukni tri pati");
	
	// Errors and warnings
	logger.print_error("Pesni stihovi");
	logger.print_warning("Celi fermani! :)");

	return true;
}

bool basic_file_test()
{
	Logger logger("test.log");

	logger.print_time();
	logger.log("Veliki je put, velika su bespuca");
	logger.log_with_time("Veliki je grad, velika provincija");
	logger.print_error("Ima jedno nesto");
	logger.print_warning("nesto najvece");

	return true;
}

