#ifndef IScenesSwitcher_H
#define IScenesSwitcher_H

#include "Common.h"

namespace BulletsMng
{

	class IScenesSwitcher
	{

		public:

			IScenesSwitcher(){};
			virtual ~IScenesSwitcher(){};
			MAKE_UNCOPYABLE(IScenesSwitcher);

			virtual void switchToScene( const std::string& newSceneID ) = 0;
	};

};



#endif