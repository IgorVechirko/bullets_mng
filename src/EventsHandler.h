#ifndef EventsHandler_H
#define EventsHandler_H

#include "Common.h"

namespace BulletsMng
{

	class EventsHandler
	{


		bool _handledEnable;

	protected:

		virtual void onEventHandled( const sf::Event& handledEvent ) = 0;


	public:

		EventsHandler();
		virtual ~EventsHandler();

		void handleEvent( const sf::Event& handleEvent );

		bool isHandleEnable();
		void setHandleEnable( bool isEnable );

	};

}


#endif