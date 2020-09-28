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

			virtual void switchToScene( const std::string& newSceneID, const std::map<std::string,std::string>& newSceneParams = std::map<std::string,std::string>() ) = 0;
			virtual sf::Vector2u getWindowSize() const = 0;
	};

};



#endif