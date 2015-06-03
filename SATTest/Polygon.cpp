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

	m_shape.setPosition(position);
}

void Polygon::SetPosition(sf::Vector2f newPosition)
{
	m_shape.setPosition(newPosition);
}
sf::Vector2f Polygon::GetPosition() const
{
	return m_shape.getPosition();
}

void Polygon::SetColor(sf::Color newColor)
{
	m_shape.setFillColor(newColor);
}
sf::Color Polygon::GetColor() const
{
	return m_shape.getFillColor();
}

std::vector<sf::Vector2f> Polygon::GetPoints() const
{
	//Used to find the change in x and y from original location to return the actual position of the points
	sf::Vector2f change(GetPosition().x - m_originalCentroidPosition.x, GetPosition().y -  m_originalCentroidPosition.y);

	//Container vector for the points
	std::vector<sf::Vector2f> result;
	sf::Vector2f currPoint;

	//Get number of points of the polygon
	unsigned int numberOfPoints = m_shape.getPointCount();

	//Add each point to the container vector
	for (unsigned int i = 0; i < numberOfPoints; ++i)
	{
		currPoint = m_shape.getPoint(i);
		currPoint.x += change.x;
		currPoint.y += change.y;

		result.push_back(currPoint);
	}

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
	//Get number of points
	m_shape.setPointCount(points.size());

	//Set points to SFML shape
	for (int i = 0; i < points.size(); ++i)
		m_shape.setPoint(i, points[i]);

	//Set the fill color
	m_shape.setFillColor(color);

	//Set centroid to origin
	CalculateCentroid();
}

void Polygon::CalculateCentroid()
{
	auto points = GetPoints();
	int nextIndex;
	sf::Vector2f centroid;
	double area = 0;

	for (int i = 0; i < points.size(); ++i)
	{
		nextIndex = (i + 1) % points.size();
		area += (points[i].x*points[nextIndex].y - points[nextIndex].x*points[i].y);
	}
	area /= 2;

	for (int i = 0; i < points.size(); ++i)
	{
		nextIndex = (i + 1) % points.size();
		centroid.x += (points[i].x + points[nextIndex].x)*(points[i].x*points[nextIndex].y - points[nextIndex].x*points[i].y);
		centroid.y += (points[i].y + points[nextIndex].y)*(points[i].x*points[nextIndex].y - points[nextIndex].x*points[i].y);

	}

	centroid.x /= (6 * area);
	centroid.y /= (6 * area);

	m_originalCentroidPosition = centroid;

	m_shape.setOrigin(centroid);

}

void Polygon::Draw(sf::RenderWindow& rw)
{
	rw.draw(m_shape);
	//DrawAxes(rw);
}

std::vector<sf::Vector2f> Polygon::GetAxes()
{
	std::vector<sf::Vector2f> result;

	auto points = GetPoints();
	double length;
	sf::Vector2f axis, edge;
	for (int i = 0; i < points.size(); ++i)
	{
		//Calculate the edge between each point and its neighbor
		edge.x = points[(i + 1) % points.size()].x - points[i].x;
		edge.y = points[(i + 1) % points.size()].y - points[i].y;

		//Get length of edge
		length = sqrt(edge.x * edge.x + edge.y * edge.y);

		//Normalize
		edge.x /= length;
		edge.y /= length;

		//Push the pependiular vector to edge into the axes vector
		result.push_back(sf::Vector2f(-edge.y, edge.x));
	}

	return result;
}

void Polygon::DrawAxes(sf::RenderWindow& rw)
{
	//Length of axes
	const int lineLength = 200;

	//Gets SAT axes
	auto axes = GetAxes();
	
	double angle, x, y, length;

	//Stores line information
	sf::VertexArray line(sf::Lines, 2);
	line[0].color = sf::Color::Black;
	line[1].color = sf::Color::Black;

	sf::Vector2f edge;

	for (int i = 0; i < axes.size(); ++i)
	{
		//Get the slope of the axis
		angle = atan(axes[i].y / axes[i].x);

		//Centroid of polygon acts as midpoint of all axes, so we calculate x and y using half length
		x = cos(angle)*lineLength / 2;
		y = sin(angle)*lineLength / 2;

		//Set position of the endpoints of the axes
		line[0].position = sf::Vector2f(m_shape.getPosition().x - x, m_shape.getPosition().y - y);
		line[1].position = sf::Vector2f(m_shape.getPosition().x + x, m_shape.getPosition().y + y);

		//Draw the line
		rw.draw(line);
	}
}