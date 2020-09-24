#include "Button.h"

namespace BulletsMng
{

	Button::Button()
		: _pressedCallBack( nullptr )
	{
	}
	Button::~Button()
	{
	}
	void Button::draw( sf::RenderWindow* window )
	{
		window->draw(_text);
	}
	void Button::handleEvent( const sf::Event& handleEvent )
	{
	}
	sf::Text& Button::getText()
	{
		return _text;
	}
	void Button::setPressedCallBack( buttonPressedCallBack callBack )
	{
		_pressedCallBack = callBack;
	}
}