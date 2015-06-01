#include "Polygon.h"

Polygon::Polygon(std::vector<sf::Vector2f> points, sf::Color color, sf::Vector2f position)
{
	//No points = nothing to do
	if (points.size() < 0)
		return;

	//Check if shape is concave or convex
	if (IsConvex(points))
	{
		//If it is convex, we only have 1 shape in m_shapes
		CreateConvexPolygon(points, color);
	}
	else
	{
		CreateConcavePolygon(points, color, position);
	}
}

void Polygon::SetPosition(sf::Vector2f newPosition)
{
	//m_shapes.setPosition(newPosition);
}
sf::Vector2f Polygon::GetPosition() const
{
	return m_shapes[0].getPosition();
}

void Polygon::SetColor(sf::Color newColor)
{
	for (int i = 0; i < m_shapes.size(); ++i)
		m_shapes[i].setFillColor(newColor);
}
sf::Color Polygon::GetColor() const
{
	return m_shapes[0].getFillColor();
}

std::vector<sf::Vector2f> Polygon::GetPoints() const
{
	//Container vector for the points
	std::vector<sf::Vector2f> result;

	//Get number of points of the polygon
	//unsigned int numberOfPoints = m_shape.getPointCount();

	//Add each point to the container vector
	//for (unsigned int i = 0; i < numberOfPoints; ++i)
	//{
	//	result.push_back(m_shape.getPoint(i));
	//}

	return result;
}

bool Polygon::IsConvex(std::vector<sf::Vector2f> points)
{
	//If we have less than 3 points (dot or line) return
	if (points.size() < 3)
		return true;

	//lengths on x and y axes between points i, i+1 and i+2
	float dx1, dy1, dx2, dy2, zComp;

	//Sign of z-components, false = positive, true = negative
	bool hasSign;

	//We get the sign of the z-component for the first 3 points to serve as a basis for the rest of out comparisons
	dx1 = points[1].x - points[0].x;
	dy1 = points[1].y - points[0].y;
	dx2 = points[2].x - points[1].x;
	dy2 = points[2].y - points[1].y;
	zComp = dx1*dy2 - dx2*dy1;
	hasSign = zComp < 0;

	//Check sign of z-component of the rest of the points on the polygon. 
	//If theyre all the same, we have a convex polygon, otherwise it is concave.
	unsigned int numberOfPoints = points.size();

	for (unsigned int i = 1; i < numberOfPoints; ++i)
	{
		dx1 = points[(i + 1) % numberOfPoints].x - points[i].x;
		dy1 = points[(i + 1) % numberOfPoints].y - points[i].y;
		dx2 = points[(i + 2) % numberOfPoints].x - points[(i + 1) % numberOfPoints].x;
		dy2 = points[(i + 2) % numberOfPoints].y - points[(i + 1) % numberOfPoints].y;
		zComp = dx1*dy2 - dx2*dy1;

		//If any of the z-components has the opposite sign of the base case, shape is concave.
		if ((zComp < 0 && !hasSign) || (zComp > 0 && hasSign))
			return false;
	}

	return true;
}

void Polygon::CreateConvexPolygon(std::vector<sf::Vector2f> points, sf::Color color)
{
	sf::ConvexShape newShape;
	//Get number of points
	newShape.setPointCount(points.size());

	//Set points to SFML shape
	for (int i = 0; i < points.size(); ++i)
		newShape.setPoint(i, points[i]);

	//Set the fill color
	newShape.setFillColor(color);

	//Push into polygons shape vector
	m_shapes.push_back(newShape);
}

void Polygon::Draw(sf::RenderWindow& rw) const
{
	for (int i = 0; i < m_shapes.size(); ++i)
		rw.draw(m_shapes[i]);
}
