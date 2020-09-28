#include "NumberField.h"


namespace BulletsMng
{

	NumberField::NumberField()
		: _state( FieldState::VIEW )
		, _placeholderText( "click and enter number")
	{
		if ( _font.loadFromFile( "arial.ttf" ) )
		{
			_text.setFont( _font );
			_text.setString( _placeholderText );
			_text.setColor( sf::Color::Red );
		}
	}
	NumberField::~NumberField()
	{
	}
	void NumberField::switchToState( FieldState newState )
	{
		if ( _state != newState )
		{
			switch (newState)
			{
				case BulletsMng::NumberField::FieldState::VIEW:
				{
					_text.setFillColor( sf::Color::Red );

					_text.setString( _enteredString.empty() ? _placeholderText : _enteredString );

					break;
				}
				case BulletsMng::NumberField::FieldState::EDIT:
				{
					_text.setFillColor( sf::Color::Green );
					_text.setString( _enteredString );
					break;
				}
				default:
					break;
			}

			_state = newState;
		}
	}
	void NumberField::pushCharacter( const std::string& character )
	{
		if ( character.size() == 1 )
		{
			if ( character[0] > 48 && character[0] < 58 )
			{
				_enteredString += character;
			}
			else if ( character[0] == 48 && !_enteredString.empty() )
			{
				_enteredString += character;
			}
		}

		_text.setString( _enteredString );
	}
	void NumberField::popCharacter()
	{
		if ( !_enteredString.empty() )
			_enteredString.resize( _enteredString.size() - 1 );

		_text.setString( _enteredString );
	}
	void NumberField::onEventHandled( const sf::Event& handleEvent )
	{
		switch ( handleEvent.type )
		{
			case sf::Event::MouseButtonPressed:
			{
				if ( handleEvent.mouseButton.button == sf::Mouse::Button::Left )
				{
					if ( _text.getGlobalBounds().contains( handleEvent.mouseButton.x, handleEvent.mouseButton.y ) )
						switchToState( FieldState::EDIT );
					else
						switchToState( FieldState::VIEW );
				}

				break;
			}
			case sf::Event::TextEntered:
			{
				if( _state == FieldState::EDIT )
				{
					sf::String unicode;
					unicode = handleEvent.text.unicode;
					pushCharacter( unicode.toAnsiString() );
				}

				break;
			}
			case sf::Event::KeyPressed:
			{
				if ( _state == FieldState::EDIT )
				{
					if ( handleEvent.key.code == sf::Keyboard::BackSpace )
						popCharacter();
					else if ( handleEvent.key.code == sf::Keyboard::Enter )
						switchToState( FieldState::VIEW );

					break;
				}
			}
			default:
				break;
		}

	}
	void NumberField::draw( sf::RenderWindow* window )
	{
		window->draw(_text);
	}
	int NumberField::getNumber()
	{
		return std::atoi( _enteredString.c_str() );
	}
}