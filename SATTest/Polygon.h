#ifndef POLYGON_H
#define POLYGON_H

#include "SFML\Window.hpp"
#include "SFML\Graphics.hpp"

#include <vector>
#include <iostream>

class Polygon
{
public:

	//Polygon requires points and a color to be constructed.
	Polygon(std::vector<sf::Vector2f> points, sf::Color color, sf::Vector2f position);

	//Set position of the polygon
	void SetPosition(sf::Vector2f newPosition);
	//Get position of the polygon
	sf::Vector2f GetPosition() const;

	//Set color of the shape
	void SetColor(sf::Color newColor);
	//Get color of the shape
	sf::Color GetColor() const;

	//Returns vector of SAT axes
	std::vector<sf::Vector2f> GetAxes();

	//Draw the shape
	void Draw(sf::RenderWindow& rw);

	//Get polygon points
	std::vector<sf::Vector2f> GetPoints() const;

private:
	//Draws axes of SAT
	void DrawAxes(sf::RenderWindow& rw);

	//Returns true if shape is convex, otherwise false
	bool IsConvex(std::vector<sf::Vector2f> points);
	//Creates a single convex polygon from given points
	void CreateConvexPolygon(std::vector<sf::Vector2f> points, sf::Color color);

	//Calculates centroid of the polygon
	void CalculateCentroid();

	//SFML shape object for polygon
	sf::ConvexShape m_shape;

	//Couldnt figure out how to get the position of the points of the sf::ConvexShape, 
	//so I store the default centroid and use it to calculate the current position of the points
	sf::Vector2f m_originalCentroidPosition;
};

#endif
