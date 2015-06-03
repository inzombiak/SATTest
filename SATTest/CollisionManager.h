#ifndef COLLISION_MANAGER_H
#define COLLISION_MANAGER_H

#include "Polygon.h"

class CollisionManager
{
public:
	
	//Check for collision between points, returns true if colliding, otherwise false. Also pushes objects out of one another
	bool CheckCollision(Polygon& poly1,Polygon& poly2);



private:

	//Struct for polygon projection
	struct Projection
	{
		float min, max;
	};

	//Get projection of a polygon on an axis
	Projection GetProjection(const Polygon& poly, sf::Vector2f axis);

	//Checks if projections overlap.
	bool DoProjectionsOverlap(const Projection& proj1, const Projection& proj2);

	//Returns overlap of two projections
	double CalculateOverlap(const Projection& proj1, const Projection& proj2);

	//Returns the MTV. Takes MTV axis, magnitude and the direction vector between the two polygons
	sf::Vector2f CalculateMTV(sf::Vector2f axis, double magnitude, sf::Vector2f directionVec);
};

#endif