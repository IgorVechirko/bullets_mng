#ifndef EventsHandler_H
#define EventsHandler_H

#include "Common.h"

namespace BulletsMng
{

	class EventsHandler
	{

	public:

		EventsHandler();
		virtual ~EventsHandler();


		virtual void handleEvent( const sf::Event& handleEvent ) = 0;



	};

}


#endif