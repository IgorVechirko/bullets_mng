#ifndef Button_H
#define Button_H

#include "RenderedUnit.h"
#include "EventsHandler.h"

namespace BulletsMng
{

	typedef std::function<void()> buttonPressedCallBack;

	class Button
		: public RenderedUnit
		, public EventsHandler
	{

		sf::Text _text;

		buttonPressedCallBack _pressedCallBack;


	protected:

		virtual void draw( sf::RenderWindow* window ) override;

		virtual void handleEvent( const sf::Event& handleEvent ) override;


	public:

		Button();
		virtual ~Button();

		sf::Text& getText();

		void setPressedCallBack( buttonPressedCallBack callBack );

	};

}


#endif