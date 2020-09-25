#include "TestScene.h"

#include "Button.h"


namespace BulletsMng
{

	TestScene::TestScene( IApplication* switcher )
		: SceneBase( switcher )
	{
	}
	TestScene::~TestScene()
	{
	}
	void TestScene::onButtonReleased()
	{
		Console::log( "Button released" );
	}
	void TestScene::onOpened()
	{

		if ( _font.loadFromFile( "arial.ttf" ) )
		{
			_text.setFont( _font );
			_text.setString( "hello word" );
			_text.setColor( sf::Color::Red );
		}

		auto button = createRenderedUnit<Button>();
		if ( button )
		{
			addEventsHandler( button );
			button->setPressedCallBack( std::bind( &TestScene::onButtonReleased, this ) );
			button->getText().setString( "BUTTON" );
		}
	}

}