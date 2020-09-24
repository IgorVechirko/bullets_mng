#ifndef TestScene_H
#define TestScene_H

#include "SceneBase.h"


namespace BulletsMng
{

	class TestScene : public SceneBase
	{

		sf::Font _font;
		sf::Text _text;


	protected:

		virtual void onOpened() override;

		virtual void render( sf::RenderWindow* window ) override;

	public:

		TestScene( IApplication* switcher);
		virtual ~TestScene();


	};


}


#endif