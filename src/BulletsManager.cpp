#include "BulletsManager.h"

#include "BulletsManagerListener.h"

namespace BulletsMng
{

	BulletsManager::BulletsManager()
		: _time( 0.0f )
		, _wallsIDGenerator( 0 )
		, _bulletsIDGenerator( 0 )
	{
	}
	BulletsManager::~BulletsManager()
	{
	}
	bool BulletsManager::isPointInSegment( const glm::vec2& p1, const glm::vec2& p2, const glm::vec2& point )
	{
		auto Xrange = std::minmax( p1.x, p2.x );
		auto Yrange = std::minmax( p1.y, p2.y );

		auto epsilon = 0.1f;
		
		return Xrange.first - epsilon <= point.x && Xrange.second + epsilon >= point.x && Yrange.first - epsilon <= point.y && Yrange.second + epsilon >= point.y;
	}
	void BulletsManager::shootNewBullets()
	{
		std::unique_lock<std::recursive_mutex> lock(_waitStartFlyBulletsLock);

		std::vector<Bullet> shotedBullets;

		for( auto bulletIt = _waitStartFlyBullets.begin(); bulletIt != _waitStartFlyBullets.end(); )
		{
			if ( _time >= bulletIt->timeForPos )
			{
				shotedBullets.push_back(*bulletIt);
				bulletIt = _waitStartFlyBullets.erase( bulletIt );
			}
			else
			{
				bulletIt++;
			}
		}

		lock.unlock();

		for( auto listener : _listeners )
		{
			for( const auto& bullet : shotedBullets )
			{
				listener->onBulletAdded( bullet.id, bullet.pos );
			}
		}

		for( const auto& bullet : shotedBullets )
		{
			auto earliestCollistionForBullet = getEarliestCollision( bullet );

			if( earliestCollistionForBullet.exist() )
			{
				if ( !_earliestCollision.exist() || _earliestCollision.time > earliestCollistionForBullet.time )
				{
					_earliestCollision = earliestCollistionForBullet;
				}
			}
		}

		if ( _earliestCollision.exist() )
		{
			for( auto listener : _listeners )
				listener->onNewEarliestCollision( _earliestCollision.bulletID, _earliestCollision.wallID, _earliestCollision.collisionPoint );
		}
		else
		{
			for( auto listener : _listeners )
				listener->noEarliestCollision();
		}

		std::move( shotedBullets.begin(), shotedBullets.end(), std::inserter( _flyingBullets,_flyingBullets.end() ) );
	}
	BulletsManager::Collision BulletsManager::getEarliestCollision( const Bullet& bullet )
	{
		Collision result;

		auto moveTime = bullet.finishFlyTime - bullet.timeForPos;
		auto destPoint = bullet.pos + bullet.dir * moveTime * bullet.speed;

		LinearFunction bulletLinearFunc( bullet.pos, destPoint );

		for( const auto& wall : _walls )
		{
			if( !wall.linearFunction.isParallel( bulletLinearFunc ) )
			{
				const auto& crossPoint = wall.linearFunction.crossPoint( bulletLinearFunc );

				if ( isPointInSegment( wall.p1, wall.p2, crossPoint ) && isPointInSegment( bullet.pos, destPoint, crossPoint ) )
				{
					float distToCollision = glm::length( crossPoint - bullet.pos );
					float collisionTime = bullet.timeForPos + distToCollision/bullet.speed;

					if ( !result.exist() || result.time > collisionTime )
					{
						result.bulletID = bullet.id;
						result.wallID = wall.id;
						result.time = collisionTime;
						result.collisionPoint = crossPoint;
					}
				}
			}
		}

		return result;
	}
	void BulletsManager::moveBulletsLinearTillTime( float time )
	{
		std::vector<int> removedBulletsIDs;

		for( auto it = _flyingBullets.begin(); it != _flyingBullets.end(); )
		{
			auto& bullet = (*it);

			if ( bullet.finishFlyTime < time )
			{
				removedBulletsIDs.push_back(bullet.id);
				it = _flyingBullets.erase( it );
			}
			else if ( bullet.timeForPos < time )
			{
				bullet.pos += (time - bullet.timeForPos)*bullet.speed*bullet.dir;
				bullet.timeForPos = time;
				it++;
			}
			else
			{
				it++;
			}
		}

		for( auto listener : _listeners )
		{
			for( auto bulletID : removedBulletsIDs )
			{
				listener->onBulletDeleted( bulletID );
			}
		}
	}
	void BulletsManager::doCollision( const Collision& collision )
	{
		int removedWallId = -1;

		auto findWallIt = std::find_if( _walls.begin(), _walls.end(), 
			[collision](const Wall& wall)->bool {
				return wall.id == collision.wallID;
			} );

		auto findBulletIt = std::find_if( _flyingBullets.begin(), _flyingBullets.end(),
			[collision](const Bullet& bullet) -> bool {
				return bullet.id == collision.bulletID;
			});

		if ( findWallIt != _walls.end() && findBulletIt != _flyingBullets.end() )
		{
			glm::vec2 wallNormal = glm::normalize( glm::vec2( findWallIt->linearFunction.Acoef, findWallIt->linearFunction.Bcoef ) );
			
			//auto projectionOnWallNormall = glm::dot( wallNormal, findBulletIt->dir );
			//projectionOnWallNormall = -abs(projectionOnWallNormall);

			//findBulletIt->dir = glm::normalize( findBulletIt->dir - projectionOnWallNormall*2.0f*wallNormal );
			findBulletIt->dir = glm::reflect( findBulletIt->dir, wallNormal );

			removedWallId = findWallIt->id;
			_walls.erase( findWallIt );
		}

		if( removedWallId != -1 )
		{
			for( auto listener : _listeners )
				listener->onWallDeleted( removedWallId );
		}
	}
	void BulletsManager::addBulletsManagerListener( BulletsManagerListener* lst )
	{
		if ( lst )
		{
			auto findIt = std::find( _listeners.begin(), _listeners.end(), lst );

			if ( findIt == _listeners.end() )
			{
				_listeners.push_back( lst );
			}
		}
	}
	void BulletsManager::delBulletsManagerListener( BulletsManagerListener* lst )
	{
		if ( lst )
		{
			auto findIt = std::find( _listeners.begin(), _listeners.end(), lst );

			if ( findIt != _listeners.end() )
			{
				_listeners.erase( findIt );
			}
		}
	}
	void BulletsManager::addWall( const glm::vec2& p1, const glm::vec2& p2 )
	{
		if ( _walls.capacity() == _walls.size() )
		{
			_walls.reserve( _walls.size() + 10 );
		}

		_walls.resize( _walls.size() + 1 );

		auto& wall = _walls[_walls.size() - 1];
		wall.id = _wallsIDGenerator++;

		wall.p1 = p1;
		wall.p2 = p2;
		wall.linearFunction.initWithTwoPoints( p1, p2 );

		for( auto listener : _listeners )
			listener->onWallAdded( wall.id, wall.p1, wall.p2 );

	}

	void BulletsManager::fire( const glm::vec2& pos, const glm::vec2& dir, float speed, float shotTime, float lifeTime )
	{
		std::scoped_lock<std::recursive_mutex> locker(_waitStartFlyBulletsLock);

		if( _waitStartFlyBullets.size() == _waitStartFlyBullets.capacity() )
		{
			_waitStartFlyBullets.reserve( _waitStartFlyBullets.size() + 10 );
		}

		_waitStartFlyBullets.resize( _waitStartFlyBullets.size() + 1 );

		auto& bullet = _waitStartFlyBullets[ _waitStartFlyBullets.size() -1 ];
		bullet.id = _bulletsIDGenerator++;

		bullet.pos = pos;
		bullet.dir = glm::normalize( dir );
		bullet.speed = speed;
		bullet.timeForPos = shotTime;
		bullet.finishFlyTime = shotTime + lifeTime;
	}
	void BulletsManager::update( float deltaTime )
	{
		float lastBulletsPosForTime = _time;// rename;

		_time += deltaTime;

		shootNewBullets();

		do
		{
			if ( _earliestCollision.exist() && _earliestCollision.time <= _time )
			{
				lastBulletsPosForTime = _earliestCollision.time;

				moveBulletsLinearTillTime( _earliestCollision.time );
				doCollision( _earliestCollision );

				_earliestCollision.reset();

				for( const auto& bullet : _flyingBullets )
				{
					auto earliestCollistionForBullet = getEarliestCollision( bullet );
					
					if ( earliestCollistionForBullet.exist() )
					{
						if ( !_earliestCollision.exist() || _earliestCollision.time > earliestCollistionForBullet.time )
						{
							_earliestCollision = earliestCollistionForBullet;
						}
					}
				}

				if ( _earliestCollision.exist() )
				{
					for( auto listener : _listeners )
						listener->onNewEarliestCollision( _earliestCollision.bulletID, _earliestCollision.wallID, _earliestCollision.collisionPoint );
				}
				else
				{
					for( auto listener : _listeners )
						listener->noEarliestCollision();
				}
			}
			else
			{
				moveBulletsLinearTillTime( _time );
				lastBulletsPosForTime = _time;
			}

		} while( abs( lastBulletsPosForTime - _time ) > std::numeric_limits<float>::epsilon() );

		for( auto listener : _listeners )
		{
			for( const auto& bullet : _flyingBullets )
			{
				listener->onBulletChangePos( bullet.id, bullet.pos );
			}
		}

	}
}