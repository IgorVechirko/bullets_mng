#ifndef BulletsManagerListener_H
#define BulletsManagerListener_H

#include "Common.h"

namespace BulletsMng
{

	class BulletsManagerListener
	{

	public:

		virtual void onWallAdded( int id, const glm::vec2& p1, const glm::vec2& p2 ){};
		virtual void onWallDeleted( int id ){};

		virtual void onBulletAdded( int id, const glm::vec2& pos ){};
		virtual void onBulletChangePos( int id, const glm::vec2& newPos ){};
		virtual void onBulletDeleted( int id ){};
	};


}




#endif