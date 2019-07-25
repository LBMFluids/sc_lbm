#ifndef RECTANGLE_H
#define RECTANGLE_H 

/*************************************************************** 
 * class: rectangle
 *
 * Drived from geom_object, defines a rectangle by computing
 * the nodes it should occupy from given dimensions and 
 * center.  
***************************************************************/

/**
 * \brief Rectangle object
 */
class Rectangle : public geom_object
{
public:
	Rectangle() = default;
	/// \brief Create a rectangle with
	/// @param Lx [in] - number nodes in x direction
	/// @param Ly [in] - number of nodes in y direction
	/// @param xc [in] - center x coordinate
	/// @param yc [in] - center y coordinate
	Rectangle(size_t Lx, size_t Ly, size_t xc, size_t yc) :
		  geom_object(xc, yc), _Lx(Lx), _Ly(Ly) { find_nodes(); }	
protected:
	void find_nodes();
private:
	size_t _Lx = 0, _Ly = 0; 
};

#endif
