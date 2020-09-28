#include "SetupScene.h"

#include "Button.h"
#include "IApplication.h"

namespace BulletsMng
{


	SetupScene::SetupScene(IApplication* application)
		: SceneBase( application )
	{
	}
	SetupScene::~SetupScene()
	{}
	void SetupScene::runBulletsMngScene()
	{
		std::map<std::string,std::string> newSceneParams;
		newSceneParams[ "bullets_amount" ] = "100";
		newSceneParams[ "walls_amount" ] = "100";

		getApplication()->switchToScene( "mng_display", newSceneParams );
	}
	void SetupScene::onOpened( const std::map<std::string,std::string>& sceneParams )
	{

		auto startBtn = createRenderedUnit<Button>();
		if ( startBtn )
		{
			addEventsHandler( startBtn );
			startBtn->setPressedCallBack( std::bind( &SetupScene::runBulletsMngScene, this ) );
			startBtn->getText().setString( "START" );
		}

	}

}