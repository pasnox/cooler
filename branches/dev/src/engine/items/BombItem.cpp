#include "BombItem.h"

#include <QApplication>
#include <QTimer>

BombItem::BombItem( AbstractTile* tile, QGraphicsItem* parent )
	: QObject( QApplication::instance() ), MapObjectItem( tile, parent )
{
	mAnimationStep = -1;
	mAnimationTimer = new QTimer( this );
	mExplodeTimer = new QTimer( this );
	
	connect( mAnimationTimer, SIGNAL( timeout() ), this, SLOT( animationTimer_timeout() ) );
	connect( mExplodeTimer, SIGNAL( timeout() ), this, SLOT( explodeTimer_timeout() ) );
	
	mAnimationTimer->start( 100 );
}

BombItem::~BombItem()
{
}

void BombItem::setTileStep( int step )
{
	mAnimationStep = step;
	setPixmap( mTile->tile( mAnimationStep ) );
}

void BombItem::animationTimer_timeout()
{
	mAnimationStep++;
	
	if ( mAnimationStep > mTile->steps() )
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
