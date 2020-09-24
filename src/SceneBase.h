#ifndef SceneBase_H
#define SceneBase_H

#include "Common.h"

namespace BulletsMng
{
	class RenderedUnit;
	class IApplication;
	class SceneBase
	{
		IApplication* _application;

		std::vector<std::unique_ptr<RenderedUnit>> _renderedUnits;


	public:

		SceneBase( IApplication* application );
		virtual ~SceneBase();
		MAKE_UNCOPYABLE( SceneBase );

		virtual void onOpened() {};

		template<typename T>
		T* createRunderedUnit()
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

		IApplication* getApplication();

	};


}


#endif