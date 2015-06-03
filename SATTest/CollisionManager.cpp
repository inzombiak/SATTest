#include "CollisionManager.h"

#include <algorithm>

bool CollisionManager::CheckCollision(Polygon& p1, Polygon& p2)
{
	bool result = true;

	//Get the points of each shape
	auto p1Points = p1.GetPoints();
	auto p2Points = p2.GetPoints();
	
	//Minimum translation axis.
	sf::Vector2f mtAxis;
	//Minimum translation value. When united with mtAxis we get the minimum translation vector
	double mtValue = DBL_MAX;
	double temp;
	//If no points return
	if (p1Points.size() < 1 || p2Points.size() < 1)
		return false;

	//Generate axes to check for each shape
	std::vector<sf::Vector2f> p1Axes = p1.GetAxes(), p2Axes = p2.GetAxes();

	Projection p1Proj, p2Proj;
	bool test = true;

	//After calculating the axes, we can begin checking for overlaps
	for (int i = 0; i < p1Axes.size(); ++i)
	{

		//Get projections
		p1Proj = GetProjection(p1, p1Axes[i]);
		p2Proj = GetProjection(p2, p1Axes[i]);

		//Check for overlap. If no overlap, then no interection between polygons so we return
		//Otherwise we compare to mtValue
		if (!DoProjectionsOverlap(p1Proj, p2Proj))
			return false;
		else
		{
			//Get value of overlap
			temp = CalculateOverlap(p1Proj, p2Proj);

			//If it is less than pervious, change mtValue and mtAxis accordingly
			if (temp < mtValue)
			{
				mtValue = temp;
				mtAxis = p1Axes[i];
			}
		}
	}

	//After calculating the axes, we can begin checking for overlaps
	for (int i = 0; i < p2Axes.size(); ++i)
	{

		//Get projections
		p1Proj = GetProjection(p1, p2Axes[i]);
		p2Proj = GetProjection(p2, p2Axes[i]);

		//Check for overlap. If no overlap, then no interection between polygons so we return
		//Otherwise we compare to mtValue
		if (!DoProjectionsOverlap(p1Proj, p2Proj))
			return false;
		else
		{
			//Get value of overlap
			temp = CalculateOverlap(p1Proj, p2Proj);

			//If it is less than pervious, change mtValue and mtAxis accordingly
			if (temp < mtValue)
			{
				mtValue = temp;
				mtAxis = p2Axes[i];
			}
		}
	}

	//If we got this far then the objects are intersecting and we have an MTV.
	//We use this MTV to push objects out of on another, in this case p2 out of p1
	sf::Vector2f directionVector, MTV;

	//Calculate direction vector
	directionVector = sf::Vector2f(p2.GetPosition().x - p1.GetPosition().x, 
		p2.GetPosition().y - p1.GetPosition().y);
	
	//Get MTV
	MTV = CalculateMTV(mtAxis, mtValue, directionVector);

	//Move p2
	p2.SetPosition(sf::Vector2f(p2.GetPosition().x + MTV.x, p2.GetPosition().y + MTV.y));

	return true;
}

CollisionManager::Projection CollisionManager::GetProjection(const Polygon& poly, sf::Vector2f axis)
{
	Projection result;
	auto points = poly.GetPoints();

	//Set min/max to the first point for a base case
	result.min = points[0].x * axis.x + points[0].y * axis.y;
	result.max = result.min;

	float newProj;

	for (int i = 1; i < points.size(); ++i)
	{
		//Get projection of the each point, change min/max if needed
		newProj = points[i].x * axis.x + axis.y * points[i].y;

		if (newProj < result.min)
			result.min = newProj;
		else if (newProj > result.max)
			result.max = newProj;
	}
	return result;
}

bool CollisionManager::DoProjectionsOverlap(const CollisionManager::Projection& proj1, const CollisionManager::Projection& proj2)
{
	bool result = true;

	//If there is a gap, then no overlap
	if ((proj1.max <= proj2.min) || (proj2.max <= proj1.min))
		result = false;

	return result;
}

double CollisionManager::CalculateOverlap(const CollisionManager::Projection& proj1, const CollisionManager::Projection& proj2)
{
	return std::max(0.f, std::min(proj1.max, proj2.max) - std::max(proj1.min, proj2.min));
}

sf::Vector2f CollisionManager::CalculateMTV(sf::Vector2f axis, double magnitude, sf::Vector2f directionVector)
{
	//Calculate slope of axis
	double angle = atan(axis.y / axis.x);

	//Calculate change along x and y axes
	sf::Vector2f result(cos(angle)*magnitude, sin(angle)*magnitude);

	//Check to make sure MTV isn't pointing towards a polygon
	if (result.x*directionVector.x + result.y * directionVector.y < 0)
	{
		result.x = -result.x;
		result.y = -result.y;
	}

	return result;
}