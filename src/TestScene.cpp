#include "TestScene.h"


namespace BulletsMng
{

	TestScene::TestScene( IApplication* switcher )
		: SceneBase( switcher )
	{
	}
	TestScene::~TestScene()
	{
	}
	void TestScene::onOpened()
	{

		if ( _font.loadFromFile( "arial.ttf" ) )
		{
			_text.setFont( _font );
			_text.setString( "hello word" );
			_text.setColor( sf::Color::Red );
		}

	}
	void TestScene::render( sf::RenderWindow* window )
	{
		SceneBase::render( window );

		window->draw( _text );
	}

}