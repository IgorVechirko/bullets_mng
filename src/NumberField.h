#ifndef NumberField_H
#define NumberField_H

#include "RenderedUnit.h"
#include "EventsHandler.h"

namespace BulletsMng
{

	class NumberField
		: public RenderedUnit
		, public EventsHandler
	{

		enum class FieldState
		{
			VIEW,
			EDIT
		};

		FieldState _state;

		sf::Font _font;
		sf::Text _text;

		std::string _placeholderText;
		std::string _enteredString;


		void switchToState( FieldState newState );

		void pushCharacter( const std::string& character );
		void popCharacter();

	protected:

		virtual void onEventHandled( const sf::Event& handledEvent ) override;

		virtual void draw( sf::RenderWindow* window ) override;


	public:

		NumberField();
		virtual ~NumberField();

		int getNumber();

	};


}


#endif