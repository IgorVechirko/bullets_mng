#ifndef Label_H
#define Label_H

#include "RenderedUnit.h"

namespace BulletsMng
{

	class Label
		: public RenderedUnit
	{

		sf::Font _font;
		sf::Text _text;


	protected:

		virtual void draw( sf::RenderWindow* window ) override;


	public:

		Label();
		virtual ~Label();

		sf::Text& getText();

	};

}


#endif