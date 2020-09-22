#ifndef Line_H
#define Line_H

#include "RenderedUnit.h"

namespace BulletsMng
{

	class Line
		: public RenderedUnit
	{
		sf::Vertex points[2];
		sf::Color _color;


	public:

		Line();
		virtual ~Line();
		MAKE_UNCOPYABLE(Line);

		virtual void draw( sf::RenderWindow* window ) override;

		void setStartPoint( const sf::Vector2f& point );
		void setFinishPoint( const sf::Vector2f& point );

		void setColor( const sf::Color& color );
		

	};


}


#endif