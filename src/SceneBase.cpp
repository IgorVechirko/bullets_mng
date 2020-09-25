#include "SceneBase.h"

#include "IApplication.h"
#include "RenderedUnit.h"
#include "EventsHandler.h"


namespace BulletsMng
{

	SceneBase::SceneBase( IApplication* application )
		: _application( application )
	{
	}
	SceneBase::~SceneBase()
	{
	}
	void SceneBase::removeRenderedUnit( RenderedUnit* unit )
	{
		auto findIt = std::find_if( _renderedUnits.begin(), _renderedUnits.end(),
			[unit](std::unique_ptr<RenderedUnit>& unitUnique)->bool{
				return unitUnique.get() == unit;
			});

		if ( findIt != _renderedUnits.end() )
		{
			_renderedUnits.erase( findIt );
		}
	}
	void SceneBase::render( sf::RenderWindow* window )
	{
		for( auto& renderedUnit : _renderedUnits )
			renderedUnit->draw( window );

	}
	void SceneBase::handleEvent( const sf::Event& handleEvent )
	{
		for( auto handler : _eventsHandlers )
			handler->handleEvent( handleEvent );
	}
	void SceneBase::addEventsHandler( EventsHandler* handler )
	{
		if ( handler )
		{
			auto findIt = std::find( _eventsHandlers.begin(), _eventsHandlers.end(), handler );
			if ( findIt == _eventsHandlers.end() )
			{
				_eventsHandlers.push_back( handler );
			}
		}
	}
	void SceneBase::delEventsHandler( EventsHandler* handler )
	{
		if ( handler )
		{
			auto findIt = std::find( _eventsHandlers.begin(), _eventsHandlers.end(), handler );
			if ( findIt != _eventsHandlers.end() )
			{
				_eventsHandlers.erase( findIt );
			}
		}
	}
	IApplication* SceneBase::getApplication()
	{
		assert( _application );
		return _application;
	}
}