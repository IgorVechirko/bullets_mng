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
			_text.setFillColor( sf::Color::Red );
		}
	}
	Button::~Button()
	{
	}
	void Button::draw( sf::RenderWindow* window )
	{
		window->draw(_text);
	}
	void Button::onEventHandled( const sf::Event& handleEvent )
	{
		switch ( handleEvent.type )
		{
			case sf::Event::MouseButtonPressed:
			{
				if ( _pressedCallBack 
					&& handleEvent.mouseButton.button == sf::Mouse::Button::Left
					&& _text.getGlobalBounds().contains( static_cast<float>(handleEvent.mouseButton.x), static_cast<float>(handleEvent.mouseButton.y) ) )
				{
					_pressedCallBack();
				}
				break;
			}
			case sf::Event::MouseMoved:
			{
				_text.setFillColor( _text.getGlobalBounds().contains( static_cast<float>(handleEvent.mouseMove.x), static_cast<float>(handleEvent.mouseMove.y) ) ? sf::Color::Green : sf::Color::Red );
				
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