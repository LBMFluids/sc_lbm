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

	std::string the_text = {"Space: The final frontier" 
								" These are the voyages of the Starship, Enterprise"
								" Its 5 year mission, to explore strange new worlds"
								" To seek out new life and new civilizations"
								" To boldly go where no man has gone before "};
	std::string header = {"This is a logger test, you should see a header printed"
							" and followed by some basic print outs"
							" some with time stamps; finally red error message"
							" and a yellow warning"};

	// Titles, headers, information  
	logger.log("\n");
	logger.name_current_step("Running the basic_screen_test");
	logger.log("\n");
	logger.print_with_header(header);
	logger.log("\n");
	logger.log_details(the_text);
	logger.log("\n");

	// General functionality
	logger.print_time();
	logger.log("Cukni vo drvo");
	logger.log_with_time("Cukni tri pati");
	
	// Errors and warnings
	logger.log("\n");
	logger.print_error("Pesni stihovi");
	logger.print_warning("Celi fermani! :)");
	logger.log("\n");

	return true;
}

bool basic_file_test()
{
	Logger logger("test.log");
	
	std::string the_text = {"Space: The final frontier" 
								" These are the voyages of the Starship, Enterprise"
								" Its 5 year mission, to explore strange new worlds"
								" To seek out new life and new civilizations"
								" To boldly go where no man has gone before "};
	std::string header = {"This is a logger test, you should see a header printed"
							" and followed by some basic print outs"
							" some with time stamps; finally red error message"
							" and a yellow warning"};

	// Titles, headers, information  
	logger.log("\n");
	logger.name_current_step("Running the basic_screen_test");
	logger.log("\n");
	logger.print_with_header(header);
	logger.log("\n");
	logger.log_details(the_text);
	logger.log("\n");
	
	// General functionality
	logger.print_time();
	logger.log("Veliki je put, velika su bespuca");
	logger.log_with_time("Veliki je grad, velika provincija");

	// Errors and warnings
	logger.log("\n");
	logger.print_error("Ima jedno nesto");
	logger.print_warning("nesto najvece");
	logger.log("\n");

	return true;
}

