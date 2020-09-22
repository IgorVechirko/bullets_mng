#include "BulletsMngDisplayScene.h"

#include "Line.h"
#include "Point.h"

namespace BulletsMng
{

	BulletsMngDisplayScene::BulletsMngDisplayScene( IScenesSwitcher* switcher )
		: SceneBase( switcher )
	{
	}
	BulletsMngDisplayScene::~BulletsMngDisplayScene()
	{
	}
	void BulletsMngDisplayScene::onOpened()
	{
		auto line = createRunderedUnit<Line>();
		if ( line )
		{
			line->setColor( sf::Color::Red );
			line->setStartPoint( sf::Vector2f( 20, 200 ) );
			line->setFinishPoint( sf::Vector2f( 202, 20 ) );
		}

		auto point = createRunderedUnit<Point>();
		if ( point )
		{
			point->getShape().setRadius( 5 );
			point->getShape().setPosition( 500, 500 );
			point->getShape().setFillColor( sf::Color::Blue	 );
		}
	}

}