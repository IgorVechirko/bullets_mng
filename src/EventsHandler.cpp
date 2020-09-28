#include "EventsHandler.h"


namespace BulletsMng
{


	EventsHandler::EventsHandler()
		: _handledEnable( true )
	{
	}
	EventsHandler::~EventsHandler()
	{
	}
	void EventsHandler::handleEvent( const sf::Event& handleEvent )
	{
		if ( _handledEnable )
			onEventHandled( handleEvent );
	}
	bool EventsHandler::isHandleEnable()
	{
		return _handledEnable;
	}
	void EventsHandler::setHandleEnable( bool isEnable )
	{
		_handledEnable = isEnable;
	}
}