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
	bool BulletsManager::isPointInSegment( const glm::vec2& s1, const glm::vec2& s2, const glm::vec2& point ) const
	{
		const auto& Xrange = std::minmax( s1.x, s2.x );
		const auto& Yrange = std::minmax( s1.y, s2.y );

		auto epsilon = 0.001f;
		
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
				shotedBullets.push_back(*bulletIt);//copied data
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

		for( auto& bullet : shotedBullets )
		{
			calculateBulletFutureCollisions( bullet );

			if ( !bullet.sortedFutureCollisions.empty() )
			{
				if ( !_earliestCollision.exist() || ( _earliestCollision.time >= bullet.sortedFutureCollisions.begin()->time  ) )
				{
					_earliestCollision = *bullet.sortedFutureCollisions.begin();
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
	BulletsManager::Collision BulletsManager::getBulletEarliestCollision( Bullet& bullet )
	{
		Collision result;

		while( !bullet.sortedFutureCollisions.empty() )
		{
			if ( isWallExist( bullet.sortedFutureCollisions.begin()->wallID ) )
			{
				result = *bullet.sortedFutureCollisions.begin();
				break;
			}
			else
			{
				bullet.sortedFutureCollisions.erase( bullet.sortedFutureCollisions.begin() );
			}
		}

		return result;
	}
	void BulletsManager::calculateBulletFutureCollisions( Bullet& bullet )
	{
		auto moveDist = ( bullet.finishFlyTime - bullet.timeForPos ) * bullet.speed;
		auto bulletFinishPos = bullet.pos + bullet.dir * moveDist;

		LinearFunction bulletPathLinearFunc( bullet.pos, bulletFinishPos );

		for( const auto& wall : _walls )
		{
			if ( !wall.linearFunction.isParallel( bulletPathLinearFunc ) )
			{
				auto crossPoint = wall.linearFunction.crossPoint( bulletPathLinearFunc );

				if ( isPointInSegment( wall.p1, wall.p2, crossPoint ) && isPointInSegment( bullet.pos, bulletFinishPos, crossPoint ) )
				{
					Collision newCollision;
					newCollision.bulletID = bullet.id;
					newCollision.wallID = wall.id;
					newCollision.collisionPoint = crossPoint;

					float distToCollision = glm::length( crossPoint - bullet.pos );
					newCollision.time = bullet.timeForPos + distToCollision/bullet.speed;

					auto collisionIt = bullet.sortedFutureCollisions.begin();
					for( ; collisionIt != bullet.sortedFutureCollisions.end(); collisionIt++ )
					{
						if ( collisionIt->time > newCollision.time )
							break;
					}

					bullet.sortedFutureCollisions.insert( collisionIt, newCollision );// data copyied
				}
			}
		}

	}
	bool BulletsManager::isWallExist( int wallID )
	{
		return _walls.end() != std::find_if( _walls.begin(), _walls.end(), [wallID](Wall& wall) -> bool {
			return wallID == wall.id;
			} );
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

			findBulletIt->dir = glm::reflect( findBulletIt->dir, wallNormal );

			removedWallId = findWallIt->id;
			_walls.erase( findWallIt );

			findBulletIt->sortedFutureCollisions.clear();
			calculateBulletFutureCollisions( *findBulletIt );
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
		float lastBulletsCalculatedPosTimeStamp = _time;

		_time += deltaTime;

		shootNewBullets();

		do
		{
			if ( _earliestCollision.exist() && _earliestCollision.time <= _time )
			{
				lastBulletsCalculatedPosTimeStamp = _earliestCollision.time;

				moveBulletsLinearTillTime( _earliestCollision.time );
				doCollision( _earliestCollision );

				_earliestCollision.reset();

				for( auto& bullet : _flyingBullets )
				{
					auto earliestCollistionForBullet = getBulletEarliestCollision( bullet );
					
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
				lastBulletsCalculatedPosTimeStamp = _time;
			}

		} while( abs( lastBulletsCalculatedPosTimeStamp - _time ) > std::numeric_limits<float>::epsilon() );

		for( auto listener : _listeners )
		{
			for( const auto& bullet : _flyingBullets )
			{
				listener->onBulletChangePos( bullet.id, bullet.pos );
			}
		}

	}
}