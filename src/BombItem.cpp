#include "BombItem.h"

#include <QTimer>

BombItem::BombItem( const BombTile& bombTile, QObject* parent )
	: QObject( parent ), QGraphicsPixmapItem()
{
	mBombTile = bombTile;
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

int BombItem::type() const
{
	return Type;
}

QRectF BombItem::boundingRect() const
{
	return QRectF( QPointF( 0, 0 ), pixmap().size() );
}

void BombItem::paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget )
{
	QGraphicsPixmapItem::paint( painter, option, widget );
}

void BombItem::setTileStep( int step )
{
	mAnimationStep = step;
	setPixmap( mBombTile.tile( mAnimationStep ) );
}

void BombItem::animationTimer_timeout()
{
	mAnimationStep++;
	
	if ( mAnimationStep > mBombTile.steps() )
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
