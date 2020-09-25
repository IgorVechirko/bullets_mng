#include "Button.h"

namespace BulletsMng
{

	Button::Button()
		: _pressedCallBack( nullptr )
	{
		if ( _font.loadFromFile( "arial.ttf" ) )
		{
			_text.setFont( _font );
			_text.setString( "hello word" );
			_text.setColor( sf::Color::Red );
		}
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
		switch ( handleEvent.type )
		{
			case sf::Event::MouseButtonPressed:
			{
				if ( _pressedCallBack && handleEvent.mouseButton.button == sf::Mouse::Button::Left )
				{
					_pressedCallBack();
				}
				break;
			}
			case sf::Event::MouseMoved:
			{
				_text.setColor( _text.getGlobalBounds().contains( handleEvent.mouseMove.x, handleEvent.mouseMove.y ) ? sf::Color::Green : sf::Color::Red );
				
				break;
			}
		}

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