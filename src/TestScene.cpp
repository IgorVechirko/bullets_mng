#include "TestScene.h"

#include "Button.h"
#include "Label.h"


namespace BulletsMng
{

	TestScene::TestScene( IApplication* application )
		: SceneBase( application )
	{
	}
	TestScene::~TestScene()
	{
	}
	void TestScene::onButtonReleased()
	{
		Console::log( "Button released" );
	}
	void TestScene::onOpened( const std::map<std::string,std::string>& sceneParams )
	{

		auto field = createRenderedUnit<Label>();
		if ( field )
			field->getText().setString( "Hello" );
	}

}