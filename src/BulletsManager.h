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

			LinearFunction linearFunction;

			int id;

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

		struct Collision
		{
			int bulletID;
			int wallID;

//#ifdef _DEBUG
			glm::vec2 collisionPoint;
//#endif
			float time;

			Collision()
				: bulletID( -1 )
				, wallID( -1 )
				, time( 0.0f )
			{
			}

			bool exist() const
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

		struct Bullet
		{
			std::list<Collision> sortedFutureCollisions;

			int id;

			glm::vec2 pos;
			glm::vec2 dir;

			float speed;
			float timeForPos;
			float finishFlyTime;

			bool remove;//48

			Bullet()
				: speed( 0.0f )
				, timeForPos( 0.0f )
				, finishFlyTime( 0.0f )
				, id( -1 )
				, remove( false )
			{
			}

		};



		std::vector<Wall> _walls;
		std::set<int> _existingWallsIDs;

		std::recursive_mutex _waitStartFlyBulletsLock;		
		std::list<Bullet> _waitStartFlyBullets;

		std::vector<Bullet> _flyingBullets;

		float _time;

		int _wallsIDGenerator;
		int _bulletsIDGenerator;

		std::vector<BulletsManagerListener*> _listeners;

		Collision _earliestCollision;

		std::chrono::steady_clock::duration _avarageUpdateTime;




		bool isPointInSegment( const glm::vec2& s1, const glm::vec2& s2, const glm::vec2& point ) const;

		void shootNewBullets();

		Collision getBulletEarliestCollision( Bullet& bullet );
		void calculateBulletFutureCollisions( Bullet& bullet );

		bool isWallExist( int wallID );

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