#include "BulletsMngDisplayScene.h"

#include "Line.h"
#include "Point.h"
#include "IApplication.h"
#include "Button.h"

namespace BulletsMng
{

	BulletsMngDisplayScene::BulletsMngDisplayScene( IApplication* application )
		: SceneBase( application )
		, _wallsAmount( 10 )
		, _bulletsAmount( 10 )
		, _timeMultiplier( 10 )
	{
	}
	BulletsMngDisplayScene::~BulletsMngDisplayScene()
	{
	}
	void BulletsMngDisplayScene::returnToSetupScene()
	{
		getApplication()->switchToScene( "setup" );
	}
	void BulletsMngDisplayScene::generateWalls()
	{
		//_bulletsMng.addWall( glm::vec2( getApplication()->getWindowSize().y * 1.3f, 0.0f ), glm::vec2( 0.0f, getApplication()->getWindowSize().y * 1.3f ) );
		//_bulletsMng.addWall( glm::vec2( 0.0f, 200.0f ), glm::vec2( 1124.0f, 200.0f ) );

		std::default_random_engine dre;
		std::uniform_real_distribution<float> xGen( 0.0f, static_cast<float>(getApplication()->getWindowSize().x) );
		std::uniform_real_distribution<float> yGen( 0.0f, static_cast<float>(getApplication()->getWindowSize().y) );

		for( int wallNum = 0; wallNum < _wallsAmount; wallNum++ )
			_bulletsMng.addWall( glm::vec2( xGen(dre), yGen(dre) ), glm::vec2( xGen(dre), yGen(dre) ) );
	}
	void BulletsMngDisplayScene::generateBullets()
	{
		//glm::vec2 startPos( getApplication()->getWindowSize().x / 2.0f, getApplication()->getWindowSize().y / 2.0f );
		//_bulletsMng.fire( startPos, glm::vec2( 1.0f, 0.0f ), 500.0f, 2.0f, 10.0f );

		std::default_random_engine dre;
		std::uniform_real_distribution<float> dirGen( -1.0f, 1.0f );
		std::uniform_real_distribution<float> xGen(getApplication()->getWindowSize().x * 0.25f, getApplication()->getWindowSize().x * 0.75f );
		std::uniform_real_distribution<float> yGen( getApplication()->getWindowSize().y * 0.25f, getApplication()->getWindowSize().y * 0.75f );

		std::uniform_real_distribution<float> speedGen( 100.0f, 500.0f );
		std::uniform_real_distribution<float> lifeTimeGen( 5.0f, 5.0f );

		std::uniform_real_distribution<float> fireTimeGen( 2.0f, 15.0f );

		auto threadFunc = []( BulletsManager& mng, glm::vec2 pos, glm::vec2 dir, float speed, float fireTime, float lifeTime ) {
			std::this_thread::sleep_for( std::chrono::duration<float,std::ratio<1,1>>(fireTime) );
			mng.fire( pos, dir, speed, fireTime, lifeTime );
		};

		for( int i = 0; i < _bulletsAmount; i++ )
		{
			auto dir = glm::normalize( glm::vec2( dirGen(dre), dirGen(dre) ) );
			glm::vec2 pos( xGen(dre), yGen(dre) );
			auto speed = speedGen(dre);
			auto fireTime = fireTimeGen(dre);
			auto lifeTime = lifeTimeGen(dre);

			_bulletsMng.fire( pos, dir, speed, fireTime, lifeTime );

			//auto thread = new std::thread( threadFunc, std::ref(_bulletsMng), pos, dir, speed, fireTime, lifeTime );

			//_shotedThreads.push_back( std::unique_ptr<std::thread>(thread) );
		}
	}
	void BulletsMngDisplayScene::initUI()
	{
		float buttonHeight = 50.0f;

		auto openSetupSceneBtn = createRenderedUnit<Button>();
		if ( openSetupSceneBtn )
		{
			openSetupSceneBtn->getText().setCharacterSize( 15 );
			openSetupSceneBtn->setPressedCallBack( std::bind( &BulletsMngDisplayScene::returnToSetupScene, this ) );
			openSetupSceneBtn->getText().setString( "RETURN TO SETUP" );
			addEventsHandler( openSetupSceneBtn );

			buttonHeight = openSetupSceneBtn->getText().getCharacterSize();
		}

		addEventsHandler( this );

		if ( _font.loadFromFile( "arial.ttf" ) )
		{
			_multiplierInfoLabel.setFont( _font );
			_multiplierInfoLabel.setColor( sf::Color::Blue );
			_multiplierInfoLabel.setCharacterSize( 15 );

			_multiplierInfoLabel.setPosition( 0.0f, buttonHeight );
		}

		updateTimeMultiplierLabel();
	}
	void BulletsMngDisplayScene::increaseTimeMultiplier()
	{
		int maxMultiplier = 200;

		if ( _timeMultiplier >= 10 )
		{
			if ( _timeMultiplier < maxMultiplier )
				_timeMultiplier += 10;
		}
		else
		{
			_timeMultiplier += 1;
		}

		updateTimeMultiplierLabel();
	}
	void BulletsMngDisplayScene::decreaseTimeMultiplier()
	{
		int minMultiplier = 1;

		if ( _timeMultiplier <= 10 )
		{
			if ( _timeMultiplier > minMultiplier )
				_timeMultiplier -= 1;
		}
		else
		{
			_timeMultiplier -= 10;
		}

		updateTimeMultiplierLabel();
	}
	void BulletsMngDisplayScene::updateTimeMultiplierLabel()
	{
		int wholePart = static_cast<int>( _timeMultiplier / 10.0f );
		int decPart = static_cast<int>( _timeMultiplier - (wholePart*10.0f) );

		_multiplierInfoLabel.setString( std::to_string(wholePart) + "." + std::to_string(decPart) + " " + "scroll mouse wheel to change time multiplier" );
	}
	void BulletsMngDisplayScene::onOpened( const std::map<std::string,std::string>& sceneParams )
	{
		initUI();

		auto findParamIt = sceneParams.find( "walls_amount" );
		if ( findParamIt != sceneParams.end() )
		{
			_wallsAmount = std::atoi( findParamIt->second.c_str() );
		}

		findParamIt = sceneParams.find( "bullets_amount" );
		if ( findParamIt != sceneParams.end() )
		{
			_bulletsAmount = std::atoi( findParamIt->second.c_str() );
		}

		_earliestCollision = createRenderedUnit<Point>();
		if ( _earliestCollision )
		{
			_earliestCollision->getShape().setOrigin( 10, 10 );
			_earliestCollision->getShape().setFillColor( sf::Color::Red );
			_earliestCollision->getShape().setRadius( 0 );
		}

		_bulletsMng.addBulletsManagerListener( this );

		generateWalls();
		generateBullets();
	}
	void BulletsMngDisplayScene::update( float deltaTime )
	{
		_bulletsMng.update( deltaTime * ( _timeMultiplier * 0.1f ) );
	}
	void BulletsMngDisplayScene::render( sf::RenderWindow* window )
	{
		SceneBase::render( window );

		window->draw( _multiplierInfoLabel );
	}
	void BulletsMngDisplayScene::onEventHandled( const sf::Event& handledEvent )
	{
		switch (handledEvent.type)
		{
			case sf::Event::MouseWheelScrolled:
			{
				if ( handledEvent.mouseWheelScroll.delta > 0.0f )
					increaseTimeMultiplier();
				else
					decreaseTimeMultiplier();

				break;
			}
			default:
				break;
		}
	}
	void BulletsMngDisplayScene::onWallAdded( int id, const glm::vec2& p1, const glm::vec2& p2 )
	{
		auto line = createRenderedUnit<Line>();
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
		auto point = createRenderedUnit<Point>();
		if ( point )
		{
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
	void BulletsMngDisplayScene::noEarliestCollision()
	{
		if ( _earliestCollision )
			_earliestCollision->getShape().setRadius( 0 );
	}
	void BulletsMngDisplayScene::onNewEarliestCollision( int bulletID, int wallID, const glm::vec2 pos )
	{
		if ( _earliestCollision )
		{
			_earliestCollision->getShape().setPosition( pos.x, getApplication()->getWindowSize().y - pos.y );
			_earliestCollision->getShape().setRadius( 10 );
		}
	}

}