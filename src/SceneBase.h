#ifndef SceneBase_H
#define SceneBase_H

#include "Common.h"

namespace BulletsMng
{
	class RenderedUnit;
	class IApplication;
	class EventsHandler;
	class SceneBase
	{
		IApplication* _application;

		std::vector<std::unique_ptr<RenderedUnit>> _renderedUnits;

		std::vector<EventsHandler*> _eventsHandlers;


	public:

		SceneBase( IApplication* application );
		virtual ~SceneBase();
		MAKE_UNCOPYABLE( SceneBase );

		virtual void onOpened( const std::map<std::string,std::string>& sceneParams ) {};

		template<typename T>
		T* createRenderedUnit()
		{
			auto result = new T();
			RenderedUnit* unit = result;
			_renderedUnits.push_back( std::unique_ptr<RenderedUnit>(unit) );

			return result;
		}
		void removeRenderedUnit( RenderedUnit* unit );


		virtual void update( float deltaTime ){};

		virtual void render( sf::RenderWindow* window );

		virtual void handleEvent( const sf::Event& handleEvent );

		void addEventsHandler( EventsHandler* handler );
		void delEventsHandler( EventsHandler* handler );


		IApplication* getApplication();

	};


}


#endif