#include "BulletsMngDisplayScene.h"

#include "Line.h"
#include "Point.h"
#include "IApplication.h"

namespace BulletsMng
{

	BulletsMngDisplayScene::BulletsMngDisplayScene( IApplication* switcher )
		: SceneBase( switcher )
	{
	}
	BulletsMngDisplayScene::~BulletsMngDisplayScene()
	{
	}
	void BulletsMngDisplayScene::generateWalls()
	{
		//_bulletsMng->addWall( glm::vec2( getGLContext()->getWindowSize().y * 1.3f, 0.0f ), glm::vec2( 0.0f, getGLContext()->getWindowSize().y * 1.3f ) );
		//_bulletsMng->addWall( glm::vec2( 0.0f, 200.0f ), glm::vec2( 1124.0f, 200.0f ) );

		std::default_random_engine dre;
		std::uniform_real_distribution<float> xGen( 0.0f, getApplication()->getWindowSize().x );
		std::uniform_real_distribution<float> yGen( 0.0f, getApplication()->getWindowSize().y );

		int wallsAmount = 10;
		for( int wallNum = 0; wallNum < wallsAmount; wallNum++ )
			_bulletsMng.addWall( glm::vec2( xGen(dre), yGen(dre) ), glm::vec2( xGen(dre), yGen(dre) ) );
	}
	void BulletsMngDisplayScene::generateBullets()
	{
		//Vec3 startPos( getGLContext()->getWindowSize().x / 2.0f, getGLContext()->getWindowSize().y / 2.0f, 0.0f );
		//_bulletsMng->fire( startPos, glm::vec2( 1.0f, 0.0f ), 500.0f, 2.0f, 10.0f );

		std::default_random_engine dre;
		std::uniform_real_distribution<float> dirGen( -1.0f, 1.0f );
		std::uniform_real_distribution<float> xGen(getApplication()->getWindowSize().x * 0.25f, getApplication()->getWindowSize().x * 0.75f );
		std::uniform_real_distribution<float> yGen( getApplication()->getWindowSize().y * 0.25f, getApplication()->getWindowSize().y * 0.75f );

		std::uniform_real_distribution<float> speedGen( 100.0f, 500.0f );
		std::uniform_real_distribution<float> lifeTimeGen( 5.0f, 10.0f );

		std::uniform_real_distribution<float> fireTimeGen( 2.0f, 15.0f );
			
		int bulletsAmount = 10;

		for( int i = 0; i < bulletsAmount; i++ )
		{
			auto dir = glm::normalize( glm::vec2( dirGen(dre), dirGen(dre) ) );
			glm::vec2 pos( xGen(dre), yGen(dre) );
			auto speed = speedGen(dre);
			auto fireTime = fireTimeGen(dre);
			auto lifeTime = lifeTimeGen(dre);

			if ( i == 4 )
				_bulletsMng.fire( pos, dir, speed, fireTime, lifeTime );
		}
	}
	void BulletsMngDisplayScene::onOpened()
	{
		_bulletsMng.addBulletsManagerListener( this );

		generateWalls();
		generateBullets();
	}
	void BulletsMngDisplayScene::update( float deltaTime )
	{
		_bulletsMng.update( deltaTime * 20.7f );
	}
	void BulletsMngDisplayScene::onWallAdded( int id, const glm::vec2& p1, const glm::vec2& p2 )
	{
		auto line = createRunderedUnit<Line>();
		if ( line )
		{
			line->setStartPoint( sf::Vector2f( p1.x, getApplication()->getWindowSize().y - p1.y ) );
			line->setFinishPoint( sf::Vector2f( p2.x, getApplication()->getWindowSize().y - p2.y ) );

			_wallsVisual[id] = line;
		}
	}
	void BulletsMngDisplayScene::onWallDeleted( int id )
	{
		auto findIt = _wallsVisual.find( id );
		if ( findIt != _wallsVisual.end() )
		{
			removeRenderedUnit( findIt->second );
			_wallsVisual.erase( findIt );
		}
	}
	void BulletsMngDisplayScene::onBulletAdded( int id, const glm::vec2& pos )
	{
		auto point = createRunderedUnit<Point>();
		if ( point )
		{
			Console::log( "bullet ", id, " added to position ", pos.x, " ", pos.y );

			point->getShape().setFillColor( sf::Color::Blue );
			point->getShape().setRadius( 5 );
			point->getShape().setOrigin( 5, 5 );
			point->getShape().setPosition( pos.x, getApplication()->getWindowSize().y - pos.y );

			_bulletsVisual[id] = point;
		}
	}
	void BulletsMngDisplayScene::onBulletChangePos( int id, const glm::vec2& newPos )
	{
		auto findIt = _bulletsVisual.find( id );
		if ( findIt != _bulletsVisual.end() )
		{
			findIt->second->getShape().setPosition( newPos.x, getApplication()->getWindowSize().y - newPos.y );
		}
	}
	void BulletsMngDisplayScene::onBulletDeleted( int id )
	{
		auto findIt = _bulletsVisual.find( id );
		if ( findIt != _bulletsVisual.end() )
		{
			removeRenderedUnit( findIt->second );
			_bulletsVisual.erase( findIt );
		}
	}

}