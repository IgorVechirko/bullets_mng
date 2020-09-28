#ifndef BulletsMngDisplayScene_H
#define BulletsMngDisplayScene_H

#include "SceneBase.h"
#include "BulletsManager.h"
#include "BulletsManagerListener.h"

namespace BulletsMng
{
	class Button;
	class Line;
	class Point;
	class BulletsMngDisplayScene
		: public SceneBase
		, public BulletsManagerListener
	{

		BulletsManager _bulletsMng;

		int _wallsAmount;
		int _bulletsAmount;


		std::map<int,Line*> _wallsVisual;
		std::map<int,Point*> _bulletsVisual;

		Point* _earliestCollision;

		std::vector<std::unique_ptr<std::thread>> _shotedThreads;

		Button* _startBtn;
		Button* _restartBtn;



		void generateWalls();
		void generateBullets();


	protected:

		virtual void onOpened( const std::map<std::string,std::string>& sceneParams ) override;

		virtual void update( float deltaTime ) override;

		//BulletsManagerListener
		virtual void onWallAdded( int id, const glm::vec2& p1, const glm::vec2& p2 );
		virtual void onWallDeleted( int id );
		virtual void onBulletAdded( int id, const glm::vec2& pos );
		virtual void onBulletChangePos( int id, const glm::vec2& newPos );
		virtual void onBulletDeleted( int id );
		virtual void noEarliestCollision();
		virtual void onNewEarliestCollision( int bulletID, int wallID, const glm::vec2 pos );

	public:

		BulletsMngDisplayScene( IApplication* switcher );
		virtual ~BulletsMngDisplayScene();
		MAKE_UNCOPYABLE( BulletsMngDisplayScene );

		


	};

}


#endif