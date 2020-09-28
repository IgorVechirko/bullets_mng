#include "Label.h"

namespace BulletsMng
{

	Label::Label()
	{
		if ( _font.loadFromFile( "arial.ttf" ) )
		{
			_text.setFont( _font );
			_text.setString( "" );
			_text.setColor( sf::Color::Red );
		}
	}
	Label::~Label()
	{
	}
	void Label::draw( sf::RenderWindow* window )
	{
		window->draw(_text);
	}
	sf::Text& Label::getText()
	{
		return _text;
	}
}