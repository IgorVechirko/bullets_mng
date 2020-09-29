#ifndef Application_H
#define Application_H

#include "Common.h"
#include "IApplication.h"

namespace BulletsMng
{

	class SceneBase;
	class Application
		: public IApplication
	{
		typedef std::function<SceneBase*()> createSceneFunc;

		std::map<std::string, createSceneFunc > _scenesCreateFunc;
		std::unique_ptr<SceneBase> _currentScene;

		std::map<std::string,std::string> _newSceneParams;
		std::string _newSceneName;

		std::unique_ptr<sf::RenderWindow> _window;

		std::chrono::time_point<std::chrono::steady_clock> _lasUpdateTime;
		int _fps;
		int _lowestFps;

		sf::Text _text;


	protected:

		virtual void switchToScene( const std::string& newSceneID, const std::map<std::string,std::string>& newSceneParams = std::map<std::string,std::string>() ) override;
		virtual sf::Vector2u getWindowSize() const override;

	public:
		
		Application();
		virtual ~Application();
		MAKE_UNCOPYABLE(Application);


		void run();


	};

}


#endif