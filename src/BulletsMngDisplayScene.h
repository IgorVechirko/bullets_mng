#ifndef BulletsMngDisplayScene_H
#define BulletsMngDisplayScene_H

#include "SceneBase.h"

namespace BulletsMng
{

	class BulletsMngDisplayScene
		: public SceneBase
	{

	public:

		BulletsMngDisplayScene( IScenesSwitcher* switcher );
		virtual ~BulletsMngDisplayScene();
		MAKE_UNCOPYABLE( BulletsMngDisplayScene );

		virtual void onOpened() override;


	};

}


#endif