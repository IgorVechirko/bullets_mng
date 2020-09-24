#include "SceneBase.h"

#include "IApplication.h"
#include "RenderedUnit.h"


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
	}
	IApplication* SceneBase::getApplication()
	{
		assert( _application );
		return _application;
	}
}