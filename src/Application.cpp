#include "Application.h"

#include "SceneBase.h"
#include "BulletsMngDisplayScene.h"


namespace BulletsMng
{
	Application::Application()
		: _fps( 0 )
		, _lowestFps( std::numeric_limits<int>::max() )
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
	sf::Vector2u Application::getWindowSize() const
	{
		return _window->getSize();
	}
	void Application::run()
	{
		sf::Font font;
		if ( font.loadFromFile( "arial.ttf" ) )
		{
			_text.setFont( font );
			_text.setString( "0" );
		}
		else
		{
			Console::log( "can't load font" );
		}

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

			auto prevSeconds = std::chrono::duration_cast<std::chrono::seconds>( _lasUpdateTime.time_since_epoch() ).count();
			auto nowSeconds = std::chrono::duration_cast<std::chrono::seconds>( nowTimePoint.time_since_epoch() ).count();

			if ( nowSeconds > prevSeconds )
			{
				//if ( _fps < _lowestFps )
				{
					_lowestFps = _fps;
					_text.setString( std::to_string( _lowestFps) );
				}

				_fps = 0;
			}
			else
			{
				_fps++;
			}


			auto deltaTime = std::chrono::duration_cast<std::chrono::duration<float>>( nowTimePoint - _lasUpdateTime );
			_lasUpdateTime = nowTimePoint;

			if ( _currentScene )
				_currentScene->update( deltaTime.count() );

			_window->clear( sf::Color( 119, 136, 153, 255 ) );

			if ( _currentScene )
				_currentScene->render( _window.get() );

			
			_window->draw( _text );

			_window->display();
		}

	}

}