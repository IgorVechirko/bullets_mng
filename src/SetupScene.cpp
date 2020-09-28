#include "SetupScene.h"

#include "Button.h"
#include "IApplication.h"
#include "Label.h"
#include "NumberField.h"

namespace BulletsMng
{


	SetupScene::SetupScene(IApplication* application)
		: SceneBase( application )
		, _wallsNumField( nullptr )
		, _bulletsNumField( nullptr )
	{
	}
	SetupScene::~SetupScene()
	{}
	void SetupScene::runBulletsMngScene()
	{
		std::map<std::string,std::string> newSceneParams;

		int wallsAmount = 100;
		int bulletsAmount = 100;

		if ( _wallsNumField )
		{
			if ( _wallsNumField->getNumber() > 0 )
				wallsAmount = _wallsNumField->getNumber();
		}

		if ( _bulletsNumField )
		{
			if ( _bulletsNumField->getNumber() > 0 )
				bulletsAmount = _bulletsNumField->getNumber();
		}

		newSceneParams[ "bullets_amount" ] = std::to_string( bulletsAmount );
		newSceneParams[ "walls_amount" ] = std::to_string( wallsAmount );;

		getApplication()->switchToScene( "mng_display", newSceneParams );
	}
	void SetupScene::onOpened( const std::map<std::string,std::string>& sceneParams )
	{
		const auto& winSize = getApplication()->getWindowSize();

		_wallsNumField = createRenderedUnit<NumberField>();
		if ( _wallsNumField )
		{
			_wallsNumField->getText().setPosition( winSize.x * 0.2f, winSize.y * 0.25f );
			addEventsHandler( _wallsNumField );

			auto label = createRenderedUnit<Label>();
			if ( label )
			{
				label->getText().setFillColor( sf::Color::Blue );
				label->getText().setString( "Enter walls amount" );
				label->getText().setPosition(_wallsNumField->getText().getPosition().x, _wallsNumField->getText().getPosition().y - _wallsNumField->getText().getCharacterSize() );
			}
		}

		_bulletsNumField = createRenderedUnit<NumberField>();
		if ( _bulletsNumField )
		{
			_bulletsNumField->getText().setPosition( winSize.x * 0.6f, winSize.y * 0.25f );
			addEventsHandler( _bulletsNumField );

			auto label = createRenderedUnit<Label>();
			if ( label )
			{
				label->getText().setFillColor( sf::Color::Blue );
				label->getText().setString( "Enter bullets amount" );
				label->getText().setPosition( _bulletsNumField->getText().getPosition().x, _bulletsNumField->getText().getPosition().y - _bulletsNumField->getText().getCharacterSize() );
			}
		}
		
		
		auto startBtn = createRenderedUnit<Button>();
		if ( startBtn )
		{
			addEventsHandler( startBtn );
			startBtn->setPressedCallBack( std::bind( &SetupScene::runBulletsMngScene, this ) );
			startBtn->getText().setString( "START" );
		}

	}

}