#ifndef LBM_H
#define LBM_H

#include <iostream>
#include <algorithm>
#include <vector>
#include "geometry.h"
#include "fluid.h"
#include "logger.h"
#include "common.h"
#include "utils.h"
#include "./io_operations/lbm_io.h"

/***************************************************** 
 * class: LBM 
 * 
 * Interface class that provides all the LBM operations
 *
 ******************************************************/

class LBM {
public:

	LBM() = default;

	/// Collision step for a single fluid
	void collide(Fluid&);

	/// Add an external volume force to a single fluid (gravity, pressure drop)	
	void add_volume_force(const Geometry&, Fluid&, const std::vector<double>&); 

	/// Streaming step for a single fluid
	void stream(const Geometry& geom, Fluid&);

// stream(geom, fluid1, fluid2)
// stream(geom, fluid1)

private:
	// Number of directions
	const size_t Ndir = 9;
};

#endif
