#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <algorithm>
#include <stdexcept>
#include <cmath>
#include "common.h"

// Model geometry
class Geometry{
public:
	// Constructor
	Geometry(const size_t, const size_t);
	// Copy constructor and operator
	
	// Move constructor and operator
	
	// Walls 	
	void add_walls(const size_t dH=1, const std::string where ="y");
	// Objects
	void add_rectangle(const std::vector<size_t>);
	void add_ellipse(const std::vector<size_t>);
	// Arrays of objects 
	void add_array(const std::vector<size_t>, const std::vector<size_t>, 
					const std::string, const size_t alpha=0);
	// Indexing
	const bool operator()(const size_t i, const size_t j) const { return geom[i][j]; } 	
	// I/O
	void write(const std::string) const;
	void read(const std::string);
	// Destructor
	~Geometry() { delete geom[]; }
private:
	// Geometric domain
	bool** geom;
	// Pi
	const double pi = std::acos(-1);
};

#endif

