#include "Point.h"


namespace BulletsMng
{

	Point::Point()
	{
	}
	Point::~Point()
	{
	}
	void Point::draw( sf::RenderWindow* window )
	{
		window->draw( _shape );
	}
	sf::CircleShape& Point::getShape()
	{
		return _shape;
	}
}