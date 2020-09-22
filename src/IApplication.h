#ifndef IApplication_H
#define IApplication_H

#include "Common.h"

namespace BulletsMng
{

	class IApplication
	{

		public:

			IApplication(){};
			virtual ~IApplication(){};
			MAKE_UNCOPYABLE(IApplication);

			virtual void switchToScene( const std::string& newSceneID ) = 0;
			virtual sf::Vector2u getWindowSize() const = 0;
	};

};



#endif