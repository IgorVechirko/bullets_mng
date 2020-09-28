#ifndef TestScene_H
#define TestScene_H

#include "SceneBase.h"


namespace BulletsMng
{

	class TestScene : public SceneBase
	{

		sf::Font _font;
		sf::Text _text;




		void onButtonReleased();

	protected:

		virtual void onOpened( const std::map<std::string,std::string>& sceneParams ) override;

	public:

		TestScene( IApplication* switcher);
		virtual ~TestScene();


	};


}


#endif