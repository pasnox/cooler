#include "BombItem.h"

#include <QApplication>
#include <QTimer>

BombItem::BombItem( AbstractTile* tile, QGraphicsItem* parent, const BombSettings& settings )
	: QObject( QApplication::instance() ), MapObjectItem( tile, parent )
{
	mSettings = settings;
	
	setZValue( Globals::BombLayer );
	setTile( tile );
	
	mAnimationStep = 0;
	mAnimationTimer = new QTimer( this );
	mExplodeTimer = new QTimer( this );
	
	connect( mAnimationTimer, SIGNAL( timeout() ), this, SLOT( animationTimer_timeout() ) );
	connect( mExplodeTimer, SIGNAL( timeout() ), this, SLOT( explodeTimer_timeout() ) );
	
	mAnimationTimer->start( 150 );
}

BombItem::~BombItem()
{
}

const BombSettings& BombItem::settings() const
{
	return mSettings;
}

void BombItem::setSettings( const BombSettings& settings )
{
	mSettings = settings;
}

void BombItem::start()
{
	//
}

void BombItem::setTileStep( int step )
{
	mAnimationStep = step;
	setPixmap( mTile->tile( mAnimationStep ) );
}

void BombItem::animationTimer_timeout()
{
	mAnimationStep++;
	
	if ( mAnimationStep >= mTile->steps() )
	{
		mAnimationStep = 0;
	}
	
	setTileStep( mAnimationStep );
}

void BombItem::explodeTimer_timeout()
{
	mExplodeTimer->stop();
	deleteLater();
}
