#ifndef SceneBase_H
#define SceneBase_H

#include "Common.h"

namespace BulletsMng
{
	class RenderedUnit;
	class IScenesSwitcher;
	class SceneBase
	{
		IScenesSwitcher* _scenesSwitcher;

		std::vector<std::unique_ptr<RenderedUnit>> _renderedUnits;


	public:

		SceneBase( IScenesSwitcher* switcher );
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


		void render( sf::RenderWindow* window );

		void switchToScene( const std::string& newSceneID );

	};


}


#endif