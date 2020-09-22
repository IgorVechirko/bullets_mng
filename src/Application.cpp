#include "Application.h"

#include "SceneBase.h"
#include "BulletsMngDisplayScene.h"


namespace BulletsMng
{
	Application::Application()
	{
	}
	Application::~Application()
	{
	}
	void Application::switchToScene( const std::string& newSceneID )
	{
		if ( !newSceneID.empty() )
		{
			_newSceneName = newSceneID;
		}
	}
	sf::Vector2u Application::getWindowSize()
	{
		return _window->getSize();
	}
	void Application::run()
	{
		_lasUpdateTime = std::chrono::steady_clock::now();

		_scenesCreateFunc[ "mng_display" ] = [this](){ return new BulletsMngDisplayScene(this); };

		switchToScene( "mng_display" );

		sf::ContextSettings _settings;
		_settings.antialiasingLevel = 8;

		_window = std::make_unique<sf::RenderWindow>( sf::VideoMode( 1124, 860 ), "Bullets Manager", sf::Style::Titlebar | sf::Style::Close, _settings );

		while( _window->isOpen() )
		{

			sf::Event handleEvent;

			while( _window->pollEvent(handleEvent) )
			{
				if ( handleEvent.type == sf::Event::Closed )
					_window->close();
			}

			if ( !_newSceneName.empty() )
			{
				auto findIt = _scenesCreateFunc.find( _newSceneName );
				if ( findIt != _scenesCreateFunc.end() )
				{
					auto newScene = findIt->second();
					if ( newScene )
					{
						_currentScene.reset( newScene );
						_currentScene->onOpened();
					}
				}

				_newSceneName.clear();
			}

			auto nowTimePoint = std::chrono::steady_clock::now();
			auto deltaTime = std::chrono::duration_cast<std::chrono::duration<float>>( nowTimePoint - _lasUpdateTime );
			_lasUpdateTime = nowTimePoint;

			if ( _currentScene )
				_currentScene->update( deltaTime.count() );

			_window->clear( sf::Color( 119, 136, 153, 255 ) );

			if ( _currentScene )
				_currentScene->render( _window.get() );

			_window->display();
		}

	}

}