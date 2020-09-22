#ifndef RenderedUnit_H
#define RenderedUnit_H

#include "Common.h"

namespace BulletsMng
{

	class RenderedUnit
	{

		

	public:

		RenderedUnit();
		virtual ~RenderedUnit();
		MAKE_UNCOPYABLE( RenderedUnit );

		virtual void draw( sf::RenderWindow* window );

	};

}



#endif