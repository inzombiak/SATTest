#ifndef POLYGON_FACTORY_H
#define POLYGON_FACTORY_H

#include "Polygon.h"

//Singleton, use GetInstance to get this object
class PolygonFactory
{
public:

	//Get instance of PolygonFactory
	static PolygonFactory& GetInstance();

	//Create polygon from a set of points

private:
	//Un-constructable, un-copyable
	PolygonFactory();
	PolygonFactory(const PolygonFactory& other) = delete;
	void operator=(const PolygonFactory& other) = delete;

	static PolygonFactory m_factory;
};

#endif

