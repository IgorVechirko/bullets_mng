#ifndef Application_H
#define Application_H

#include "Common.h"
#include "IScenesSwitcher.h"

namespace BulletsMng
{

	class SceneBase;
	class Application
		: public IScenesSwitcher
	{
		typedef std::function<SceneBase*()> createSceneFunc;

		std::map<std::string, createSceneFunc > _scenesCreateFunc;
		std::unique_ptr<SceneBase> _currentScene;
		std::string _newSceneName;

		std::unique_ptr<sf::RenderWindow> _window;


	protected:

		template<typename T>
		T* createScene()
		{
			auto scene = new T(this);
			return scene;
		}


		virtual void switchToScene( const std::string& newSceneID ) override;

	public:
		
		Application();
		virtual ~Application();
		MAKE_UNCOPYABLE(Application);


		void run();


	};

}


#endif