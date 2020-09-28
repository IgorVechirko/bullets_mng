#ifndef BulletsMngDisplayScene_H
#define BulletsMngDisplayScene_H

#include "SceneBase.h"
#include "BulletsManager.h"
#include "BulletsManagerListener.h"
#include "EventsHandler.h"

namespace BulletsMng
{
	class Button;
	class Line;
	class Point;
	class BulletsMngDisplayScene
		: public SceneBase
		, public BulletsManagerListener
		, public EventsHandler
	{

		BulletsManager _bulletsMng;

		int _wallsAmount;
		int _bulletsAmount;
		int _timeMultiplier;


		std::map<int,Line*> _wallsVisual;
		std::map<int,Point*> _bulletsVisual;

		Point* _earliestCollision;

		std::vector<std::unique_ptr<std::thread>> _shotedThreads;

		sf::Text _multiplierInfoLabel;
		sf::Font _font;


		void returnToSetupScene();

		void generateWalls();
		void generateBullets();

		void initUI();

		void increaseTimeMultiplier();
		void decreaseTimeMultiplier();
		void updateTimeMultiplierLabel();

	protected:

		virtual void onOpened( const std::map<std::string,std::string>& sceneParams ) override;

		virtual void update( float deltaTime ) override;

		virtual void render( sf::RenderWindow* window ) override;

		virtual void onEventHandled( const sf::Event& handledEvent ) override;

		//BulletsManagerListener
		virtual void onWallAdded( int id, const glm::vec2& p1, const glm::vec2& p2 );
		virtual void onWallDeleted( int id );
		virtual void onBulletAdded( int id, const glm::vec2& pos );
		virtual void onBulletChangePos( int id, const glm::vec2& newPos );
		virtual void onBulletDeleted( int id );
		virtual void noEarliestCollision();
		virtual void onNewEarliestCollision( int bulletID, int wallID, const glm::vec2 pos );

	public:

		BulletsMngDisplayScene( IApplication* application );
		virtual ~BulletsMngDisplayScene();
		MAKE_UNCOPYABLE( BulletsMngDisplayScene );

		


	};

}


#endif