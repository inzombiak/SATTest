#ifndef POLYGON_H
#define POLYGON_H

#include "SFML\Window.hpp"
#include "SFML\Graphics.hpp"

#include <vector>

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

	//Draw the shape
	void Draw(sf::RenderWindow& rw) const;

	//Get polygon points
	std::vector<sf::Vector2f> GetPoints() const;

private:
	//Returns true if shape is convex, otherwise false
	bool IsConvex(std::vector<sf::Vector2f> points);
	//Creates a single convex polygon from given points
	void CreateConvexPolygon(std::vector<sf::Vector2f> points, sf::Color color);

	//SFML shape objects for polygon. Since concave shapes need to be split into multiple convex shapes, we use a vector
	std::vector<sf::ConvexShape> m_shapes;
};

#endif
