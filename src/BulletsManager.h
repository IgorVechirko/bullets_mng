#ifndef BulletsManager_H
#define BulletsManager_H

#include "Common.h"
#include "LinearFunction.h"

namespace BulletsMng
{
	class BulletsManagerListener;
	class BulletsManager
	{

		struct Wall
		{
			glm::vec2 p1;
			glm::vec2 p2;

			int id;

			LinearFunction linearFunction;

			Wall()
				: id( -1 )
			{
			}

			Wall( const glm::vec2& p1, const glm::vec2& p2 )
				: p1( p1 )
				, p2( p2 )
				, id( -1 )
				, linearFunction( p1, p2 )
			{
			}
		};

		struct Bullet
		{
			glm::vec2 pos;
			glm::vec2 dir;

			float speed;
			float timeForPos;
			float finishFlyTime;

			int id;

			Bullet()
				: speed( 0.0f )
				, timeForPos( 0.0f )
				, finishFlyTime( 0.0f )
				, id( -1 )
			{
			}

		};

		struct Collision
		{
			int bulletID;
			int wallID;

			float time;
			// glm::vec2 collisionPoint;

			Collision()
				: bulletID( -1 )
				, wallID( -1 )
				, time( 0.0f )
			{
			}

			bool exist()
			{
				return wallID >= 0 && bulletID >= 0;
			}

			void reset()
			{
				wallID = -1;
				bulletID = -1;
				time = 0.0f;
			}
		};

		std::vector<Wall> _walls;

		std::recursive_mutex _waitStartFlyBulletsLock;		
		std::vector<Bullet> _waitStartFlyBullets;

		std::vector<Bullet> _flyingBullets;

		float _time;

		int _wallsIDGenerator;
		int _bulletsIDGenerator;

		std::list<BulletsManagerListener*> _listeners;

		Collision _earliestCollision;


		bool isPointInSegment( const glm::vec2& p1, const glm::vec2& p2, const glm::vec2& point );

		void shootNewBullets();

		Collision getEarliestCollision( const Bullet& bullet );

		void moveBulletsLinearTillTime( float time );

		void doCollision( const Collision& collision );



	public:

		BulletsManager();
		virtual ~BulletsManager();

		void addBulletsManagerListener( BulletsManagerListener* lst );
		void delBulletsManagerListener( BulletsManagerListener* lst );

		void addWall( const glm::vec2& p1, const glm::vec2& p2 );

		void fire( const glm::vec2& pos, const glm::vec2& dir, float speed, float shotTime, float lifeTime );

		void update( float deltaTime );


	};

}


#endif