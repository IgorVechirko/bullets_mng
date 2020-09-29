#include "BulletsManager.h"

#include "BulletsManagerListener.h"
#include "Timer.h"

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

		auto epsilon = std::numeric_limits<float>::epsilon();
		
		return Xrange.first - epsilon <= point.x && Xrange.second + epsilon >= point.x && Yrange.first - epsilon <= point.y && Yrange.second + epsilon >= point.y;
	}
	void BulletsManager::shootNewBullets()
	{
		std::vector<Bullet> shotedBullets;

		std::unique_lock<std::recursive_mutex> lock(_waitStartFlyBulletsLock);
		for( auto bulletIt = _waitStartFlyBullets.begin(); bulletIt != _waitStartFlyBullets.end(); bulletIt++ )
		{
			if ( _time >= bulletIt->timeForPos )
			{
				shotedBullets.push_back( *bulletIt );
				bulletIt->remove = true;
			}
			else
			{
				break;
			}
		}

		if ( !shotedBullets.empty() )
		{
			std::remove_if( _waitStartFlyBullets.begin(), _waitStartFlyBullets.end(), [](const Bullet& bullet) -> bool {
				return bullet.remove;		
			} );
			_waitStartFlyBullets.resize( _waitStartFlyBullets.size() - shotedBullets.size() );
		}
		lock.unlock();

		Collision* newBulletsEarliestCollision = nullptr;

		for( auto& bullet : shotedBullets )
		{
			calculateBulletFutureCollisions( bullet );

			if ( !bullet.sortedFutureCollisions.empty() )
			{
				if ( !newBulletsEarliestCollision || ( newBulletsEarliestCollision->time >= bullet.sortedFutureCollisions.front().time  ) )
				{
					newBulletsEarliestCollision = &bullet.sortedFutureCollisions.front();
				}
			}
		}

		if( newBulletsEarliestCollision )
		{
			if ( !_earliestCollision.exist() || _earliestCollision.time > newBulletsEarliestCollision->time )
				_earliestCollision = *newBulletsEarliestCollision;
		}


		for( auto listener : _listeners )
		{
			for( const auto& bullet : shotedBullets )
			{
				listener->onBulletAdded( bullet.id, bullet.pos );
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


		auto neededSize = _flyingBullets.size() + shotedBullets.size();
		if( _flyingBullets.capacity() < neededSize )
		{
			_flyingBullets.reserve( std::max<size_t>( neededSize, 50 ) );
		}

		std::move( shotedBullets.begin(), shotedBullets.end(), std::inserter( _flyingBullets,_flyingBullets.end() ) );
	}
	void BulletsManager::calculateBulletFutureCollisions( Bullet& bullet )
	{
		auto moveDist = ( bullet.finishFlyTime - bullet.timeForPos ) * bullet.speed;
		const auto& bulletFinishPos = bullet.pos + bullet.dir * moveDist;

		LinearFunction bulletPathLinearFunc( bullet.pos, bulletFinishPos );

		for( const auto& wall : _walls )
		{
			if ( !wall.linearFunction.isParallel( bulletPathLinearFunc ) )
			{
				const auto& crossPoint = wall.linearFunction.crossPoint( bulletPathLinearFunc );

				if ( isPointInSegment( wall.p1, wall.p2, crossPoint ) && isPointInSegment( bullet.pos, bulletFinishPos, crossPoint ) )
				{
					float distToCollision = glm::length( crossPoint - bullet.pos );
					auto newCollisionTime = bullet.timeForPos + distToCollision/bullet.speed;

					auto collisionIt = bullet.sortedFutureCollisions.begin();
					for( ; collisionIt != bullet.sortedFutureCollisions.end(); collisionIt++ )
					{
						if ( collisionIt->time > newCollisionTime )
							break;
					}

					auto newCollisionIt = bullet.sortedFutureCollisions.insert( collisionIt, Collision() );

					newCollisionIt->bulletID = bullet.id;
					newCollisionIt->wallID = wall.id;
					newCollisionIt->time = newCollisionTime;
					newCollisionIt->collisionPoint = crossPoint;
				}
			}
		}

	}
	bool BulletsManager::isWallExist( int wallID ) const
	{
		return _existingWallsIDs.find( wallID ) != _existingWallsIDs.end();
	}
	void BulletsManager::moveBulletsLinearTillTime( float time )
	{
		std::vector<int> removedBulletsIDs;

		for( auto it = _flyingBullets.begin(); it != _flyingBullets.end(); it++ )
		{
			auto& bullet = (*it);

			if ( bullet.finishFlyTime < time )
			{
				removedBulletsIDs.push_back(bullet.id);
				it->remove = true;
			}
			else if ( bullet.timeForPos < time )
			{
				bullet.pos += (time - bullet.timeForPos)*bullet.speed*bullet.dir;
				bullet.timeForPos = time;
			}
		}

		std::remove_if( _flyingBullets.begin(), _flyingBullets.end(), [](Bullet& bullet) {
			return bullet.remove;
		});

		if ( !removedBulletsIDs.empty() )
			_flyingBullets.resize( _flyingBullets.size() - removedBulletsIDs.size() );


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
			const auto& wallNormal = glm::normalize( glm::vec2( findWallIt->linearFunction.Acoef, findWallIt->linearFunction.Bcoef ) );

			findBulletIt->pos = collision.collisionPoint;
			findBulletIt->timeForPos = collision.time;
			findBulletIt->dir = glm::reflect( findBulletIt->dir, wallNormal );

			removedWallId = findWallIt->id;
			_existingWallsIDs.erase(removedWallId);
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
	void BulletsManager::findEarliestCollision()
	{
		Collision* earliestCollistion = nullptr;

		for( auto& bullet : _flyingBullets )
		{
			Collision* earliestCollistionForBullet = nullptr;

			while( !bullet.sortedFutureCollisions.empty() )
			{
				if ( isWallExist( bullet.sortedFutureCollisions.front().wallID ) )
				{
					earliestCollistionForBullet = &bullet.sortedFutureCollisions.front();
					break;
				}
				else
				{
					bullet.sortedFutureCollisions.pop_front();
				}
			}
				
			if ( earliestCollistionForBullet )
			{
				if ( !earliestCollistion || earliestCollistion->time > earliestCollistionForBullet->time )
				{
					earliestCollistion = earliestCollistionForBullet;
				}
			}
		}

		if ( earliestCollistion )
			_earliestCollision = *earliestCollistion;
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
			_walls.reserve( _walls.size() + 50 );
		}

		_walls.resize( _walls.size() + 1 );

		auto& wall = _walls[_walls.size() - 1];
		wall.id = _wallsIDGenerator++;

		wall.p1 = p1;
		wall.p2 = p2;
		wall.linearFunction.initWithTwoPoints( p1, p2 );

		_existingWallsIDs.insert( wall.id );

		for( auto listener : _listeners )
			listener->onWallAdded( wall.id, wall.p1, wall.p2 );

	}

	void BulletsManager::fire( const glm::vec2& pos, const glm::vec2& dir, float speed, float shotTime, float lifeTime )
	{
		std::scoped_lock<std::recursive_mutex> locker(_waitStartFlyBulletsLock);

		auto bulletInsertIt = _waitStartFlyBullets.begin();
		for( ; bulletInsertIt != _waitStartFlyBullets.end(); bulletInsertIt++ )
		{
			if ( bulletInsertIt->timeForPos > shotTime )
				break;
		}

		auto newBulletIt = _waitStartFlyBullets.insert( bulletInsertIt, Bullet() );
		newBulletIt->id = _bulletsIDGenerator++;

		newBulletIt->pos = pos;
		newBulletIt->dir = glm::normalize( dir );
		newBulletIt->speed = speed;
		newBulletIt->timeForPos = shotTime;
		newBulletIt->finishFlyTime = shotTime + lifeTime;
	}
	void BulletsManager::update( float deltaTime )
	{
		_time += deltaTime;

		shootNewBullets();

		while( _earliestCollision.exist() && _earliestCollision.time <= _time )
		{
			doCollision( _earliestCollision );
			_earliestCollision.reset();

			findEarliestCollision();
		}

		moveBulletsLinearTillTime( _time );	

		for( auto listener : _listeners )
		{
			for( const auto& bullet : _flyingBullets )
			{
				listener->onBulletChangePos( bullet.id, bullet.pos );
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
}