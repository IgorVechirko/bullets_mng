#include "Line.h"


namespace BulletsMng
{

	Line::Line()
	{
	}
	Line::~Line()
	{
	}
	void Line::draw( sf::RenderWindow* window )
	{
		window->draw( points, 2, sf::Lines );
	}
	void Line::setStartPoint( const sf::Vector2f& point )
	{
		points[0] = sf::Vertex( point, _color );;
	}
	void Line::setFinishPoint( const sf::Vector2f& point )
	{
		points[1] = sf::Vertex( point, _color );
	}
	void Line::setColor( const sf::Color& color )
	{
		_color = color;
	}

}