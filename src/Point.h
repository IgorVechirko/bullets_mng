#ifndef Point_H
#define Point_H

#include "RenderedUnit.h"

namespace BulletsMng
{

	class Point
		: public RenderedUnit
	{
		sf::CircleShape _shape;


	public:

		Point();
		virtual ~Point();
		MAKE_UNCOPYABLE(Point);

		virtual void draw( sf::RenderWindow* window ) override;

		sf::CircleShape& getShape();
		

	};


}


#endif