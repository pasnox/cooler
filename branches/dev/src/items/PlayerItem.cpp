#include "PlayerItem.h"

#include <TilesManager.h>
#include <PadSettings.h>
#include <MapItem.h>

#include <QTimer>
#include <QGraphicsScene>
#include <QKeyEvent>
#include <QPainter>
#include <QDebug>

PlayerItem::PlayerItem( AbstractTile* tile, QGraphicsItem* parent )
	: QObject(), MapObjectItem( tile, parent )
{
	mPad = 0;
	setTile( tile );
	mStroke = Globals::PadStrokeDown;
	mStrokeStep = 0;
	mStrokeSpeed = 10;
	mStrokeTimer = new QTimer( this );
	
	connect( mStrokeTimer, SIGNAL( timeout() ), this, SLOT( strokeTimer_timeout() ) );
}

PlayerItem::~PlayerItem()
{
}

void PlayerItem::paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget )
{
	MapObjectItem::paint( painter, option, widget );
}

QRectF PlayerItem::explosiveBoundingRect() const
{
	QRectF br = boundingRect();
	int height = br.height() /2;
	br.moveTop( height );
	br.setHeight( height );
	return br;
}

void PlayerItem::setTile( AbstractTile* tile )
{
	Q_ASSERT( mTile->Type == Globals::PlayerTile );
	MapObjectItem::setTile( tile );
	mPlayerTile = static_cast<PlayerTile*>( tile );
	setZValue( Globals::PlayerLayer );
}

PadSettings* PlayerItem::pad() const
{
	return mPad;
}

void PlayerItem::setPad( PadSettings* pad )
{
	mPad = pad;
}

void PlayerItem::setPosAt( qreal step, const QPoint& pos )
{
	mStrokeStep = step;
	setPixmap( mPlayerTile->tile( mStroke, mStrokeStep ) );
	setPos( pos );
	update();
}

void PlayerItem::handleKeyboard( QKeyEvent* event )
{	
	switch ( event->type() )
	{
		case QEvent::KeyPress:
			if ( mPad->isStrokeKeyUsed( event->key() ) )
			{
				mStroke = mPad->keyStroke( event->key() );
				
				if ( !mStrokeTimer->isActive() )
				{
					mStrokeTimer->start( mStrokeSpeed );
				}
			}
			break;
		case QEvent::KeyRelease:
			if ( event->isAutoRepeat() )
			{
				return;
			}
			
			if ( mPad->isStrokeKeyUsed( event->key() ) )
			{
				if ( mStrokeTimer->isActive() )
				{
					mStrokeTimer->stop();
				}
				
				setPosAt( 0, pos().toPoint() );
			}
			else if ( mPad->isActionKeyUsed( event->key() ) )
			{
				Globals::PadAction action = mPad->keyAction( event->key() );
				handleAction( action );
			}
			break;
		default:
			Q_ASSERT( 0 );
			break;
	}
}

void PlayerItem::handleAction( Globals::PadAction action )
{
	switch ( action )
	{
		case Globals::PadAction1:
			dropBomb();
			break;
		case Globals::PadAction2:
			break;
		case Globals::PadAction3:
			break;
		case Globals::PadAction4:
			break;
		case Globals::PadActionNo:
			break;
	}
}

void PlayerItem::dropBomb()
{
	/*
	const BombTile bombTile = TilesManager::instance()->bombTiles().values().first();
	QPoint thisPos = mapRectToItem( map(), bodyBoundingRect() ).toRect().topLeft();
	BombItem* bomb = new BombItem( bombTile, scene() );
	bomb->setZValue( Globals::BombLayer );
	bomb->setPos( map()->closestPos( thisPos ) );
	bomb->setParentItem( map() );
	
	//scene()->addItem( bomb );
	*/
}

void PlayerItem::strokeTimer_timeout()
{
	mStrokeStep += 0.2;
	
	if ( mStrokeStep > mPlayerTile->steps() )
	{
		mStrokeStep = 0;
	}
	
	QPoint newPos = pos().toPoint();
	const QPoint strokablePos = map()->canStrokeTo( this, mStroke );
	
	newPos += strokablePos;
	
	setPosAt( mStrokeStep, newPos );
}
