#include "../../include/geometry.h"
#include "../../include/fluid.h"
#include "../../include/utils.h"
#include "../common/test_utils.h"

/***************************************************** 
 *
 * Test suite for the Fluid class
 *
 ******************************************************/

// Tests
bool empty_geom();
bool fluid_with_walls();
bool staggered_array();

int main()
{
	test_pass(empty_geom(), "Just the fluid");
	test_pass(fluid_with_walls(), "Fluid surrounded by walls");
	test_pass(staggered_array(), "Fluid and a staggered array");
}

bool empty_geom()
{
	return true;
}

bool fluid_with_walls()
{
	return true;
}

bool staggered_array()
{
	return true;
}


